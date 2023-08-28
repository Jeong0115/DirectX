#pragma once

#define SINGLETON(Class)\
    static Class& GetInst()\
        {\
            static Class instance;\
            return instance;\
        }

#define PI 3.14159265359

#define PIXEL_WORLD_Z 0.05f
#define FRONT_PIXEL_WORLD_Z     (PIXEL_WORLD_Z - 0.01f)
#define BACK_PIXEL_WORLD_Z      (PIXEL_WORLD_Z + 0.01f)