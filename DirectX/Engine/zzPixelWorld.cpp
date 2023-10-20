#include "zzPixelWorld.h"
#include "zzPixelGrid.h"

#include "zzRenderer.h"
#include "zzInput.h"
#include "zzApplication.h"

#include "zzThreadPool.h"
#include "zzTexture.h"
#include <algorithm>

#include "zzInput.h"
#include "zzTransform.h"
#include "zzTime.h"

#include "zzVegetation.h"
#include "zzTransform.h"

#include "zzCentipede.h"

#define STB_HERRINGBONE_WANG_TILE_IMPLEMENTATION
#include "..\External\Herringbone\include\stb_herringbone_wang_tile.h"
#include "zzGraphicsDevice.h"
#include "zzShotGunner_Weak.h"
#include "zzZombie_weak.h"

namespace zz
{
    std::vector<uint32_t> PixelWorld::mPixelColor(2048 * 1536);
    int PixelWorld::mWorldWidth = 1536;
    int PixelWorld::mWorldHeight = 2048;
    const UINT PixelWorld::mChunkMapWidth = 512;
    const UINT PixelWorld::mChunkMapHeight = 512;
    PixelGridColor* PixelWorld::mImage = new PixelGridColor(mWorldWidth, mWorldHeight);

    std::map<char, Element> PixelWorld::mElementMap = {};
    Element PixelWorld::mSelectElement = {};

    std::vector<PixelChunkMap*> PixelWorld::mChunkMaps = {};
    std::unordered_map<std::pair<int, int>, PixelChunkMap*, pair_hash> PixelWorld::mChunkMapLookUp = {};
    std::unordered_map<std::wstring, PixelWorld::MaterialImage*> PixelWorld::mMaterialImages = {};
    ThreadPool PixelWorld::threadPool(4);
    uint16_t PixelWorld::FrameCount = 0;

    float PixelWorld::mTime = 0.f;

    PixelWorld::PixelWorld()
    {

    }

    PixelWorld::~PixelWorld()
    {
    }

    void PixelWorld::Initialize()
    {
        InitializeElement();
        for (int i = 0; i <= 3; i++)
        {
            for (int j = 0; j <= 2; j++)
            {
                CreateChunkMap({ j, i });
            }
        }


        mElementMap.insert({ 'w', WATER });
        mElementMap.insert({ 'o', OIL });
        mElementMap.insert({ 's', SAND });
        mElementMap.insert({ 'r', ROCK });
        mElementMap.insert({ 't', WOOD });
        mElementMap.insert({ 'f', FIRE });
        mElementMap.insert({ 'g', SMOKE });
        mElementMap.insert({ 'e', EMPTY });
        mSelectElement = mElementMap.find('e')->second;
        loadMaterialImage();

        graphics::GetDevice()->CreateVisibility((UINT)mWorldWidth, (UINT)mWorldHeight);

        CreateNewWorld();
        Box2dWorld::Initialize();
    }

    void PixelWorld::Update()
    {
        mTime += (float)Time::DeltaTime();

        if (mTime <= 1.f / 100.f)
        {
            mTime += (float)Time::DeltaTime();
            return;
        }
        renderer::debugMeshs.clear();
        mTime -= 1.f / 100.f;

        std::vector<std::pair<int, int>> aroundMap;

        //Vector3 playerPos = Input::GetPlayer()->GetPosition();
        Vector3 playerPos = Input::GetMouseWorldPos();

        float posX = playerPos.x / 512.f;
        float posY = -playerPos.y / 512.f;

        aroundMap.push_back({ (int)posX, (int)posY });

        int nearX = (int)(posX + 0.5f) == (int)posX ? (int)posX - 1 : (int)(posX + 0.5f);
        int nearY = (int)(posY + 0.5f) == (int)posY ? (int)posY - 1 : (int)(posY + 0.5f);

        aroundMap.push_back({ (int)nearX, (int)posY });
        aroundMap.push_back({ (int)posX, (int)nearY });
        aroundMap.push_back({ (int)nearX, (int)nearY });

        std::vector<PixelChunkMap*> updateMaps;

        for (int i = 0; i < 4; i++)
        {
            PixelChunkMap* chunkMap = GetChunkMapDirect(aroundMap[i]);

            if (chunkMap != nullptr)
            {
                updateMaps.push_back(chunkMap);
            }
        }

        DrawPixels();

        static int k = 0;
        for (int i = 0; i < 4; i++)
        {
            k++;
            if (k >= 4)
                k -= 4;

            //for (PixelChunkMap* chunkMap : mChunkMaps)
            //{
            //    chunkMap->UpdateStep(k);
            //};
            //
            for (PixelChunkMap* chunkMap : updateMaps)
            {
                chunkMap->UpdateStep(k);
            };

        }
        k++;
        threadPool.wait();

        if (FrameCount == std::numeric_limits<uint16_t>::max())
        {
            FrameCount = 0;
        }
        else
        {
            FrameCount++;
        }

        //for (PixelChunkMap* chunkMap : mChunkMaps)
        //{
        //    chunkMap->UpdateRect();

        //}

        for (PixelChunkMap* chunkMap : updateMaps)
        {
            chunkMap->UpdateRect();
        };


        mImage->Update(mPixelColor, NULL, 0, 0);

        Temp();
        threadPool.wait();
        //threadPool.enqueue([=]() { Temp(); });
    }

    void PixelWorld::Release()
    {
        for (auto chunkMap : mChunkMaps)
        {
            delete chunkMap;
        }

        for (auto iter = mMaterialImages.begin(); iter != mMaterialImages.end(); )
        {
            delete iter->second;
            iter->second = nullptr;

            iter++;
        }
        mMaterialImages.clear();

        delete mImage;

        Box2dWorld::Release();
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

    void PixelWorld::InsertElement(int x, int y, const Element& element)
    {
        if (PixelChunk* chunk = GetChunk(x, y))
        {
            chunk->InsertElement(x, y, element);
        }
    }

    void PixelWorld::InsertElementIfEmpty(int x, int y, const Element& element)
    {
        if (PixelChunk* chunk = GetChunk(x, y))
        {
            chunk->InsertElementIfEmpty(x, y, element);
        }
    }

    void PixelWorld::InsertEndingMat(int x, int y)
    {
        if (PixelChunk* chunk = GetChunk(x, y))
        {
            if (chunk->GetElement(x, y).Type != eElementType::EMPTY)
            {
                Element gold = SAND;
                gold.Color = getMaterialColor(L"gold", x, y);
                chunk->InsertElement(x, y, gold);
            }
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
                chunk->RenewalBody();
            }
        }
    }

    void PixelWorld::DeleteStaticElement(std::vector<Box2dWorld::StaticElementInfo>& elements)
    {
        for (auto& element : elements)
        {
            PixelChunk* chunk = GetChunk(element.x, element.y);

            if (chunk == nullptr)
                continue;

            Element& pixelWorldElement = chunk->GetElement(element.x, element.y);
            size_t index = chunk->GetIndex(element.x, element.y);
            if (chunk->mStaticCount[index] > 0)
            {
                chunk->mStaticCount[index]--;
            }

            if (pixelWorldElement.Type != eElementType::SOLID)
            {
                //cal = true;
            }

            if (chunk->mStaticCount[index] == 0)
            {
                element.element = pixelWorldElement;

                //if (*element.element.destoryBody)
                //{
                //    int a = 0;
                //}
                chunk->DelteElement(element.x, element.y);
            }
        }

    }

