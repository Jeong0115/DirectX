#pragma once


#include "zzEngine.h"


namespace zz
{
    enum class eElementType : uint8_t
    {
        EMPTY,
        SOLID,
        LIQUID,
        GAS,
    };

    enum class eSolidType : uint8_t
    {
        SAND,
        DYNAMIC,
        STATIC,
        NONE,
    };

    enum class eElementID : uint8_t
    {
        EMPTY   ,
        SAND    ,
        WATER   ,
        OIL     ,
        BLOOD   ,
        LAVA    ,
        PARTICLE,
        ROCK    ,
        WOOD    ,
        FIRE    ,
        SMOKE   ,
        GRASS   ,
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
        OIL     = 0xE63D3728,
        BLOOD   = 0xAA830000,
        LAVA    = 0xFFFF6000,
        PARTICLE= 0xAA830000,
        ROCK    = 0xFF808080,
        WOOD    = 0xFF413F24,
        FIRE    = 0x00000000,//= 0x7FFF6060,
        SMOKE   = 0x7F848485,
        GRASS   = 0xFF33B828
    };

    struct Element
    {
        eElementType    Type        = eElementType::EMPTY;
        eSolidType      SolidType   = eSolidType::NONE;
        eElementID      Id          = eElementID::EMPTY;
        eElementUpdate  Temp        = eElementUpdate::NONE;

        uint32_t        Color   = (uint32_t)eElementColor::EMPTY;
        std::wstring    Name    = L"Empty";

        math::Vector2   Velocity            = { 0.f, 0.f };
        math::Vector2   RemainderVelocity   = { 0.f, 0.f };

        uint8_t StopThreshold   = 0;
        uint8_t StopCount       = 0;
               
        bool isIgnite           = false;
        bool onFire             = false;

        float Density           = 0.0f;
        float FireHP            = 0.0f;
        float Temperature       = 0.0f;
        float IgnitionPoint     = 0.0f;
        float FlashPoint        = 0.0f;
        float LifeTime          = 0.0f;
        float Hp                = 0.0f;
        float x                 = 0.0f;
        float y                 = 0.0f;

        uint16_t ElementFrameCount = 0;

        bool* destoryBody = nullptr;
    };

    Element EMPTY, SAND, WATER, OIL, BLOOD, LAVA, ROCK, WOOD, FIRE, SMOKE, GRASS, PARTICLE;

    inline bool operator&(eElementUpdate a, eElementUpdate b)
    {
        return (int)a & (int)b;
    }
    inline eElementUpdate operator|(eElementUpdate a, eElementUpdate b)
    {
        return eElementUpdate(int(a) | int(b));
    }

