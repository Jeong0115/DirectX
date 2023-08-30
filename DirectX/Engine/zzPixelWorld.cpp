#include "zzPixelWorld.h"
#include "zzPixelGrid.h"
#include "zzPixelUpdater.h"

#include "zzRenderer.h"
#include "zzInput.h"
#include "zzApplication.h"

#include "zzThreadPool.h"
#include "zzTexture.h"
#include <algorithm>

#include "zzInput.h"

#include "zzTime.h"

#define STB_HERRINGBONE_WANG_TILE_IMPLEMENTATION
#include "..\External\Herringbone\include\stb_herringbone_wang_tile.h"

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
    ThreadPool PixelWorld::threadPool(1); 
    uint16_t PixelWorld::FrameCount = 0;

    float PixelWorld::mTime = 0.f;
    //std::vector<std::vector<Box2dWorld::StaticElementInfo>>& PixelWorld::mStaticElements = {};

    PixelWorld::PixelWorld()
    {

    }

    PixelWorld::~PixelWorld()
    {
        int c = 0;
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

        mElementMap.insert({ 'w', WATER });
        mElementMap.insert({ 's', SAND });
        mElementMap.insert({ 'r', ROCK });
        mElementMap.insert({ 't', WOOD });
        mElementMap.insert({ 'f', FIRE });
        mElementMap.insert({ 'e', {eElementType::EMPTY, eElementMove::NONE, eElementUpdate::NONE, (uint32_t)eElementColor::EMPTY, L"DeleteEmpty", math::Vector2(0.f, 0.f)} });
        mSelectElement = mElementMap.find('e')->second;

        CreateNewWorld();
        Box2dWorld::Initialize();
    } 

    void PixelWorld::Update()
    {  

       
        //RemoveEmptyChunks();

        mTime += (float)Time::DeltaTime();

        if (mTime <= 1.f / 100.f)
        {
            mTime += (float)Time::DeltaTime();
            return;
        }
        renderer::debugMeshs.clear();
        mTime -= 1.f / 100.f;

        std::vector<std::vector<Box2dWorld::StaticElementInfo>>& mStaticElements = Box2dWorld::GetTemp();

        std::vector<int> a = {};
        for (int i = 0; i < mStaticElements.size(); i++)
        {
            DeleteStaticElement(mStaticElements[i], i, a);
        }


        Box2dWorld::ReconstructBody(a);


        Box2dWorld::Update();

        std::vector<std::vector<Box2dWorld::StaticElementInfo>>& mStaticElements1 = Box2dWorld::GetTemp();

        for (int i = 0; i < mStaticElements1.size(); i++)
        {
            MoveStaticElement(mStaticElements1[i]);
        }

        DrawPixels();
               
        for (int i = 0; i < 4; i++)
        {
            for (PixelChunkMap* chunkMap : mChunkMaps)
            {
                chunkMap->UpdateStep(i);
            };
            threadPool.wait();
        }
        FrameCount++;

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
        /*if (PixelChunk* chunk = GetChunk(x, y))
        {
            chunk->SwapElement(x, y, element);
        }*/
    }

    void PixelWorld::SwapElement(int x, int y, int xto, int yto)
    {
 
        //if (PixelChunk* src = GetChunk(x, y))
        //{
        //    if (PixelChunk* dst = GetChunk(xto, yto))  // 수정예정
        //    {
        //        if (x == xto && y == yto) // 왜 이것땜에 모래가 물 으로 가라앉았다가 말까 할까...
        //        {
        //            return;
        //        }
        //       
        //        int pingX = 0, pingY = 0; 

        //        if (x == src->mStartX)                           pingX = -1;
        //        if (x == src->mStartX + src->mWidth - 1)      pingX = 1;
        //        if (y == src->mStartY)                           pingY = -1;
        //        if (y == src->mStartY + src->mHeight - 1)     pingY = 1;

        //        if (pingX != 0)               PixelWorld::KeepAlive(x + pingX, y);
        //        if (pingY != 0)               PixelWorld::KeepAlive(x, y + pingY);
        //        if (pingX != 0 && pingY != 0) PixelWorld::KeepAlive(x + pingX, y + pingY);

        //        dst->ResiterChanges(src, x, y, xto, yto);
        //    }
        //}
    }

    void PixelWorld::InsertElement(int x, int y, const Element& element)
    {
        if (PixelChunk* chunk = GetChunk(x, y))
        {
            chunk->InsertElement(x, y, element);
        }
    }

    void PixelWorld::DeleteElement(int x, int y)
    {
        if (PixelChunk* chunk = GetChunk(x, y))
        {
            chunk->InsertElement(x, y, EMPTY);
        }
    }

    void PixelWorld::MoveStaticElement(std::vector<Box2dWorld::StaticElementInfo>& elements)
    {
        for (auto& element : elements)
        {
            if (PixelChunk* chunk = GetChunk(element.x, element.y))
            {

                chunk->RegisterElement(element.x, element.y, element.element);
            }
        }
    }

    void PixelWorld::DeleteStaticElement(std::vector<Box2dWorld::StaticElementInfo>& elements, int index, std::vector<int>& a)
    {
        bool cal = false;


        for (auto& element : elements)
        {
            PixelChunk* chunk = GetChunk(element.x, element.y);

            if (chunk == nullptr)
                continue;

            Element& pixelWorldElement = chunk->GetElement(element.x, element.y);
            size_t index = chunk->GetIndex(element.x, element.y);
            chunk->mStaticCount[index]--;

            if (pixelWorldElement.Type != eElementType::SOLID)
            {
                cal = true;
            }

            if(chunk->mStaticCount[index] == 0)
            {
                element.element = pixelWorldElement;
                chunk->InsertElement(element.x, element.y, EMPTY);
            }           
        }
        if (cal)
        {
            a.push_back(index);
            cal = false;
        }
    }

    void PixelWorld::CreateNewWorld()
    {
        srand(time(NULL));

        int x = 260;
        int y = 260;

        cv::Mat wangTileImage = cv::imread("..\\Resources\\Texture\\WangTiles\\Coalmine\\coalmine.png", cv::IMREAD_COLOR);
        cv::cvtColor(wangTileImage, wangTileImage, cv::COLOR_BGR2RGB);
        stbhw_tileset tileset;

        stbhw_build_tileset_from_image(&tileset, (unsigned char*)wangTileImage.data, wangTileImage.cols * 3, wangTileImage.cols, wangTileImage.rows);
        unsigned char* tileData = (unsigned char*)malloc(3 * x * y);

        stbhw_generate_image(&tileset, NULL, tileData, x*3, x, y);
        cv::Mat randTileImage(x, y, CV_8UC3, tileData);

        cv::Mat rock = cv::imread("..\\Resources\\Texture\\Material\\edge\\rock.png", cv::IMREAD_COLOR);
        cv::cvtColor(rock, rock, cv::COLOR_BGR2RGB);

        cv::Mat edge_rock_land = cv::imread("..\\Resources\\Texture\\Material\\edge\\edge_rock_land.png", cv::IMREAD_COLOR);
        cv::cvtColor(edge_rock_land, edge_rock_land, cv::COLOR_BGR2RGB);

        cv::Mat edge_rock_wall_temp = cv::imread("..\\Resources\\Texture\\Material\\edge\\edge_rock_wall_temp.png", cv::IMREAD_COLOR);
        cv::cvtColor(edge_rock_wall_temp, edge_rock_wall_temp, cv::COLOR_BGR2RGB);

        cv::Mat edge_rock_slope = cv::imread("..\\Resources\\Texture\\Material\\edge\\edge_rock_slope.png", cv::IMREAD_COLOR);
        cv::cvtColor(edge_rock_slope, edge_rock_slope, cv::COLOR_BGR2RGB);

        cv::Mat edge_rock_convex = cv::imread("..\\Resources\\Texture\\Material\\edge\\edge_rock_convex.png", cv::IMREAD_COLOR);
        cv::cvtColor(edge_rock_convex, edge_rock_convex, cv::COLOR_BGR2RGB);

        cv::Mat edge_rock_alone = cv::imread("..\\Resources\\Texture\\Material\\edge\\edge_rock_alone.png", cv::IMREAD_COLOR);
        cv::cvtColor(edge_rock_alone, edge_rock_alone, cv::COLOR_BGR2RGB);

        cv::Mat wood = cv::imread("..\\Resources\\Texture\\Material\\edge\\wood.png", cv::IMREAD_COLOR);
        cv::cvtColor(wood, wood, cv::COLOR_BGR2RGB);

        cv::Mat edge_wood_temp = cv::imread("..\\Resources\\Texture\\Material\\edge\\edge_wood_temp.png", cv::IMREAD_COLOR);
        cv::cvtColor(edge_wood_temp, edge_wood_temp, cv::COLOR_BGR2RGB);

        cv::Mat edge_wood_land = cv::imread("..\\Resources\\Texture\\Material\\edge\\edge_wood_land.png", cv::IMREAD_COLOR);
        cv::cvtColor(edge_wood_land, edge_wood_land, cv::COLOR_BGR2RGB);

        cv::Mat edge_wood_corner = cv::imread("..\\Resources\\Texture\\Material\\edge\\edge_wood_corner.png", cv::IMREAD_COLOR);
        cv::cvtColor(edge_wood_corner, edge_wood_corner, cv::COLOR_BGR2RGB);

        cv::Mat wood_vertical_temp = cv::imread("..\\Resources\\Texture\\Material\\edge\\wood_vertical_temp.png", cv::IMREAD_COLOR);
        cv::cvtColor(wood_vertical_temp, wood_vertical_temp, cv::COLOR_BGR2RGB);

        cv::Scalar white(255, 255, 255);

        cv::Mat mask;
        cv::inRange(randTileImage, white, white, mask);

        cv::Scalar color_wood(65, 63, 36);
        cv::Scalar color_wood_vertical(65, 63, 58);

        cv::Mat mask_wood;
        cv::inRange(randTileImage, color_wood, color_wood_vertical, mask_wood);

        mask = mask | mask_wood;

        std::bitset<4> surrounding;

        int dx[] = { 0, 1, 0,-1 };
        int dy[] = { -1,0, 1, 0 };

        for (int i = 0; i < y; i++)
        {
            for (int j = 0; j < x; j++)
            {           
                uint32_t color = Vec3bToColor(randTileImage.at<cv::Vec3b>(i, j));

                if (color == 0xFFFFFFFF)
                {
                    for (int dir = 0; dir < 4; dir++)
                    {
                        if (j + dx[dir] >= 0 && j + dx[dir] < mask.cols && i + dy[dir] >= 0 && i + dy[dir] < mask.rows)
                        {
                            surrounding[dir] = (mask.at<uchar>(i + dy[dir], j + dx[dir]) > 0) ? 1 : 0;
                        }
                    }

                    if (surrounding.count() == 4)
                    {
                        int rand = randi(7);
                        cv::Rect rect(rand * 10, 0, 10, 10);
                        cv::Mat rotateImg;

                        rock(rect).copyTo(rotateImg);

                        InsertElementFromImage(i, j, rotateImg, ROCK);
                    }
                    else if (surrounding.count() == 3)
                    {
                        int rand = randi(7);
                        bool flip = randi(1) > 0 ? true : false;

                        cv::Rect rect(rand * 10, 0, 10, 10);
                        cv::Mat rotateImg;

                        if (!surrounding[0])
                        {
                            edge_rock_land(rect).copyTo(rotateImg);

                            if (flip)
                            {
                                cv::flip(rotateImg, rotateImg, 1);
                            }
                        }
                        else if (!surrounding[1])
                        {
                            cv::transpose(edge_rock_land(rect), rotateImg);
                            cv::flip(rotateImg, rotateImg, 1);
                            if (flip)
                            {
                                cv::flip(rotateImg, rotateImg, 0);
                            }
                        }
                        else if (!surrounding[2])
                        {
                            edge_rock_land(rect).copyTo(rotateImg);
                            cv::flip(rotateImg, rotateImg, 0);

                            if (flip)
                            {
                                cv::flip(rotateImg, rotateImg, 1);
                            }
                        }
                        else if (!surrounding[3])
                        {
                            cv::transpose(edge_rock_land(rect), rotateImg);

                            if (flip)
                            {
                                cv::flip(rotateImg, rotateImg, 0);
                            }
                        }

                        InsertElementFromImage(i, j, rotateImg, ROCK);
                    }
                    else if (surrounding.count() == 2)
                    {
                        int rand = randi(5);
                        bool flip = randi(1) > 0 ? true : false;
                        cv::Rect rect(rand * 10, 0, 10, 10);
                        cv::Mat rotateImg;

                        if (surrounding[1] && surrounding[2])
                        {
                            edge_rock_slope(rect).copyTo(rotateImg);

                            if (flip)
                            {
                                cv::transpose(rotateImg, rotateImg);
                            }
                        }
                        else if (surrounding[2] && surrounding[3])
                        {
                            edge_rock_slope(rect).copyTo(rotateImg);
                            cv::flip(rotateImg, rotateImg, 1);

                            if (flip)
                            {
                                cv::transpose(rotateImg, rotateImg);
                                cv::flip(rotateImg, rotateImg, -1);
                            }
                        }
                        else if (surrounding[3] && surrounding[0])
                        {
                            edge_rock_slope(rect).copyTo(rotateImg);
                            cv::flip(rotateImg, rotateImg, -1);

                            if (flip)
                            {
                                cv::transpose(rotateImg, rotateImg);
                            }
                        }
                        else if (surrounding[0] && surrounding[1])
                        {
                            edge_rock_slope(rect).copyTo(rotateImg);
                            cv::flip(rotateImg, rotateImg, 0);

                            if (flip)
                            {
                                cv::transpose(rotateImg, rotateImg);
                                cv::flip(rotateImg, rotateImg, -1);
                            }
                        }
                        else
                        {
                            rand = randi(7);
                            rect = cv::Rect(rand * 10, 0, 10, 10);

                            if (surrounding[0] && surrounding[2])
                            {
                                edge_rock_wall_temp(rect).copyTo(rotateImg);
                                cv::transpose(rotateImg, rotateImg);

                                if (flip)
                                {
                                    cv::flip(rotateImg, rotateImg, 0);
                                }

                            }
                            else
                            {
                                edge_rock_wall_temp(rect).copyTo(rotateImg);

                                if (flip)
                                {
                                    cv::flip(rotateImg, rotateImg, 1);
                                }
                            }
                        }

                        InsertElementFromImage(i, j, rotateImg, ROCK);
                    }
                    else if (surrounding.count() == 1)
                    {
                        int rand = randi(5);
                        cv::Rect rect(rand * 10, 0, 10, 10);
                        cv::Mat rotateImg;

                        if (surrounding[2])
                        {
                            rotateImg = edge_rock_convex(rect).clone();
                        }
                        else if (surrounding[3])
                        {
                            cv::transpose(edge_rock_convex(rect), rotateImg);
                            cv::flip(rotateImg, rotateImg, 1);
                        }
                        else if (surrounding[0])
                        {
                            edge_rock_convex(rect).copyTo(rotateImg);
                            cv::flip(rotateImg, rotateImg, -1);
                        }
                        else if (surrounding[1])
                        {
                            cv::transpose(edge_rock_convex(rect), rotateImg);
                            cv::flip(rotateImg, rotateImg, 0);
                        }

                        InsertElementFromImage(i, j, rotateImg, ROCK);
                    }
                    else
                    {
                        int rand = randi(3);
                        cv::Rect rect(rand * 10, 0, 10, 10);
                        cv::Mat rotateImg;

                        edge_rock_alone(rect).copyTo(rotateImg);

                        if (randi(1) > 0)
                        {
                            cv::transpose(rotateImg, rotateImg);
                        }

                        rand = randi(2);
                        if (rand == 2)
                        {
                            cv::flip(rotateImg, rotateImg, -1);
                        }
                        else
                        {
                            cv::flip(rotateImg, rotateImg, rand);
                        }

                        InsertElementFromImage(i, j, rotateImg, ROCK);
                    }

                }
                else if (color == 0xFF413F24)
                {
                    for (int dir = 0; dir < 4; dir++)
                    {
                        if (j + dx[dir] >= 0 && j + dx[dir] < mask.cols && i + dy[dir] >= 0 && i + dy[dir] < mask.rows)
                        {
                            surrounding[dir] = (mask.at<uchar>(i + dy[dir], j + dx[dir]) > 0) ? 1 : 0;
                        }
                    }

                    if (surrounding.count() >= 3)
                    {
                        int rand = randi(3);
                        cv::Rect rect(rand * 10, 0, 10, 10);
                        cv::Mat rotateImg;

                        edge_wood_land(rect).copyTo(rotateImg);

                        InsertElementFromImage(i, j, rotateImg, WOOD);
                    }
                    if (surrounding.count() == 2)
                    {
                        int rand = 0;
                        cv::Rect rect(rand * 10, 0, 10, 10);
                        cv::Mat rotateImg;
                        edge_wood_corner(rect).copyTo(rotateImg);

                        if (surrounding[2] && surrounding[3])
                        {
                            cv::flip(rotateImg, rotateImg, 1);
                        }
                        else if (surrounding[3] && surrounding[0])
                        {
                            cv::flip(rotateImg, rotateImg, -1);
                        }
                        else if (surrounding[0] && surrounding[1])
                        {
                            cv::flip(rotateImg, rotateImg, 0);
                        }
                        else if (surrounding[1] && surrounding[2])
                        {

                        }
                        else
                        {
                            edge_wood_land(rect).copyTo(rotateImg);
                        }
                        InsertElementFromImage(i, j, rotateImg, WOOD);
                    }
                    else if(surrounding.count() == 1)
                    {
                        int rand = 0;
                        cv::Rect rect(rand * 10, 0, 10, 10);
                        cv::Mat rotateImg;
                        edge_wood_temp(rect).copyTo(rotateImg);

                        if (surrounding[3])
                        {
                            RoatateImage(RotateOption::Right90, rotateImg);
                            cv::flip(rotateImg, rotateImg, 0);
                        }
                        else if (surrounding[0])
                        {
                            RoatateImage(RotateOption::Right180, rotateImg);
                        }
                        else if (surrounding[1])
                        {
                            RoatateImage(RotateOption::Right270, rotateImg);
                        }

                        InsertElementFromImage(i, j, rotateImg, WOOD);
                    }
                    else
                    {
                        int rand = randi(3);
                        cv::Rect rect(rand * 10, 0, 10, 10);
                        cv::Mat rotateImg;

                        wood(rect).copyTo(rotateImg);

                        InsertElementFromImage(i, j, rotateImg, WOOD);
                    }
                }
                else if (color == 0xFF413f3A)
                {
                    for (int dir = 0; dir < 4; dir++)
                    {
                        if (j + dx[dir] >= 0 && j + dx[dir] < mask_wood.cols && i + dy[dir] >= 0 && i + dy[dir] < mask_wood.rows)
                        {
                            surrounding[dir] = (mask_wood.at<uchar>(i + dy[dir], j + dx[dir]) > 0) ? 1 : 0;
                        }
                    }

                    if (true)
                    {
                        int rand = 0;
                        cv::Rect rect(rand * 10, 0, 10, 10);
                        cv::Mat rotateImg;

                        wood_vertical_temp(rect).copyTo(rotateImg);

                        InsertElementFromImage(i, j, rotateImg, WOOD);
                    }
                }
                else if (color == 0xFF2F554C)
                {
                    
                    for (int k = i * 10; k < i * 10 + 10; k++)
                    {
                        for (int l = j * 10; l < j * 10 + 10; l++)
                        {
                            InsertElement(l, k, WATER);
                        }
                    }
                }
                else if (color == 0xFF505052 || color == 0xFF3B3B3C)
                {
                    for (int k = i * 10; k < i * 10 + 10; k++)
                    {
                        for (int l = j * 10; l < j * 10 + 10; l++)
                        {
                            Element sand = SAND;
                            sand.Color = RandomSandColor();
                            InsertElement(l, k, sand);
                        }
                    }
                }
            }
        }

        cv::cvtColor(randTileImage, randTileImage, cv::COLOR_RGB2BGR);
        cv::resize(randTileImage, randTileImage, cv::Size(), 2,2, cv::INTER_NEAREST);

        cv::imshow("Generated Map", randTileImage);

        free(tileData);
    }

    void PixelWorld::InsertElementFromImage(int y, int x, const cv::Mat& image, Element& element)
    {
        for (int i = y * 10; i < y * 10 + 10; i++)
        {
            for (int j = x * 10; j < x * 10 + 10; j++)
            {
                cv::Vec3b colorVec = image.at<cv::Vec3b>(i % 10, j % 10);
                uint32_t color = Vec3bToColor(colorVec);

                if (color != 0xFF000000 && color != 0xFFFFFFFF)
                {
                    element.Color = color;
                    InsertElement(j, i, element);
                }
            }
        }
    }

    uint32_t PixelWorld::Vec3bToColor(const cv::Vec3b& vec3b)
    {
        uint32_t color = 0xFF000000;
        color |= (vec3b[0] << 16);
        color |= (vec3b[1] << 8);
        color |= vec3b[2];

        return color;
    }

    void PixelWorld::RoatateImage(RotateOption option, cv::Mat& image)
    {
        switch (option)
        {
        case RotateOption::Right90:
        {
            cv::transpose(image, image);
            cv::flip(image, image, 1);
            break;
        }
        case RotateOption::Right180:
        {
            cv::flip(image, image, -1);
            break;
        }
        case RotateOption::Right270:
        {
            cv::transpose(image, image);
            cv::flip(image, image, 0);
            break;
        }
        default: break;
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
        if (!Application::OnDebugMode) return;

        if (Input::GetKey(eKeyCode::W) || Input::GetKey(eKeyCode::R) || Input::GetKey(eKeyCode::S)
            || Input::GetKey(eKeyCode::E) || Input::GetKey(eKeyCode::F) || Input::GetKey(eKeyCode::T))
        {
            if ((Input::GetKey(eKeyCode::W)))
                mSelectElement = mElementMap.find('w')->second;
            else if ((Input::GetKey(eKeyCode::R)))
                mSelectElement = mElementMap.find('r')->second;
            else if ((Input::GetKey(eKeyCode::S)))
                mSelectElement = mElementMap.find('s')->second;
            else if ((Input::GetKey(eKeyCode::E)))
                mSelectElement = mElementMap.find('e')->second;
            else if ((Input::GetKey(eKeyCode::F)))
                mSelectElement = mElementMap.find('f')->second;
            else if ((Input::GetKey(eKeyCode::T)))
                mSelectElement = mElementMap.find('t')->second;
        }

        
        if (Input::GetKey(eKeyCode::LBUTTON) || Input::GetKey(eKeyCode::RBUTTON) || Input::GetKeyDown(eKeyCode::P) || Input::GetKeyDown(eKeyCode::T))
        {
            HWND Hwnd = Application::GetInst().GetHwnd();
            HWND nowHwnd = GetFocus();

            if (nowHwnd == Hwnd)
            {
                Vector3 pos = Input::GetMouseWorldPos();

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
                    else if (Input::GetKey(eKeyCode::RBUTTON))
                    {
                        for (int y = (int)pos.y - 1; y < pos.y + 1; y++)
                        {
                            for (int x = (int)pos.x - 1; x < pos.x + 1; x++)
                            {
                                InsertElement(x, -y, mSelectElement);
                            }
                        }
                    }
                    else if (Input::GetKeyDown(eKeyCode::P))
                    {
                        Box2dWorld::Draw2(pos.x, -pos.y);
                    }
                    else
                    {
                        
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









