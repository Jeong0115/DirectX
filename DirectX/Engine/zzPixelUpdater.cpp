#include "zzPixelUpdater.h"

namespace zz
{
    PixelUpdater::PixelUpdater(PixelChunk* chunk)
        : mChunk(chunk)
    {
    }

    PixelUpdater::~PixelUpdater()
    {
    }

    //std::mutex as;
    void PixelUpdater::UpdateChunk()
    {
        //as.lock();
        //OutputDebugStringW(L"lock\n");
        for (int x = mChunk->mMinX; x < mChunk->mMaxX; x++)
        {
            for (int y = mChunk->mMinY; y < mChunk->mMaxY; y++) 
            {
                Element* cell = mChunk->GetElement(x + y * mChunk->mWidth);
                int px = x + mChunk->mStartX;
                int py = y + mChunk->mStartY;

                UpdateCell(px, py, cell);
            }
        }
        //as.unlock();
        //OutputDebugStringW(L"unlock\n");
    }

    Element* PixelUpdater::GetElement(int x, int y)
    {
        if (mChunk->InBounds(x, y)) 
        {
            return mChunk->GetElement(x, y);
        }

        return PixelWorld::GetElement(x, y);
    }

    void PixelUpdater::SwapElement(int x, int y, Element* cell)
    {
        if (mChunk->InBounds(x, y)) 
        {
            return mChunk->SwapElement(x, y, cell);
        }

        return PixelWorld::SwapElement(x, y, cell);
    }

    void PixelUpdater::SwapElement(int x, int y, int xto, int yto)
    {
        int pingX = 0, pingY = 0;

        if (x == mChunk->mStartX)                           pingX = -1;
        if (x == mChunk->mStartX + mChunk->mWidth - 1)      pingX = 1;
        if (y == mChunk->mStartY)                           pingY = -1;
        if (y == mChunk->mStartY + mChunk->mHeight - 1)     pingY = 1;

        if (pingX != 0)               PixelWorld::KeepAlive(x + pingX, y);
        if (pingY != 0)               PixelWorld::KeepAlive(x, y + pingY);
        if (pingX != 0 && pingY != 0) PixelWorld::KeepAlive(x + pingX, y + pingY);

        if (mChunk->InBounds(x, y) && mChunk->InBounds(xto, yto))
        {
            return mChunk->ResiterChanges(mChunk, x, y, xto, yto);
        }

        return PixelWorld::SwapElement(x, y, xto, yto);
    }

    bool PixelUpdater::IsEmpty(int x, int y)
    {
        if (mChunk->InBounds(x, y)) 
        {
            return mChunk->IsEmpty(x, y);
        }

        return PixelWorld::IsEmpty(x, y);
    }
}