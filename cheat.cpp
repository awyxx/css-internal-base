
#include "cheat.hpp"

#include "utils.h"


static cheat* c;

typedef bool(__thiscall* CreateMove_t)(void*, float, CUserCmd*);
typedef void(__thiscall* PaintTraverse_t)(void*, int, bool, bool);

bool __fastcall hkCreateMove(void* ecx, void* edx, float flSampleTime, CUserCmd* pUserCmd) {
    static CreateMove_t oCreateMove = nullptr;

    if (!oCreateMove)
        oCreateMove = (CreateMove_t) c->hkClientmode()->GetOriginal(21);

    CBaseEntity* localplayer = c->GetLocal();

    if (pUserCmd && pUserCmd->commandNumber && localplayer) {
        if (pUserCmd->buttons & IN_JUMP && !(localplayer->GetFlags() & FL_ONGROUND))
            pUserCmd->buttons &= ~IN_JUMP;
    }

    return oCreateMove(ecx, flSampleTime, pUserCmd);
}
void __fastcall hkPaintTraverse(void* ecx, void* edx, VPANEL vguiPanel, bool forceRepaint, bool allowForce = true) {
    static PaintTraverse_t oPaintTraverse = nullptr;

    if (!oPaintTraverse)
        oPaintTraverse = (PaintTraverse_t) c->hkPanel()->GetOriginal(41);

    oPaintTraverse(ecx, vguiPanel, forceRepaint, allowForce);

    if (!strcmp("FocusOverlayPanel", c->pPanel->GetName(vguiPanel))) {
        c->ESP();
    }
}

void cheat::initialize() {
    c = new cheat();
}
void cheat::dispose() {
    c->pCvar->ConsoleColorPrintf(red, "\n ->>>>>>");
    c->pCvar->ConsoleColorPrintf(green, " [awx-css.dll] Unloaded from game successfully.\n\n");
    c->clientmode_hook->Disable();
    c->panel_hook->Disable();
    delete c;
}
cheat& cheat::singleton() {
    return *c;
}


void cheat::LoadInterfaces() {

    // Get modules
    dll_client         = get_module_handle("client.dll");
    dll_engine         = get_module_handle("engine.dll");
    dll_vgui2          = get_module_handle("vgui2.dll");
    dll_vstdlib        = get_module_handle("vstdlib.dll");
    dll_vguimatsurface = get_module_handle("vguimatsurface.dll");

    // Get interfaces list
    client_interface_list =
        **(InterfaceReg***) find_pattern(dll_client, 0x2BD05F, "\x83\x89\x01\xa1\x04\x85\x89\x41\x08\x8b\xc1", 3);
    engine_interface_list =
        **(InterfaceReg***) find_pattern(dll_engine, 0x243FF5, "\x81\xa1\x04\x85\x89\x41\x08\x8b\xc1", 1);
    vguimatsurface_interface_list =
        **(InterfaceReg***) find_pattern(dll_vguimatsurface, 0x6DF51, "\x81\xa1\x04\x85\x89\x41\x08\x8b\xc1", 1);
    vstdlib_interface_list =
        **(InterfaceReg***) find_pattern(dll_vstdlib, 0xBCDA, "\x81\xa1\x04\x85\x89\x41\x08\x8b\xc1", 1);
    vgui2_interface_list =
        **(InterfaceReg***) find_pattern(dll_vgui2, 0x1EE45, "\x81\xa1\x04\x85\x89\x41\x08\x8b\xc1", 1);

    // Get pointers
    pClientDLL  = GetInterfacePtr<IBaseClientDLL*>("VClient017", client_interface_list); // also FF D0 83 C4 08 A3 ? ? ? ? C6 (engine.dll) 0xA3949 from engine.dll
    pPanel      = GetInterfacePtr<IPanel*>("VGUI_Panel009", vgui2_interface_list);
    pSurface    = GetInterfacePtr<ISurface*>("VGUI_Surface030", vguimatsurface_interface_list);
    pCvar       = GetInterfacePtr<ICvar*>("VEngineCvar004", vstdlib_interface_list);
    pEngine     = GetInterfacePtr<IVEngineClient*>("VEngineClient013", engine_interface_list);
    pEntityList = GetInterfacePtr<IClientEntityList*>("VClientEntityList003", client_interface_list);

    bool error = !(pClientDLL && pPanel && pSurface && pCvar && pEngine && pEntityList);
    if (error) {
        MessageBox(NULL, L"LoadInterfaces( ) failed! Check interface names or dll modules.",
            L"cheat::LoadInterfaces( )", NULL);
        return;
    }

    // Success!
    pCvar->ConsoleColorPrintf(green, "\n -> Interfaces loaded sucessfully! \n");
}

