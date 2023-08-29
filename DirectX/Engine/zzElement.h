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
        MOVE_GAS            = 0b00100000,
    };

    enum class eElementUpdate
    {
        NONE = 0b00000000,
        DECREASE_LIFE_TIME = 0b00000001,
        HEAT_TRANSFER = 0b00000010,
    };

    enum class eElementColor : uint32_t
    {
        EMPTY   = 0x00000000,
        SAND    = 0xFFFFFF00,
        WATER   = 0xA0376259,
        ROCK    = 0xFF808080,
        WOOD    = 0xFF413F24,
        FIRE    = 0x7FFF6060,
        SMOKE   = 0x7F848485,
    };

    struct Element
    {
        eElementType    Type = eElementType::EMPTY;
        eElementMove    Move = eElementMove::NONE;
        eElementUpdate  Temp = eElementUpdate::NONE;

        uint32_t        Color = (uint32_t)eElementColor::EMPTY;
        std::wstring    Name = L"Empty";
        math::Vector2   Velocity = { 0.f, 0.f };

        uint8_t StopThreshold = 0;
        uint8_t StopCount = 0;

               
        bool isIgnite = false;
        bool onFire = false;

        float FireHP = 0.0f;

        float Temperature = 0.f;
        float MaxTemperature = 0.f;

        float LifeTime;

        float Hp = 0.0f;
        float x = 0.0f;
        float y = 0.0f;

        uint16_t ElementFrameCount = 0;
    };

    Element EMPTY, SAND, WATER, ROCK, WOOD, FIRE, SMOKE;

    inline bool operator&(eElementMove a, eElementMove b)
    {
        return (int)a & (int)b;
    }
    inline bool operator&(eElementUpdate a, eElementUpdate b)
    {
        return (int)a & (int)b;
    }
    inline eElementMove operator|(eElementMove a, eElementMove b)
    {
        return eElementMove(int(a) | int(b));
    }
    inline eElementUpdate operator|(eElementUpdate a, eElementUpdate b)
    {
        return eElementUpdate(int(a) | int(b));
    }

    void InitializeElement()
    {
        EMPTY.Type = eElementType::EMPTY;
        EMPTY.Move = eElementMove::NONE;
        EMPTY.Color = (uint32_t)eElementColor::EMPTY;
        EMPTY.Name = L"Empty";
        EMPTY.Velocity = math::Vector2(0.f, 0.f);
        EMPTY.StopThreshold = 0;
        EMPTY.StopCount = 0;
        EMPTY.isIgnite = false;
        EMPTY.onFire = false;

        SAND.Type = eElementType::SOLID;
        SAND.Move = eElementMove::MOVE_DOWN_SOLID | eElementMove::MOVE_DOWN_SIDE;
        SAND.Color = (uint32_t)eElementColor::SAND;
        SAND.Name = L"Sand";
        SAND.Velocity = math::Vector2(2.f, 2.f);
        SAND.StopThreshold = 5;
        SAND.StopCount = 5;
        SAND.isIgnite = false;
        SAND.onFire = false;

        WATER.Type = eElementType::LIQUID;
        WATER.Move = eElementMove::MOVE_DOWN_LIQUID | eElementMove::MOVE_SIDE;
        WATER.Color = (uint32_t)eElementColor::WATER;
        WATER.Name = L"Water";
        WATER.Velocity = math::Vector2(5.f, 2.f);
        WATER.StopThreshold = 10;
        WATER.StopCount = 10;
        WATER.isIgnite = false;
        WATER.onFire = false;
        WATER.FireHP = 10000.f;

        ROCK.Type = eElementType::SOLID;
        ROCK.Move = eElementMove::NONE;
        ROCK.Color = (uint32_t)eElementColor::ROCK;
        ROCK.Name = L"Rock";
        ROCK.Velocity = math::Vector2(0.f, 0.f);
        ROCK.StopThreshold = 0;
        ROCK.StopCount = 0;
        ROCK.isIgnite = false;
        ROCK.onFire = false;

        WOOD.Type = eElementType::SOLID;
        WOOD.Move = eElementMove::NONE;
        WOOD.Temp = eElementUpdate::HEAT_TRANSFER;
        WOOD.Color = (uint32_t)eElementColor::WOOD;
        WOOD.Name = L"Wood";
        WOOD.Velocity = math::Vector2(0.f, 0.f);
        WOOD.StopThreshold = 0;
        WOOD.StopCount = 0;
        WOOD.isIgnite = true;
        WOOD.onFire = false;
        WOOD.FireHP = 5.f;
        WOOD.MaxTemperature = 450.f;
        WOOD.Temperature = 0.f;

        FIRE.Type = eElementType::SOLID;
        FIRE.Move = eElementMove::NONE;
        FIRE.Temp = eElementUpdate::HEAT_TRANSFER;
        FIRE.Color = (uint32_t)eElementColor::FIRE;
        FIRE.Name = L"Fire";
        FIRE.Velocity = math::Vector2(0.f, 0.f);
        FIRE.StopThreshold = 0;
        FIRE.StopCount = 0;
        FIRE.isIgnite = true;
        FIRE.onFire = true;
        FIRE.FireHP = 0.5f;
        FIRE.Temperature = 2000.f;

        SMOKE.Type = eElementType::GAS;
        SMOKE.Move = eElementMove::MOVE_GAS;
        SMOKE.Temp = eElementUpdate::DECREASE_LIFE_TIME;
        SMOKE.Color = (uint32_t)eElementColor::SMOKE;
        SMOKE.Name = L"Smoke";
        SMOKE.Velocity = math::Vector2(2.f, 5.f);
        SMOKE.StopThreshold = 5;
        SMOKE.StopCount = 5;
        SMOKE.isIgnite = false;
        SMOKE.onFire = false;
        SMOKE.LifeTime = 1.5f;
    }


    const uint32_t Sand_0 = { 0xFFFFFF00 };
    const uint32_t Sand_1 = { 0xFFB2C906 };
    const uint32_t Sand_2 = { 0xFFE9FC5A };

    std::vector<uint32_t> SandColors = { Sand_0, Sand_1, Sand_2 };
    inline uint32_t RandomSandColor() { return SandColors[static_cast<int>(random() * SandColors.size())]; }

    const uint32_t Red      = { 0xFFFF0000 };
    const uint32_t Yellow   = { 0xFFFFFF00 };
    const uint32_t Orange   = { 0xFFFF7F00 };

    std::vector<uint32_t> FireColors = { Red, Yellow, Yellow, Orange, Orange, Orange };
    inline uint32_t RandomFireColor() { return FireColors[static_cast<int>(random() * FireColors.size())]; }
}