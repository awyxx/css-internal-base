#pragma once

#include "utils.h"

// https://github.com/VSES/SourceEngine2007/blob/master/src_main/vguimatsurface/MatSystemSurface.cpp
// https://prnt.sc/v1ygrk

enum FontFeature
{
    FONT_FEATURE_ANTIALIASED_FONTS = 1,
    FONT_FEATURE_DROPSHADOW_FONTS  = 2,
    FONT_FEATURE_OUTLINE_FONTS     = 6,
};

enum FontDrawType
{
    FONT_DRAW_DEFAULT = 0,
    FONT_DRAW_NONADDITIVE,
    FONT_DRAW_ADDITIVE,
    FONT_DRAW_TYPE_COUNT = 2,
};

enum FontFlags
{
    FONTFLAG_NONE,
    FONTFLAG_ITALIC       = 0x001,
    FONTFLAG_UNDERLINE    = 0x002,
    FONTFLAG_STRIKEOUT    = 0x004,
    FONTFLAG_SYMBOL       = 0x008,
    FONTFLAG_ANTIALIAS    = 0x010,
    FONTFLAG_GAUSSIANBLUR = 0x020,
    FONTFLAG_ROTARY       = 0x040,
    FONTFLAG_DROPSHADOW   = 0x080,
    FONTFLAG_ADDITIVE     = 0x100,
    FONTFLAG_OUTLINE      = 0x200,
    FONTFLAG_CUSTOM       = 0x400,
    FONTFLAG_BITMAP       = 0x800,
};

typedef unsigned long HFont;

struct ISurface 
{
    void DrawSetColor(const Color color)             { GetVFunction<void(__thiscall*)(ISurface*, const Color)>(this, 10)(this, color);              } // 0xA (10)
    void DrawSetColorRGB(int r, int g, int b, int a) { GetVFunction<void(__thiscall*)(ISurface*, int, int, int, int)>(this, 11)(this, r, g, b, a);  } // 0xB (11)
    
    void DrawFilledRect(int x, int y, int w, int h)       { GetVFunction<void(__thiscall*)(ISurface*, int, int, int, int)>(this, 12)(this, x, y, w, h); } // 0xC (12)
    void DrawFilledRectArray(int* pRects, int numRects)   { GetVFunction<void(__thiscall*)(ISurface*, int*, int)>(this, 13)(this, pRects, numRects);    } // 0xD (13)
    void DrawOutlinedRect(int x, int y, int w, int h)     { GetVFunction<void(__thiscall*)(ISurface*, int, int, int, int)>(this, 14)(this, x, y, w, h); } // 0xE (14)

    void DrawLine(int x, int y, int w, int h)          { GetVFunction<void(__thiscall*)(ISurface*, int, int, int, int)>(this, 15)(this, x, y, w, h);     } // 0xF (15)
    void DrawPolyLine(int* px, int* py, int numPoints) { GetVFunction<void(__thiscall*)(ISurface*, int*, int*, int)>(this, 16)(this, px, py, numPoints); } // 0x10 (16)

    void DrawSetTextFont(HFont font)                     { GetVFunction<void(__thiscall*)(ISurface*, HFont)>(this, 17)(this, font);                         } // 0x11 (17)
    void DrawSetTextColor(const Color color)             { GetVFunction<void(__thiscall*)(ISurface*, const Color)>(this, 18)(this, color);                  } // 0x12 (18)
    void DrawSetTextColorRGB(int r, int g, int b, int a) { GetVFunction<void(__thiscall*)(ISurface*, int, int, int, int)>(this, 19)(this, r, g, b, a);      } // 0x13 (19)
    void DrawSetTextPos(int x, int y)                    { GetVFunction<void(__thiscall*)(ISurface*, int, int)>(this, 20)(this, x, y);                      } // 0x14 (20)
    void DrawGetTextPos(int& x, int& y)                  { GetVFunction<void(__thiscall*)(ISurface*, int&, int&)>(this, 21)(this, x, y);                    } // 0x15 (21)
    void DrawPrintText(wchar_t* t, int len, FontDrawType type) { GetVFunction<void(__thiscall*)(ISurface*, wchar_t*, int, FontDrawType)>(this, 22)(this, t, len, type); } // 0x16 (22)
};

