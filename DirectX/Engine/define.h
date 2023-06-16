#pragma once

#define SINGLETON(Class)\
    static Class& GetInst()\
        {\
            static Class instance;\
            return instance;\
        }