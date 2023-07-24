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

        virtual void UpdateCell( int x, int y, Element& cell) = 0;

        Element& GetElement(int x, int y);
        void SwapElement(int x, int y, const Element& cell);
        void SwapElement(int x, int y, int xto, int yto);

        bool InBounds(int x, int y) { return mChunk->InBounds(x, y) || PixelWorld::InBounds(x, y); }
        bool IsEmpty(int x, int y);

    protected:
        PixelChunk* mChunk;

    };

    class SimplePixelUpdater : public PixelUpdater {
    public:
        SimplePixelUpdater(PixelChunk* chunk) : PixelUpdater(chunk) {}

        void UpdateCell(int x, int y, Element& cell) override
        {
            if (cell.Move& eElementMove ::MOVE_DOWN_SOLID && MoveDownSolid(x, y, cell)) {}
            else if (cell.Move & eElementMove::MOVE_DOWN_SIDE && MoveDownSide(x, y, cell)) {}
            else if (cell.Move & eElementMove::MOVE_DOWN_LIQUID && MoveDownLiquid(x, y, cell)) {}
            else if (cell.Move & eElementMove::MOVE_SIDE && MoveSide(x, y, cell)) {}
        }

    private:
        bool MoveDownSolid(int x, int y, Element& cell)
        {
            float posY = y;
            for(int i=1; i <= cell.Velocity.y ; i++)
            {
                const Element& target = GetElement(x, y + i);
                if (target.Type == eElementType::EMPTY || target.Type == eElementType::LIQUID)
                {
                    if (target.Type == eElementType::LIQUID) 
                        cell.Velocity.y -= 1;
                    posY = y + i;
                }
                else
                {
                    break;
                }
            }

            if (posY == y) return false;
            
            SwapElement(x, y, x, posY);
            return true;
        }
        bool MoveDownLiquid(int x, int y, Element& cell)
        {
            const Element& target = GetElement(x, y + 1);
            if (target.Type == eElementType::EMPTY)
            {
                SwapElement(x, y, x, y + 1);
                return true;
            }
            return false;
        }

        bool MoveDown(int x, int y, Element& cell) 
        { 
            if (GetElement(x, y + 1).Type == eElementType::EMPTY)
            {
                SwapElement(x, y, x, y + 1);
                return true;
            }
            return false;
        }

        bool MoveSide(int x, int y, Element& cell) 
        {
            int dir = random() > 0.5 ? -1 : 1;
            float posX = x;
            for(int i = 1; i <= cell.Velocity.x; i++)
            {
                const Element& target = GetElement(x + i * dir, y);
                if (i != 1 && (target.Type == eElementType::EMPTY || target.Type == eElementType::LIQUID))
                {
                    posX = x + i * dir;
                }
                else if (target.Type == eElementType::EMPTY)
                {
                    posX = x + i * dir;
                }
                else
                {
                    break;
                }
            }

            if(posX == x) return false;

            SwapElement(x, y, posX, y );
            return true;
        }

        bool MoveDownSide(int x, int y, Element& cell)
        {
            int dir = random() > 0.5 ? -1 : 1;
            const Element& target = GetElement(x + dir, y + 1);

            if (target.Type == eElementType::EMPTY || target.Type == eElementType::LIQUID)
            {
                SwapElement(x, y, x + dir, y + 1);
                return true;
            }
            else
            {
                const Element& target = GetElement(x - dir, y + 1);
                if (target.Type == eElementType::EMPTY || target.Type == eElementType::LIQUID)
                {
                    SwapElement(x, y, x - dir, y + 1);
                    return true;
                }
            }
            return false;
        }
    };
}