    void InitializeElement()
    {
        EMPTY.Type          = eElementType::EMPTY;
        EMPTY.Id            = eElementID::EMPTY;
        EMPTY.Color         = (uint32_t)eElementColor::EMPTY;
        EMPTY.Name          = L"Empty";
        EMPTY.Velocity      = math::Vector2(0.f, 0.f);
        EMPTY.StopThreshold = 0;
        EMPTY.StopCount     = 0;
        EMPTY.isIgnite      = false;
        EMPTY.onFire        = false;

        SAND.Type           = eElementType::SOLID;
        SAND.SolidType      = eSolidType::SAND;
        SAND.Id             = eElementID::SAND;
        SAND.Color          = (uint32_t)eElementColor::SAND;
        SAND.Name           = L"Sand";
        SAND.Velocity       = math::Vector2(2.f, 2.f);
        SAND.StopThreshold  = 10;
        SAND.StopCount      = 10;
        SAND.isIgnite       = false;
        SAND.onFire         = false;

        WATER.Type          = eElementType::LIQUID;
        WATER.Id            = eElementID::WATER;
        WATER.Color         = (uint32_t)eElementColor::WATER;
        WATER.Name          = L"Water";
        WATER.Velocity      = math::Vector2(0.f, 2.f);
        WATER.StopThreshold = 15;
        WATER.StopCount     = 15;
        WATER.isIgnite      = false;
        WATER.onFire        = false;
        WATER.FireHP        = 10000.f;
        WATER.Density       = 1.0f;
        WATER.Temperature = 100.f;

        OIL.Type            = eElementType::LIQUID;
        OIL.Id              = eElementID::OIL;
        OIL.Color           = (uint32_t)eElementColor::OIL;
        OIL.Temp            = eElementUpdate::HEAT_TRANSFER;
        OIL.Name            = L"Oil";
        OIL.Velocity        = math::Vector2(0.f, 2.f);
        OIL.StopThreshold   = 15;
        OIL.StopCount       = 15;
        OIL.isIgnite        = true;
        OIL.onFire          = false;
        OIL.FireHP          = 5.f;
        OIL.Density         = 0.89f;
        OIL.IgnitionPoint   = 260.f;
        OIL.Temperature     = 0.f;
        OIL.FlashPoint      = 5.f;

        BLOOD.Type          = eElementType::LIQUID;
        BLOOD.Id            = eElementID::BLOOD;
        BLOOD.Color         = (uint32_t)eElementColor::BLOOD;
        BLOOD.Name          = L"Blood";
        BLOOD.Velocity      = math::Vector2(0.f, 2.f);
        BLOOD.StopThreshold = 15;
        BLOOD.StopCount     = 15;
        BLOOD.isIgnite      = false;
        BLOOD.onFire        = false;
        BLOOD.FireHP        = 10000.f;
        BLOOD.Density       = 1.21f;
        BLOOD.Temperature   = 0.f;

        LAVA.Type           = eElementType::LIQUID;
        LAVA.Id             = eElementID::LAVA;
        LAVA.Color          = (uint32_t)eElementColor::LAVA;
        LAVA.Name           = L"LAVA";
        LAVA.Velocity       = math::Vector2(0.f, 1.f);
        LAVA.StopThreshold  = 15;
        LAVA.StopCount      = 15;
        LAVA.isIgnite       = false;
        LAVA.onFire         = false;
        LAVA.FireHP         = 10000.f;
        LAVA.Density        = 5.21f;
        LAVA.Temperature    = 0.f;

        PARTICLE.Type           = eElementType::GAS;
        PARTICLE.Id             = eElementID::PARTICLE;
        PARTICLE.Color          = (uint32_t)eElementColor::PARTICLE;
        PARTICLE.Name           = L"Blood_Particle";
        PARTICLE.Velocity       = math::Vector2(0.f, 2.f);
        PARTICLE.StopThreshold  = 20;
        PARTICLE.StopCount      = 20;
        PARTICLE.isIgnite       = false;
        PARTICLE.onFire         = false;
        PARTICLE.FireHP         = 10000.f;
        PARTICLE.Density        = 1.21f;
        PARTICLE.Temperature    = 0.f;

        GRASS.Type          = eElementType::LIQUID;
        GRASS.Id            = eElementID::GRASS;
        GRASS.Color         = (uint32_t)eElementColor::GRASS;
        GRASS.Name          = L"Grass";
        GRASS.Velocity      = math::Vector2(0.f, 2.f);
        GRASS.StopThreshold = 8;
        GRASS.StopCount     = 0;
        GRASS.isIgnite      = false;
        GRASS.onFire        = false;
        GRASS.FireHP        = 1.f;

        ROCK.Type           = eElementType::SOLID;
        ROCK.SolidType      = eSolidType::STATIC;
        ROCK.Id             = eElementID::ROCK;
        ROCK.Color          = (uint32_t)eElementColor::ROCK;
        ROCK.Name           = L"Rock";
        ROCK.Velocity       = math::Vector2(0.f, 0.f);
        ROCK.StopThreshold  = 0;
        ROCK.StopCount      = 0;
        ROCK.isIgnite       = false;
        ROCK.onFire         = false;

        WOOD.Type           = eElementType::SOLID;
        WOOD.SolidType      = eSolidType::STATIC;
        WOOD.Id             = eElementID::WOOD;
        WOOD.Temp           = eElementUpdate::HEAT_TRANSFER;
        WOOD.Color          = (uint32_t)eElementColor::WOOD;
        WOOD.Name           = L"Wood";
        WOOD.Velocity       = math::Vector2(0.f, 0.f);
        WOOD.StopThreshold  = 0;
        WOOD.StopCount      = 0;
        WOOD.isIgnite       = true;
        WOOD.onFire         = false;
        WOOD.FireHP         = 5.f;
        WOOD.IgnitionPoint  = 450.f;
        WOOD.Temperature    = 0.f;
        WOOD.FlashPoint     = 260.f;

        FIRE.Type           = eElementType::GAS;
        FIRE.Id             = eElementID::FIRE;
        FIRE.Temp           = eElementUpdate::DECREASE_LIFE_TIME | eElementUpdate::HEAT_TRANSFER;
        FIRE.Color          = (uint32_t)eElementColor::FIRE;
        FIRE.Name           = L"Fire";
        FIRE.Velocity       = math::Vector2(0.f, 0.f);
        FIRE.StopThreshold  = 0;
        FIRE.StopCount      = 0;
        FIRE.isIgnite       = true;
        FIRE.onFire         = true;
        FIRE.FireHP         = 100.0f;
        FIRE.LifeTime       = 1.0f;
        FIRE.Temperature    = 2000.f;

        SMOKE.Type          = eElementType::GAS;
        SMOKE.Id            = eElementID::SMOKE;
        SMOKE.Temp          = eElementUpdate::DECREASE_LIFE_TIME;
        SMOKE.Color         = (uint32_t)eElementColor::SMOKE;
        SMOKE.Name          = L"Smoke";
        SMOKE.Velocity      = math::Vector2(-2.f, 4.f);
        SMOKE.StopThreshold = 5;
        SMOKE.StopCount     = 5;
        SMOKE.isIgnite      = false;
        SMOKE.onFire        = false;
        SMOKE.LifeTime      = 4.0f;
    }

