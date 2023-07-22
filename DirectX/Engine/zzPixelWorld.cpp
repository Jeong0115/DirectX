#include "zzPixelWorld.h"
#include "zzPixelGrid.h"
#include "zzPixelUpdater.h"
#include "zzSand.h"
#include "zzRock.h"
#include "zzWater.h"
#include "zzSpark.h"
#include "zzOil.h"
#include "zzEmptyElement.h"

#include "zzRenderer.h"
#include "zzInput.h"
#include "zzApplication.h"

#include "zzThreadPool.h"

namespace zz
{
    std::vector<uint8_t> PixelWorld::mPixelColor(2048 * 2048 * 4);
    const UINT PixelWorld::mChunkMapWidth = 512;
    const UINT PixelWorld::mChunkMapHeight = 512;
    PixelGridColor* PixelWorld::mImage = new PixelGridColor();;

    std::map<char, Element*> PixelWorld::mElementMap = {};
    Element* PixelWorld::mSelectElement = {};

    std::vector<PixelChunkMap*> PixelWorld::mChunkMaps = {};
    std::unordered_map<std::pair<int, int>, PixelChunkMap*, pair_hash> PixelWorld::mChunkMapLookUp = {};
    ThreadPool PixelWorld::threadPool(64);

    PixelWorld::PixelWorld()
    {

    }

    PixelWorld::~PixelWorld()
    {
    }

    void PixelWorld::Initialize()
    {
        for (int i = -1; i <= 1; i++)
        {
            for (int j = -1; j <= 1; j++)
            {
                CreateChunkMap({ j, i });
            }
        }

        mElementMap.insert({ 'z', new Water });
        mElementMap.insert({ 'x', new Sand });
        mElementMap.insert({ 'c', new Rock });
        mElementMap.insert({ 'o', new Oil });
        mElementMap.insert({ 'f', new Spark });
        mElementMap.insert({ 'e', new EmptyElement });
        mSelectElement = mElementMap.find('e')->second;

        for (int i = 10; i <= 15; i++)
        {
            for (int j = 10; j <= 100; j++)
            {
                InsertElement(i,j,new Sand());
            }
        }

    }

    void print()
    {
        OutputDebugStringW(L"pirnt!!!!!!!!!!!!!1-----\n");
    }
    void PixelWorld::Update()
    {
        renderer::debugMeshs.clear();
        //RemoveEmptyChunks();
        DrawPixels();


        
        for (int i = 0; i < 4; i++)
        {
           
            for (PixelChunkMap* chunkMap : mChunkMaps)
            {
                //threadPool.enqueue(std::bind(&PixelChunkMap::UpdateStep, chunkMap, i));
                Sleep(0.5);
                chunkMap->UpdateStep(i);

            }
            Sleep(0.5);
            threadPool.wait();
        }


        //threadPool.wait();
        //for(int i=0; i<4; i++)
        //{
        //    for (PixelChunkMap* chunkMap : mChunkMaps)
        //    {
        //        chunkMap->UpdateStep(i);
        //    }
        //}
     

        for (int i = 0; i < 4; i++)
        {
            for (PixelChunkMap* chunkMap : mChunkMaps)
            {
                Sleep(0.5);
                chunkMap->MoveElementStep(i);
            }
            Sleep(0.5);
            threadPool.wait();
        }
        //for (int i = 0; i < 4; i++)
        //{
        //    for (PixelChunkMap* chunkMap : mChunkMaps)
        //    {
        //        threadPool.enqueue(std::bind(&PixelChunkMap::MoveElementStep, chunkMap, i));

        //    }
        //    Sleep(0.5);
        //    threadPool.wait();
        //}

        //for (PixelChunkMap* chunkMap : mChunkMaps)
        //{
        //    chunkMap->MoveElementStep1();
        //    chunkMap->MoveElementStep2();
        //    chunkMap->MoveElementStep3();
        //    chunkMap->MoveElementStep4();
        //}

        for (PixelChunkMap* chunkMap : mChunkMaps)
        {
            chunkMap->UpdateRect();
        }

        mImage->Update(mPixelColor, NULL, 0, 0);
    }

    void PixelWorld::Release()
    {
        for (auto chunkMap : mChunkMaps)
        {
            delete chunkMap;
        }

        for (auto map : mElementMap)
        {
            delete map.second;
            map.second = nullptr;
        }

        delete mImage;
       // std::unordered_map<std::pair<int, int>, PixelChunk*, pair_hash> PixelWorld::mChunkMapLookUp = {};
    }

    PixelChunk* PixelWorld::GetChunk(int x, int y)
    {
        mx.lock();
        auto location = GetChunkMapLocation(x, y);
        PixelChunkMap* chunkMap = GetChunkMapDirect(location);
        PixelChunk* chunk = chunkMap->GetChunk(x, y);
        
        mx.unlock();
        return chunk;
        
    }

    PixelChunkMap* PixelWorld::GetChunkMapDirect(std::pair<int, int> location)
    {
        auto itr = mChunkMapLookUp.find(location);
        auto end = mChunkMapLookUp.end();

        return itr != end ? itr->second : nullptr;
    }

    std::pair<int, int> PixelWorld::GetChunkMapLocation(int x, int y)
    {
        return { floor(float(x) / mChunkMapWidth), floor(float(y) / mChunkMapHeight) };
    }

