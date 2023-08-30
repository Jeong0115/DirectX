#pragma once

#include "zzElement.h"

namespace zz
{
    class PixelChunk
    {
    public:
        PixelChunk(size_t width, size_t height, int x, int y);
        ~PixelChunk();

        void Update();
        void UpdateElement(int x, int y);

        void SwapElement(Element& dstElement, int dstX, int dstY, Element& srcElement, int srcX, int srcY);

        void InsertElement(int x, int y, const Element& element) { InsertElement(GetIndex(x, y), element); }
        void InsertElement(size_t index, const Element& element);

        void RegisterElement(int x, int y, const Element& element) { RegisterElement(GetIndex(x, y), element); }
        void RegisterElement(size_t index, const Element& element);

        void DelteElement(int x, int y) { DelteElement(GetIndex(x, y)); }
        void DelteElement(size_t index);

        bool TakeElement(int x, int y) { return TakeElement(GetIndex(x, y)); }
        bool TakeElement(size_t index);

        void ResiterChanges(PixelChunk* source, int x, int y, int toX, int toY);
            
        // 임시 나중에 수정
        void KeepAlive(int x, int y);
        void KeepAlive(size_t index) { KeepAlive(index % mWidth + mStartX, index / mWidth + mStartY); }

        void KeepAliveBoundary(int x, int y);

        void UpdateRect();

        Element& GetElement(int x, int y);
        Element& GetInChunkElement(size_t index) { return mElements[index]; }

        bool MoveDownSolid(int x, int y, Element& element);
        bool MoveDownLiquid(int x, int y, Element& elemen);
        bool MoveDown(int x, int y, Element& element);
        bool MoveSide(int x, int y, Element& element);
        bool MoveDownSide(int x, int y, Element& element);
        bool MoveGas(int x, int y, Element& element);

        void HeatNeighbors(int x, int y, Element& element);
        void DecreaseLifeTime(int x, int y, Element& element);

        bool InBounds(int x, int y) { return x >= mStartX && x < mStartX + mWidth && y >= mStartY && y < mStartY + mHeight;}
        bool IsEmpty(int x, int y) { return IsEmpty(GetIndex(x, y)); }
        bool IsEmpty(size_t index) { return GetInChunkElement(index).Type == eElementType::EMPTY; }

        size_t GetIndex(int x, int y) { return (x - mStartX) + (y - mStartY) * mWidth; };


    public:
        const int mWidth;
        const int mHeight;
        const int mStartX;
        const int mStartY;

        int mMinX;
        int mMinY;
        int mMaxX; 
        int mMaxY;

        size_t mElementCount;

        uint8_t mStaticCount[64* 64];

    private:
        std::vector<std::tuple<PixelChunk*, size_t, size_t>> mChanges;
        std::mutex mDirtyBoxMutex;
        Element* mElements;
        std::bitset<1> mUpdateDir;

    private:
        int m_minXw, m_minYw,
            m_maxXw, m_maxYw;
    };
}
