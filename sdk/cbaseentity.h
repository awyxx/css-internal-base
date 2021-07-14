#pragma once

#include "utils.h"

#include "cheat.hpp"

namespace offsets {
    constexpr uintptr_t m_iHealth       = 0x94;
    constexpr uintptr_t m_iTeamNum      = 0x9C;
    constexpr uintptr_t m_fFlags        = 0x350;
    constexpr uintptr_t m_vecAbsOrigin  = 0x260;
    constexpr uintptr_t m_vecAbsAngles  = 0x26C;
    constexpr uintptr_t m_vecViewOffset = 0xE8;
} // namespace offsets


struct CBaseEntity {

    int GetHealth(void) {
        return *(int*) ((uint8_t*) this + offsets::m_iHealth);
    }

    int GetTeam(void) {
        return *(int*) ((uint8_t*) this + offsets::m_iTeamNum);
    }

    int GetFlags(void) {
        return *(int*) ((uint8_t*) this + offsets::m_fFlags);
    }

    float* GetAbsOrigin(void) {
        return (float*) ((uint8_t*) this + offsets::m_vecAbsOrigin);
    }

    float* GetAbsAngles(void) {
        return (float*) ((uint8_t*) this + offsets::m_vecAbsAngles);
    }

    float* GetViewOffset(void) {
        return (float*) ((uint8_t*) this + offsets::m_vecViewOffset);
    }

    vector GetEyePos() 
    {
        vector* orig = (vector*) this->GetAbsOrigin();
        vector* view = (vector*) this->GetViewOffset();

        vector eye = {orig->x + view->x,  orig->y + view->y,  orig->z + view->z};
        return eye;
    }
};
