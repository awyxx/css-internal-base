#pragma once

#include "utils.h"

typedef unsigned long VPANEL;

struct IPanel
{
	// Gets name of panel
	const char* GetName(VPANEL vpanel)
	{
		return GetVFunction <const char*(__thiscall*)(IPanel*, VPANEL)> (this, 36)(this, vpanel);
	}
};
