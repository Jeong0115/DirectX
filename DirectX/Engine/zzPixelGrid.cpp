#include "zzPixelGrid.h"
#include "zzTexture.h"
#include "zzResourceManager.h"

#include "zzInput.h"
#include "zzTime.h"
#include "zzSand.h"
#include "zzWater.h"

#include <minwindef.h>

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
        , mFixedTime(0.0)
    {
        mPixelColor.resize(mWidth * mHeight * 4);
        mElements.resize(mHeight, std::vector<Element*>(mWidth));

        //for (int i = 0; i < mHeight; i++)
        //{
        //    for (int j = 0; j < mWidth; j++)
        //    {
        //        if(mElements[i][j] != nullptr)
        //            memcpy(&mPixelColor[(i * mWidth + j) * 4], mElements[i][j]->GetColor(), 4);
        //    }
        //}

        /*uint32_t x = 0xFFFFFFFF;

        for(int i = 300 * mWidth; i <= 301 * mWidth; i++)
        {
            memcpy(&mPixelColor[i*4], &x, 4);
        }

        uint32_t yellow = 0xFFFF00FF;

        for(int j=0; j<30; j++)
        {
            for (int i = 100; i <= 250; i++)
            {
                memcpy(&mPixelColor[(256 + j + mWidth * i) * 4], &yellow, 4);
            }
        }*/

        //for (int i = 220; i < 221; i++)
        //{
        //    for (int j = 60; j < 200; j++)
        //    {
        //        mElements[j][i] = new Sand();
        //        mElements[j][i]->SetPos(i, j);
        //    }
        //}

        uint32_t a = 0xFFFFFFFF;
        for (int i = 300; i < 310; i++)
        {
            for (int j = 100; j < 420; j++)
            {
                mElements[i][j] = new Sand();
                mElements[i][j]->SetColor(a);
                mElements[i][j]->SetPos(j, i);
                mElements[i][j]->isFalling = false;
            }
        }

        uint32_t x = 0x00000000;
        for (UINT i = 0; i < mHeight; i++)
        {
            for (UINT j = 0; j < mWidth; j++)
            {
                if (mElements[i][j] != nullptr)
                    memcpy(&mPixelColor[(i * mWidth + j) * 4], mElements[i][j]->GetColor(), 4);
                else
                    memcpy(&mPixelColor[(i * mWidth + j) * 4], &x, 4);
            }
        }

    }

    PixelGrid::~PixelGrid()
    {
        delete mImage;
        for (UINT i = 0; i < mHeight; i++)
        {
            for (UINT j = 0; j < mWidth; j++)
            {
                delete mElements[i][j];
            }
        }
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


    //float x = 0, y = 0;
    void PixelGrid::Update()
    {
        if(Input::GetKeyDown(eKeyCode::LBUTTON) || Input::GetKeyDown(eKeyCode::RBUTTON))
        {
            POINT mousePos = {};
            GetCursorPos(&mousePos);

            ScreenToClient(mHwnd, &mousePos);

            wchar_t text[50] = {};

            swprintf_s(text, 50, L"Mouse Position : %d, %d\n", (int)mousePos.x, (int)mousePos.y);

            OutputDebugStringW(text);

            HWND nowHwnd = GetFocus();

            if(nowHwnd == mHwnd)
            {
                if(mousePos.x > 50 && mousePos.y >50)
                {
                    if (Input::GetKeyDown(eKeyCode::LBUTTON))
                    {
                        for (int i = (int)mousePos.y - 25; i < mousePos.y + 25; i++)
                        {
                            for (int j = (int)mousePos.x - 5; j < mousePos.x + 5; j++)
                            {
                                mElements[i][j] = new Sand();
                                mElements[i][j]->SetPos(j, i);

                                SetActiveChunk(j, i);

                                memcpy(&mPixelColor[(i * mWidth + j) * 4], mElements[i][j]->GetColor(), 4);
                            }
                        }
                    }
                    else
                    {
                        for (int i = (int)mousePos.y - 50; i < mousePos.y + 50; i++)
                        {
                            for (int j = (int)mousePos.x - 50; j < mousePos.x + 50; j++)
                            {
                                //delete mElements[i][j];
                                //mElements[i][j] = nullptr;

                                //SetActiveChunk(j, i);

                                //uint32_t x = 0x00000000;
                                //memcpy(&mPixelColor[(i * mWidth + j) * 4], &x, 4);

                                mElements[i][j] = new Water();
                                mElements[i][j]->SetPos(j, i);

                                SetActiveChunk(j, i);

                                memcpy(&mPixelColor[(i * mWidth + j) * 4], mElements[i][j]->GetColor(), 4);
                            }
                        }
                    }
                }
            }
        }


        mFixedTime += Time::DeltaTime();
        if (mFixedTime >= 1.0 / 50.0)
        {
            mFixedTime -= 1.0 / 50.0;
            FixedUpdate();
        }

        mImage->Update(mPixelColor, mBackHDC, x, y);
    }

    void PixelGrid::FixedUpdate()
    {
        LARGE_INTEGER cpuFreq = {};
        LARGE_INTEGER prevFreq = {};
        LARGE_INTEGER curFreq = {};

        QueryPerformanceFrequency(&cpuFreq);
        QueryPerformanceCounter(&prevFreq);

       /* for(int i = mWidth * mHeight - 1; i >= 0; i--)
        {
            uint32_t x;
            memcpy(&x, &mPixelColor[i * 4], 4);

            if (x!= 0xFFFFFFFF && x != 0x00000000)
            {
                uint32_t target[3];
                memcpy(target, &mPixelColor[(i + mHeight - 1) * 4], 12);

                if (target[1] == 0x00000000)
                {
                    memcpy(&mPixelColor[i * 4 + mWidth * 4], &mPixelColor[i * 4], 4);
                    ZeroMemory(&mPixelColor[i * 4], 4);
                }
                else if (target[0] == 0x00000000)
                {
                    memcpy(&mPixelColor[(i - 1) * 4 + mWidth * 4], &mPixelColor[i * 4], 4);
                    ZeroMemory(&mPixelColor[i * 4], 4);
                }
                else if (target[2] == 0x00000000)
                {
                    memcpy(&mPixelColor[(i + 1) * 4 + mWidth * 4], &mPixelColor[i * 4], 4);
                    ZeroMemory(&mPixelColor[i * 4], 4);
                }
            }
        }*/

        for (int i = mHeight - 1; i >= 0; i--)
        {
            for (int j = mWidth - 1; j >= 0; j--)
            {
                if (mElements[i][j] == nullptr) continue;
                mElements[i][j]->Move();
            }
        }


        
        //int chunkSize = 10; // 청크의 크기를 10으로 설정. 이는 문제의 특성에 따라 다를 수 있음.
        //for (int i = mHeight - 1; i >= 1; i -= chunkSize) {
        //    for (int j = mWidth - 1; j >= 1; j -= chunkSize) {
        //        for (int ci = i; ci > (((0) > (i - chunkSize)) ? (0) : (i - chunkSize)); --ci) {
        //            for (int cj = j; cj > (((0) > (j - chunkSize)) ? (0) : (j - chunkSize)); --cj) {
        //                if (mElements[ci][cj] == nullptr) continue;
        //                mElements[ci][cj]->Move();
        //            }
        //        }
        //    }
        //}

        /*for (int ci = 31; ci >= 0; ci--)
        {
            for (int cj = 31; cj >= 0; cj--)
            {
                if (mChunks[ci][cj].isActive())
                {
                    mChunks[ci][cj].SetDeath();

                    for (int i = 63; i >= 0; i--)
                    {
                        for (int j = 63; j >= 0; j--)
                        {
                            if (mElements[ci * 64 + i][cj * 64 + j] == nullptr) continue;
                            mElements[ci * 64 + i][cj * 64 + j]->Move();

                        }
                    }
                }
            }
        }*/

        //uint32_t x = 0x00000000;
        //for (UINT i = 0; i < mHeight; i++)
        //{
        //    for (UINT j = 0; j < mWidth; j++)
        //    {
        //        if (mElements[i][j] != nullptr)
        //            memcpy(&mPixelColor[(i * mWidth + j) * 4], mElements[i][j]->GetColor(), 4);
        //        else
        //            memcpy(&mPixelColor[(i * mWidth + j) * 4], &x, 4);
        //    }
        //}

        QueryPerformanceCounter(&curFreq);

        double latency = (double)(curFreq.QuadPart - prevFreq.QuadPart) / (double)cpuFreq.QuadPart;
        wchar_t text[100] = {};

        swprintf_s(text, 100, L"FixedUpdate Latency : %f", latency);

        SetWindowText(mHwnd, text);
    }

    void PixelGrid::Render()
    {
        if (Input::GetKey(eKeyCode::A))
        {
            x -= 305.0f * (float)Time::DeltaTime();
        }
        else if (Input::GetKey(eKeyCode::D))
        {
            x += 305.0f * (float)Time::DeltaTime();
        }
        else if (Input::GetKey(eKeyCode::Q))
        {
            y -= 305.0f * (float)Time::DeltaTime();
        }
        else if (Input::GetKey(eKeyCode::E))
        {
            y += 305.0f * (float)Time::DeltaTime();
        }

        //BitBlt(mHdc, 0, 0, 512, 512, mBackHDC, 0, 0, SRCCOPY);
        StretchBlt(
            mHdc,     // Destination device context
            0, 0,        // Destination rectangle origin (x,y)
            512 ,   // Destination rectangle width
            512 ,  // Destination rectangle height
            mBackHDC,      // Source device context
            0, 0,        // Source rectangle origin (x,y)
            512,    // Source rectangle width
            512,   // Source rectangle height
            SRCCOPY      // Raster-operation code
        );

        HPEN greenPen = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
        HPEN oldPen = (HPEN)SelectObject(mHdc, greenPen);
        HBRUSH hollowBrush = (HBRUSH)GetStockObject(HOLLOW_BRUSH);
        HBRUSH _oldBrush = (HBRUSH)SelectObject(mHdc, hollowBrush);
        for (int i = 0; i < 32; i++)
        {
            for (int j = 0; j < 32; j++)
            {
                if(mChunks[i][j].isActive())
                    ::Rectangle(mHdc, j * 64, i * 64, (j + 1) * (64), (i + 1) * (64));
            }
        }
        SelectObject(mHdc, oldPen);
        DeleteObject(greenPen);

        SelectObject(mHdc, _oldBrush);
        DeleteObject(hollowBrush);

        HBRUSH grayBrush = CreateSolidBrush(RGB(121, 121, 121));
        HBRUSH oldBrush = (HBRUSH)SelectObject(mBackHDC, grayBrush);
        ::Rectangle(mBackHDC, -1, -1, 514, 514);
        SelectObject(mBackHDC, oldBrush);
        DeleteObject(grayBrush);
    }

    void PixelGrid::Clear()
    {
        //HBRUSH grayBrush = CreateSolidBrush(RGB(121, 121, 121));
        //HBRUSH oldBrush = (HBRUSH)SelectObject(mBackHDC, grayBrush);
        //Rectangle(mBackHDC, -1, -1, 1602, 902);
        //SelectObject(mBackHDC, oldBrush);
        //DeleteObject(grayBrush);
    }

    void PixelGrid::SwapElement(int x1, int y1, int x2, int y2)
    {
        Element* temp = mElements[y1][x1];

        mElements[y1][x1] = mElements[y2][x2];

        if(mElements[y1][x1] != nullptr)
            mElements[y1][x1]->SetPos(x1, y1);

        mElements[y2][x2] = temp;

        if (mElements[y2][x2] != nullptr)
            mElements[y2][x2]->SetPos(x2, y2);

        uint32_t x;

        //uint32_t x = mPixelColor[(y1 * mWidth + x1) * 4];

        memcpy(&x, &mPixelColor[(y1 * mWidth + x1) * 4], 4);
        memcpy(&mPixelColor[(y1 * mWidth + x1) * 4], &mPixelColor[(y2 * mWidth + x2) * 4], 4);
        memcpy(&mPixelColor[(y2 * mWidth + x2) * 4], &x, 4);

    }

    void PixelGrid::SetActiveChunk(int x, int y)
    {
        x /= 64;
        y /= 64;

        mChunks[y][x].SetActive();
        mChunks[y + 1][x + 1].SetActive();
        if(x > 0 && y > 0)
        {
            mChunks[y - 1][x - 1].SetActive();
        }
    }

    void PixelGrid::SetImage(int x, int y, std::shared_ptr<Texture> texture)
    {   
        return;
        uint8_t* texPixels = texture->GetPixels();
        DXGI_FORMAT format = texture->GetFormat();
        UINT texSize = texture->GetImageSize() * 4;
        UINT texWidth = texture->GetImageWidth() * 4;

        x *= 4;
        UINT row = x;
        UINT col = y;

        if (format == DXGI_FORMAT::DXGI_FORMAT_B8G8R8A8_UNORM)
        {
            for (UINT i = 0; i < texSize; i += 4, row += 4)
            {
                if (row >= texWidth + x)
                {
                    col++;
                    row = x;
                }
                uint8_t* currPixel = texPixels + i;
                if (*(currPixel + 3) != (uint8_t)0)
                {
                    memcpy(&mPixelColor[col * mWidth * 4 + row], currPixel, 4);
                }
            }
        }
        else if (format == DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM)
        {
            for (UINT i = 0; i < texSize; i += 4, row += 4)
            {
                if (row >= texWidth + x)
                {
                    col++;
                    row = 0;
                }
                uint8_t* currPixel = texPixels + i;
                if (*(currPixel + 3) != (uint8_t)0)
                {
                    for (int j = 0; j <= 2; j++)
                    {
                        mPixelColor[col * mWidth * 4 + row + 2 - j] = *(currPixel + j);
                    }
                    mPixelColor[col * mWidth * 4 + row + 3] = *(currPixel + 3);
                }
            }
        }

        /*if (format == DXGI_FORMAT::DXGI_FORMAT_B8G8R8A8_UNORM)
        {
            for (UINT i = 0; i < texSize; i += 4, row += 4)
            {
                if (row >= texWidth + x * 4)
                {
                    col++;
                    row = x * 4;
                }
                if (*(texPixels + i + 3) != (uint8_t)0)
                {
                    for (int j = 0; j <= 3; j++)
                    {
                        pixels[col * mWidth * 4 + row + j] = *(texPixels + i + j);
                    }
                }
            }
        }
        else if (format == DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM)
        {
            for (UINT i = 0; i < texSize; i += 4, row += 4)
            {
                if (row >= texWidth + x)
                {
                    col++;
                    row = 0;
                }
                if (*(texPixels + i + 3) != (uint8_t)0)
                {
                    for (int j = 0; j <= 2; j++)
                    {
                        pixels[col * mWidth * 4 + row + 2 - j] = *(texPixels + i + j);
                    }
                    pixels[col * mWidth * 4 + row + 3] = *(texPixels + i + 3);
                }
            }
        }*/
    }


    Image::Image()
        : mHdc(NULL)
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

    void Image::Update(std::vector<uint8_t>& pixelColor, HDC BackDC, float x, float y)
    {
        memcpy(bits, pixelColor.data(), pixelColor.size());
        BitBlt(BackDC, 0, 0, 2048, 2048, mHdc, (int)x, (int)-y, SRCCOPY);
    }


}

