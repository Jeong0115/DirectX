#pragma once

#include "zzEngine.h"
#include "zzElement.h"
#include "zzChunk.h"

namespace zz
{
    class PixelGrid
    {
    public:
        SINGLETON(PixelGrid)

    private:
        PixelGrid();
        ~PixelGrid();

    public:
        void Initialize();
        void Update();
        void FixedUpdate();
        void Render();
        void Clear();

        void SetHwnd(HWND hwnd) { mHwnd = hwnd; }
        void SetImage(int x, int y, std::shared_ptr<class Texture> texture);
        Element* GetElement(int x, int y) { return mElements[y][x]; }
        void SwapElement(int x1, int y1, int x2, int y2);
        void SetActiveChunk(int x, int y);

    private:
        HWND        mHwnd;
        HDC         mHdc;
        HBITMAP     mBackBuffer;
        HDC         mBackHDC;

        std::vector<uint8_t> mPixelColor;
        std::vector<std::vector<Element*>> mElements;

        UINT mWidth;
        UINT mHeight;

        void* bits;
        class Image* mImage;

        double mFixedTime;
        Chunk mChunks[32][32];

        

        float x = 0, y = 0;
    };



    class Image
    {
    public:
        Image();
        ~Image();

        void Update(std::vector<uint8_t>& pixelColor, HDC BackDC, float x, float y);

    private:
        HBITMAP mBitmap;
        HDC mHdc;

        void* bits;
    };
}

