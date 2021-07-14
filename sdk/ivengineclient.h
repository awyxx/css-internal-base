#pragma once

#include "utils.h"

typedef struct player_info_s {
    // scoreboard information
    char name[32];

    char pad[512]; // maybe less

    /*
    // local server user ID, unique while server is running
    int				userID;
    // global unique player identifer
    char			guid[SIGNED_GUID_LEN + 1];
    // friends identification number
    uint32			friendsID;
    // friends name
    char			friendsName[MAX_PLAYER_NAME_LENGTH];
    // true, if player is a bot controlled by game.dll
    bool			fakeplayer;
    // true if player is the HLTV proxy
    bool			ishltv;
    // custom files CRC for this player
    CRC32_t			customFiles[MAX_CUSTOM_FILES];
    // this counter increases each time the server downloaded a new file
    unsigned char	filesDownloaded;*/
} player_info_t;



struct IVEngineClient {

    // Gets the dimensions of the game window
    void GetScreenSize(int& w, int& h) {
        return GetVFunction<void(__thiscall*)(IVEngineClient*, int&, int&)>(this, 5)(this, w, h);
    }

    // Get player info (player_info_t)
    bool GetPlayerInfo(int ent, player_info_t* pinfo) {
        // Dont use this, use sigscan one!
        return GetVFunction<bool(__thiscall*)(IVEngineClient*, int, player_info_t*)>(this, 8)(this, ent, pinfo);
    }

    // Retrieve the player entity number for a specified userID
    int GetPlayerForUserID(int userID) {
        return GetVFunction<int(__thiscall*)(IVEngineClient*, int)>(this, 9)(this, userID);
    }

    // Returns local player entity index
    int GetLocalPlayer(void) {
        return GetVFunction<int(__thiscall*)(IVEngineClient*)>(this, 12)(this);
    }

    float Time(void) {
        return GetVFunction<float(__thiscall*)(IVEngineClient*)>(this, 14)(this);
    } // useless? xd

    // Copy current view orientation into va
    void GetViewAngles(int& va){

    }; // its not int its QAngle! 19

    // Set current view orientation from va
    void SetViewAngles(int& va){

    }; // its not int its QAngle! 20

    // Retrieve the current game's maxclients setting
    int GetMaxClients(void) {
        return GetVFunction<int(__thiscall*)(IVEngineClient*)>(this, 21)(this);
    }

    // Returns true if the player is fully connected and active in game (i.e, not still loading)
    bool IsInGame(void) {
        // Dont use this, use sigscan one!
        return GetVFunction<bool(__thiscall*)(IVEngineClient*)>(this, 26)(this);
    }

    // Returns true if the player is connected, but not necessarily active in game (could still be loading)
    bool IsConnected(void) {
        return GetVFunction<bool(__thiscall*)(IVEngineClient*)>(this, 27)(this);
    }
};