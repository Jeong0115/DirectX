#include "zzPixelGrid.h"
#include "zzTexture.h"
#include "zzResourceManager.h"

#include "zzInput.h"
#include "zzTime.h"

#include "zzSand.h"
#include "zzWater.h"
#include "zzRock.h"

#include <minwindef.h>

#include <random>
#include <algorithm>

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

        mElementMap.insert({ 'z', new Water});
        mElementMap.insert({ 'x', new Sand });
        mElementMap.insert({ 'c', new Rock });
        mElementMap.insert({ 'v', nullptr });
        mSelectElement = mElementMap.find('v')->second;

        //for (int i = 220; i < 221; i++)
        //{
        //    for (int j = 60; j < 200; j++)
        //    {
        //        mElements[j][i] = new Sand();
        //        mElements[j][i]->SetPos(i, j);
        //    }
        //}


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

        for (auto map : mElementMap)
        {
            delete map.second;
            map.second = nullptr;
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

        mImage = new PixelGridColor();
    }

    void PixelGrid::Update()
    {
        if (Input::GetKeyDown(eKeyCode::Z) || Input::GetKeyDown(eKeyCode::X) || Input::GetKeyDown(eKeyCode::C)
            || Input::GetKeyDown(eKeyCode::V))
        {
            if ((Input::GetKeyDown(eKeyCode::Z)))
                mSelectElement = mElementMap.find('z')->second;
            else if ((Input::GetKeyDown(eKeyCode::X)))
                mSelectElement = mElementMap.find('x')->second;
            else if ((Input::GetKeyDown(eKeyCode::C)))
                mSelectElement = mElementMap.find('c')->second;
            else if ((Input::GetKeyDown(eKeyCode::V)))
                mSelectElement = mElementMap.find('v')->second;
        }

        if(Input::GetKey(eKeyCode::LBUTTON) || Input::GetKeyDown(eKeyCode::RBUTTON))
        {
            HWND nowHwnd = GetFocus();

            if(nowHwnd == mHwnd)
            {
                POINT mousePos = {};
                GetCursorPos(&mousePos);
                ScreenToClient(mHwnd, &mousePos);

                mousePos.x += x;
                mousePos.y -= y;

                if(mousePos.x > 50 && mousePos.y >50)
                {
                    if (Input::GetKey(eKeyCode::LBUTTON))
                    {
                        for (int i = (int)mousePos.y - 10; i < mousePos.y + 10; i++)
                        {
                            for (int j = (int)mousePos.x - 10; j < mousePos.x + 10; j++)
                            {
                                if (mElements[i][j] != nullptr)
                                    delete mElements[i][j];

                                if (mSelectElement == nullptr)
                                {
                                    mElements[i][j] = nullptr;
                                    uint32_t x = 0x00000000;
                                    memcpy(&mPixelColor[(i * mWidth + j) * 4], &x, 4);
                                    SetActiveChunk(j, i);
                                }
                                else
                                {
                                    mElements[i][j] = mSelectElement->Clone();
                                    mElements[i][j]->SetPos(j, i);

                                    SetActiveChunk(j, i);

                                    memcpy(&mPixelColor[(i * mWidth + j) * 4], mElements[i][j]->GetColor(), 4);
                                }
                            }
                        }
                    }
                    else
                    {
                        for (int i = (int)mousePos.y - 20; i < mousePos.y + 20; i++)
                        {
                            for (int j = (int)mousePos.x - 20 ; j < mousePos.x + 20; j ++)
                            {
                                if (mElements[i][j] != nullptr)
                                    delete mElements[i][j];

                                if (mSelectElement == nullptr)
                                {
                                    mElements[i][j] = nullptr;
                                    uint32_t x = 0x00000000;
                                    memcpy(&mPixelColor[(i * mWidth + j) * 4], &x, 4);
                                    SetActiveChunk(j, i);
                                }
                                else
                                {
                                    mElements[i][j] = mSelectElement->Clone();
                                    mElements[i][j]->SetPos(j, i);

                                    SetActiveChunk(j, i);

                                    memcpy(&mPixelColor[(i * mWidth + j) * 4], mElements[i][j]->GetColor(), 4);
                                }
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

        //for (int i = mHeight - 1; i >= 0; i--)
        //{
        //    for (int j = mWidth - 1; j >= 0; j--)
        //    {
        //        if (mElements[i][j] == nullptr) continue;
        //        mElements[i][j]->Move();
        //    }
        //}


        
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

        std::vector<int> numbers;
        for (int i = 0; i <= 63; ++i) {
            numbers.push_back(i);
        }

        // 랜덤 생성기 초기화
        std::random_device rd;
        std::mt19937 g(rd());

        // 벡터 섞기
        std::shuffle(numbers.begin(), numbers.end(), g);

        std::vector<Element*> elementsToMove;

        for (int ci = 31; ci >= 0; ci--)
        {
            for (int cj = 31; cj >= 0; cj--)
            {
                if (mChunks[ci][cj].isActive())
                {
                    mChunks[ci][cj].SetDeath();

                    for (int i = 63; i>=0; i--)
                    {
                        //std::shuffle(numbers.begin(), numbers.end(), g);
                        for (int j : numbers)
                        {
                            if (mElements[ci * 64 + i][cj * 64 + j] == nullptr) continue;
                            //mElements[ci * 64 + i][cj * 64 + j]->Move();

                            if (mElements[ci * 64 + i][cj * 64 + j]->isFalling) {
                                elementsToMove.push_back((mElements[ci * 64 + i][cj * 64 + j]));
                            }

                        }
                    }
                }
            }
        }

        for (const auto& element : elementsToMove)
        {
            element->Move();
        }

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
                if (mChunks[i][j].isActive())
                    ::Rectangle(mHdc, j * 64 -x, (i) * 64 + y, (j + 1 ) * (64) - x, (i + 1 ) * (64) + y);
            }
        }
        SelectObject(mHdc, oldPen);
        DeleteObject(greenPen);

        SelectObject(mHdc, _oldBrush);
        DeleteObject(hollowBrush);

        {
            POINT mousePos = {};
            GetCursorPos(&mousePos);
            ScreenToClient(mHwnd, &mousePos);

            HPEN greenPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
            HPEN oldPen = (HPEN)SelectObject(mHdc, greenPen);
            HBRUSH hollowBrush = (HBRUSH)GetStockObject(HOLLOW_BRUSH);
            HBRUSH _oldBrush = (HBRUSH)SelectObject(mHdc, hollowBrush);

            ::Rectangle(mHdc, mousePos.x - 10, mousePos.y - 10, mousePos.x + 10, mousePos.y + 10);

            SelectObject(mHdc, oldPen);
            DeleteObject(greenPen);

            SelectObject(mHdc, _oldBrush);
            DeleteObject(hollowBrush);
        }

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
        //mChunks[y + 1][x + 1].SetActive();
        //if(x > 0 && y > 0)
        //{
        //    mChunks[y - 1][x - 1].SetActive();
        //}
    }

    void PixelGrid::SetImage(int x, int y, std::shared_ptr<Texture> texture)
    {   
        //return;
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
                    uint32_t color;
                    memcpy(&color, currPixel, 4);

                    if (color == 0xFFFFFFFF)
                    {
                        if (mElements[col][row / 4] == nullptr)
                        {
                            uint32_t none = 0x00000000;
                            memcpy(&mPixelColor[(col * mWidth + row / 4) * 4], &none, 4);
                        }
                    }
                    
                    else if (color == 0xFF0A3344) 
                    {
                        mElements[col][row / 4] = new Rock();
                        mElements[col][row / 4]->SetPos(row / 4, col);
                        memcpy(&mPixelColor[(col * mWidth + row / 4) * 4], mElements[col][row / 4]->GetColor(), 4);
                        SetActiveChunk(row / 4, col);
                    }
                    else if (color == 0xFF000042)
                    {
                        mElements[col][row / 4] = new Water();
                        mElements[col][row / 4]->SetPos(row / 4, col);
                        memcpy(&mPixelColor[(col * mWidth + row / 4) * 4], mElements[col][row / 4]->GetColor(), 4);
                        SetActiveChunk(row / 4, col);
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
                uint8_t* currPixel = texPixels + i;
                if (*(currPixel + 3) != (uint8_t)0)
                {
                    uint32_t color;
                    memcpy(&color, currPixel, 4);
                    if (color == 0xFFFFFFFF)
                    {
                        if(mElements[col][row / 4] == nullptr)
                        {
                            uint32_t none = 0x00000000;
                            memcpy(&mPixelColor[(col * mWidth + row / 4) * 4], &none, 4);
                        }
                    }
                    else if (color == 0xFF44330A)
                    {
                        mElements[col][row / 4] = new Rock();
                        mElements[col][row / 4]->SetPos(row / 4, col);
                        memcpy(&mPixelColor[(col * mWidth + row / 4) * 4], mElements[col][row / 4]->GetColor(), 4);

                        SetActiveChunk(row / 4, col);
                    }
                    else if (color == 0xFF420000)
                    {
                        mElements[col][row / 4] = new Water();
                        mElements[col][row / 4]->SetPos(row / 4, col);
                        memcpy(&mPixelColor[(col * mWidth + row / 4) * 4], mElements[col][row / 4]->GetColor(), 4);
                        SetActiveChunk(row / 4, col);
                    }

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


    PixelGridColor::PixelGridColor()
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

    PixelGridColor::~PixelGridColor()
    {

    }

    void PixelGridColor::Update(std::vector<uint8_t>& pixelColor, HDC BackDC, float x, float y)
    {
        memcpy(bits, pixelColor.data(), pixelColor.size());
        BitBlt(BackDC, 0, 0, 2048, 2048, mHdc, (int)x, (int)-y, SRCCOPY);


    }



}

