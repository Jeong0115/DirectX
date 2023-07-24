#pragma once

#include "zzEngine.h"
#include "zzPixelChunk.h"

namespace zz
{
    class PixelChunkMap
    {
    public:
        PixelChunkMap(UINT mapWidth, UINT mapHeight, int x, int y);
        ~PixelChunkMap();

        void UpdateStep(int index);
        void MoveElementStep(int index);

        void UpdateStep1();
        void UpdateStep2();
        void UpdateStep3();
        void UpdateStep4();

        void MoveElementStep1();
        void MoveElementStep2();
        void MoveElementStep3();
        void MoveElementStep4();

        void UpdateRect();

        PixelChunk* GetChunk(int x, int y);

    public:
        const int mStartX;
        const int mStartY;
        const int mChunkWidth;
        const int mChunkHeight;
        const UINT mHeight;
        const UINT mWidth;
        const int mYChunkCnt;
        const int mXChunkCnt;

    private:
        std::vector<PixelChunk*> mChunks;
    };
}
