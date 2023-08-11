#pragma once

#include <random>
#include "zzEngine.h"

namespace zz
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(0.0, 1.0);

    __forceinline float random() { return dis(gen); }

    std::random_device i_rd;
    std::mt19937 i_gen(i_rd());

    __forceinline int randi(int max)
    {
        std::uniform_int_distribution<> distr(0, max);
        return distr(i_gen);
    }

    uint32_t emptyColor = 0x00000000;
    uint32_t whiteColor = 0xFFFFFFFF;

#define EMPTY_COLOR &emptyColor
#define WHITE_COLOR &whiteColor

    void CreateObject(class GameObject* obj, eLayerType type);
    void DeleteObject(class GameObject* obj, eLayerType type);
}