    void PixelWorld::CreateNewWorld()
    {
        srand(time(NULL));

        int x = 260;
        int y = 260;

        int* a = new int; // 왜 릭 나냐 모르겠다...
        cv::Mat wangTileImage = cv::imread("..\\Resources\\Texture\\WangTiles\\Coalmine\\coalmine.png", cv::IMREAD_COLOR);
        int* b = new int;
        //int yux = wangTileImage.u->refcount;
        //wangTileImage.deallocate();
        //int eqwd = wangTileImage.u->refcount;

        cv::cvtColor(wangTileImage, wangTileImage, cv::COLOR_BGR2RGB);

        stbhw_tileset tileset;

        stbhw_build_tileset_from_image(&tileset, (unsigned char*)wangTileImage.data, wangTileImage.cols * 3, wangTileImage.cols, wangTileImage.rows);

        unsigned char* tileData = (unsigned char*)malloc(3 * x * y);

        stbhw_generate_image(&tileset, NULL, tileData, x * 3, x, y);
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

        cv::Mat edge_wood_center_0 = cv::imread("..\\Resources\\Texture\\Material\\edge\\edge_wood_center_0.png", cv::IMREAD_COLOR);
        cv::cvtColor(edge_wood_center_0, edge_wood_center_0, cv::COLOR_BGR2RGB);

        cv::Mat edge_wood_center_1 = cv::imread("..\\Resources\\Texture\\Material\\edge\\edge_wood_center_1.png", cv::IMREAD_COLOR);
        cv::cvtColor(edge_wood_center_1, edge_wood_center_1, cv::COLOR_BGR2RGB);

        cv::Mat edge_wood_corner = cv::imread("..\\Resources\\Texture\\Material\\edge\\edge_wood_corner.png", cv::IMREAD_COLOR);
        cv::cvtColor(edge_wood_corner, edge_wood_corner, cv::COLOR_BGR2RGB);

        cv::Mat edge_wood_convex = cv::imread("..\\Resources\\Texture\\Material\\edge\\edge_wood_convex.png", cv::IMREAD_COLOR);
        cv::cvtColor(edge_wood_convex, edge_wood_convex, cv::COLOR_BGR2RGB);

        cv::Mat edge_wood_up = cv::imread("..\\Resources\\Texture\\Material\\edge\\edge_wood_up.png", cv::IMREAD_COLOR);
        cv::cvtColor(edge_wood_up, edge_wood_up, cv::COLOR_BGR2RGB);

        cv::Mat edge_wood_line = cv::imread("..\\Resources\\Texture\\Material\\edge\\edge_wood_line.png", cv::IMREAD_COLOR);
        cv::cvtColor(edge_wood_line, edge_wood_line, cv::COLOR_BGR2RGB);

        cv::Mat edge_wood_vertical = cv::imread("..\\Resources\\Texture\\Material\\edge\\edge_wood_vertical.png", cv::IMREAD_COLOR);
        cv::cvtColor(edge_wood_vertical, edge_wood_vertical, cv::COLOR_BGR2RGB);

        cv::Mat edge_wood_vertical_convex = cv::imread("..\\Resources\\Texture\\Material\\edge\\edge_wood_vertical_convex.png", cv::IMREAD_COLOR);
        cv::cvtColor(edge_wood_vertical_convex, edge_wood_vertical_convex, cv::COLOR_BGR2RGB);


        cv::Scalar white(255, 255, 255);

        cv::Mat mask_rock;
        cv::inRange(randTileImage, white, white, mask_rock);

        cv::Scalar color_wood(65, 63, 36);
        cv::Scalar color_wood_vertical(65, 63, 58);

        cv::Mat mask_wood;
        cv::inRange(randTileImage, color_wood, color_wood_vertical, mask_wood);

        cv::Mat mask;
        mask = mask_rock | mask_wood;

        std::bitset<4> surrounding;

        int dx[] = { 0, 1, 0,-1 };
        int dy[] = { -1,0, 1, 0 };

        int vertical_index = 0;

        for (int i = 0; i < 172; i++)
        {
            for (int j = 0; j < 154; j++)
            {
                if (i >= 0 && i <= 25 && j >= 65 && j <= 77) continue;
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

                    if (surrounding.count() == 3)
                    {
                        if (!surrounding[0])
                        {
                            int rand = randi(15);
                            cv::Rect rect(rand * 10, 0, 10, 10);
                            cv::Mat rotateImg;

                            edge_wood_up(rect).copyTo(rotateImg);

                            InsertElementFromImage(i, j, rotateImg, WOOD);
                        }
                        else if (!surrounding[2])
                        {
                            int rand = randi(15);
                            cv::Rect rect(rand * 10, 0, 10, 10);
                            cv::Mat rotateImg;

                            //if (i % 2 == 0)
                            //{
                            //    edge_wood_center_0(rect).copyTo(rotateImg);
                            //}
                            //else
                            //{
                            //    edge_wood_center_1(rect).copyTo(rotateImg);
                            //}
                            edge_wood_up(rect).copyTo(rotateImg);
                            InsertElementFromImage(i, j, rotateImg, WOOD);
                        }
                        else
                        {
                            int rand = randi(15);
                            cv::Rect rect(rand * 10, 0, 10, 10);
                            cv::Mat rotateImg;

                            //if (i % 2 == 0)
                            //{
                            //    edge_wood_center_0(rect).copyTo(rotateImg);
                            //}
                            //else
                            //{
                            //    edge_wood_center_1(rect).copyTo(rotateImg);
                            //}

                            edge_wood_up(rect).copyTo(rotateImg);

                            cv::Rect lineRect(0, 0, 3, 10);
                            cv::Mat lineImg;

                            edge_wood_line(lineRect).copyTo(lineImg);
                            bool isLeft = true;

                            if (!surrounding[1])
                            {
                                isLeft = false;
                            }

                            InsertElementFromImage(i, j, rotateImg, lineImg, WOOD, isLeft);
                        }
                    }
                    else if (surrounding.count() == 2)
                    {
                        if (surrounding[1] && surrounding[3])
                        {
                            int rand = randi(15);
                            cv::Rect rect(rand * 10, 0, 10, 10);
                            cv::Mat rotateImg;

                            edge_wood_up(rect).copyTo(rotateImg);

                            InsertElementFromImage(i, j, rotateImg, WOOD);

                            continue;
                        }
                        cv::Rect rect(0, 0, 10, 10);
                        cv::Mat rotateImg;

                        if (surrounding[2] && surrounding[3])
                        {
                            rect = { 10,0,10,10 };
                        }
                        else if (surrounding[3] && surrounding[0])
                        {
                            rect = { 10,10,10,10 };
                        }
                        else if (surrounding[0] && surrounding[1])
                        {
                            rect = { 0,10,10,10 };
                        }
                        else if (surrounding[0] && surrounding[2])
                        {
                            int a = 0;
                        }

                        edge_wood_corner(rect).copyTo(rotateImg);
                        InsertElementFromImage(i, j, rotateImg, WOOD);
                    }
                    else if (surrounding.count() == 1)
                    {
                        int rand = 0;
                        cv::Rect rect(0, 0, 10, 10);
                        cv::Mat rotateImg;

                        if (surrounding[3])
                        {
                            rect = { 10,0,10,10 };
                        }


                        edge_wood_convex(rect).copyTo(rotateImg);
                        InsertElementFromImage(i, j, rotateImg, WOOD);
                    }
                    else
                    {
                        int rand = randi(15);
                        cv::Rect rect(rand * 10, 0, 10, 10);
                        cv::Mat rotateImg;

                        //if (i % 2 == 0)
                        //{
                        //    edge_wood_center_0(rect).copyTo(rotateImg);
                        //}
                        //else
                        //{
                        //    edge_wood_center_1(rect).copyTo(rotateImg);
                        //}

                        edge_wood_up(rect).copyTo(rotateImg);
                        InsertElementFromImage(i, j, rotateImg, WOOD);
                    }
                }
                else if (color == 0xFF413f3A)
                {
                    for (int dir = 0; dir < 4; dir++)
                    {
                        if (j + dx[dir] >= 0 && j + dx[dir] < mask.cols && i + dy[dir] >= 0 && i + dy[dir] < mask.rows)
                        {
                            surrounding[dir] = (mask.at<uchar>(i + dy[dir], j + dx[dir]) > 0) ? 1 : 0;
                        }
                    }
                    if (!surrounding[2])
                    {
                        cv::Rect rect(0, 0, 10, 10);
                        cv::Mat rotateImg;

                        edge_wood_vertical_convex(rect).copyTo(rotateImg);

                        InsertElementFromImage(i, j, rotateImg, WOOD);
                    }
                    else if (!surrounding[0])
                    {
                        cv::Rect rect(10, 0, 10, 10);
                        cv::Mat rotateImg;

                        edge_wood_vertical_convex(rect).copyTo(rotateImg);

                        InsertElementFromImage(i, j, rotateImg, WOOD);
                    }
                    else
                    {
                        if (vertical_index > 7)
                        {
                            vertical_index = 0;
                        }
                        cv::Rect rect(0, vertical_index++ * 10, 10, 10);
                        cv::Mat rotateImg;

                        edge_wood_vertical(rect).copyTo(rotateImg);

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
                else if (color == 0xFF505052) // coal
                {
                    for (int k = i * 10; k < i * 10 + 10; k++)
                    {
                        for (int l = j * 10; l < j * 10 + 10; l++)
                        {
                            Element coal = SAND;
                            coal.Color = getMaterialColor(L"coal", l, k);
                            InsertElement(l, k, coal);
                        }
                    }
                }
                else if (color == 0xFF3B3B3C) // oil
                {
                    for (int k = i * 10; k < i * 10 + 10; k++)
                    {
                        for (int l = j * 10; l < j * 10 + 10; l++)
                        {
                            Element oil = OIL;
                            oil.Color = 0xE63D3728;
                            InsertElement(l, k, oil);
                        }
                    }
                }
                else if (color == 0xFF800000 || color == 0xFFFF0000)
                {
                    int rand = randi(1);
                    
                    switch (rand)
                    {
                    case 0: CreateObject(new ShotGunner_Weak(), eLayerType::Monster, j * 10, -i * 10);  break;
                    case 1: CreateObject(new Zombie_weak(), eLayerType::Monster, j * 10, -i * 10);      break;
                    default: break;
                    } 
                    
                }
                else if (color == 0xFFFF0AFF)
                {
                    LoadRandomScene_01(j * 10, i * 10);
                }
                else if (color == 0xFFFF0080)
                {
                    LoadRandomScene_02(j * 10, i * 10);
                }
                else if (color == 0xFFC35700)
                {
                    LoadRandomScene_03(j * 10, i * 10);
                }
            }
        }
        LoadStartTile(65 * 10, 0);

        for (int i = 0; i < 1720; i++)
        {
            for (int j = 0; j < 1536; j++)
            {
                if (GetElement(i, j).Id == eElementID::ROCK)
                {
                    if (GetElement(i, j - 1).Id == eElementID::EMPTY)
                    {
                        if (random() > 0.5f)
                        {
                            Element grass = GRASS;
                            grass.Color = getMaterialColor(L"grass", i, j - 1);

                            InsertElement(i, j - 1, grass);
                        }
                        else if (random() > 0.97f)
                        {
                            createVegetation(i, j - 1);
                        }
                    }
                }
            }
        }

        cv::Mat visual_image_left = cv::imread("..\\Resources\\Texture\\Material\\rock_hard_left.png", cv::IMREAD_UNCHANGED);
        for (int i = 0; i < 1747; i++)
        {
            for (int j = 0; j < 10; j++)
            {
                cv::Vec4b visual_color = visual_image_left.at<cv::Vec4b>(i % 92, j);

                if (visual_color[3] != 0)
                {
                    uint32_t converted_color =
                        (visual_color[3] << 24) |
                        (visual_color[2] << 16) |
                        (visual_color[1] << 8) |
                        (visual_color[0]);

                    Element element = ROCK;
                    element.Color = converted_color;

                    InsertElement(j, i, element);
                }
            }
        }

        cv::Mat visual_image_right = cv::imread("..\\Resources\\Texture\\Material\\rock_hard_right.png", cv::IMREAD_UNCHANGED);
        for (int i = 0; i < 1747; i++)
        {
            for (int j = 1526; j < 1536; j++)
            {
                cv::Vec4b visual_color = visual_image_right.at<cv::Vec4b>(i % 92, j - 1526);

                if (visual_color[3] != 0)
                {
                    uint32_t converted_color =
                        (visual_color[3] << 24) |
                        (visual_color[2] << 16) |
                        (visual_color[1] << 8) |
                        (visual_color[0]);

                    Element element = ROCK;
                    element.Color = converted_color;

                    InsertElement(j, i, element);
                }
            }
        }

        cv::Mat visual_image_top = cv::imread("..\\Resources\\Texture\\Material\\rock_hard_top.png", cv::IMREAD_UNCHANGED);
        for (int i = 0; i < 10; i++)
        {
            for (int j = 0; j < 1536; j++)
            {
                cv::Vec4b visual_color = visual_image_top.at<cv::Vec4b>(i, j % 92);

                if (visual_color[3] != 0)
                {
                    uint32_t converted_color =
                        (visual_color[3] << 24) |
                        (visual_color[2] << 16) |
                        (visual_color[1] << 8) |
                        (visual_color[0]);

                    Element element = ROCK;
                    element.Color = converted_color;

                    InsertElement(j, i, element);
                }
            }
        }

        cv::Mat material_image = cv::imread("..\\Resources\\Texture\\Temple\\altar_top.png", cv::IMREAD_COLOR);
        cv::Mat visual_image = cv::imread("..\\Resources\\Texture\\Temple\\altar_top_visual.png", cv::IMREAD_UNCHANGED);
        cv::cvtColor(material_image, material_image, cv::COLOR_BGR2RGB);
        for (int i = 1747; i < 2047; i++)
        {
            for (int cnt = 0; cnt < 3; cnt++)
            {
                for (int j = 0; j < 512; j++)
                {
                    uint32_t color = Vec3bToColor(material_image.at<cv::Vec3b>(i - 1747, j));
                    if (color == 0xFF786C42)
                    {
                        cv::Vec4b visual_color = visual_image.at<cv::Vec4b>(i - 1747, j);

                        if (visual_color[3] == 0)
                        {
                            Element element = ROCK;
                            element.Color = getMaterialColor(L"templebrick", j + cnt * 512, i);

                            InsertElement(j + cnt * 512, i, element);
                        }
                        else
                        {
                            uint32_t converted_color =
                                (visual_color[3] << 24) |
                                (visual_color[2] << 16) |
                                (visual_color[1] << 8) |
                                (visual_color[0]);

                            Element element = ROCK;
                            element.Color = converted_color;

                            InsertElement(j + cnt * 512, i, element);
                        }
                    }
                    else
                    {
                        InsertElement(j + cnt * 512, i, EMPTY);
                    }
                }
            }
        }

        //cv::cvtColor(randTileImage, randTileImage, cv::COLOR_RGB2BGR);
        //cv::resize(randTileImage, randTileImage, cv::Size(), 2,2, cv::INTER_NEAREST);

        //cv::imshow("Generated Map", randTileImage);
        wangTileImage.release();
        stbhw_free_tileset(&tileset);
        free(tileData);
    }

    void PixelWorld::CreateNextWorld()
    {
        DeletePrevWorld();

        mImage = new PixelGridColor(mWorldWidth, mWorldHeight);
        mPixelColor.resize(mWorldWidth * mWorldHeight);

        for (int i = 0; i <= 3; i++)
        {
            for (int j = 0; j <= 2; j++)
            {
                CreateChunkMap({ j, i });
            }
        }

        srand(time(NULL));

        int x = 260;
        int y = 260;

        int* a = new int; // 왜 릭 나냐 모르겠다...
        cv::Mat wangTileImage = cv::imread("..\\Resources\\Texture\\WangTiles\\Excavationsite\\excavationsite.png", cv::IMREAD_COLOR);
        int* b = new int;
        //int yux = wangTileImage.u->refcount;
        //wangTileImage.deallocate();
        //int eqwd = wangTileImage.u->refcount;

        cv::cvtColor(wangTileImage, wangTileImage, cv::COLOR_BGR2RGB);

        stbhw_tileset tileset;

        stbhw_build_tileset_from_image(&tileset, (unsigned char*)wangTileImage.data, wangTileImage.cols * 3, wangTileImage.cols, wangTileImage.rows);

        unsigned char* tileData = (unsigned char*)malloc(3 * x * y);

        stbhw_generate_image(&tileset, NULL, tileData, x * 3, x, y);
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

        cv::Mat edge_wood_center_0 = cv::imread("..\\Resources\\Texture\\Material\\edge\\edge_wood_center_0.png", cv::IMREAD_COLOR);
        cv::cvtColor(edge_wood_center_0, edge_wood_center_0, cv::COLOR_BGR2RGB);

        cv::Mat edge_wood_center_1 = cv::imread("..\\Resources\\Texture\\Material\\edge\\edge_wood_center_1.png", cv::IMREAD_COLOR);
        cv::cvtColor(edge_wood_center_1, edge_wood_center_1, cv::COLOR_BGR2RGB);

        cv::Mat edge_wood_corner = cv::imread("..\\Resources\\Texture\\Material\\edge\\edge_wood_corner.png", cv::IMREAD_COLOR);
        cv::cvtColor(edge_wood_corner, edge_wood_corner, cv::COLOR_BGR2RGB);

        cv::Mat edge_wood_convex = cv::imread("..\\Resources\\Texture\\Material\\edge\\edge_wood_convex.png", cv::IMREAD_COLOR);
        cv::cvtColor(edge_wood_convex, edge_wood_convex, cv::COLOR_BGR2RGB);

        cv::Mat edge_wood_up = cv::imread("..\\Resources\\Texture\\Material\\edge\\edge_wood_up.png", cv::IMREAD_COLOR);
        cv::cvtColor(edge_wood_up, edge_wood_up, cv::COLOR_BGR2RGB);

        cv::Mat edge_wood_line = cv::imread("..\\Resources\\Texture\\Material\\edge\\edge_wood_line.png", cv::IMREAD_COLOR);
        cv::cvtColor(edge_wood_line, edge_wood_line, cv::COLOR_BGR2RGB);

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

        for (int i = 0; i < 172; i++)
        {
            for (int j = 0; j < 154; j++)
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
                        if (!surrounding[0])
                        {
                            int rand = randi(15);
                            cv::Rect rect(rand * 10, 0, 10, 10);
                            cv::Mat rotateImg;

                            edge_wood_up(rect).copyTo(rotateImg);

                            InsertElementFromImage(i, j, rotateImg, WOOD);
                        }
                        else if (!surrounding[2])
                        {
                            int rand = randi(3);
                            cv::Rect rect(rand * 10, 0, 10, 10);
                            cv::Mat rotateImg;

                            if (i % 2 == 0)
                            {
                                edge_wood_center_0(rect).copyTo(rotateImg);
                            }
                            else
                            {
                                edge_wood_center_1(rect).copyTo(rotateImg);
                            }

                            InsertElementFromImage(i, j, rotateImg, WOOD);
                        }
                        else
                        {
                            int rand = randi(3);
                            cv::Rect rect(rand * 10, 0, 10, 10);
                            cv::Mat rotateImg;

                            if (i % 2 == 0)
                            {
                                edge_wood_center_0(rect).copyTo(rotateImg);
                            }
                            else
                            {
                                edge_wood_center_1(rect).copyTo(rotateImg);
                            }

                            cv::Rect lineRect(0, 0, 3, 10);
                            cv::Mat lineImg;

                            edge_wood_line(lineRect).copyTo(lineImg);
                            bool isLeft = true;

                            if (!surrounding[1])
                            {
                                isLeft = false;
                            }

                            InsertElementFromImage(i, j, rotateImg, lineImg, WOOD, isLeft);
                        }
                    }
                    else if (surrounding.count() == 2)
                    {
                        if (surrounding[0] && surrounding[2])
                        {
                            int rand = randi(15);
                            cv::Rect rect(rand * 10, 0, 10, 10);
                            cv::Mat rotateImg;

                            edge_wood_up(rect).copyTo(rotateImg);

                            InsertElementFromImage(i, j, rotateImg, WOOD);

                            continue;
                        }
                        cv::Rect rect(0, 0, 10, 10);
                        cv::Mat rotateImg;

                        if (surrounding[2] && surrounding[3])
                        {
                            rect = { 10,0,10,10 };
                        }
                        else if (surrounding[3] && surrounding[0])
                        {
                            rect = { 10,10,10,10 };
                        }
                        else if (surrounding[0] && surrounding[1])
                        {
                            rect = { 0,10,10,10 };
                        }
                        else if (surrounding[1] && surrounding[2])
                        {
                            int a = 0;
                        }

                        edge_wood_corner(rect).copyTo(rotateImg);
                        InsertElementFromImage(i, j, rotateImg, WOOD);
                    }
                    else if (surrounding.count() == 1)
                    {
                        int rand = 0;
                        cv::Rect rect(0, 0, 10, 10);
                        cv::Mat rotateImg;

                        if (surrounding[3])
                        {
                            rect = { 10,0,10,10 };
                        }
                        else if (surrounding[0])
                        {
                            rect = { 30,0,10,10 };
                        }
                        else if (surrounding[2])
                        {
                            rect = { 20,0,10,10 };
                        }

                        edge_wood_convex(rect).copyTo(rotateImg);
                        InsertElementFromImage(i, j, rotateImg, WOOD);
                    }
                    else
                    {
                        int rand = randi(3);
                        cv::Rect rect(rand * 10, 0, 10, 10);
                        cv::Mat rotateImg;

                        if (i % 2 == 0)
                        {
                            edge_wood_center_0(rect).copyTo(rotateImg);
                        }
                        else
                        {
                            edge_wood_center_1(rect).copyTo(rotateImg);
                        }

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
                else if (color == 0xFF505052) // coal
                {
                    for (int k = i * 10; k < i * 10 + 10; k++)
                    {
                        for (int l = j * 10; l < j * 10 + 10; l++)
                        {
                            Element coal = SAND;
                            coal.Color = getMaterialColor(L"coal", l, k);
                            InsertElement(l, k, coal);
                        }
                    }
                }
                else if (color == 0xFF3B3B3C) // oil
                {
                    for (int k = i * 10; k < i * 10 + 10; k++)
                    {
                        for (int l = j * 10; l < j * 10 + 10; l++)
                        {
                            Element oil = OIL;
                            oil.Color = 0xE63D3728;
                            InsertElement(l, k, oil);
                        }
                    }
                }
                else if (color == 0xFFFF0AFF)
                {
                    LoadRandomScene_01(j * 10, i * 10);
                }
                else if (color == 0xFFFF0080)
                {
                    LoadRandomScene_02(j * 10, i * 10);
                }
                else if (color == 0xFFC35700)
                {
                    LoadRandomScene_03(j * 10, i * 10);
                }
            }
        }

        for (int i = 0; i < 1720; i++)
        {
            for (int j = 0; j < 1536; j++)
            {
                if (GetElement(i, j).Id == eElementID::ROCK)
                {
                    if (GetElement(i, j - 1).Id == eElementID::EMPTY)
                    {
                        if (random() > 0.5f)
                        {
                            Element grass = GRASS;
                            grass.Color = getMaterialColor(L"grass", i, j - 1);

                            InsertElement(i, j - 1, grass);
                        }
                        else if (random() > 0.97f)
                        {
                            createVegetation(i, j - 1);
                        }
                    }
                }
            }
        }


        cv::Mat material_image = cv::imread("..\\Resources\\Texture\\Temple\\altar_top.png", cv::IMREAD_COLOR);
        cv::Mat visual_image = cv::imread("..\\Resources\\Texture\\Temple\\altar_top_visual.png", cv::IMREAD_UNCHANGED);

        cv::cvtColor(material_image, material_image, cv::COLOR_BGR2RGB);

        for (int i = 1747; i < 2047; i++)
        {

            for (int cnt = 0; cnt < 3; cnt++)
            {
                for (int j = 0; j < 512; j++)
                {

                    uint32_t color = Vec3bToColor(material_image.at<cv::Vec3b>(i - 1747, j));
                    if (color == 0xFF786C42)
                    {
                        cv::Vec4b visual_color = visual_image.at<cv::Vec4b>(i - 1747, j);

                        uint32_t converted_color =
                            (visual_color[3] << 24) |
                            (visual_color[2] << 16) |
                            (visual_color[1] << 8) |
                            (visual_color[0]);

                        Element element = ROCK;
                        element.Color = converted_color;

                        InsertElement(j + cnt * 512, i, element);
                    }
                    else
                    {
                        InsertElement(j + cnt * 512, i, EMPTY);
                    }
                }
            }
        }

        //cv::cvtColor(randTileImage, randTileImage, cv::COLOR_RGB2BGR);
        //cv::resize(randTileImage, randTileImage, cv::Size(), 2,2, cv::INTER_NEAREST);

        //cv::imshow("Generated Map", randTileImage);
        wangTileImage.release();
        stbhw_free_tileset(&tileset);
        free(tileData);
    }

