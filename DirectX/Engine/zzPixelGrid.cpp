#include "zzPixelGrid.h"
#include "zzTexture.h"
#include "zzResourceManager.h"

#include "zzInput.h"
#include "zzTime.h"

#include "zzSand.h"
#include "zzWater.h"
#include "zzRock.h"
#include "zzOil.h"
#include "zzSpark.h"

#include "zzEmptyElement.h"

#include <minwindef.h>

#include <algorithm>
#include <thread>
#include <mutex>
#include <functional>
#include <atomic>
#include <queue>
#include <future>
#include <deque>
#include <list>
#include <type_traits>

namespace zz
{
    class ThreadPool {
    public:
        std::vector<std::thread> workers;
        std::queue<std::function<void()>> tasks;
        std::mutex queue_mutex;
        std::condition_variable condition;
        std::condition_variable all_idle;
        bool stop;
        std::atomic<int> active{0};

        ThreadPool(size_t threads) : stop(false) {
            for (size_t i = 0; i < threads; ++i)
            {
                workers.emplace_back([this] {
                    for (;;) {
                        std::function<void()> task;
                        {
                            std::unique_lock<std::mutex> lock(this->queue_mutex);
                            this->condition.wait(lock, [this] { return this->stop || !this->tasks.empty(); });
                            if (this->stop && this->tasks.empty()) return;
                            task = std::move(this->tasks.front());
                            this->tasks.pop();
                        }

                        task();
                        --this->active;
                        if (this->active == 0) all_idle.notify_one();
                    }
                    });
            }
        }
        template<class F, class... Args>
        auto enqueue(F&& f, Args&&... args) -> std::future<typename std::invoke_result<F, Args...>::type> {
            using return_type = typename std::invoke_result<F, Args...>::type;
            auto task = std::make_shared<std::packaged_task<return_type()>>(std::bind(std::forward<F>(f), std::forward<Args>(args)...));
            std::future<return_type> res = task->get_future();
            {
                std::unique_lock<std::mutex> lock(queue_mutex);
                if (stop) throw std::runtime_error("enqueue on stopped ThreadPool");
                ++this->active;
                tasks.emplace([task]() { (*task)(); });
            }
            condition.notify_one();
            return res;
        }
        ~ThreadPool() { // 맵 밖으로 픽셀이 나간다음 종료하면 여기서 터지는듯??
            {
                std::unique_lock<std::mutex> lock(queue_mutex);
                stop = true;
            }
            condition.notify_all();
            for (std::thread& worker : workers) 
                worker.join();
        }
        void wait() {
            std::unique_lock<std::mutex> lock(queue_mutex);
            all_idle.wait(lock, [this]() { return this->active == 0; });
        }

        void Shutdown() {
            {
                std::unique_lock<std::mutex> lock(queue_mutex);
                stop = true;
            }
            condition.notify_all();  // 모든 스레드에게 종료 신호를 보냅니다.
            for (std::thread& worker : workers) {
                if (worker.joinable()) {
                    worker.join();  // 각 스레드가 종료될 때까지 기다립니다.
                }
            }
            while (!tasks.empty()) 
            {
                tasks.pop();
            }
            // 이 시점에서 모든 스레드는 종료되었습니다.
        }
    private:


    };

    int PixelGrid::mFrameCount = 0;
    UINT PixelGrid::mWidth = 2048;
    UINT PixelGrid::mHeight = 2048;

    HWND        PixelGrid::mHwnd = {};
    HDC         PixelGrid::mHdc = {};
    HBITMAP     PixelGrid::mBackBuffer = {};
    HDC         PixelGrid::mBackHDC = {};

    std::vector<uint8_t> PixelGrid::mPixelColor(mWidth* mHeight * 4);
    std::vector<std::vector<Element*>> PixelGrid::mElements(mHeight, std::vector<Element*>(mWidth));

    void* PixelGrid::bits;
    PixelGridColor* PixelGrid::mImage;

