#pragma once

#include "zzEngine.h"

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
        void Render();
        void Clear();

        void SetHwnd(HWND hwnd) { mHwnd = hwnd; }
        void SetImage(int x, int y, std::shared_ptr<class Texture> texture);

    private:
        HWND        mHwnd;
        HDC         mHdc;
        HBITMAP     mBackBuffer;
        HDC         mBackHDC;

        std::vector<uint8_t> pixels;
        UINT mWidth;
        UINT mHeight;

        void* bits;
        class Image* mImage;
    };



    class Image
    {
    public:
        Image();
        ~Image();

        void Update(std::vector<uint8_t>& pixels, HDC BackDC);

    private:
        HBITMAP mBitmap;
        HDC mHdc;

        void* bits;
    };
}

