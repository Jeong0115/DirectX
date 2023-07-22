#pragma once

#include "tElement.h"

namespace zz
{
    class tSand : public tElement
    {
    public:
        tSand();
        virtual ~tSand();

        virtual Element* Clone() override;
        virtual bool ReceiveHeat(int heat) override;


    private:
    };
}