    void PixelWorld::RemoveEmptyChunks()
    {
        //for (size_t i = 0; i < mChunks.size(); i++) 
        //{
        //    PixelChunk* chunk = mChunks.at(i);

        //    if (chunk->mElementCount == 0) 
        //    {
        //        mChunkMapLookUp.erase(GetChunkLocation(chunk->mStartX, chunk->mStartY));
        //        mChunks[i] = mChunks.back(); mChunks.pop_back();
        //        i--;

        //        delete chunk;
        //        chunk = nullptr;
        //    }
        //}
    }

    void PixelWorld::SwapElement(int x, int y, Element* element)
    {
        if (PixelChunk* chunk = GetChunk(x, y))
        {
            chunk->SwapElement(x, y, element);
        }
    }

    std::mutex asdfs;
    void PixelWorld::SwapElement(int x, int y, int xto, int yto)
    {
        //std::lock_guard<std::mutex> lock(asdfs);
        if (PixelChunk* src = GetChunk(x, y))
        {
            if (PixelChunk* dst = GetChunk(xto, yto))  // 수정예정
            {
                if (x == xto && y == yto) // 왜 이것땜에 모래가 물 으로 가라앉았다가 말까 할까...
                {
                    return;
                }
               
                int pingX = 0, pingY = 0; 

                if (x == src->mStartX)                           pingX = -1;
                if (x == src->mStartX + src->mWidth - 1)      pingX = 1;
                if (y == src->mStartY)                           pingY = -1;
                if (y == src->mStartY + src->mHeight - 1)     pingY = 1;

                if (pingX != 0)               PixelWorld::KeepAlive(x + pingX, y);
                if (pingY != 0)               PixelWorld::KeepAlive(x, y + pingY);
                if (pingX != 0 && pingY != 0) PixelWorld::KeepAlive(x + pingX, y + pingY);

                dst->ResiterChanges(src, x, y, xto, yto);
            }
        }
    }

    void PixelWorld::InsertElement(int x, int y, Element* element)
    {
        if (PixelChunk* chunk = GetChunk(x, y))
        {
            chunk->InsertElement(x, y, element);
        }
    }


    bool PixelWorld::InBounds(int x, int y)
    {
        if (PixelChunk* chunk = GetChunk(x, y)) 
        {
            return chunk->InBounds(x, y);
        }

        return false;
    }

    void PixelWorld::DrawPixels()
    {
        if (Input::GetKey(eKeyCode::Z) || Input::GetKey(eKeyCode::X) || Input::GetKey(eKeyCode::C)
            || Input::GetKey(eKeyCode::E) || Input::GetKey(eKeyCode::O) || Input::GetKey(eKeyCode::F))
        {
            if ((Input::GetKey(eKeyCode::Z)))
                mSelectElement = mElementMap.find('z')->second;
            else if ((Input::GetKey(eKeyCode::X)))
                mSelectElement = mElementMap.find('x')->second;
            else if ((Input::GetKey(eKeyCode::C)))
                mSelectElement = mElementMap.find('c')->second;
            else if ((Input::GetKey(eKeyCode::E)))
                mSelectElement = mElementMap.find('e')->second;
            else if ((Input::GetKey(eKeyCode::F)))
                mSelectElement = mElementMap.find('f')->second;
            else if ((Input::GetKey(eKeyCode::O)))
                mSelectElement = mElementMap.find('o')->second;
        }

        if (Input::GetKey(eKeyCode::LBUTTON) || Input::GetKeyDown(eKeyCode::RBUTTON))
        {
            HWND Hwnd = Application::GetInst().GetHwnd();
            HWND nowHwnd = GetFocus();

            if (nowHwnd == Hwnd)
            {
                POINT mousePos = {};
                GetCursorPos(&mousePos);
                ScreenToClient(Hwnd, &mousePos);

                mousePos.x += -800;
                mousePos.y = (450 - (int)mousePos.y);
          
                mousePos.x += (1024 / 2  - 256);   //+1024 - 256, -1024 + 256
                mousePos.y = ( - 1024 / 2 + 256 + 512) - mousePos.y;

                if (mousePos.x > 15 && mousePos.y > 15)
                {
                    if (Input::GetKey(eKeyCode::LBUTTON))
                    {
                        for (int y = (int)mousePos.y - 10; y < mousePos.y + 10; y++)
                        {
                            for (int x = (int)mousePos.x - 10; x < mousePos.x + 10; x++)
                            {
                                InsertElement(x, y, mSelectElement->Clone());
                            }
                        }
                    }
                    else
                    {
                        for (int y = (int)mousePos.y - 2; y < mousePos.y + 0; y++)
                        {
                            for (int x = (int)mousePos.x - 2; x < mousePos.x + 0; x++)
                            {
                                InsertElement(x, y, mSelectElement->Clone());
                            }
                        }
                    }
                }
            }
        }
    }

    PixelChunkMap* PixelWorld::CreateChunkMap(std::pair<int, int> location)
    {
        auto [lx, ly] = location;

        if (lx < -10 || ly < -10 || lx >  10 || ly >  10) 
        {
            return nullptr;
        }

        PixelChunkMap* chunkMap = new PixelChunkMap(mChunkMapWidth, mChunkMapHeight, lx, ly);

        mChunkMapLookUp.insert({ location, chunkMap });
        mChunkMaps.push_back(chunkMap);

        return chunkMap;
    }
}
