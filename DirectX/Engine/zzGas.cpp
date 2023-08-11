//#include "zzGas.h"
//#include "zzPixelGrid.h"
//
//namespace zz
//{
//    Gas::Gas()
//        : mDensity(0)
//        , mDispersionRate(1)
//    {
//        mType = eElementType::Gas;
//    }
//    Gas::~Gas()
//    {
//    }
//    void Gas::Update()
//    {
//        if (mStep != PixelGrid::Step)
//            return;
//        mStep.flip();
//        //vel.sub(CellularAutomaton.gravity);
//
//        mVelocity.y = max(mVelocity.y, -124);
//
//        if (mVelocity.y == -124 && random() > 0.7f) 
//        {
//            mVelocity.y = -64;
//        }
//
//        mVelocity.x *= 0.9f;
//
//        int yModifier = mVelocity.y < 0 ? -1 : 1;
//        int xModifier = mVelocity.x < 0 ? -1 : 1;
//
//        float velYDeltaTimeFloat = (fabs(mVelocity.y) * 1 / 60);
//        float velXDeltaTimeFloat = (fabs(mVelocity.x) * 1 / 60);
//
//        int velXDeltaTime;
//        int velYDeltaTime;
//
//        if (velXDeltaTimeFloat < 1.f) 
//        {
//            mVelocityRemainder.x += velXDeltaTimeFloat;
//            velXDeltaTime = (int)mVelocityRemainder.x;
//
//            if (abs(velXDeltaTime) > 0) 
//            {
//                mVelocityRemainder.x = 0.f;
//            }
//        }
//        else 
//        {
//            mVelocityRemainder.x = 0.f;
//            velXDeltaTime = (int)velXDeltaTimeFloat;
//        }
//
//        if (velYDeltaTimeFloat < 1.f)
//        {
//            mVelocityRemainder.x += velYDeltaTimeFloat;
//            velYDeltaTime = (int)mVelocityRemainder.y;
//
//            if (abs(velYDeltaTime) > 0) 
//            {
//                mVelocityRemainder.y = 0.f;
//            }
//        }
//        else 
//        {
//            mVelocityRemainder.y = 0.f;
//            velYDeltaTime = (int)velYDeltaTimeFloat;
//        }
//
//        bool isXDeltaTimeLarger = abs(velXDeltaTime) > abs(velYDeltaTime);
//
//        int largeDelatTime = max(abs(velXDeltaTime), abs(velYDeltaTime));
//        int smallDeltaTime = min(abs(velXDeltaTime), abs(velYDeltaTime));
//
//        float slope = (smallDeltaTime == 0 || largeDelatTime == 0) ? 0 : ((float)(smallDeltaTime + 1) / (largeDelatTime + 1));
//
//        int smallerCount;
//        
//        Position startPos = mPos;
//        Position lastPos = mPos;
//
//        for (int i = 1; i <= largeDelatTime; i++)
//        {
//            smallerCount = (int)floor(i * slope);
//
//            int yIncrease, xIncrease;
//            if (isXDeltaTimeLarger)
//            {
//                xIncrease = i;
//                yIncrease = smallerCount;
//            }
//            else
//            {
//                yIncrease = i;
//                xIncrease = smallerCount;
//            }
//
//            Position targetPos = Position(mPos.x + (xIncrease * xModifier), mPos.y + (yIncrease * yModifier));
//
//            Element* target = PixelGrid::GetElement(targetPos.x, targetPos.y);
//            if (target != nullptr)
//            {
//                if (target == this) continue;
//
//                bool stopped = InteractElement(target, targetPos, i == largeDelatTime, i == 1, lastPos, 0);
//
//                if (stopped) break;
//
//                lastPos = targetPos;
//            }
//            else
//            {
//                die();
//                return;
//            }
//        }
//
//        transferHeatToNeighbors();
//        //checkIgnitedAndSetColor();
//        spawnSparkIfIgnited();
//        //checkLifeSpan(matrix);
//        takeEffectsDamage();
//
//        //if (matrix.useChunks) {
//        //    if (isIgnited) {
//        //        matrix.reportToChunkActive(this);
//        //    }
//        //}
//
//        if (1/*mbIgnited*/)
//        {
//            PixelGrid::SetActiveChunks(mPos.x, mPos.y);
//        }
//    }
//    bool Gas::InteractElement(Element* target, Position targetPos, bool isFinal, bool isFirst, Position lastPos, int depth)
//    {
//        //boolean acted = actOnOther(neighbor, matrix);
//        //if (acted) return true;
//
//        eElementType targetType = target->GetType();
//
//        if (targetType == eElementType::Empty || targetType == eElementType::TempParticle)
//        {
//            if (isFinal) 
//            {
//                SwapTarget(targetPos);
//            }
//            else 
//            {
//                return false;
//            }
//        }
//        else if (targetType == eElementType::Gas)
//        {
//            Gas* gasElement = dynamic_cast<Gas*>(target); // static_cast로 사용할지 고려
//            if (compareDensity(gasElement))
//            {
//                swapGas(targetPos, lastPos);
//                return false;
//            }
//            if (depth > 0) 
//            {
//                return true;
//            }
//            if (isFinal) 
//            {
//                MoveLastPosition(lastPos);
//                return true;
//            }
//
//            math::Vector2 copyVelocity = mVelocity;
//            copyVelocity.Normalize();
//            math::Vector2 normalizedVel = copyVelocity;
//
//            int additionalX = getAdditional(normalizedVel.x);
//            int additionalY = getAdditional(normalizedVel.y);
//
//            int distance = additionalX * (random() > 0.5 ? mDispersionRate + 2 : mDispersionRate - 1);
//
//            Element* diagonalElement = PixelGrid::GetElement(mPos.x + additionalX, mPos.y + additionalY);
//
//            if (isFirst) 
//            {
//                mVelocity.y = getAverageVelOrGravity(mVelocity.y, target->GetVelocity().y);
//            }
//            else 
//            {
//                mVelocity.y = -124.f;
//            }
//
//            target->SetVelocity(math::Vector2(target->GetVelocity().x, mVelocity.y));
//            mVelocity.x *= mFrictionFactor;
//
//            if (diagonalElement != nullptr) 
//            {
//                bool stoppedDiagonally 
//                    = iterateToAdditional(mPos.x + additionalX, mPos.y, distance);
//
//                if (!stoppedDiagonally) 
//                {
//                    return true;
//                }
//            }
//
//            Element* sideElement = PixelGrid::GetElement(mPos.x + additionalX, mPos.y);
//
//            if (sideElement != nullptr && sideElement != diagonalElement) 
//            {
//                bool stoppedSide 
//                    = iterateToAdditional(mPos.x + additionalX, mPos.y, distance);
//
//                if (stoppedSide) 
//                {
//                    mVelocity.x *= -1;
//                }
//                if (!stoppedSide) 
//                {
//                    return true;
//                }
//            }
//
//            MoveLastPosition(lastPos);
//            return true;
//        }
//        else if (targetType == eElementType::Liquid) 
//        {
//            if (depth > 0) 
//            {
//                return true;
//            }
//            if (isFinal) 
//            {
//                MoveLastPosition(lastPos);
//                return true;
//            }
//            if (target->IsFreeFalling()) 
//            {
//                return true;
//            }
//
//            float absY = max(fabs(mVelocity.y) / 31, 105);
//            mVelocity.x = mVelocity.x < 0 ? -absY : absY;
//
//            math::Vector2 copyVelocity = mVelocity;
//            copyVelocity.Normalize();
//            math::Vector2 normalizedVel = copyVelocity;
//
//            int additionalX = getAdditional(normalizedVel.x);
//            int additionalY = getAdditional(normalizedVel.y);
//
//            int distance = additionalX * (random() > 0.5 ? mDispersionRate + 2 : mDispersionRate - 1);
//
//            Element* diagonalElement = PixelGrid::GetElement(mPos.x + additionalX, mPos.y + additionalY);
//            if (isFirst) 
//            {
//                mVelocity.y = getAverageVelOrGravity(mVelocity.y, target->GetVelocity().y);
//            }
//            else {
//                mVelocity.y = -124.f;
//            }
//
//            target->SetVelocity(math::Vector2(target->GetVelocity().x, mVelocity.y));
//            mVelocity.x *= mFrictionFactor;
//
//            if (diagonalElement != nullptr) 
//            {
//                bool stoppedDiagonally 
//                    = iterateToAdditional(mPos.x + additionalX, mPos.y + additionalY, distance);
//
//                if (!stoppedDiagonally) 
//                {
//                    return true;
//                }
//            }
//
//            Element* sideElement = PixelGrid::GetElement(mPos.x + additionalX, mPos.y);
//
//            if (sideElement != nullptr && sideElement != diagonalElement) 
//            {
//                bool stoppedSide 
//                    = iterateToAdditional(mPos.x + additionalX, mPos.y, distance);
//
//                if (stoppedSide) 
//                {
//                    mVelocity.x *= -1;
//                }
//
//                if (!stoppedSide) 
//                {
//                    return true;
//                }
//            }
//
//            MoveLastPosition(lastPos);
//            return true;
//        }
//        else if (targetType == eElementType::Solid) 
//        {
//            if (depth > 0) 
//            {
//                return true;
//            }
//            if (isFinal) 
//            {
//                MoveLastPosition(lastPos);
//                return true;
//            }
//            if (target->IsFreeFalling()) 
//            {
//                return true;
//            }
//
//            float absY = max(fabs(mVelocity.y) / 31, 105);
//            mVelocity.x = mVelocity.x < 0 ? -absY : absY;
//
//            math::Vector2 copyVelocity = mVelocity;
//            copyVelocity.Normalize();
//            math::Vector2 normalizedVel = copyVelocity;
//
//            int additionalX = getAdditional(normalizedVel.x);
//            int additionalY = getAdditional(normalizedVel.y);
//
//            int distance = additionalX * (random() > 0.5 ? mDispersionRate + 2 : mDispersionRate - 1);
//
//            Element* diagonalElement = PixelGrid::GetElement(mPos.x + additionalX, mPos.y + additionalY);
//            if (isFirst) 
//            {
//                mVelocity.y = getAverageVelOrGravity(mVelocity.y, target->GetVelocity().y);
//            }
//            else 
//            {
//                mVelocity.y = -124.f;
//            }
//
//            target->SetVelocity(math::Vector2(target->GetVelocity().x, mVelocity.y));
//            mVelocity.x *= mFrictionFactor;
//
//            if (diagonalElement != nullptr) 
//            {
//                bool stoppedDiagonally 
//                    = iterateToAdditional(mPos.x + additionalX, mPos.y + additionalY, distance);
//
//                if (!stoppedDiagonally) 
//                {
//                    return true;
//                }
//            }
//
//            Element* sideElement = PixelGrid::GetElement(mPos.x + additionalX, mPos.y);
//            if (sideElement != nullptr)
//            {
//                bool stoppedSide 
//                    = iterateToAdditional(mPos.x + additionalX, mPos.y, distance);
//
//                if (stoppedSide) 
//                {
//                    mVelocity.x *= -1;
//                }
//                if (!stoppedSide) 
//                {
//                    return true;
//                }
//            }
//
//            MoveLastPosition(lastPos);
//            return true;
//        }
//        return false;
//    }
//
//    bool Gas::compareDensity(Gas* gasElement)
//    {
//        return (mDensity > gasElement->GetDensity() && (gasElement->GetPos().y <= mPos.y));
//    }
//    void Gas::swapGas(Position targetPos, Position lastPos)
//    {
//        mVelocity.y = -62.f;
//        MoveLastPosAndSwapTarget(targetPos, lastPos);
//    }
//    int Gas::getAdditional(float val) 
//    {
//        if (val < -0.1f)        return (int)floor(val);
//        else if (val > 0.1f)    return (int)ceil(val);
//        else                    return 0;
//    }
//    float Gas::getAverageVelOrGravity(float vel, float targetvel) 
//    {
//        if (targetvel < -125.f) return -124.f;
//
//        float avg = (vel + targetvel) / 2;
//
//        if (avg < 0)    return avg;
//        else            return max(avg, -124.f);
//    }
//
//
//    bool Gas::iterateToAdditional(int startingX, int startingY, int distance)
//    {
//        int distanceModifier = distance > 0 ? 1 : -1;
//
//        Position lastPos = mPos;
//
//        for (int i = 0; i <= abs(distance); i++) 
//        {
//            Element* target = PixelGrid::GetElement(startingX + i * distanceModifier, startingY);
//
//            //boolean acted = actOnOther(neighbor, matrix);
//            //if (acted) return false;
//            bool isFirst = i == 0;
//            bool isFinal = i == abs(distance);
//
//            if (target == nullptr) continue;
//
//            if (target->GetType() == eElementType::Empty || target->GetType() == eElementType::TempParticle) 
//            {
//                if (isFinal) 
//                {
//                    SwapTarget(target->GetPos());
//                    return false;
//                }
//
//                lastPos.x = startingX + i * distanceModifier;
//                lastPos.y = startingY;
//                continue;
//            }
//            else if (target->GetType() == eElementType::Gas)
//            {
//                Gas* gasElement = dynamic_cast<Gas*>(target);
//
//                if (compareDensity(gasElement))
//                {
//                    swapGas(gasElement->GetPos(), lastPos);
//                    return false;
//                }
//            }
//            else if (target->GetType() == eElementType::Solid || target->GetType() == eElementType::Liquid)
//            {
//                if (isFirst) 
//                {
//                    return true;
//                }
//
//                MoveLastPosition(lastPos);
//                return false;
//            }
//        }
//        return true;
//    }
//}