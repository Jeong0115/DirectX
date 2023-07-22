#include "zzLiquid.h"
#include "zzPixelWorld.h"

namespace zz
{
    Liquid::Liquid()
    {
        mType = eElementType::Liquid;
        mVelocity = math::Vector2(0, 124.f);
        mStopThreshold = 10;
    }
    Liquid::~Liquid()
    {
    }

    
    void Liquid::Update()
    {
       
        //if (mStep != PixelGrid::Step) 
        //    return;
        //mStep.flip();

        //mVelocity += math::Vector2(0.0f, 5.0f);
        if (mbFreeFalling)
        {
            mVelocity.x *= 0.4f;
        }

        int yModifier = mVelocity.y < 0 ? -1 : 1;
        int xModifier = mVelocity.x < 0 ? -1 : 1;

        float velYDeltaTimeFloat = (fabs(mVelocity.y) * 1 / 60);
        float velXDeltaTimeFloat = (fabs(mVelocity.x) * 1 / 60);

        int velXDeltaTime;
        int velYDeltaTime;

        if (velXDeltaTimeFloat < 1) 
        {
            mVelocityRemainder.x += velXDeltaTimeFloat;
            velXDeltaTime = (int)mVelocityRemainder.x;

            if (abs(velXDeltaTime) > 0) 
                mVelocityRemainder.x = 0;
        }
        else 
        {
            mVelocityRemainder.x = 0;
            velXDeltaTime = (int)velXDeltaTimeFloat;
        }

        if (velYDeltaTimeFloat < 1) 
        {
            mVelocityRemainder.y += velYDeltaTimeFloat;
            velYDeltaTime = (int)mVelocityRemainder.y;

            if (abs(velYDeltaTime) > 0) 
                mVelocityRemainder.y = 0;
        }
        else
        {
            mVelocityRemainder.y = 0;
            velYDeltaTime = (int)velYDeltaTimeFloat;
        }

        bool isXDeltaTimeLarger = abs(velXDeltaTime) > abs(velYDeltaTime);

        int largeDelatTime = max(abs(velXDeltaTime), abs(velYDeltaTime));
        int smallDeltaTime = min(abs(velXDeltaTime), abs(velYDeltaTime));

        float slope = (smallDeltaTime == 0 || largeDelatTime == 0) ? 0 : ((float)(smallDeltaTime + 1) / (largeDelatTime + 1));

        int smallerCount;

        Position startPos = mPos;
        Position lastPos = mPos;

        for (int i = 1; i <= largeDelatTime; i++) 
        {
            smallerCount = (int)floor(i * slope);

            int yIncrease, xIncrease;
            if (isXDeltaTimeLarger) 
            {
                xIncrease = i;
                yIncrease = smallerCount;
            }
            else 
            {
                yIncrease = i;
                xIncrease = smallerCount;
            }

            // 어떻게 할까 수정해야됨
            Position targetPos = Position(mPos.x + (xIncrease * xModifier), mPos.y + (yIncrease * yModifier));

            Element* target = PixelWorld::GetElement(targetPos.x, targetPos.y);
            if (target != nullptr)
            {
                if (target == this) continue;

                bool stopped = InteractElement(target, targetPos, i == largeDelatTime, i == 1, lastPos, 0);
                
                if (stopped) break;
                    
                lastPos = targetPos;
            }
            else 
            {
                die();
                return;
            }
            
        }

        //applyHeatToNeighborsIfIgnited(matrix);
            //modifyColor();
            //spawnSparkIfIgnited(matrix);
            //checkLifeSpan(matrix);
            //takeEffectsDamage(matrix);
        mStopCount = IsStop(startPos) ? mStopCount + 1 : 0;
        if (mStopCount > mStopThreshold)
        {
            mStopCount = mStopThreshold;
        }

        ////if (mStopCount != mStopThreshold)
        ////{
        //    if(!Is() || mbIgnited) 
        //    {
        //        PixelGrid::SetActiveChunks(mPos.x, mPos.y);
        //        PixelGrid::SetActiveChunks(startPos.x, startPos.y);
        //    }
        //}
        //if (matrix.useChunks) {
        //    if (isIgnited || !hasNotMovedBeyondThreshold()) {
        //        matrix.reportToChunkActive(this);
        //        matrix.reportToChunkActive((int)formerLocation.x, (int)formerLocation.y);
        //    }
        //}
    }

