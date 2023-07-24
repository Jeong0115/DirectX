#include "zzPixelChunkMap.h"
#include "zzPixelUpdater.h"
#include "zzRenderer.h"
#include "zzPixelWorld.h"

namespace zz
{
    PixelChunkMap::PixelChunkMap(UINT mapWidth, UINT mapHeight, int x, int y)
        : mHeight(mapHeight)
        , mWidth(mapWidth)
        , mStartX(x * mapWidth)
        , mStartY(y * mapHeight)
        , mChunkWidth(64)
        , mChunkHeight(64)
        , mYChunkCnt(mHeight / mChunkHeight)
        , mXChunkCnt(mWidth / mChunkWidth)
    {
        for (int i = 0; i < 8; i++)
        {
            for (int j = 0; j < 8; j++)
            {
                mChunks.push_back(new PixelChunk(mChunkWidth, mChunkHeight
                    , mStartX + mChunkWidth * j, mStartY + mChunkHeight * i));
            }
        }
    }
    PixelChunkMap::~PixelChunkMap()
    {
        for (int i = 0; i < 8; i++)
        {
            for (int j = 0; j < 8; j++)
            {
                delete mChunks[j + i * mXChunkCnt];
            }
        }
    }

    void PixelChunkMap::UpdateStep(int index)
    {
        switch (index)
        {
        case 0:
            UpdateStep1();
            break;

        case 1:
            UpdateStep2();
            break;

        case 2:
            UpdateStep3();
            break;

        case 3:
            UpdateStep4();
            break;

        default: 
            break;

        }
    }

    void PixelChunkMap::MoveElementStep(int index)
    {
        switch (index)
        {
        case 0:
            MoveElementStep1();
            break;

        case 1:
            MoveElementStep2();
            break;

        case 2:
            MoveElementStep3();
            break;

        case 3:
            MoveElementStep4();
            break;

        default:
            break;

        }
    }

    void PixelChunkMap::UpdateStep1()
    {
        for (int i = 0; i < 8; i += 2)
        {
            for (int j = 0; j < 8; j += 2)
            {
                if (mChunks[j * mXChunkCnt + i]->mElementCount == 0) continue;
                PixelWorld::threadPool.enqueue([=]() {SimplePixelUpdater updater(mChunks[j * mXChunkCnt + i]); updater.UpdateChunk(); });

                //SimplePixelUpdater(mChunks[j * mChunkWidth + i]).UpdateChunk();

                //DebugMesh mesh = {};
                //mesh.temp = 1;
                //mesh.position = math::Vector3(mChunks[j * mChunkWidth + i]->mStartX + 32.f - 256.f, -mChunks[j * mChunkWidth + i]->mStartY - 32.f + 256.f, 0.01f);
                //mesh.scale = math::Vector3(64, 64, 1.0f);
                //mesh.rotation = math::Vector3::Zero;
                //renderer::PushDebugMeshAttribute(mesh);
            }
        }
    }
    void PixelChunkMap::UpdateStep2()
    {
        for (int i = 0; i < 8; i += 2)
        {
            for (int j = 1; j < 8; j += 2)
            {
                if (mChunks[j * mXChunkCnt + i]->mElementCount == 0) continue;
                //SimplePixelUpdater(mChunks[j * mChunkWidth + i]).UpdateChunk();
                PixelWorld::threadPool.enqueue([=]() {SimplePixelUpdater updater(mChunks[j * mXChunkCnt + i]); updater.UpdateChunk(); });

            }
        }
    }
    void PixelChunkMap::UpdateStep3()
    {
        for (int i = 1; i < 8; i += 2)
        {
            for (int j = 0; j < 8; j += 2)
            {
                if (mChunks[j * mXChunkCnt + i]->mElementCount == 0) continue;
                PixelWorld::threadPool.enqueue([=]() {SimplePixelUpdater updater(mChunks[j * mXChunkCnt + i]); updater.UpdateChunk(); });

            }
        }
    }
    void PixelChunkMap::UpdateStep4()
    {
        for (int i = 1; i < 8; i += 2)
        {
            for (int j = 1; j < 8; j += 2)
            {
                if (mChunks[j * mXChunkCnt + i]->mElementCount == 0) continue;
                PixelWorld::threadPool.enqueue([=]() {SimplePixelUpdater updater(mChunks[j * mXChunkCnt + i]); updater.UpdateChunk(); });
                //SimplePixelUpdater(mChunks[j * mChunkWidth + i]).UpdateChunk();
            }
        }
    }

    void PixelChunkMap::MoveElementStep1()
    {
        for (int i = 0; i < 8; i += 2)
        {
            for (int j = 0; j < 8; j += 2)
            {
                if (mChunks[j * mXChunkCnt + i]->mElementCount == 0) continue;
                PixelWorld::threadPool.enqueue([=]() {mChunks[j * mXChunkCnt + i]->MoveElements(); });
                //mChunks[j * mChunkWidth + i]->MoveElements();
            }
        }
    }
    void PixelChunkMap::MoveElementStep2()
    {
        for (int i = 0; i < 8; i += 2)
        {
            for (int j = 1; j < 8; j += 2)
            {
                if (mChunks[j * mXChunkCnt + i]->mElementCount == 0) continue;
                PixelWorld::threadPool.enqueue([=]() {mChunks[j * mXChunkCnt + i]->MoveElements(); });
                //mChunks[j * mChunkWidth + i]->MoveElements();
            }
        }
    }
    void PixelChunkMap::MoveElementStep3()
    {
        for (int i = 1; i < 8; i += 2)
        {
            for (int j = 0; j < 8; j += 2)
            {
                if (mChunks[j * mXChunkCnt + i]->mElementCount == 0) continue;
                PixelWorld::threadPool.enqueue([=]() {mChunks[j * mXChunkCnt + i]->MoveElements(); });
                //mChunks[j * mChunkWidth + i]->MoveElements();
            }
        }
    }
    void PixelChunkMap::MoveElementStep4()
    {
        for (int i = 1; i < 8; i += 2)
        {
            for (int j = 1; j < 8; j += 2)
            {
                if (mChunks[j * mXChunkCnt + i]->mElementCount == 0) continue;
                //mChunks[j * mChunkWidth + i]->MoveElements();
                PixelWorld::threadPool.enqueue([=]() {mChunks[j * mXChunkCnt + i]->MoveElements(); });
            }
        }
    }

    void PixelChunkMap::UpdateRect()
    {
        for (int i = 0; i < 8; i ++)
        {
            for (int j = 0; j < 8; j ++)
            {
                PixelWorld::threadPool.enqueue([=]() {mChunks[j * mXChunkCnt + i]->UpdateRect(); });
            }
        }
    }

    PixelChunk* PixelChunkMap::GetChunk(int x, int y)
    {
        x -= mStartX;
        y -= mStartY;
        return mChunks[(y / mChunkHeight * mXChunkCnt) + x / mChunkWidth];
    }
}