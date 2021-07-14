#pragma once

struct Color 
{
    Color(int r, int g, int b, int a = 255) {
        value[0] = static_cast<unsigned char>(r);
        value[1] = static_cast<unsigned char>(g);
        value[2] = static_cast<unsigned char>(b);
        value[3] = static_cast<unsigned char>(a);
    }
    Color(const std::array<int, 3> rgb) : Color(rgb[0], rgb[1], rgb[2], 255) {}

    unsigned char value[4];
};

typedef std::array<int, 3> color;
constexpr color red   = {255, 0, 0};
constexpr color green = {0, 255, 0};
constexpr color blue  = {0, 0, 255};
constexpr color cyan  = {224, 255, 255};
constexpr color pink  = {255, 192, 203};
constexpr color white = {255, 255, 255};
constexpr color black = {0, 0, 0};