void cheat::GetFunctions() {

    //IClientEntityList (client.dll)
    IClientEntityList__GetClientEntity = (GetClientEntity_t)find_pattern(dll_client, 0xDD74B, "\x89\x55\x8b\xec\x8b\x45\x08\x85\xc0\x78\x01\x81\x05");

    //IVEngineClient (engine.dll)
    IVEngineClient__GetPlayerInfo      = (GetPlayerInfo_t)  find_pattern(dll_engine, 0x2F3AE0, "\x89\x55\x8B\xec\x51\x56\x8b\x75\x08\x4e");
    IVEngineClient__IsInGame           = (IsInGame_t) get_function_by_call(dll_engine, 0x8227A, "\x81\xe8\x04\x83\x84\xc0\x74\x01\x87\x8b\x06\x8b\xce\xff\x50\x68");
    // IVDebugOverlay (engine.dll)
    IVDebugOverlay__ScreenPosition     = (ScreenPosition_t) find_pattern(dll_engine, 0xE5000, "\x89\x55\x8b\xec\x83\xec\x10\x53\xff\x15");

    //ISurface (vguimatsurface.dll)
    ISurface__DrawOutlinedRect = (DrawOutlinedRect_t)find_pattern(dll_vguimatsurface, 0x3FE80, "\x88\x55\x8b\xec\x51\x56\x8b\xf1\x89");
    
} 


cheat::cheat() {

    LoadInterfaces(); // Get interface pointers and game stuff..
    GetFunctions();  // Get some virtual functions by sigscanning

    pCvar->ConsoleColorPrintf(red, "\n ->>>>>>");
    pCvar->ConsoleColorPrintf(green, " [awx-css.dll] Injected.\n");

    // Get pointer to IClientMode
    pClientMode = **(IClientMode***) find_pattern(dll_client, 0x367B5A,
        "\x82\x8B\x0D\x04\x87\x8B\x01\x5D\xFF\x60\x28\xCC", 2); // 8b 0d ? ? ? ? 8b 01 5d ff 60 28 cc

    // Hook CreateMove and PaintTraverse (replacing vtbls)
    clientmode_hook = new vmt_hook((uintptr_t) pClientMode, 204, {std::make_pair<size_t, void*>(21, (void*) hkCreateMove)});
    panel_hook      = new vmt_hook((uintptr_t) pPanel, 512, {std::make_pair<size_t, void*>(41, (void*) hkPaintTraverse)});


    clientmode_hook->Enable();
    panel_hook->Enable();
}

cheat::~cheat() {
    delete clientmode_hook;
    delete panel_hook;
}

bool cheat::Update() {
    return true;
}

CBaseEntity* cheat::GetLocal() {
    constexpr uintptr_t local_player = 0x4C6708;
    return *(CBaseEntity**) (dll_client + local_player);
}

void cheat::ESP() {

    if (!IVEngineClient__IsInGame(pEngine))
        return;

    CBaseEntity* local = (CBaseEntity*) IClientEntityList__GetClientEntity(pEntityList, pEngine->GetLocalPlayer());

    for (int i = 1; i < pEngine->GetMaxClients(); i++) {

        CBaseEntity* ent = (CBaseEntity*) IClientEntityList__GetClientEntity(pEntityList, i);

        if (!ent /*|| ent == local */ || ent->GetHealth() <= 1) // check GetDormant ??
            continue;

        vector* origin = (vector*) ent->GetAbsOrigin();
        vector  eyepos = (vector)  ent->GetEyePos();

        vector head, foot; // Will store player 2D positions

        // W2S (Convert 3D to 2D)
        IVDebugOverlay__ScreenPosition((float*) origin,  (float*) &foot); // Get foot 2d pos
        IVDebugOverlay__ScreenPosition((float*) &eyepos, (float*) &head); // Get head 2d pos

        if (ent->GetTeam() == local->GetTeam())
            pSurface->DrawSetColor(Color(green));
        else
            pSurface->DrawSetColor(Color(red));

        float height = head.y - foot.y, width = height * 0.25f;
        ISurface__DrawOutlinedRect(pSurface, head.x - width, head.y, width + head.x, head.y - height);



        char Buffer[36] = "ROBYYYYYYYYYYYY";

        size_t Size = strlen( Buffer ) + 1;

		/* set up widebuffer*/
		wchar_t* WideBuffer = new wchar_t[ Size ];

		/* char -> wchar */
		mbstowcs_s( 0, WideBuffer, Size, Buffer, Size - 1 );

        pSurface->DrawSetTextColor(red);
        pSurface->DrawSetTextFont(5);
        pSurface->DrawSetTextPos(head.x - ( width * .5 ), head.y - 12);
        pSurface->DrawPrintText(WideBuffer,  wcslen( WideBuffer ), FontDrawType::FONT_DRAW_ADDITIVE);



        player_info_t ent_info;
        IVEngineClient__GetPlayerInfo(pEngine, i, &ent_info);
    }
}
