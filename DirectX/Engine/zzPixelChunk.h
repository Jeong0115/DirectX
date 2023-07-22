#pragma once

#include "zzElement.h"

namespace zz
{
    class PixelChunk
    {
    public:
        PixelChunk(size_t width, size_t height, int x, int y);
        ~PixelChunk();

        void MoveElements();

        size_t GetIndex(int x, int y) { 
            int a = (x - mStartX) + (y - mStartY) * mWidth;
            if (a < 0)
                int c = 0;
            return (x - mStartX) + (y - mStartY) * mWidth;
        }

        Element* GetElement(int x, int y) { return GetElement(GetIndex(x, y)); }
        Element* GetElement(size_t index);

        void SwapElement(int x, int y, Element* cell) { SwapElement(GetIndex(x, y), cell); }   
        void SwapElement( size_t index, Element* cell);

        void InsertElement(int x, int y, Element* element) { InsertElement(GetIndex(x, y), element); }
        void InsertElement(size_t index, Element* element);

        void ResiterChanges(PixelChunk* source, int x, int y, int toX, int toY);

        void KeepAlive(int x, int y) { KeepAlive(GetIndex(x, y)); }
        void KeepAlive(size_t index);

        void UpdateRect();

        bool InBounds(int x, int y) { return x >= mStartX && x < mStartX + mWidth && y >= mStartY && y < mStartY + mHeight;}
        bool IsEmpty(int x, int y) { return IsEmpty(GetIndex(x, y)); }
        bool IsEmpty(size_t index) { return GetElement(index)->GetType() == eElementType::Empty; }


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

    private:
        std::vector<std::tuple<PixelChunk*, size_t, size_t>> mChanges;
        std::mutex resiterMutex;
        Element** mElements;
        static std::mutex tempMutex;
    private:
        int m_minXw, m_minYw,
            m_maxXw, m_maxYw;
    };
}
