#include "zzMovableSolid.h"
#include "zzPixelGrid.h"

namespace zz
{
    MovableSolid::MovableSolid()
    {
        mStopThreshold = 5;
    }

    MovableSolid::~MovableSolid()
    {

    }

    void MovableSolid::Update()
    {
        if (mStep != PixelGrid::Step)
            return;
        mStep.flip();

        //if (this.owningBody != null) {
        //    stepAsPartOfPhysicsBody(matrix);
        //    return;
        //}

        //mVelocity += math::Vector2(0.0f, 5.0f);
        if (mbFreeFalling)
            mVelocity.x *= 0.9f;

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

            if (fabs(velXDeltaTime) > 0)
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
            Position targetPos = Position(mPos.x + (xIncrease * xModifier), mPos.y + (yIncrease * yModifier));

            Element* target = PixelGrid::GetElement(targetPos.x, targetPos.y);
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

        transferHeatToNeighbors();
        takeEffectsDamage();
        spawnSparkIfIgnited();
        //checkLifeSpan(matrix);
        checkIgnitedAndSetColor();
        
        mStopCount = IsStop(startPos) && !mbIgnited ? mStopCount + 1 : 0;
        if (mStopCount > mStopThreshold)
        {
            mStopCount = mStopThreshold;
        }

        if (!Is() || mbIgnited || mbFreeFalling)
        {
            PixelGrid::SetActiveChunks(mPos.x, mPos.y);
            PixelGrid::SetActiveChunks(startPos.x, startPos.y);
        }

    }

    bool MovableSolid::InteractElement(Element* target, Position targetPos, bool isFinal, bool isFirst, Position lastPos, int depth)
    {
        eElementType targetType = target->GetType();

        if (targetType == eElementType::Empty)
        {
            setAroundElementFreeFalling(lastPos, depth);

            if (isFinal)
            {
                mbFreeFalling = true;
                SwapTarget(targetPos);
            }
            else
            {
                return false;
            }
        }
        else if (targetType == eElementType::Liquid) 
        {
            if (depth > 0) 
            {
                mbFreeFalling = true;
                setAroundElementFreeFalling(lastPos, depth);
                SwapTarget(targetPos);
            }
            else 
            {
                mbFreeFalling = true;
                MoveLastPosAndSwapTarget(targetPos, lastPos);
                return true;
            }
        }
        else if (targetType == eElementType::Solid) 
        {
            if (depth > 0) return true;

            if (isFinal) 
            {
                MoveLastPosition(lastPos);
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

            Element* diagonalElement = PixelGrid::GetElement(mPos.x + additionalX, mPos.y + additionalY);
            if (isFirst)
                mVelocity.y = getAverageVelOrGravity(mVelocity.y, target->GetVelocity().y);
            else
                mVelocity.y = 124;

            target->SetVelocity(math::Vector2(target->GetVelocity().x, mVelocity.y));
            mVelocity.x *= mFrictionFactor * target->GetFrictionFactor();

            if (diagonalElement != nullptr)
            {
                bool stoppedDiagonally
                    = InteractElement(diagonalElement, Position(mPos.x + additionalX, mPos.y + additionalY), true, false, lastPos, depth + 1);
                
                if (!stoppedDiagonally) 
                {
                    mbFreeFalling = true;
                    return true;
                }
            }

            Element* sideElement = PixelGrid::GetElement(mPos.x + additionalX, mPos.y);
            if (sideElement != nullptr && sideElement != diagonalElement)
            {
                bool stoppedSide 
                    = InteractElement(sideElement, Position(mPos.x + additionalX, mPos.y), true, false, lastPos, depth + 1);
                
                if (stoppedSide) 
                    mVelocity.x *= -1;

                if (!stoppedSide) 
                {
                    mbFreeFalling = false;
                    return true;
                }
            }

            mbFreeFalling = false;

            MoveLastPosition(lastPos);
            return true;
        }
        else if (targetType == eElementType::Gas) 
        {
            if (isFinal) 
            {
                MoveLastPosAndSwapTarget(targetPos, lastPos);
                return true;
            }
            return false;
        }
        return false;
    }



    void MovableSolid::setAroundElementFreeFalling(Position targetPos, int depth)
    {
        if (depth > 0) return;

        Element* rightElement = PixelGrid::GetElement(targetPos.x + 1, targetPos.y);
        if (rightElement != nullptr)
        {
            if (rightElement->GetType() == eElementType::Solid)
            {
                bool isFalling = setElementFreeFalling(rightElement);
                if (isFalling)
                    PixelGrid::SetActiveChunks(targetPos.x + 1, targetPos.y);
            }
        }

        Element* leftElement = PixelGrid::GetElement(targetPos.x - 1, targetPos.y);
        if (leftElement != nullptr)
        {
            if (leftElement->GetType() == eElementType::Solid)
            {
                bool isFalling = setElementFreeFalling(leftElement);
                if (isFalling)
                    PixelGrid::SetActiveChunks(targetPos.x - 1, targetPos.y);
            }
        }
    }

    bool MovableSolid::setElementFreeFalling(Element* element)
    {
        element->SetFreeFalling(random() > element->GetInertialResistance() || element->IsFreeFalling());
        return element->IsFreeFalling();
    }

    int MovableSolid::getAdditional(float val)
    {
        if (val < -0.1f)        return (int)floor(val);
        else if (val > 0.1f)    return (int)ceil(val);
        else                    return 0;
    }

    float MovableSolid::getAverageVelOrGravity(float vel, float targetVel)
    {
        if (targetVel > 125.f) return 124.f;

        float avg = (vel + targetVel) / 2;

        if (avg < 0)
            return avg;
        else
            return max(avg, 124.f);
    }
}