    void PixelWorld::CreateBossArena()
    {
        DeletePrevWorld();

        mWorldWidth = 2560;
        mWorldHeight = 1536;

        graphics::GetDevice()->CreateVisibility((UINT)mWorldWidth, (UINT)mWorldHeight);

        mImage = new PixelGridColor(mWorldWidth, mWorldHeight);
        mPixelColor.resize(mWorldWidth * mWorldHeight);

        for (int i = 0; i <= 2; i++)
        {
            for (int j = 0; j <= 4; j++)
            {
                CreateChunkMap({ j, i });
            }
        }

        cv::Mat material_image = cv::imread("..\\Resources\\Texture\\BossArena\\boss_arena.png", cv::IMREAD_COLOR);
        cv::Mat visual_image = cv::imread("..\\Resources\\Texture\\BossArena\\boss_arena_visual.png", cv::IMREAD_UNCHANGED);

        cv::cvtColor(material_image, material_image, cv::COLOR_BGR2RGB);

        for (int i = 0; i < 1536; i++)
        {
            for (int j = 0; j < 2560; j++)
            {
                uint32_t color = Vec3bToColor(material_image.at<cv::Vec3b>(i, j));
                if (color == 0xFF786C42 || color == 0xFFAAF06E || color == 0xFF606C5A 
                    || color == 0xFF786C44 || color == 0xFFC931 || color == 0xFF003345)
                {
                    cv::Vec4b visual_color = visual_image.at<cv::Vec4b>(i, j);

                    if (visual_color[3] == 0)
                    {
                        Element element = ROCK;
                        element.Color = getMaterialColor(L"templebrick", j, i);

                        InsertElement(j, i, element);
                    }
                    else
                    {
                        uint32_t converted_color =
                            (visual_color[3] << 24) |
                            (visual_color[2] << 16) |
                            (visual_color[1] << 8) |
                            (visual_color[0]);

                        Element element = ROCK;
                        element.Color = converted_color;

                        InsertElement(j, i, element);
                    }
                }
                else if (color == 0xFF2F554C)
                {
                    Element element = WATER;

                    InsertElement(j, i, element);
                }
                else if (color == 0xFFFF6000)
                {
                    Element element = LAVA;

                    InsertElement(j, i, element);
                }
                else if (color == 0xFF003344)
                {
                    cv::Vec4b visual_color = visual_image.at<cv::Vec4b>(i, j);

                    if (visual_color[3] == 0)
                    {
                        Element element = ROCK;
                        element.Color = getMaterialColor(L"templebrick", j, i);

                        InsertElement(j, i, element);
                    }
                    else
                    {
                        uint32_t converted_color =
                            (visual_color[3] << 24) |
                            (visual_color[2] << 16) |
                            (visual_color[1] << 8) |
                            (visual_color[0]);

                        Element element = ROCK;
                        element.Color = converted_color;

                        InsertElement(j, i, element);
                    }
                }
                else if (color == 0xFF00FF00)
                {
                    CreateObject(new Centipede(), eLayerType::Monster, j, -i , 0.19f);
                }
            }
        }
    }

