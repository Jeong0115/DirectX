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

        size_t GetIndex(int x, int y) { return (x - mStartX) + (y - mStartY) * mWidth; }

        Element& GetElement(int x, int y) { return GetElement(GetIndex(x, y)); }
        Element& GetElement(size_t index);

        void SwapElement(int x, int y, const Element& element) { SwapElement(GetIndex(x, y), element); }
        void SwapElement( size_t index, const Element& element);

        void InsertElement(int x, int y, const Element& element) { InsertElement(GetIndex(x, y), element); }
        void InsertElement(size_t index, const Element& element);

        void RegisterElement(int x, int y, const Element& element) { RegisterElement(GetIndex(x, y), element); }
        void RegisterElement(size_t index, const Element& element);

        void DelteElement(int x, int y) { DelteElement(GetIndex(x, y)); }
        void DelteElement(size_t index);

        bool TakeElement(int x, int y) { return TakeElement(GetIndex(x, y)); }
        bool TakeElement(size_t index);

        void ResiterChanges(PixelChunk* source, int x, int y, int toX, int toY);
            
        void KeepAlive(int x, int y) { KeepAlive(GetIndex(x, y)); }
        void KeepAlive(size_t index);

        void UpdateRect();

        bool InBounds(int x, int y) { return x >= mStartX && x < mStartX + mWidth && y >= mStartY && y < mStartY + mHeight;}
        bool IsEmpty(int x, int y) { return IsEmpty(GetIndex(x, y)); }
        bool IsEmpty(size_t index) { return GetElement(index).Type == eElementType::EMPTY; }


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
        Element* mElements;

    private:
        int m_minXw, m_minYw,
            m_maxXw, m_maxYw;
    };
}
