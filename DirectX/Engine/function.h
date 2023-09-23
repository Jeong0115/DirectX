#pragma once

#include <random>
#include "zzEngine.h"

namespace zz
{
    thread_local std::mt19937 gen(std::random_device{}());
    thread_local std::uniform_real_distribution<float> dis(0.0, 1.0);

    // [0.0f, 1.0f) 
    __forceinline float random() { return dis(gen); }

    std::random_device i_rd;
    std::mt19937 i_gen(i_rd());

    // [0, max]
    __forceinline int randi(int max)
    {
        std::uniform_int_distribution<int> distr(0, max);
        return distr(i_gen);
    }

    std::random_device f_rd;
    std::mt19937 f_gen(f_rd());

    // [0.0f, max)
    __forceinline float randf(float max)
    {
        std::uniform_real_distribution<float> distr(0, max);
        return distr(f_gen);
    }

    uint32_t emptyColor = 0x00000000;
    uint32_t whiteColor = 0xFFFFFFFF;

#define EMPTY_COLOR &emptyColor
#define WHITE_COLOR &whiteColor

    void CreateObject(class GameObject* obj, eLayerType type);
    void DeleteObject(class GameObject* obj, eLayerType type);
}