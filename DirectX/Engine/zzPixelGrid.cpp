#include "zzPixelGrid.h"
#include "zzTexture.h"
#include "zzResourceManager.h"
namespace zz
{
    PixelGrid::PixelGrid()
        : mWidth(2048)
        , mHeight(2048)
        , mHwnd(NULL)
        , mHdc(NULL)
        , mBackHDC(NULL)
        , mBackBuffer(NULL)
        , mImage(nullptr)
        , bits(nullptr)
    {
        pixels.resize(mWidth * mHeight * 4);
    }

    PixelGrid::~PixelGrid()
    {
        delete mImage;
    }

    void PixelGrid::Initialize()
    {
        mHdc = GetDC(mHwnd);
        BITMAPINFO bmi = { 0 };
        bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bmi.bmiHeader.biWidth = 512;
        bmi.bmiHeader.biHeight = -512; // top-down
        bmi.bmiHeader.biPlanes = 1;
        bmi.bmiHeader.biBitCount = 32;
        bmi.bmiHeader.biCompression = BI_RGB;

        HBITMAP mBackBuffer = CreateDIBSection(NULL, &bmi, DIB_RGB_COLORS, &bits, NULL, 0);
        if (!mBackBuffer) return;
            
        mBackHDC = CreateCompatibleDC(mHdc);
        HBITMAP defaultBitmap = (HBITMAP)SelectObject(mBackHDC, mBackBuffer);
        DeleteObject(defaultBitmap);

        mImage = new Image();
    }



    void PixelGrid::Update()
    {
        mImage->Update(pixels, mBackHDC);
    }

    void PixelGrid::Render()
    {
        BitBlt(mHdc, 0, 0, 512, 512, mBackHDC, 0, 0, SRCCOPY);
    }

    void PixelGrid::Clear()
    {
        //HBRUSH grayBrush = CreateSolidBrush(RGB(121, 121, 121));
        //HBRUSH oldBrush = (HBRUSH)SelectObject(mBackHDC, grayBrush);
        //Rectangle(mBackHDC, -1, -1, 1602, 902);
        //SelectObject(mBackHDC, oldBrush);
        //DeleteObject(grayBrush);
    }

    void PixelGrid::SetImage(int x, int y, std::shared_ptr<Texture> texture)
    {   
        uint8_t* texPixels = texture->GetPixels();
        DXGI_FORMAT format = texture->GetFormat();
        UINT texSize = texture->GetImageSize() * 4;
        UINT texWidth = texture->GetImageWidth() * 4;

        UINT row = x;
        UINT col = y;

        if (format == DXGI_FORMAT::DXGI_FORMAT_B8G8R8A8_UNORM)
        {
            for (UINT i = 0; i < texSize; i += 4, row += 4)
            {
                if (row >= texWidth)
                {
                    col++;
                    row = x;
                }
                uint8_t* currPixel = texPixels + i;
                if (*(currPixel + 3) != (uint8_t)0)
                {
                    memcpy(&pixels[col * mWidth * 4 + row], currPixel, 4);
                }
            }
        }
        else if (format == DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM)
        {
            for (UINT i = 0; i < texSize; i += 4, row += 4)
            {
                if (row >= texWidth)
                {
                    col++;
                    row = 0;
                }
                uint8_t* currPixel = texPixels + i;
                if (*(currPixel + 3) != (uint8_t)0)
                {
                    for (int j = 0; j <= 2; j++)
                    {
                        pixels[col * mWidth * 4 + row + 2 - j] = *(currPixel + j);
                    }
                    pixels[col * mWidth * 4 + row + 3] = *(currPixel + 3);
                }
            }
        }
        //if (format == DXGI_FORMAT::DXGI_FORMAT_B8G8R8A8_UNORM)
        //{
        //    for (UINT i = 0; i < texSize; i += 4, row += 4)
        //    {
        //        if (row >= texWidth)
        //        {
        //            col++;
        //            row = x;
        //        }
        //        if (*(texPixels + i + 3) != (uint8_t)0)
        //        {
        //            for (int j = 0; j <= 3; j++)
        //            {
        //                pixels[col * mWidth * 4 + row + j] = *(texPixels + i + j);
        //            }
        //        }
        //    }
        //}
        //else if (format == DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM)
        //{
        //    for (UINT i = 0; i < texSize; i += 4, row += 4)
        //    {
        //        if (row >= texWidth)
        //        {
        //            col++;
        //            row = 0;
        //        }

        //        if (*(texPixels + i + 3) != (uint8_t)0)
        //        {
        //            for (int j = 0; j <= 2; j++)
        //            {
        //                pixels[col * mWidth * 4 + row + 2 - j] = *(texPixels + i + j);
        //            }
        //            pixels[col * mWidth * 4 + row + 3] = *(texPixels + i + 3);
        //        }
        //    }
        //}
    }


    Image::Image()
    {
        BITMAPINFO bmi = { 0 };
        bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bmi.bmiHeader.biWidth = 2048;
        bmi.bmiHeader.biHeight = -2048; // top-down
        bmi.bmiHeader.biPlanes = 1;
        bmi.bmiHeader.biBitCount = 32;
        bmi.bmiHeader.biCompression = BI_RGB;

        mBitmap = CreateDIBSection(NULL, &bmi, DIB_RGB_COLORS, &bits, NULL, 0);
        if (!mBitmap) return;

        HDC hdcScreen = GetDC(NULL);
        mHdc = CreateCompatibleDC(hdcScreen);

        HBITMAP defaultBitmap = (HBITMAP)SelectObject(mHdc, mBitmap);
        DeleteObject(defaultBitmap);

        ReleaseDC(NULL, hdcScreen);
    }

    Image::~Image()
    {

    }

    void Image::Update(std::vector<uint8_t>& pixels, HDC BackDC)
    {
        memcpy(bits, pixels.data(), pixels.size());
        BitBlt(BackDC, 0, 0, 2048, 2048, mHdc, 0, 0, SRCCOPY);
    }


}

