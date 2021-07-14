#pragma once

#include "utils.h"
#include "color.h"

struct ICvar 
{
	// Prints to console (colors)
	template <typename... Values> 
	void ConsoleColorPrintf(const Color& msgcolor, const char* format, Values... parameters) 
	{
		GetVFunction <void(__cdecl*)(ICvar*, const Color&, const char*, ...)> (this, 23)(this, msgcolor, format, parameters...);
	}

	// Prints to console
	template <typename... Values> 
	void ConsoleDPrintf(const char* format, Values... parameters) 
	{
		GetVFunction<void(__cdecl*)(ICvar*, const char*, ...)> (this, 24)(this, format, parameters...);
	}

};

