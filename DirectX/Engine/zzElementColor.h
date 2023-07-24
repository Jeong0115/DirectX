#pragma once

#include "zzEngine.h"

namespace zz
{
   /* struct ElementColor 
    {
        uint32_t color;
        const ElementColor(uint32_t color) : color(color) {}

        static const ElementColor Water;
        static const ElementColor Oil;
        static const ElementColor Rock;
        static const ElementColor Empty;

        static const ElementColor Sand_0;
        static const ElementColor Sand_1;
        static const ElementColor Sand_2;

        static const ElementColor Red;
        static const ElementColor Yellow;
        static const ElementColor Orange;

        static std::vector<ElementColor> SandColors;
        static std::vector<ElementColor> FireColors;

        static inline const ElementColor RandomSandColor();
        static inline const ElementColor RandomFireColor();

    };*/

  /*  const ElementColor ElementColor::Water  = { 0xA0376259 };
    const ElementColor ElementColor::Oil    = { 0xE63D3728 };
    const ElementColor ElementColor::Rock   = { 0xFF808080 };
    const ElementColor ElementColor::Empty  = { 0x00000000 };

    const ElementColor ElementColor::Sand_0 = { 0xFFFFFF00 };
    const ElementColor ElementColor::Sand_1 = { 0xFFB2C906 };
    const ElementColor ElementColor::Sand_2 = { 0xFFE9FC5A };

    const ElementColor ElementColor::Red    = { 0xFFFF0000 };
    const ElementColor ElementColor::Yellow = { 0xFFFFFF00 };
    const ElementColor ElementColor::Orange = { 0xFFFF7F00 };

    std::vector<ElementColor> ElementColor::SandColors = { Sand_0, Sand_1, Sand_2 };
    std::vector<ElementColor> ElementColor::FireColors = { Red, Yellow, Yellow, Orange, Orange, Orange };

    inline const ElementColor ElementColor::RandomSandColor() { return SandColors[static_cast<int>(random() * SandColors.size())]; }
    inline const ElementColor ElementColor::RandomFireColor() { return FireColors[static_cast<int>(random() * FireColors.size())]; }*/


    uint32_t Sand_0 = { 0xFFFFFF00 };
    uint32_t Sand_1 = { 0xFFB2C906 };
    uint32_t Sand_2 = { 0xFFE9FC5A };

    std::vector<uint32_t> SandColors = { Sand_0, Sand_1, Sand_2 };
    inline uint32_t RandomSandColor() { return SandColors[static_cast<int>(random() * SandColors.size())]; }
}
