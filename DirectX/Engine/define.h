#pragma once

#define SINGLETON(Class)\
    static Class& GetInst()\
        {\
            static Class instance;\
            return instance;\
        }

#define PI 3.14159265359