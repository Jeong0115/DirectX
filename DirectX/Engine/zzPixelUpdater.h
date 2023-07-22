#pragma once

#include "zzElement.h"
#include "zzPixelChunk.h"
#include "zzPixelWorld.h"

namespace zz
{
    class PixelUpdater
    {
    public:
        PixelUpdater(PixelChunk* chunk);
        ~PixelUpdater();

        void UpdateChunk();

        virtual void UpdateCell( int x, int y, Element* cell) = 0;

        Element* GetElement(int x, int y);
        void SwapElement(int x, int y, Element* cell);
        void SwapElement(int x, int y, int xto, int yto);

        bool InBounds(int x, int y) { return mChunk->InBounds(x, y) || PixelWorld::InBounds(x, y); }
        bool IsEmpty(int x, int y);

    protected:
        PixelChunk* mChunk;

    };
    std::mutex mx;
    class SimplePixelUpdater : public PixelUpdater {
    public:
        SimplePixelUpdater(PixelChunk* chunk) : PixelUpdater(chunk) {}

        void UpdateCell(int x, int y, Element* cell) override
        {
            cell->Update();
        }

    private:

    };
}
