#pragma once

#include "utils.h"

struct IClientEntityList 
{
	// 
	void* GetClientEntity(int ent) { 
		// Dont use this, use sigscan one!
		return GetVFunction<void*(__thiscall*)(IClientEntityList*, int)>(this, 3)(this, ent);
	}
	
	//?
	void* GetClientEntityFromHandle(void* hEnt) { 
		return GetVFunction<void*(__thiscall*)(IClientEntityList*, void*)>(this, 4)(this, hEnt);
	}
	
	// Returns number of entities currently in use
	int NumberOfEntities(bool bIncludeNonNetworkable) { 
		return GetVFunction<int(__thiscall*)(IClientEntityList*, bool)>(this, 5)(this, bIncludeNonNetworkable);
	}
	
	// Returns highest index actually used
	int GetHighestEntityIndex(void) {
		return GetVFunction<int(__thiscall*)(IClientEntityList*)>(this, 6)(this); 
	}
	
	// Sizes entity list to specified size
	void SetMaxEntities(int maxents) { 
		GetVFunction<void(__thiscall*)(IClientEntityList*, int)>(this, 7 )(this, maxents); 
	}

	// Returns max size of entities
	int GetMaxEntities(void) { 
		return GetVFunction<int(__thiscall*)(IClientEntityList*)>(this, 8)(this);
	} 
};