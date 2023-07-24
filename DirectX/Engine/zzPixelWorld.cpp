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
#include "zzTexture.h"

namespace zz
{
    std::vector<uint8_t> PixelWorld::mPixelColor(2048 * 2048 * 4);
    const UINT PixelWorld::mChunkMapWidth = 512;
    const UINT PixelWorld::mChunkMapHeight = 512;
    PixelGridColor* PixelWorld::mImage = new PixelGridColor();

    std::map<char, Element> PixelWorld::mElementMap = {};
    Element PixelWorld::mSelectElement = {};

    std::vector<PixelChunkMap*> PixelWorld::mChunkMaps = {};
    std::unordered_map<std::pair<int, int>, PixelChunkMap*, pair_hash> PixelWorld::mChunkMapLookUp = {};
    ThreadPool PixelWorld::threadPool(4);

    PixelWorld::PixelWorld()
    {

    }

    PixelWorld::~PixelWorld()
    {
    }

    void PixelWorld::Initialize()
    {
        InitializeElement();
        for (int i = 0; i <= 2; i++)
        {
            for (int j = 0; j <= 2; j++)
            {
                CreateChunkMap({ j, i });
            }
        }

        mElementMap.insert({ 'z', WATER });
        mElementMap.insert({ 'x', SAND });
        mElementMap.insert({ 'c', ROCK });
        //mElementMap.insert({ 'o', new Oil });
        //mElementMap.insert({ 'f', new Spark });
        mElementMap.insert({ 'e', EMPTY });
        mSelectElement = mElementMap.find('e')->second;

        /*for (int i = 60; i <= 75; i++)
        {
            for (int j = 20; j <= 30; j++)
            {
                InsertElement(i, j, SAND);
            }
        }

        for (int i = 50; i <= 200; i++)
        {
            for (int j = 50; j <= 55; j++)
            {
                InsertElement(i, j, ROCK);
            }
        }*/

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
                chunkMap->UpdateStep(i);

            };
            threadPool.wait();
        }


        for (int i = 0; i < 4; i++)
        {
            for (PixelChunkMap* chunkMap : mChunkMaps)
            {
                chunkMap->MoveElementStep(i);
            }
            threadPool.wait();
        }

        for (PixelChunkMap* chunkMap : mChunkMaps)
        {
            chunkMap->UpdateRect();
        }
        threadPool.wait();

        mImage->Update(mPixelColor, NULL, 0, 0);
    }

    void PixelWorld::Release()
    {
        for (auto chunkMap : mChunkMaps)
        {
            delete chunkMap;
        }

        delete mImage;
       // std::unordered_map<std::pair<int, int>, PixelChunk*, pair_hash> PixelWorld::mChunkMapLookUp = {};
    }

    PixelChunk* PixelWorld::GetChunk(int x, int y)
    {
        //mx.lock();
        auto location = GetChunkMapLocation(x, y);
        PixelChunkMap* chunkMap = GetChunkMapDirect(location);
        if (chunkMap == nullptr) return nullptr;
        PixelChunk* chunk = chunkMap->GetChunk(x, y);
        
        //mx.unlock();
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

    void PixelWorld::SwapElement(int x, int y, const Element& element)
    {
        if (PixelChunk* chunk = GetChunk(x, y))
        {
            chunk->SwapElement(x, y, element);
        }
    }


    void PixelWorld::SwapElement(int x, int y, int xto, int yto)
    {
 
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

    void PixelWorld::InsertElement(int x, int y, const Element& element)
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
            /*else if ((Input::GetKey(eKeyCode::F)))
                mSelectElement = mElementMap.find('f')->second;
            else if ((Input::GetKey(eKeyCode::O)))
                mSelectElement = mElementMap.find('o')->second;*/
        }

        if (Input::GetKey(eKeyCode::LBUTTON) || Input::GetKey(eKeyCode::RBUTTON))
        {
            HWND Hwnd = Application::GetInst().GetHwnd();
            HWND nowHwnd = GetFocus();

            if (nowHwnd == Hwnd)
            {
                POINT mousePos = {};
                GetCursorPos(&mousePos);
                ScreenToClient(Hwnd, &mousePos);

                Vector3 pos(mousePos.x, mousePos.y, 0.0f);
                Viewport viewport;
                viewport.width = 1600.0f;
                viewport.height = 900.0f;
                viewport.x = 0;
                viewport.y = 0;
                viewport.minDepth = 0.0f;
                viewport.maxDepth = 1.0f;

                pos = viewport.Unproject(pos, Camera::GetGpuProjectionMatrix(), Camera::GetGpuViewMatrix(), Matrix::Identity);

                //float x = (2.0f * mousePos.x) / 1600.f - 1.0f;
                //float y = 1.0f - (2.0f * mousePos.y) / 900.f;

                //Vector3 pos = Vector3(x, y, 1.0f);

                //Matrix inverseVP = (Camera::GetGpuProjectionMatrix() * Camera::GetGpuViewMatrix()).Invert();
                //Matrix mouse = Matrix::CreateTranslation(pos);

                //Matrix worldPos = (mouse * inverseVP);
                //Vector3 worldPosVec = Vector3(worldPos._41, worldPos._42, worldPos._43);
                //worldPosVec /= worldPos._44;

                //Vector3 pos = Vector3(mousePos.x, mousePos.y, 1.0f);
                //Matrix mouse;
                //mouse.Translation(pos);

                //Matrix matrix = Camera::GetGpuProjectionMatrix() * Camera::GetGpuViewMatrix();
                //matrix.Invert();

                //Matrix temp = mouse* matrix;
                //mousePos.x += -800;
                //mousePos.y = (450 - (int)mousePos.y);
          
                //mousePos.x += (1024 / 2  - 256);   //+1024 - 256, -1024 + 256
                //mousePos.y = ( - 1024 / 2 + 256 + 512) - mousePos.y;


                if (pos.x >= 5 && pos.y <= -5)
                {
                    if (Input::GetKey(eKeyCode::LBUTTON))
                    {
                        for (int y = (int)pos.y - 5; y < pos.y + 5; y++)
                        {
                            for (int x = (int)pos.x - 5; x < pos.x + 5; x++)
                            {
                                InsertElement(x, -y, mSelectElement);
                            }
                        }
                    }
                    else
                    {
                        for (int y = (int)pos.y - 0; y < pos.y + 1; y++)
                        {
                            for (int x = (int)pos.x - 0; x < pos.x + 1; x++)
                            {
                                InsertElement(x, -y, mSelectElement);
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

    void PixelWorld::SetImage(int x, int y, std::shared_ptr<Texture> texture, std::shared_ptr<Texture> texture_visual)
    {
        //return;

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
                        /*if (GetElement(row / 4, col).Type == eElementType::EMPTY)
                        {
                            memcpy(&mPixelColor[(col * 2048 + row / 4) * 4], EMPTY_COLOR, 4);
                        }*/
                    }

                    else if (color == 0xFF0A3344 || color == 0xFF0A3355)
                    {
                        if (GetElement(row / 4, col).Type == eElementType::EMPTY)
                        {
                           // mElements[col][row / 4]->SetPos(row / 4, col);
                            InsertElement(row / 4, col, ROCK);

                            if (texVisualPixels != nullptr)
                            {
                                if (*(visualPixel + 3) == (uint8_t)0)
                                    memcpy(&mPixelColor[(col * 2048 + row / 4) * 4], &ROCK.Color, 4);
                                else
                                    memcpy(&mPixelColor[(col * 2048 + row / 4) * 4], visualPixel, 4);
                            }
                            else
                            {
                                memcpy(&mPixelColor[(col * 2048 + row / 4) * 4], &ROCK.Color, 4);
                            }
                        }

                    }
                    else if (color == 0xFF000042)
                    {
                        //if (GetElement(row / 4, col).Type == eElementType::EMPTY)
                        //{

                        //    InsertElement(row / 4, col, ROCK);
                        //    //mElements[col][row / 4]->SetPos(row / 4, col);
                        //    memcpy(&mPixelColor[(col * 2048 + row / 4) * 4], &ROCK.Color, 4);
                        //}
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
                        /*if (GetElement(row / 4, col).Type == eElementType::EMPTY)
                        {
                            memcpy(&mPixelColor[(col * 2048 + row / 4) * 4], WHITE_COLOR, 4);
                        }*/
                    }
                    else if (color == 0xFF44330A || color == 0xFF55330A) // ABGR
                    {
                        if (GetElement(row / 4, col).Type == eElementType::EMPTY)
                        {
                            InsertElement(row / 4, col, ROCK);
                            //mElements[col][row / 4]->SetPos(row / 4, col);

                            if (texVisualPixels != nullptr)
                            {
                                if (*(visualPixel + 3) == (uint8_t)0)
                                {
                                    memcpy(&mPixelColor[(col * 2048 + row / 4) * 4], &ROCK.Color, 4);
                                }
                                else
                                {
                                    memcpy(&mPixelColor[(col * 2048 + row / 4) * 4], visualPixel, 4);
                                }
                            }
                            else
                            {
                                memcpy(&mPixelColor[(col * 2048 + row / 4) * 4], &ROCK.Color, 4);
                            }
                        }
                    }
                    else if (color == 0xFF420000)
                    {
                        /*if (GetElement(row / 4, col).Type == eElementType::EMPTY)
                        {
                            InsertElement(row / 4, col, ROCK);

                            memcpy(&mPixelColor[(col * 2048 + row / 4) * 4], &ROCK.Color, 4);
                        }*/
                    }

                }
            }
        }
    }
}