    const uint32_t Sand_0 = { 0xFFFFFF00 };
    const uint32_t Sand_1 = { 0xFFB2C906 };
    const uint32_t Sand_2 = { 0xFFE9FC5A };

    std::vector<uint32_t> SandColors = { Sand_0, Sand_1, Sand_2 };
    // °¡²û ÅÍÁü ¼öÁ¤ ¿ä¸Á
    inline uint32_t RandomSandColor() { return SandColors[static_cast<int>(random() * SandColors.size())]; }

    const uint32_t Red      = { 0xFFFF0000 };
    const uint32_t Yellow   = { 0xFFFFFF00 };
    const uint32_t Orange   = { 0xFFFF7F00 };

    std::vector<uint32_t> FireColors = { Red, Yellow, Yellow, Orange, Orange, Orange };

    thread_local std::mt19937 genf(std::random_device{}());
    thread_local std::uniform_int_distribution<int> disf(0, FireColors.size() - 1);

    inline uint32_t RandomFireColor() {return FireColors[disf(genf)]; }

   // inline uint32_t RandomFireColor() { return FireColors[static_cast<int>(random() * FireColors.size())]; }


    uint32_t getElementColor(eElementID id) {
        switch (id) {
        case eElementID::EMPTY:
            return (uint32_t)eElementColor::EMPTY;
        case eElementID::SAND:
            return (uint32_t)eElementColor::SAND;
        case eElementID::WATER:
            return (uint32_t)eElementColor::WATER;
        case eElementID::OIL:
            return (uint32_t)eElementColor::OIL;
        case eElementID::ROCK:
            return (uint32_t)eElementColor::ROCK;
        case eElementID::WOOD:
            return (uint32_t)eElementColor::WOOD;
        case eElementID::FIRE:
            return (uint32_t)eElementColor::FIRE;
        case eElementID::SMOKE:
            return (uint32_t)eElementColor::SMOKE;
        case eElementID::GRASS:
            return (uint32_t)eElementColor::GRASS;
        default:
            throw std::runtime_error("Invalid eElementID");
        }
    }
}