    double PixelGrid::mFixedTime;
    Chunk PixelGrid::mChunks[32][32];

    std::map<char, Element*> PixelGrid::mElementMap;
    Element* PixelGrid::mSelectElement;

    float PixelGrid::x = 0.f;
    float PixelGrid::y = 0.f;

    std::bitset<1> PixelGrid::Step = {};
    std::vector<Element*> PixelGrid::mDeadElement = {};

    ThreadPool pool(10);

    PixelGrid::PixelGrid()
    {
    }

    PixelGrid::~PixelGrid()
    {

    }

    void PixelGrid::Initialize()
    {
        Step.set();

        mElementMap.insert({ 'z', new Water });
        mElementMap.insert({ 'x', new Sand });
        mElementMap.insert({ 'c', new Rock });
        mElementMap.insert({ 'o', new Oil });
        mElementMap.insert({ 'f', new Spark });
        mElementMap.insert({ 'e', new EmptyElement });
        mSelectElement = mElementMap.find('e')->second;

        for (UINT i = 0; i < mHeight; i++)
        {
            for (UINT j = 0; j < mWidth; j++)
            {
                mElements[i][j] = new EmptyElement();
            }
        }

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
            || Input::GetKeyDown(eKeyCode::E) || Input::GetKeyDown(eKeyCode::O) || Input::GetKeyDown(eKeyCode::F))
        {
            if ((Input::GetKeyDown(eKeyCode::Z)))
                mSelectElement = mElementMap.find('z')->second;
            else if ((Input::GetKeyDown(eKeyCode::X)))
                mSelectElement = mElementMap.find('x')->second;
            else if ((Input::GetKeyDown(eKeyCode::C)))
                mSelectElement = mElementMap.find('c')->second;
            else if ((Input::GetKeyDown(eKeyCode::E)))
                mSelectElement = mElementMap.find('e')->second;
            else if ((Input::GetKeyDown(eKeyCode::F)))
                mSelectElement = mElementMap.find('f')->second;
            else if ((Input::GetKeyDown(eKeyCode::O)))
                mSelectElement = mElementMap.find('o')->second;
        }

        if (Input::GetKey(eKeyCode::LBUTTON) || Input::GetKeyDown(eKeyCode::RBUTTON))
        {
            HWND nowHwnd = GetFocus();

            if (nowHwnd == mHwnd)
            {
                POINT mousePos = {};
                GetCursorPos(&mousePos);
                ScreenToClient(mHwnd, &mousePos);

                mousePos.x += (LONG)x;
                mousePos.y -= (LONG)y;

                if (mousePos.x > 15 && mousePos.y > 15)
                {
                    if (Input::GetKey(eKeyCode::LBUTTON))
                    {
                        for (int y = (int)mousePos.y - 10; y < mousePos.y + 10; y++)
                        {
                            for (int x = (int)mousePos.x - 10; x < mousePos.x + 10; x++)
                            {
                                delete mElements[y][x];

                                mElements[y][x] = mSelectElement->Clone();
                                mElements[y][x]->SetPos(x, y);

                                SetActiveChunks(x, y);

                                memcpy(&mPixelColor[(y * mWidth + x) * 4], mElements[y][x]->GetColor(), 4);
                            }
                        }
                    }
                    else
                    {
                        for (int y = (int)mousePos.y - 15; y < mousePos.y + 15; y++)
                        {
                            for (int x = (int)mousePos.x - 15; x < mousePos.x + 15; x++)
                            {
                                delete mElements[y][x];

                                mElements[y][x] = mSelectElement->Clone();
                                mElements[y][x]->SetPos(x, y);

                                SetActiveChunks(x, y);

                                memcpy(&mPixelColor[(y * mWidth + x) * 4], mElements[y][x]->GetColor(), 4);
                            }
                        }
                    }
                }
            }
        }


        mFixedTime += Time::DeltaTime();
        if (mFixedTime >= 1.0 / 50.0)
        {
            mFixedTime = 0;
            FixedUpdate();
            Step.flip();
        }