    void PixelWorld::CreateEndWorld()
    {
        DeletePrevWorld();

        mWorldWidth = 1536;
        mWorldHeight = 1536;

        graphics::GetDevice()->CreateVisibility((UINT)mWorldWidth, (UINT)mWorldHeight);

        mImage = new PixelGridColor(mWorldWidth, mWorldHeight);
        mPixelColor.resize(mWorldWidth * mWorldHeight);

        for (int i = 0; i <= 2; i++)
        {
            for (int j = 0; j <= 2; j++)
            {
                CreateChunkMap({ j, i });
            }
        }

        cv::Mat material_image = cv::imread("..\\Resources\\Texture\\Ending\\boss_victoryroom.png", cv::IMREAD_COLOR);
        cv::Mat visual_image = cv::imread("..\\Resources\\Texture\\Ending\\boss_victoryroom_visual.png", cv::IMREAD_UNCHANGED);

        cv::cvtColor(material_image, material_image, cv::COLOR_BGR2RGB);

        for (int i = 512; i < 1024; i++)
        {
            for (int j = 512; j < 1024; j++)
            {
                uint32_t color = Vec3bToColor(material_image.at<cv::Vec3b>(i - 512, j - 512));

                if (color == 0xFF786C42)
                {
                    cv::Vec4b visual_color = visual_image.at<cv::Vec4b>(i - 512, j - 512);

                    if (visual_color[3] == 0)
                    {
                        Element element = ROCK;
                        element.Color = getMaterialColor(L"templebrick", j, i);

                        InsertElement(j, i, element);
                    }
                    else
                    {
                        uint32_t converted_color =
                            (visual_color[3] << 24) |
                            (visual_color[2] << 16) |
                            (visual_color[1] << 8) |
                            (visual_color[0]);

                        Element element = ROCK;
                        element.Color = converted_color;

                        InsertElement(j, i, element);
                    }
                }
                
            }
        }
    }

