#pragma once

#include <random>

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<float> dis(0.0, 1.0);

__forceinline float random() { return dis(gen); }
 
uint32_t emptyColor = 0x00000000;
uint32_t whiteColor = 0xFFFFFFFF;

#define EMPTY_COLOR &emptyColor
#define WHITE_COLOR &whiteColor