        int a = 0;
        mImage->Update(mPixelColor, mBackHDC, x, y);
        for (int i = 0; i < mDeadElement.size(); i++) // 먼가 먼가가 문제가 있음
        {
            if (mDeadElement[i] == nullptr)
                int c = 0;
            delete mDeadElement[i];
            mDeadElement[i] = nullptr;

        }
        mDeadElement.clear();
    }

    
    //std::vector<std::vector<Element*>> temp;
    //std::mutex mtx;

    void PixelGrid::FixedUpdate()
    {
        increaseFrameCount();

        LARGE_INTEGER cpuFreq = {};
        LARGE_INTEGER prevFreq = {};
        LARGE_INTEGER curFreq = {};

        QueryPerformanceFrequency(&cpuFreq);
        QueryPerformanceCounter(&prevFreq);

        // 멀티 쓰레드 element update
        {
            // copyChunksState을 마스크 처럼 쓸까, 아니면 지금 처럼 쓸까

            std::bitset<32> copyChunksState[32] = {};
            std::bitset<32 * 32> chunkMask = { 1, };

            for (int ci = 31; ci >= 0; ci--)
            {
                for (int cj = 31; cj >= 0; cj--)
                {
                    if (mChunks[ci][cj].isActive())
                    {
                        mChunks[ci][cj].SetDeath();
                        copyChunksState[cj].set(ci, true);
                    }
                }
            }

            int a = 100;

            while (true)
            {
                if (chunkMask.all()) break;
                chunkMask.set();

                int k = 0;

                for (int y = 31; y >= 0; y--)
                {
                    for (int x = 31; x >= 0; x--)
                    {
                        if (copyChunksState[y][x] && chunkMask[y * 32 + x])
                        {
                            for (int maskY = y - 1; maskY <= y + 1; maskY++)
                            {
                                if (maskY < 0 || maskY > 31) continue;
                                for (int maskX = x - 1; maskX <= x + 1; maskX++)
                                {
                                    if (maskX < 0 || maskX > 31) continue;
                                    chunkMask[maskY * 32 + maskX] = false;
                                }
                            }

                            copyChunksState[y][x] = false;
                            pool.enqueue(updateChunk, y, x);
                            k++;
                            if (k >= a) break;
                        }
                    }
                    if (k >= a) break;
                }

                pool.wait();
            }
        }


        QueryPerformanceCounter(&curFreq);

        double latency = (double)(curFreq.QuadPart - prevFreq.QuadPart) / (double)cpuFreq.QuadPart;
        wchar_t text[100] = {};

        swprintf_s(text, 100, L"FixedUpdate Latency : %f", latency);

        SetWindowText(mHwnd, text);
    }

    void PixelGrid::updateChunk(int x, int y)
    {
        //std::vector<Element*> elementsToMove;

        std::vector<int> numbers;
        for (int i = 0; i <= 63; ++i) {
            numbers.push_back(i);
        }

        // 랜덤 생성기 초기화
        std::random_device rd;
        std::mt19937 g(rd());

        // 벡터 섞기
        std::shuffle(numbers.begin(), numbers.end(), g);

        for (int i = 63; i >= 0; i--)
        {
            for (int j : numbers)//for (int j = 63; j >= 0; j--)
            {
                if (mElements[y * 64 + i][x * 64 + j] == nullptr) continue;

                //if (true/*mElements[y * 64 + i][x * 64 + j]->isFalling*/)
                //{
                    //if (mElements[y * 64 + i][x * 64 + j]->IsUpdate()) continue;
                mElements[y * 64 + i][x * 64 + j]->Update();
                //elementsToMove.push_back(mElements[y * 64 + i][x * 64 + j]);
          /*  }*/

            }
        }

        //for (const auto& element : elementsToMove)
        //{
        //    element->Update();
        //    element->isUpdate = true;
        //}
        //registerElements(elementsToMove);
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
            512,   // Destination rectangle width
            512,  // Destination rectangle height
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
                    ::Rectangle(mHdc, j * 64 - (int)x, (i) * 64 + (int)y, (j + 1) * (64) - (int)x, (i + 1) * (64) + (int)y);
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

    void PixelGrid::Release()
    {
        //pool.Shutdown();
        delete mImage;
        DeleteObject(mBackBuffer);
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
        {
            std::swap(mElements[y1][x1], mElements[y2][x2]);

                mElements[y1][x1]->SetPos(x1, y1);
                mElements[y2][x2]->SetPos(x2, y2);


            uint32_t color;

            memcpy(&color, &mPixelColor[(y1 * mWidth + x1) * 4], 4);
            memcpy(&mPixelColor[(y1 * mWidth + x1) * 4], &mPixelColor[(y2 * mWidth + x2) * 4], 4);
            memcpy(&mPixelColor[(y2 * mWidth + x2) * 4], &color, 4);
        }
    }

    void PixelGrid::SetActiveChunks(int x, int y)
    {
        if (x % 64 == 0)
            PixelGrid::SetActiveChunk(x - 1, y);
        else if (x % 64 == 63)
            PixelGrid::SetActiveChunk(x + 1, y);

        if (y % 64 == 0)
            PixelGrid::SetActiveChunk(x, y - 1);
        else if (y % 64 == 63)
            PixelGrid::SetActiveChunk(x, y + 1);

        SetActiveChunk(x, y);
    }

    void PixelGrid::SetActiveChunk(int x, int y)
    {
        x /= 64;
        y /= 64;

        mChunks[y][x].SetActive();
    }

    void PixelGrid::SetPixelColor(int x, int y, uint32_t color)
    {
        memcpy(&mPixelColor[(y * mWidth + x) * 4], &color, 4);
    }

    void PixelGrid::CreateElement(int x, int y, Element* element)
    {
        //delete mElements[y][x];
        mElements[y][x] = element;
        mElements[y][x]->SetPos(x, y);
        memcpy(&mPixelColor[(y * mWidth + x) * 4], mElements[y][x]->GetColor(), 4);
    }
    std::mutex mx;
    void PixelGrid::DeleteElement(int x, int y)
    {
        // 멀티 쓰레드 안전한지 확인
        
        mx.lock();
        mDeadElement.push_back(mElements[y][x]);
        mx.unlock();

        mElements[y][x] = new EmptyElement();
        mElements[y][x]->SetPos(x, y);
        memcpy(&mPixelColor[(y * mWidth + x) * 4], EMPTY_COLOR, 4);
       
    }

    void PixelGrid::SetImage(int x, int y, std::shared_ptr<Texture> texture, std::shared_ptr<Texture> texture_visual)
    {
        return;

        if (x < 0 || y < 0) return;
        uint8_t* texPixels = texture->GetPixels();
        DXGI_FORMAT format = texture->GetFormat();
        UINT texSize = texture->GetImageSize() * 4;
        UINT texWidth = texture->GetImageWidth() * 4;

        uint8_t* visualPixel = nullptr;
        uint8_t* texVisualPixels = nullptr;
        if (texture_visual != nullptr)
            texVisualPixels = texture_visual->GetPixels();

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
                if (texture_visual != nullptr)
                    visualPixel = texVisualPixels + i;

                if (*(currPixel + 3) != (uint8_t)0)
                {
                    uint32_t color;
                    memcpy(&color, currPixel, 4);

                    if (color == 0xFFFFFFFF)
                    {
                        if (mElements[col][row / 4]->GetType() == eElementType::Empty)
                        {
                            memcpy(&mPixelColor[(col * mWidth + row / 4) * 4], WHITE_COLOR, 4);
                        }
                    }

                    else if (color == 0xFF0A3344 || color == 0xFF0A3355)
                    {
                        if (mElements[col][row / 4]->GetType() == eElementType::Empty)
                        {
                            delete mElements[col][row / 4];

                            mElements[col][row / 4] = new Rock();
                            mElements[col][row / 4]->SetPos(row / 4, col);

                            if (texVisualPixels != nullptr)
                            {
                                if (*(visualPixel + 3) == (uint8_t)0)
                                    memcpy(&mPixelColor[(col * mWidth + row / 4) * 4], mElements[col][row / 4]->GetColor(), 4);
                                else
                                    memcpy(&mPixelColor[(col * mWidth + row / 4) * 4], visualPixel, 4);
                            }
                            else
                            {
                                memcpy(&mPixelColor[(col * mWidth + row / 4) * 4], mElements[col][row / 4]->GetColor(), 4);
                            }
                        }

                        SetActiveChunks(row / 4, col);
                    }
                    else if (color == 0xFF000042)
                    {
                        if (mElements[col][row / 4]->GetType() == eElementType::Empty)
                        {
                            delete mElements[col][row / 4];

                            mElements[col][row / 4] = new Water();
                            mElements[col][row / 4]->SetPos(row / 4, col);
                            memcpy(&mPixelColor[(col * mWidth + row / 4) * 4], mElements[col][row / 4]->GetColor(), 4);
                            SetActiveChunks(row / 4, col);
                        }
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
                if (texture_visual != nullptr)
                    visualPixel = texVisualPixels + i;

                if (*(currPixel + 3) != (uint8_t)0)
                {
                    uint32_t color;
                    memcpy(&color, currPixel, 4);

                    if (color == 0xFFFFFFFF)
                    {
                        if (mElements[col][row / 4]->GetType() == eElementType::Empty)
                        {
                            memcpy(&mPixelColor[(col * mWidth + row / 4) * 4], WHITE_COLOR, 4);
                        }
                    }
                    else if (color == 0xFF44330A || color == 0xFF55330A) // ABGR
                    {
                        if (mElements[col][row / 4]->GetType() == eElementType::Empty)
                        {
                            delete mElements[col][row / 4];
                            mElements[col][row / 4] = new Rock();
                            mElements[col][row / 4]->SetPos(row / 4, col);

                            if (texVisualPixels != nullptr)
                            {
                                if (*(visualPixel + 3) == (uint8_t)0)
                                {
                                    memcpy(&mPixelColor[(col * mWidth + row / 4) * 4], mElements[col][row / 4]->GetColor(), 4);
                                }
                                else
                                {
                                    memcpy(&mPixelColor[(col * mWidth + row / 4) * 4], visualPixel, 4);
                                }
                            }
                            else
                            {
                                memcpy(&mPixelColor[(col * mWidth + row / 4) * 4], mElements[col][row / 4]->GetColor(), 4);
                            }
                        }
                        SetActiveChunks(row / 4, col);
                    }
                    else if (color == 0xFF420000)
                    {
                        if (mElements[col][row / 4]->GetType() == eElementType::Empty)
                        {
                            delete mElements[col][row / 4];
                            mElements[col][row / 4] = new Water();
                            mElements[col][row / 4]->SetPos(row / 4, col);

                            memcpy(&mPixelColor[(col * mWidth + row / 4) * 4], mElements[col][row / 4]->GetColor(), 4);
                            SetActiveChunks(row / 4, col);
                        }
                    }

                }
            }
        }
    }

    Element* PixelGrid::GetElement(int x, int y)
    {
        if (x < 0 || x >= mWidth || y < 0 || y >= mHeight)
        {
            return nullptr;
        }
        else
        {
            return mElements[y][x];
        }
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
        DeleteObject(mBitmap);
    }

    void PixelGridColor::Update(std::vector<uint8_t>& pixelColor, HDC BackDC, float x, float y)
    {
        memcpy(bits, pixelColor.data(), pixelColor.size());
        BitBlt(BackDC, 0, 0, 2048, 2048, mHdc, (int)x, (int)-y, SRCCOPY);
    }
}

