#pragma once

#include "sdk/sdk.h"

struct reversal {
    IClientEntityList *pEntityList;
    IVEngineClient    *pEngine;
    IBaseClientDLL    *pClientDLL;
    IClientMode       *pClientMode;
    CUserCmd          *pCUserCmd;
    ISurface          *pSurface;
    IPanel            *pPanel;
    ICvar             *pCvar;
};

struct vfunctions {
    // Sigscanning virtual functions = profit!!! TY DEADspy

    typedef void*(__thiscall* GetClientEntity_t)(IClientEntityList*, int);           // 55 8b ec 8b 45 08 85 c0 78 ? 05
    typedef bool(__thiscall* GetPlayerInfo_t)(IVEngineClient*, int, player_info_t*); // 55 8b ec 51 56 8b 75 08 4e
    typedef bool(__thiscall* IsInGame_t)(IVEngineClient*);                           // e8 ? ? ? ? 84 c0 74 ? 8b 06 8b ce ff 50 68 (ref)
    typedef int(__cdecl* ScreenPosition_t)(const float* absOrigin, float* screen);   // 89 55 8b ec 83 ec 10 53 ff 15
    
    typedef void(__thiscall* DrawOutlinedRect_t)(ISurface*, int, int, int, int);     // 55 8b ec 51 56 8b f1 89
    

    //
    //IClientEntityList (client.dll)
    GetClientEntity_t IClientEntityList__GetClientEntity; /* (pEntityList, i) --  vtbl_idx: 3*/
    // vtbl_idx: 4; //  55 8b ec 8b 45 08 51 8b d4 89 02 8b 01 ff 50 08 8b c8 85 c9 74 ? 8b 01 ff 50 18

    //IVEngineClient (engine.dll)
    GetPlayerInfo_t IVEngineClient__GetPlayerInfo;        /* (pEngine, idx, player_info_t) -- Gets player info (name, ...)                                vtbl_idx:  8 */
    IsInGame_t IVEngineClient__IsInGame;                  /* (pEngine) -- True if the player is fully connected 'n active ingame (i.e, not still loading) vtbl_idx: 26 */
    // IVDebugOverlay (engine.dll)
    ScreenPosition_t IVDebugOverlay__ScreenPosition;      /* (origin, screen) -- Given a point (param1: 3d coords), sets the screen (param2) 2D position. */
    
    //ISurface (vguimatsurface.dll)
    DrawOutlinedRect_t ISurface__DrawOutlinedRect; /* (x, y, w, h)  vtbl_idx: 14*/
    
};

struct interfaceslist {
    InterfaceReg* client_interface_list;          /* 89 01 a1 ? ? ? ? 89 41 08 8b c1 (client.dll interfacelist)   */
    InterfaceReg* engine_interface_list;          /* a1 ? ? ? ? 89 41 08 8b c1 (engine.dll interfacelist)         */
    InterfaceReg* vguimatsurface_interface_list;  /* a1 ? ? ? ? 89 41 08 8b c1 (vguimatsurface.dll interfacelist) */
    InterfaceReg* vstdlib_interface_list;         /* a1 ? ? ? ? 89 41 08 8b c1 (vstdlib.dll interfacelist)        */
    InterfaceReg* vgui2_interface_list;           /* a1 ? ? ? ? 89 41 08 8b c1 (vgui2.dll interfacelist)          */
};

class cheat : public reversal, vfunctions, interfaceslist {
    uintptr_t dll_client;
    uintptr_t dll_vgui2;
    uintptr_t dll_engine;
    uintptr_t dll_vstdlib;
    uintptr_t dll_vguimatsurface;

    vmt_hook* clientmode_hook;
    vmt_hook* panel_hook;

public:
    // singleton
    static void initialize();
    static void dispose();
    static cheat& singleton();

    // ctor/dtor
    cheat();
    ~cheat();

    // Stuff
    void LoadInterfaces();
    void GetFunctions();

    // Cheat
    bool Update();
    void ESP();

    // hooks
    vmt_hook* hkClientmode() {
        return clientmode_hook;
    }
    vmt_hook* hkPanel() {
        return panel_hook;
    }

    // engine
    CBaseEntity* GetLocal();
};