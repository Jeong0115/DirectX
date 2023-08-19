#pragma once

#include "zzEngine.h"
#include "zzElementColor.h"

namespace zz
{
    

    enum class eElementType
    {
        EMPTY,
        SOLID,
        LIQUID,
        GAS,
    };

    enum class eElementMove
    {
        NONE                = 0b00000000,
        MOVE_DOWN_SOLID     = 0b00000001,
        MOVE_DOWN_LIQUID    = 0b00000010,
        MOVE_DOWN_SIDE      = 0b00000100,
        MOVE_UP             = 0b00001000,
        MOVE_SIDE           = 0b00010000,
    };

    enum class eElementColor : uint32_t
    {
        EMPTY = 0x00000000,
        SAND = 0xFFFFFF00,
        WATER = 0xA0376259,
        ROCK = 0xFF808080,
    };

    inline eElementMove operator|(eElementMove a, eElementMove b)
    {
        return eElementMove(int(a) | int(b));
    }

    struct Element
    {
        //Element() {}

        //Element(eElementType type, eElementMove move, uint32_t color, std::wstring name, math::Vector2 vel)
        //    : Type(type), Move(move), Color(color), Name(name), Velocity(vel)
        //{

        //}
        //
        //Element(const Element& other)
        //    : Type(other.Type), Move(other.Move), x(other.x), y(other.y), Name(other.Name)
        //    , Velocity(other.Velocity)
        //{
        //    if (other.Name == L"Sand")
        //    {
        //        Color = RandomSandColor();
        //    }
        //    else Color = other.Color;
        //}

        //Element& operator=(const Element& other)
        //{
        //    Type = other.Type;
        //    Move = other.Move;
        //    Name = other.Name;
        //    x = other.x;
        //    y = other.y;
        //    Velocity = other.Velocity;
        //    if (other.Name == L"Sand")
        //    {
        //        Color = RandomSandColor();
        //    }
        //    else Color = other.Color;
        //    return *this;
        //}

        eElementType    Type = eElementType::EMPTY;
        eElementMove    Move = eElementMove::NONE;
        uint32_t        Color = (uint32_t)eElementColor::EMPTY;
        std::wstring    Name = L"Empty";
        math::Vector2   Velocity = { 0.f, 0.f };

        uint16_t ElementFrameCount = 0;

        uint8_t MaxMove = 5;
        uint8_t CurMove = 5;

        float x = 0;
        float y = 0;
    };


    inline bool operator&(eElementMove a, eElementMove b)
    {
        return (int)a & (int)b;
    }

    Element EMPTY, SAND, WATER, ROCK;



    void InitializeElement()
    {
        EMPTY =
        {
            eElementType::EMPTY,
            eElementMove::NONE,
            (uint32_t)eElementColor::EMPTY,
            L"Empty",
            math::Vector2(0.f, 0.f)
        };

        SAND =
        {
            eElementType::SOLID,
            eElementMove::MOVE_DOWN_SOLID | eElementMove::MOVE_DOWN_SIDE,
            (uint32_t)eElementColor::SAND,
            L"Sand",
            math::Vector2(2.f, 2.f)
        };

        WATER =
        {
            eElementType::LIQUID,
            eElementMove::MOVE_DOWN_LIQUID | eElementMove::MOVE_SIDE,
            (uint32_t)eElementColor::WATER,
            L"Water",
            math::Vector2(5.f, 2.f)
        };

        ROCK =
        {
            eElementType::SOLID,
            eElementMove::NONE,
            (uint32_t)eElementColor::ROCK,
            L"Rock",
            math::Vector2(0.f, 0.f)
        };
    }
}