#include "zzSolid.h"
#include "zzPixelGrid.h"

namespace zz
{
    Solid::Solid()
    {
        mType = eElementType::Solid;
        mVelocity = math::Vector2(0, 124.f);
        mStopThreshold = 10;
    }

    Solid::~Solid()
    {

    }

    void Solid::Update()
    {
        //if (mStep != PixelGrid::Step)
        //    return;
        //mStep.flip();

        ////mVelocity += math::Vector2(0.0f, 5.0f);
        //if (isFreeFalling)
        //{
        //    mVelocity.x *= 0.8f;
        //}



        //int yModifier = mVelocity.y < 0 ? -1 : 1;
        //int xModifier = mVelocity.x < 0 ? -1 : 1;

        //float velYDeltaTimeFloat = (fabs(mVelocity.y) * 1 / 60);
        //float velXDeltaTimeFloat = (fabs(mVelocity.x) * 1 / 60);

        //int velXDeltaTime;
        //int velYDeltaTime;

        //if (velXDeltaTimeFloat < 1)
        //{
        //    mVelocityRemainder.x += velXDeltaTimeFloat;
        //    velXDeltaTime = (int)mVelocityRemainder.x;

        //    if (fabs(velXDeltaTime) > 0)
        //        mVelocityRemainder.x = 0;
        //}
        //else
        //{
        //    mVelocityRemainder.x = 0;
        //    velXDeltaTime = (int)velXDeltaTimeFloat;
        //}

        //if (velYDeltaTimeFloat < 1)
        //{
        //    mVelocityRemainder.y += velYDeltaTimeFloat;
        //    velYDeltaTime = (int)mVelocityRemainder.y;

        //    if (abs(velYDeltaTime) > 0)
        //        mVelocityRemainder.y = 0;
        //}
        //else
        //{
        //    mVelocityRemainder.y = 0;
        //    velYDeltaTime = (int)velYDeltaTimeFloat;
        //}

        //bool isXDeltaTimeLarger = abs(velXDeltaTime) > abs(velYDeltaTime);

        //int largeDelatTime = max(abs(velXDeltaTime), abs(velYDeltaTime));
        //int smallDeltaTime = min(abs(velXDeltaTime), abs(velYDeltaTime));

        //float slope = (smallDeltaTime == 0 || largeDelatTime == 0) ? 0 : ((float)(smallDeltaTime + 1) / (largeDelatTime + 1));

        //int smallerCount;

        //Position startPos = mPos;
        //Position lastPos = mPos;

        //for (int i = 1; i <= largeDelatTime; i++)
        //{
        //    smallerCount = (int)floor(i * slope);

        //    int yIncrease, xIncrease;
        //    if (isXDeltaTimeLarger)
        //    {
        //        xIncrease = i;
        //        yIncrease = smallerCount;
        //    }
        //    else
        //    {
        //        yIncrease = i;
        //        xIncrease = smallerCount;
        //    }

        //    Position targetPos = Position(mPos.x + (xIncrease * xModifier), mPos.y + (yIncrease * yModifier));

        //    if (/*matrix.isWithinBounds(carPos.x, carPos.y)*/1)
        //    {
        //        Element* target = PixelGrid::GetElement(targetPos.x, targetPos.y);
        //        if (target == this) continue;

        //        bool stopped = InteractElement(target, targetPos, i == largeDelatTime, i == 1, lastPos, 0);

        //        if (stopped) break;

        //        lastPos = targetPos;

        //    }
        //    else
        //    {
        //        //matrix.setElementAtIndex(getMatrixX(), getMatrixY(), ElementType.EMPTYCELL.createElementByMatrix(getMatrixX(), getMatrixY()));
        //        return;
        //    }



        //}

        ////applyHeatToNeighborsIfIgnited(matrix);
        //    //modifyColor();
        //    //spawnSparkIfIgnited(matrix);
        //    //checkLifeSpan(matrix);
        //    //takeEffectsDamage(matrix);
        //mStopCount = IsStop(startPos) ? mStopCount + 1 : 0;
        //if (mStopCount > mStopThreshold)
        //{
        //    mStopCount = mStopThreshold;
        //}

        ////if (mStopCount != mStopThreshold)
        ////{
        //if (!Is())
        //{
        //    PixelGrid::SetActiveChunks(mPos.x, mPos.y);
        //    PixelGrid::SetActiveChunks(startPos.x, startPos.y);
        //}
        ////}
        ////if (matrix.useChunks) {
        ////    if (isIgnited || !hasNotMovedBeyondThreshold()) {
        ////        matrix.reportToChunkActive(this);
        ////        matrix.reportToChunkActive((int)formerLocation.x, (int)formerLocation.y);
        ////    }
        ////}

    }

    bool Solid::InteractElement(Element* target, Position targetPos, bool isFinal, bool isFirst, Position& lastPos, int depth)
    {
        return false;
    }


}