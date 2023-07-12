#pragma once

#include "zzEngine.h"

namespace zz
{
    struct ElementColor {
        uint32_t color;
        constexpr ElementColor(uint32_t color) : color(color) {}

        static const ElementColor Water;
        static const ElementColor Oil;
        static const ElementColor Rock;
        static const ElementColor Sand;

    };

    const ElementColor ElementColor::Water = { 0xA0376259 };
    const ElementColor ElementColor::Oil = { 0xe63D3728 };
    const ElementColor ElementColor::Rock = { 0xFF808080 }; 
    const ElementColor ElementColor::Sand = { 0xffb89e57 };


    
}
