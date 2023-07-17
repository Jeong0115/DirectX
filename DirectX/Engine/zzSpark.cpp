#include "zzSpark.h"
#include "zzSmoke.h"
namespace zz
{
    Spark::Spark()
    {
        mInertialResistance = 0;
        mFrictionFactor = 1.f;
        mDensity = 4;

        mVelocity = math::Vector2(0, -124.f);
        //mass = 10;
        mDispersionRate = 4;
        mHeatResistance = 25;

        mbIgnited = true;
        //lifeSpan = getRandomInt(20);
        //temperature = 3;
    }
    Spark::~Spark()
    {
    }
    bool Spark::InteractElement(Element* target, Position targetPos, bool isFinal, bool isFirst, Position lastPos, int depth)
    {
        //boolean acted = actOnOther(neighbor, matrix);
        //if (acted) return true;
        eElementType targetType = target->GetType();

        if (targetType == eElementType::Empty) 
        {
            if (isFinal) 
            {
                SwapTarget(targetPos);
            }
            else 
            {
                return false;
            }
        }
        else if (typeid(target) == typeid(Spark)) 
        {
            return false;
        }
        else if (typeid(target) == typeid(Smoke))
        {
            target->SetDead();
            return false;
        }
        else if (targetType == eElementType::Liquid || targetType == eElementType::Solid || targetType == eElementType::Gas) 
        {
            target->ReceiveHeat(mHeatFactor);
            die();
            return true;
        }
        return false;
    }

    bool Spark::ReceiveHeat(int heat)
    {
        return false;
    }

    Element* Spark::Clone()
    {
        return new Spark();
    }
}