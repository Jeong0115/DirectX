#pragma once

#include "zzElement.h"

namespace zz
{
    class Liquid : public Element
    {
    public:
        Liquid();

        virtual ~Liquid();

        virtual Element* Clone() = 0;

        virtual void Update() override;
        virtual bool InteractElement(Element* target, Position targetPos, bool isFinal, bool isFirst, Position& lastPos, int depth) override;

        __forceinline int GetDensity() { return mDensity; }

    private:
        bool setElementFreeFalling(Element* element);   
        void setAroundElementFreeFalling(Position targetPos, int depth);
        bool compareDensity(Liquid* target);
        void swapLiquid(Element* target);
        int getAdditional(float val);
        float getAverageVelOrGravity(float vel, float targetVel);
        bool iterateToAdditional(int startingX, int startingY, int distance, Position lastPos);


    protected:
        int mDensity = 0;
        int mDispersionRate = 0;
    };  
}