    bool Liquid::InteractElement(Element* target, Position targetPos, bool isFinal, bool isFirst, Position lastPos, int depth)
    {
        //boolean acted = actOnOther(neighbor, matrix);
        //if (acted) return true    

        eElementType targetType = target->GetType();

        if (targetType == eElementType::Empty || targetType == eElementType::Particle)
        {
            setAroundElementFreeFalling(lastPos, depth);

            if (isFinal)
            {
                mbFreeFalling = true;
                //SwapTarget(targetPos);
                PixelWorld::SwapElement(mPos.x, mPos.y, targetPos.x, targetPos.y);
                //SwapElement(target); // 나중에 다시 보자
                return true;
            }
            else
            {
                return false;
            }
        }
        else if (targetType == eElementType::Liquid)
        {
            if (compareDensity((Liquid*)target))
            {
                if (isFinal) 
                {
                    swapLiquid(targetPos, lastPos);
                    return true;
                }
                else 
                {
                    lastPos = targetPos;
                    return false;
                }
            }

            if (depth > 0)
                return true;

            if (isFinal) 
            {
                PixelWorld::SwapElement(mPos.x, mPos.y, lastPos.x, lastPos.y);
                return true;
            }

            if (mbFreeFalling) 
            {
                float absY = max(fabs(mVelocity.y) / 31, 105);
                mVelocity.x = mVelocity.x < 0 ? -absY : absY; 
            }

            math::Vector2 copyVelocity = mVelocity;
            copyVelocity.Normalize();
            math::Vector2 normalizedVel = copyVelocity;

            int additionalX = getAdditional(normalizedVel.x);
            int additionalY = getAdditional(normalizedVel.y);

            int distance = additionalX * (random() > 0.5 ? mDispersionRate + 2 : mDispersionRate - 1);

            Element* diagonalElement = PixelWorld::GetElement(mPos.x + additionalX, mPos.y + additionalY); 
            
            if (isFirst) 
                mVelocity.y = getAverageVelOrGravity(mVelocity.y, target->GetVelocity().y);
            else 
                mVelocity.y = 124; // 나중에 변수로

            target->SetVelocity(math::Vector2(target->GetVelocity().x, mVelocity.y));
            mVelocity.x *= mFrictionFactor;

            if (diagonalElement != nullptr) 
            {
                bool stoppedDiagonally 
                    = iterateToAdditional(mPos.x + additionalX, mPos.y + additionalY, distance, lastPos);
                if (!stoppedDiagonally) 
                {
                    mbFreeFalling = true;
                    return true;
                }
            }

            Element* sideElement = PixelWorld::GetElement(mPos.x + additionalX, mPos.y);
            if (sideElement != nullptr && sideElement != diagonalElement) 
            {
                bool stoppedSide 
                    = iterateToAdditional(mPos.x + additionalX, mPos.y, distance, lastPos);
                if (stoppedSide) mVelocity.x *= -1;

                if (!stoppedSide) {
                    mbFreeFalling = false;
                    return true;
                }
            }

            mbFreeFalling = false;

            PixelWorld::SwapElement(mPos.x, mPos.y, lastPos.x, lastPos.y);
            return true;
            //moveToLastValid(matrix, lastValidLocation);
        }
        else if (targetType == eElementType::Solid)
        {
            if (depth > 0)
                return true;

            if (isFinal) 
                PixelWorld::SwapElement(mPos.x, mPos.y, lastPos.x, lastPos.y);
                return true;
            
            if (mbFreeFalling) 
            {
                float absY = max(fabs(mVelocity.y) / 31, 105);
                mVelocity.x = mVelocity.x < 0 ? -absY : +absY;
            }

            math::Vector2 copyVelocity = mVelocity;
            copyVelocity.Normalize();
            math::Vector2 normalizedVel = copyVelocity;

            int additionalX = getAdditional(normalizedVel.x);
            int additionalY = getAdditional(normalizedVel.y);

            int distance = additionalX * (random() > 0.5 ? mDispersionRate + 2 : mDispersionRate - 1);

            Element* diagonalElement = PixelWorld::GetElement(mPos.x + additionalX, mPos.y + additionalY); // mPos로 할껀지, lastPos로 할껀지 확인

            if (isFirst)
                mVelocity.y = getAverageVelOrGravity(mVelocity.y, target->GetVelocity().y);
            else
                mVelocity.y = 124; // 나중에 변수로

            target->SetVelocity(math::Vector2(target->GetVelocity().x, mVelocity.y));
            mVelocity.x *= mFrictionFactor;

            if (diagonalElement != nullptr)
            {
                bool stoppedDiagonally
                    = iterateToAdditional(mPos.x + additionalX, mPos.y + additionalY, distance, lastPos);
                if (!stoppedDiagonally)
                {
                    mbFreeFalling = true;
                    return true;
                }
            }

            Element* sideElement = PixelWorld::GetElement(mPos.x + additionalX, mPos.y);
            if (sideElement != nullptr && sideElement != diagonalElement)
            {
                bool stoppedSide
                    = iterateToAdditional(mPos.x + additionalX, mPos.y, distance, lastPos);
                if (stoppedSide) mVelocity.x *= -1;

                if (!stoppedSide) {
                    mbFreeFalling = false;
                    return true;
                }
            }

            mbFreeFalling = false;

            PixelWorld::SwapElement(mPos.x, mPos.y, lastPos.x, lastPos.y);
            return true;
        }
        else if (target->GetType() == eElementType::Gas) 
        {
            if (isFinal) 
            {
                PixelWorld::SwapElement(mPos.x, mPos.y, lastPos.x, lastPos.y);
                return true;
            }
            return false;
        }

        return false;
    }


