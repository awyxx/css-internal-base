#pragma once

#define IN_ATTACK				(1 << 0)  // Fire weapon
#define IN_JUMP					(1 << 1)  // Jump
#define IN_DUCK					(1 << 2)  // Crouch
#define IN_FORWARD				(1 << 3)  // Walk forward
#define IN_BACK					(1 << 4)  // Walk backwards
#define IN_USE					(1 << 5)  // Use (Defuse bomb, etc...)
#define IN_CANCEL				(1 << 6)  
#define IN_LEFT					(1 << 7)  // Walk left
#define IN_RIGHT				(1 << 8)  // Walk right
#define IN_MOVELEFT				(1 << 9)  // Alias? (not sure)
#define IN_MOVERIGHT			(1 << 10) // Alias? (not sure)
#define IN_ATTACK2				(1 << 11) // Secondary fire (Revolver, Glock change fire mode, Famas change fire mode) (not sure)
#define IN_RUN					(1 << 12)
#define IN_RELOAD				(1 << 13) // Reload weapon
#define IN_ALT1					(1 << 14) // "+alt1" in console, not really used for anything
#define IN_ALT2					(1 << 15) // "+alt2" in console, not really used for anything
#define IN_SCORE				(1 << 16) // When you open the scoreboard
#define IN_SPEED				(1 << 17) // Sprinting in other source games
#define IN_WALK					(1 << 18) // Shift
#define IN_ZOOM					(1 << 19) // Zoom weapon (not sure)
#define IN_WEAPON1				(1 << 20) // Weapon defines these bits
#define IN_WEAPON2				(1 << 21) // Weapon defines these bits
#define IN_BULLRUSH				(1 << 22) // Pulls out the knife
#define IN_GRENADE1		        (1 << 23) // Grenade 1
#define IN_GRENADE2		        (1 << 24) // Grenade 2

#define	FL_ONGROUND				(1 << 0)  // On the floor
#define FL_DUCKING				(1 << 1)  // Crouching
#define	FL_WATERJUMP			(1 << 3)  // Jumping in water
#define FL_ONTRAIN				(1 << 4) 
#define FL_INRAIN				(1 << 5)
#define FL_FROZEN				(1 << 6)  // Frozen
#define FL_ATCONTROLS			(1 << 7)
#define	FL_CLIENT				(1 << 8)
#define FL_FAKECLIENT			(1 << 9)
#define	FL_INWATER				(1 << 10) // In water
#define FL_HIDEHUD_SCOPE		(1 << 11)


struct CUserCmd 
{
    int     commandNumber;              // The command number (good for keeping track of ticks)
    int     tickCount;                  // Tick count (change for backtrack)
    std::vector<int>  viewangles;       // viewAngles (i think u know what happens here)
    //std::vector<int>  aimdirection;   // Outdated/irrelivant/don't touch it
    float   forwardmove, 
            sidemove, 
            upmove;                     // Moving in directions (450 to -450)
    int     buttons;                    // Button flags (IN_ATTACK, IN_DUCK, etc.)
    char    impulse;        
    int     weaponselect, 
            weaponsubtype;              // Held weapon slot (1 - 6) and sub-selection
    int     randomSeed;                 // Outdated (used to give the server seed but now is just 0 24/7)
    short   mousedx, 
            mousedy;                    // uselss unless ur tryna bypass facit anticheatmousedy;
    bool    hasbeenpredicted;           // don't mess with this its useful for certain prediction things
};