    void PixelWorld::DeletePrevWorld()
    {
        for (auto chunkMap : mChunkMaps)
        {
            delete chunkMap;
            chunkMap = nullptr;
        }
        mChunkMaps.clear();
        mChunkMapLookUp.clear();

        delete mImage;
        mImage = nullptr;


        mPixelColor.clear();

        std::vector<uint32_t>().swap(mPixelColor);
        //Box2dWorld::InitializePresentWorld();
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

    void PixelWorld::InsertElementFromImage(int y, int x, const cv::Mat& image, const cv::Mat& lineImage, Element& element, int type)
    {
        if (type == 1)
        {
            for (int i = y * 10; i < y * 10 + 10; i++)
            {
                for (int j = x * 10 + 3; j < x * 10 + 10; j++)
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
            for (int i = y * 10; i < y * 10 + 10; i++)
            {
                for (int j = x * 10; j < x * 10 + 3; j++)
                {
                    cv::Vec3b colorVec = lineImage.at<cv::Vec3b>(i % 10, j % 10);
                    uint32_t color = Vec3bToColor(colorVec);

                    if (color != 0xFF000000 && color != 0xFFFFFFFF)
                    {
                        element.Color = color;
                        InsertElement(j, i, element);
                    }
                }
            }
        }
        else if (type == 2)
        {
            for (int i = y * 10; i < y * 10 + 10; i++)
            {
                for (int j = x * 10; j < x * 10 + 7; j++)
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
            for (int i = y * 10; i < y * 10 + 10; i++)
            {
                for (int j = x * 10 + 7; j < x * 10 + 10; j++)
                {
                    cv::Vec3b colorVec = lineImage.at<cv::Vec3b>(i % 10, j % 10 - 7);
                    uint32_t color = Vec3bToColor(colorVec);

                    if (color != 0xFF000000 && color != 0xFFFFFFFF)
                    {
                        element.Color = color;
                        InsertElement(j, i, element);
                    }
                }
            }
        }
        else if (type == 3)
        {
            for (int i = y * 10; i < y * 10 + 10; i++)
            {
                for (int j = x * 10 + 3; j < x * 10 + 7; j++)
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
            for (int i = y * 10; i < y * 10 + 10; i++)
            {
                for (int j = x * 10; j < x * 10 + 3; j++)
                {
                    cv::Vec3b colorVec = lineImage.at<cv::Vec3b>(i % 10, j % 10);
                    uint32_t color = Vec3bToColor(colorVec);

                    if (color != 0xFF000000 && color != 0xFFFFFFFF)
                    {
                        element.Color = color;
                        InsertElement(j, i, element);
                    }
                }
            }
            for (int i = y * 10; i < y * 10 + 10; i++)
            {
                for (int j = x * 10 + 7; j < x * 10 + 10; j++)
                {
                    cv::Vec3b colorVec = lineImage.at<cv::Vec3b>(i % 10, j % 10 - 7);
                    uint32_t color = Vec3bToColor(colorVec);

                    if (color != 0xFF000000 && color != 0xFFFFFFFF)
                    {
                        element.Color = color;
                        InsertElement(j, i, element);
                    }
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

    uint32_t PixelWorld::Vec4bToColor(const cv::Vec4b& vec4b)
    {
        uint32_t color =
            (vec4b[3] << 24) |
            (vec4b[2] << 16) |
            (vec4b[1] << 8) |
            (vec4b[0]);

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

    void PixelWorld::DrawRandomSceneImage(const cv::Mat& material_image, const cv::Mat& visual_image, int x, int y)
    {
        cv::Scalar color_wood_lower(97, 62, 0);
        cv::Scalar color_wood_upper(97, 62, 2);
        cv::Scalar color_wood_vertical(65, 63, 36);
        cv::Scalar color_rock(45, 45, 172);
        cv::Scalar color_templebrick(120, 108, 66);
        cv::Scalar color_rock_static_intro(10, 51, 85);
        cv::Scalar color_rock_static(53, 57, 35);

        cv::Mat mask;

        cv::inRange(material_image, color_wood_lower, color_wood_upper, mask);
        int nonZeroPixels = cv::countNonZero(mask);
        if (nonZeroPixels != 0)
        {
            Element wood = WOOD;
            wood.SolidType = eSolidType::DYNAMIC;

            Box2dWorld::Draw(x, y, mask, visual_image, wood);
        }

        cv::inRange(material_image, color_wood_vertical, color_wood_vertical, mask);
        nonZeroPixels = cv::countNonZero(mask);
        if (nonZeroPixels != 0)
        {
            Element wood = WOOD;
            wood.SolidType = eSolidType::DYNAMIC;

            Box2dWorld::Draw(x, y, mask, visual_image, wood);
        }

        cv::inRange(material_image, color_rock, color_rock, mask);
        nonZeroPixels = cv::countNonZero(mask);
        if (nonZeroPixels != 0)
        {
            Element rock = ROCK;
            rock.SolidType = eSolidType::DYNAMIC;

            Box2dWorld::Draw(x, y, mask, visual_image, rock);
        }

        cv::inRange(material_image, color_templebrick, color_templebrick, mask);
        nonZeroPixels = cv::countNonZero(mask);
        if (nonZeroPixels != 0)
        {
            Element rock = ROCK;
            rock.SolidType = eSolidType::DYNAMIC;

            Box2dWorld::Draw(x, y, mask, visual_image, rock);
        }

        //cv::inRange(material_image, color_rock_static, color_rock_static, mask);
        //nonZeroPixels = cv::countNonZero(mask);
        //if (nonZeroPixels != 0)
        //{
        //    Element rock = ROCK;
        //    rock.SolidType = eSolidType::DYNAMIC;

        //    Box2dWorld::Draw(x, y, mask, visual_image, rock);
        //}

        cv::inRange(material_image, color_rock_static_intro, color_rock_static_intro, mask);
        nonZeroPixels = cv::countNonZero(mask);
        if (nonZeroPixels != 0)
        {
            Element rock = ROCK;
            rock.SolidType = eSolidType::DYNAMIC;

            Box2dWorld::Draw(x, y, mask, visual_image, rock);
        }
    }

    void PixelWorld::InsertElementFromWangColor(uint32_t wangColor, int x, int y)
    {
        if (wangColor == 0xFF613E02 || wangColor == 0xFF413F24)
        {
            return;
        }
        else if (wangColor == 0xFF524F2D)
        {
            Element element = ROCK;
            element.Color = getMaterialColor(L"earth", x, y);

            InsertElement(x, y, element);
        }
        else if (wangColor == 0xFF33B828)
        {
            Element element = GRASS;
            element.Color = getMaterialColor(L"grass", x, y);

            InsertElement(x, y, element);
        }
        else if (wangColor == 0xFF3ABB32) //fungi
        {
            Element element = GRASS;
            element.Color = getMaterialColor(L"grass", x, y);

            InsertElement(x, y, element);
        }
        else if (wangColor == 0xFF45FF45) //fungi green
        {
            Element element = GRASS;
            element.Color = getMaterialColor(L"grass", x, y);

            InsertElement(x, y, element);
        }
        else if (wangColor == 0xFFF0BBEE) // random liquid
        {
            Element element = OIL;

            InsertElement(x, y, element);
        }
        else if (wangColor == 0xFF505052)
        {
            Element element = SAND;
            element.Color = getMaterialColor(L"coal", x, y);

            InsertElement(x, y, element);
        }
        else if (wangColor == 0xFFF7BB43)
        {
            Element element = SAND;
            element.Color = getMaterialColor(L"gunpowder_tnt", x, y);

            InsertElement(x, y, element);
        }
        else if (wangColor == 0xFF00F344)
        {
            Element element = ROCK;
            element.Color = getMaterialColor(L"rock_radioactive", x, y);

            InsertElement(x, y, element);
        }
        else if (wangColor == 0xFFAAf06E) // mat_glowstone
        {
            Element element = ROCK;
            element.Color = 0xFFAAf06E;

            InsertElement(x, y, element);
        }
        else if (wangColor == 0xFF2F554C)
        {
            Element element = WATER;

            InsertElement(x, y, element);
        }
        else if (wangColor == 0xFF353923)
        {
            Element element = ROCK;
            element.Color = getMaterialColor(L"rock_alt2", x, y);

            InsertElement(x, y, element);
        }
        else if (wangColor != 0xFF000000)
        {
            Element element = ROCK;
            element.Color = getMaterialColor(L"rock", x, y);

            InsertElement(x, y, element);
        }
    }

    void PixelWorld::LoadRandomScene_01(int x, int y)
    {
        cv::Mat material_image;
        cv::Mat visual_image;

        switch (randi(5))
        {
        case 0:
        {
            material_image = cv::imread("..\\Resources\\Texture\\Coalmine\\coalpit01.png", cv::IMREAD_COLOR);
            visual_image = cv::imread("..\\Resources\\Texture\\Coalmine\\coalpit01_visual.png", cv::IMREAD_UNCHANGED);
            break;
        }
        case 1:
        {
            material_image = cv::imread("..\\Resources\\Texture\\Coalmine\\coalpit02.png", cv::IMREAD_COLOR);
            visual_image = cv::imread("..\\Resources\\Texture\\Coalmine\\coalpit02_visual.png", cv::IMREAD_UNCHANGED);
            break;
        }
        case 2:
        {
            material_image = cv::imread("..\\Resources\\Texture\\Coalmine\\coalpit03.png", cv::IMREAD_COLOR);
            visual_image = cv::imread("..\\Resources\\Texture\\Coalmine\\coalpit03_visual.png", cv::IMREAD_UNCHANGED);
            break;
        }
        case 3:
        {
            material_image = cv::imread("..\\Resources\\Texture\\Coalmine\\coalpit04.png", cv::IMREAD_COLOR);
            visual_image = cv::imread("..\\Resources\\Texture\\Coalmine\\coalpit04_visual.png", cv::IMREAD_UNCHANGED);
            break;
        }
        case 4:
        {
            material_image = cv::imread("..\\Resources\\Texture\\Coalmine\\coalpit05.png", cv::IMREAD_COLOR);
            visual_image = cv::imread("..\\Resources\\Texture\\Coalmine\\coalpit05_visual.png", cv::IMREAD_UNCHANGED);
            break;
        }
        case 5:
        {
            material_image = cv::imread("..\\Resources\\Texture\\Coalmine\\carthill.png", cv::IMREAD_COLOR);
            visual_image = cv::imread("..\\Resources\\Texture\\Coalmine\\carthill_visual.png", cv::IMREAD_UNCHANGED);
            break;
        }
        }

        cv::cvtColor(material_image, material_image, cv::COLOR_BGR2RGB);


        for (int i = 0; i < 260; i++)
        {
            for (int j = 0; j < 130; j++)
            {
                uint32_t color = Vec3bToColor(material_image.at<cv::Vec3b>(i, j));

                InsertElementFromWangColor(color, x + j, y + i);
            }
        }

        DrawRandomSceneImage(material_image, visual_image, x, y);

    }
    void PixelWorld::LoadRandomScene_02(int x, int y)
    {
        cv::Mat material_image;
        cv::Mat visual_image;

        switch (randi(9))
        {
        case 0:
        {
            material_image = cv::imread("..\\Resources\\Texture\\Coalmine\\shrine01.png", cv::IMREAD_COLOR);
            visual_image = cv::imread("..\\Resources\\Texture\\Coalmine\\shrine01_visual.png", cv::IMREAD_UNCHANGED);
            break;
        }
        case 1:
        {
            material_image = cv::imread("..\\Resources\\Texture\\Coalmine\\shrine02.png", cv::IMREAD_COLOR);
            visual_image = cv::imread("..\\Resources\\Texture\\Coalmine\\shrine02_visual.png", cv::IMREAD_UNCHANGED);
            break;
        }
        case 2:
        {
            material_image = cv::imread("..\\Resources\\Texture\\Coalmine\\slimepit.png", cv::IMREAD_COLOR);
            visual_image = cv::imread("..\\Resources\\Texture\\Coalmine\\slimepit_visual.png", cv::IMREAD_UNCHANGED);
            break;
        }
        case 3:
        {
            material_image = cv::imread("..\\Resources\\Texture\\Coalmine\\laboratory.png", cv::IMREAD_COLOR);
            visual_image = cv::imread("..\\Resources\\Texture\\Coalmine\\laboratory_visual.png", cv::IMREAD_UNCHANGED);
            break;
        }
        case 4:
        {
            material_image = cv::imread("..\\Resources\\Texture\\Coalmine\\swarm.png", cv::IMREAD_COLOR);
            visual_image = cv::imread("..\\Resources\\Texture\\Coalmine\\swarm_visual.png", cv::IMREAD_UNCHANGED);
            break;
        }
        case 5:
        {
            material_image = cv::imread("..\\Resources\\Texture\\Coalmine\\physics_01.png", cv::IMREAD_COLOR);
            visual_image = cv::imread("..\\Resources\\Texture\\Coalmine\\physics_01_visual.png", cv::IMREAD_UNCHANGED);
            break;
        }
        case 6:
        {
            material_image = cv::imread("..\\Resources\\Texture\\Coalmine\\physics_02.png", cv::IMREAD_COLOR);
            visual_image = cv::imread("..\\Resources\\Texture\\Coalmine\\physics_02_visual.png", cv::IMREAD_UNCHANGED);
            break;
        }
        case 7:
        {
            material_image = cv::imread("..\\Resources\\Texture\\Coalmine\\shop.png", cv::IMREAD_COLOR);
            visual_image = cv::imread("..\\Resources\\Texture\\Coalmine\\shop_visual.png", cv::IMREAD_UNCHANGED);
            break;
        }
        case 8:
        {
            material_image = cv::imread("..\\Resources\\Texture\\Coalmine\\wandtrap_h_02.png", cv::IMREAD_COLOR);
            visual_image = cv::imread("..\\Resources\\Texture\\Coalmine\\wandtrap_h_02_visual.png", cv::IMREAD_UNCHANGED);
            break;
        }
        case 9:
        {
            material_image = cv::imread("..\\Resources\\Texture\\Coalmine\\wandtrap_h_04.png", cv::IMREAD_COLOR);
            visual_image = cv::imread("..\\Resources\\Texture\\Coalmine\\wandtrap_h_04_visual.png", cv::IMREAD_UNCHANGED);
            break;
        }
        }
        //material_image = cv::imread("..\\Resources\\Texture\\Coalmine\\symbolroom.png", cv::IMREAD_COLOR);
        //material_image = cv::imread("..\\Resources\\Texture\\Coalmine\\physics_03.png", cv::IMREAD_COLOR);
        //material_image = cv::imread("..\\Resources\\Texture\\Coalmine\\radioactivecave.png", cv::IMREAD_COLOR);
        cv::cvtColor(material_image, material_image, cv::COLOR_BGR2RGB);

        for (int i = 0; i < 130; i++)
        {
            for (int j = 0; j < 260; j++)
            {
                uint32_t color = Vec3bToColor(material_image.at<cv::Vec3b>(i, j));

                InsertElementFromWangColor(color, x + j, y + i);
            }
        }


        DrawRandomSceneImage(material_image, visual_image, x, y);
    }
    void PixelWorld::LoadRandomScene_03(int x, int y)
    {
        cv::Mat material_image;
        cv::Mat visual_image;

        switch (randi(4))
        {
        case 0:
        {
            material_image = cv::imread("..\\Resources\\Texture\\Coalmine\\oiltank_1.png", cv::IMREAD_COLOR);
            visual_image = cv::imread("..\\Resources\\Texture\\Coalmine\\oiltank_1_visual.png", cv::IMREAD_UNCHANGED);
            break;
        }
        case 1:
        {
            material_image = cv::imread("..\\Resources\\Texture\\Coalmine\\oiltank_2.png", cv::IMREAD_COLOR);
            visual_image = cv::imread("..\\Resources\\Texture\\Coalmine\\oiltank_2_visual.png", cv::IMREAD_UNCHANGED);
            break;
        }
        case 2:
        {
            material_image = cv::imread("..\\Resources\\Texture\\Coalmine\\oiltank_3.png", cv::IMREAD_COLOR);
            visual_image = cv::imread("..\\Resources\\Texture\\Coalmine\\oiltank_3_visual.png", cv::IMREAD_UNCHANGED);
            break;
        }
        case 3:
        {
            material_image = cv::imread("..\\Resources\\Texture\\Coalmine\\oiltank_4.png", cv::IMREAD_COLOR);
            visual_image = cv::imread("..\\Resources\\Texture\\Coalmine\\oiltank_4_visual.png", cv::IMREAD_UNCHANGED);
            break;
        }
        case 4:
        {
            material_image = cv::imread("..\\Resources\\Texture\\Coalmine\\oiltank_puzzle.png", cv::IMREAD_COLOR);
            visual_image = cv::imread("..\\Resources\\Texture\\Coalmine\\oiltank_puzzle_visual.png", cv::IMREAD_UNCHANGED);
            break;
        }
        }

        cv::cvtColor(material_image, material_image, cv::COLOR_BGR2RGB);

        for (int i = 0; i < 260; i++)
        {
            for (int j = 0; j < 130; j++)
            {
                uint32_t color = Vec3bToColor(material_image.at<cv::Vec3b>(i, j));

                InsertElementFromWangColor(color, x + j, y + i);
            }
        }

        DrawRandomSceneImage(material_image, visual_image, x, y);
    }

    void PixelWorld::LoadStartTile(int x, int y)
    {
        cv::Mat material_image = cv::imread("..\\Resources\\Texture\\Coalmine\\coalpit01.png", cv::IMREAD_COLOR);
        cv::Mat visual_image = cv::imread("..\\Resources\\Texture\\Coalmine\\coalpit01_visual.png", cv::IMREAD_UNCHANGED);

        cv::cvtColor(material_image, material_image, cv::COLOR_BGR2RGB);


        for (int i = 0; i < 260; i++)
        {
            for (int j = 0; j < 130; j++)
            {
                uint32_t color = Vec3bToColor(material_image.at<cv::Vec3b>(i, j));

                InsertElementFromWangColor(color, x + j, y + i);
            }
        }

        DrawRandomSceneImage(material_image, visual_image, x, y);
    }

    bool PixelWorld::InBounds(int x, int y)
    {
        if (PixelChunk* chunk = GetChunk(x, y))
        {
            return chunk->InBounds(x, y);
        }

        return false;
    }

    void PixelWorld::RenewalChunkBody(int x, int y)
    {
        if (PixelChunk* chunk = GetChunk(x, y))
        {
            chunk->RenewalBody();
        }
    }

    void PixelWorld::DrawPixels()
    {
        if (!Application::OnDebugMode) return;

        if (Input::GetKey(eKeyCode::W) || Input::GetKey(eKeyCode::R) || Input::GetKey(eKeyCode::S)
            || Input::GetKey(eKeyCode::E) || Input::GetKey(eKeyCode::F) || Input::GetKey(eKeyCode::T)
            || Input::GetKey(eKeyCode::O) || Input::GetKey(eKeyCode::G))
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
            else if ((Input::GetKey(eKeyCode::O)))
                mSelectElement = mElementMap.find('o')->second;
            else if ((Input::GetKey(eKeyCode::G)))
                mSelectElement = mElementMap.find('g')->second;
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
                        Box2dWorld::Draw(pos.x, -pos.y);
                    }
                    else
                    {

                    }
                }
            }
        }
    }

    void PixelWorld::Temp()
    {
        Box2dWorld::ReconstructBody();
        Box2dWorld::Update();
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

    uint32_t PixelWorld::getMaterialColor(const std::wstring& material_name, int x, int y)
    {
        auto iter = mMaterialImages.find(material_name);
        auto image = iter->second;


        uint32_t converted_color;

        if (image->imageBits == 24)
        {
            cv::Vec3b color = image->image.at<cv::Vec3b>(y % image->height, x % image->width);

            converted_color = 0xFF000000;
            converted_color |= (color[2] << 16);
            converted_color |= (color[1] << 8);
            converted_color |= color[0];
        }
        else
        {
            cv::Vec4b color = image->image.at<cv::Vec4b>(y % image->height, x % image->width);
            converted_color = Vec4bToColor(color);
        }

        return converted_color;
    }

    void PixelWorld::loadMaterialImage()
    {
        {
            MaterialImage* image = new MaterialImage();

            image->image = cv::imread("..\\Resources\\Texture\\Material\\rock.png", cv::IMREAD_UNCHANGED);
            image->width = image->image.cols;
            image->height = image->image.rows;
            image->imageBits = 32;

            mMaterialImages.insert({ L"rock", image });
        }
        {
            MaterialImage* image = new MaterialImage();

            image->image = cv::imread("..\\Resources\\Texture\\Material\\earth.png", cv::IMREAD_UNCHANGED);
            image->width = image->image.cols;
            image->height = image->image.rows;
            image->imageBits = 32;

            mMaterialImages.insert({ L"earth", image });
        }
        {
            MaterialImage* image = new MaterialImage();

            image->image = cv::imread("..\\Resources\\Texture\\Material\\grass.png", cv::IMREAD_UNCHANGED);
            image->width = image->image.cols;
            image->height = image->image.rows;
            image->imageBits = 32;

            mMaterialImages.insert({ L"grass", image });
        }
        {
            MaterialImage* image = new MaterialImage();

            image->image = cv::imread("..\\Resources\\Texture\\Material\\rock_alt2.png", cv::IMREAD_UNCHANGED);
            image->width = image->image.cols;
            image->height = image->image.rows;
            image->imageBits = 32;

            mMaterialImages.insert({ L"rock_alt2", image });
        }
        {
            MaterialImage* image = new MaterialImage();

            image->image = cv::imread("..\\Resources\\Texture\\Material\\coal.png", cv::IMREAD_UNCHANGED);
            image->width = image->image.cols;
            image->height = image->image.rows;
            image->imageBits = 32;

            mMaterialImages.insert({ L"coal", image });
        }
        {
            MaterialImage* image = new MaterialImage();

            image->image = cv::imread("..\\Resources\\Texture\\Material\\gunpowder_tnt.png", cv::IMREAD_UNCHANGED);
            image->width = image->image.cols;
            image->height = image->image.rows;
            image->imageBits = 32;

            mMaterialImages.insert({ L"gunpowder_tnt", image });
        }
        {
            MaterialImage* image = new MaterialImage();

            image->image = cv::imread("..\\Resources\\Texture\\Material\\rock_radioactive.png", cv::IMREAD_UNCHANGED);
            image->width = image->image.cols;
            image->height = image->image.rows;
            image->imageBits = 24;

            mMaterialImages.insert({ L"rock_radioactive", image });
        }
        {
            MaterialImage* image = new MaterialImage();

            image->image = cv::imread("..\\Resources\\Texture\\Material\\templebrick.png", cv::IMREAD_UNCHANGED);
            image->width = image->image.cols;
            image->height = image->image.rows;
            image->imageBits = 24;

            mMaterialImages.insert({ L"templebrick", image });
        }
        {
            MaterialImage* image = new MaterialImage();

            image->image = cv::imread("..\\Resources\\Texture\\Material\\gold.png", cv::IMREAD_UNCHANGED);
            image->width = image->image.cols;
            image->height = image->image.rows;
            image->imageBits = 32;

            mMaterialImages.insert({ L"gold", image });
        }
    }

    void PixelWorld::createVegetation(int x, int y)
    {
        Vegetation* object = new Vegetation();
        object->GetComponent<Transform>()->SetPosition(x, -y, 0.0f);

        CreateObject(object, eLayerType::Vegetation);
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