    void Liquid::setAroundElementFreeFalling(Position targetPos, int depth) 
    {
        if (depth > 0) return;

        if (targetPos.x < 1 || targetPos.y < 0) return;

        Element* rightElement = PixelWorld::GetElement(targetPos.x + 1, targetPos.y);
        if(rightElement != nullptr) 
        {
            if (rightElement->GetType() == eElementType::Solid)
            {
                bool isFalling = setElementFreeFalling(rightElement);
               /* if (isFalling)
                    PixelGrid::SetActiveChunks(targetPos.x + 1, targetPos.y);*/
            }
        }

        Element* leftElement = PixelWorld::GetElement(targetPos.x - 1, targetPos.y);
        if (leftElement != nullptr)
        {
            if (leftElement->GetType() == eElementType::Solid)
            {
                bool isFalling = setElementFreeFalling(leftElement);
                //if (isFalling)
                //    PixelGrid::SetActiveChunks(targetPos.x - 1, targetPos.y);
            }
        }
    }

    bool Liquid::setElementFreeFalling(Element* element)
    {
        element->SetFreeFalling(random() > element->GetInertialResistance() || element->IsFreeFalling());
        return  element->IsFreeFalling();
    }

    bool Liquid::compareDensity(Liquid* target)
    {
        return ((mDensity > target->GetDensity()) && (target->GetPos().y >= mPos.y));
    }

    void Liquid::swapLiquid(Position targetPos, Position lastPos)  // 이것도 수정해야됨
    {
        mVelocity.y = 62;
        if (random() > 0.8f) 
            mVelocity.x *= -1;
        
        PixelWorld::SwapElement(mPos.x, mPos.y, targetPos.x, targetPos.y);
        PixelWorld::SwapElement(lastPos.x, lastPos.y, targetPos.x, targetPos.y);
        //SwapElement(target);
    }

    int Liquid::getAdditional(float val) 
    {
        if (val < -0.1f)        return (int)floor(val);
        else if (val > 0.1f)    return (int)ceil(val);
        else                    return 0;
    }

    float Liquid::getAverageVelOrGravity(float vel, float targetVel) 
    {
        if (targetVel > 125.f) return 124.f;

        float avg = (vel + targetVel) / 2;

        if (avg < 0)
            return avg;
        else 
            return max(avg, 124.f);
    }


    bool Liquid::iterateToAdditional(int startingX, int startingY, int distance, Position lastPos) {
        {
            int distanceModifier = distance > 0 ? 1 : -1;
            Position lastValidLocation = lastPos;

            for (int i = 0; i <= abs(distance); i++)
            {
                int modifiedX = startingX + i * distanceModifier;

                Element* target = PixelWorld::GetElement(modifiedX, startingY);
                if (target == nullptr) 
                    return true;

                //boolean acted = actOnOther(neighbor, matrix);   이거 위치도 한번 확인
                //if (acted) return false;

                bool isFirst = i == 0;
                bool isFinal = i == abs(distance);

                if (target->GetType() == eElementType::Empty)
                {
                    if (isFinal)
                    {
                        PixelWorld::SwapElement(mPos.x, mPos.y, modifiedX, startingY);
                        return false;
                    }
                    lastValidLocation.x = modifiedX;
                    lastValidLocation.y = startingY;
                }
                else if (target->GetType() == eElementType::Liquid)
                {
                    Liquid* liquidNeighbor = (Liquid*)target;
                    if (isFinal)
                    {
                        if (compareDensity(liquidNeighbor))
                        {
                            swapLiquid(liquidNeighbor->GetPos(), lastPos);
                            return false;
                        }
                    }
                }
                else if (target->GetType() == eElementType::Solid)
                {
                    if (isFirst)
                    {
                        return true;
                    }
                    PixelWorld::SwapElement(mPos.x, mPos.y, lastValidLocation.x, lastValidLocation.y);
                    return false;
                }
            }
            return true;
        }
    }
}
