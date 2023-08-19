#include "zzPixelChunk.h"
#include "zzPixelWorld.h"
#include "zzRenderer.h"

#define _DEST(x) std::get<2>(x)

namespace zz
{

    PixelChunk::PixelChunk(size_t width, size_t height, int x, int y)
        : mWidth((int)width)
        , mHeight((int)height)
        , mStartX(x)
        , mStartY(y)
        , mMinX((int)width)
        , mMinY((int)height)
        , mMaxX(-1)
        , mMaxY(-1)
        , m_minXw((int)width), m_minYw((int)height)
        , m_maxXw(-1), m_maxYw(-1)
        , mElementCount(0)
        , mDirtyBoxMutex{}
        , mUpdateDir{}
    {
        mElements = new Element[mWidth * mHeight];
        for (int i = 0; i < mWidth * mHeight; i++)
        {
            //mElements[i] = EMPTY;
            mElements[i].x = mStartX + i % mWidth;
            mElements[i].y = mStartY + i / mWidth;
        }
    }

    PixelChunk::~PixelChunk()
    {
        delete[] mElements;
    }


    void PixelChunk::Update()
    {
        mUpdateDir.flip();

        for (int y = mMaxY - 1; y >= mMinY; y--)
        {
            if ((y + mUpdateDir[0]) % 2 == 0)
            {
                for (int x = mMinX; x < mMaxX; x++)
                {
                    Element& element = mElements[y * mWidth + x];

                    if (PixelWorld::FrameCount != element.ElementFrameCount)
                    {
                        element.ElementFrameCount = PixelWorld::FrameCount;
                    }
                    else
                    {
                        continue;
                    }

                    int worldX = x + mStartX;
                    int worldY = y + mStartY;

                    if (element.Type == eElementType::EMPTY)
                        continue;

                    if (element.CurMove > 0)
                    {
                        element.CurMove--;
                        KeepAlive(worldX, worldY);
                    }

                    if (element.Move & eElementMove::MOVE_DOWN_SOLID && MoveDownSolid(worldX, worldY, element)) {}
                    else if (element.Move & eElementMove::MOVE_DOWN_SIDE && MoveDownSide(worldX, worldY, element)) {}
                    else if (element.Move & eElementMove::MOVE_DOWN_LIQUID && MoveDownLiquid(worldX, worldY, element)) {}
                    else if (element.Move & eElementMove::MOVE_SIDE && MoveSide(worldX, worldY, element)) {}
                }
            }
            else
            {
                for (int x = mMaxX - 1; x >= mMinX; x--)
                {
                    Element& element = mElements[y * mWidth + x];

                    if (PixelWorld::FrameCount != element.ElementFrameCount)
                    {
                        element.ElementFrameCount = PixelWorld::FrameCount;
                    }
                    else
                    {
                        continue;
                    }

                    int worldX = x + mStartX;
                    int worldY = y + mStartY;

                    if (element.Type == eElementType::EMPTY)
                        continue;

                    if (element.CurMove > 0)
                    {
                        element.CurMove--;
                        KeepAlive(worldX, worldY);
                    }

                    if (element.Move & eElementMove::MOVE_DOWN_SOLID && MoveDownSolid(worldX, worldY, element)) {}
                    else if (element.Move & eElementMove::MOVE_DOWN_SIDE && MoveDownSide(worldX, worldY, element)) {}
                    else if (element.Move & eElementMove::MOVE_DOWN_LIQUID && MoveDownLiquid(worldX, worldY, element)) {}
                    else if (element.Move & eElementMove::MOVE_SIDE && MoveSide(worldX, worldY, element)) {}
                }
            }
        }


        //if (m_minXw < mStartX && m_minYw < mStartY)
        //{
        //    PixelWorld::GetChunk(m_minXw, m_minYw)->TKA(m_minXw, m_minYw);
        //    PixelWorld::GetChunk(m_minXw, m_maxYw)->TKA(mStartX - 1, mStartY - 1);
        //}
        //if (m_maxXw >= mStartX + mWidth && m_minYw < mStartY)
        //{
        //    PixelWorld::GetChunk(m_maxXw, m_minYw)->TKA(m_maxXw, m_minYw);
        //    PixelWorld::GetChunk(m_maxXw, m_maxYw)->TKA(mStartX + mWidth, mStartY - 1);
        //}
        //if (m_minYw < mStartY)
        //{
        //    PixelWorld::GetChunk(m_minXw, m_minYw)->TKA(m_maxXw, m_minYw);
        //    PixelWorld::GetChunk(m_minXw, m_maxYw)->TKA(mStartX + mWidth, mStartY - 1);
        //}

            //m_minXw
            //m_minYw
            //m_maxXw
            //m_maxYw
    }

    void PixelChunk::SwapElement(Element& dstElement, int dstX, int dstY, Element& srcElement, int srcX, int srcY)
    {   
        srcElement.CurMove = srcElement.MaxMove;
        dstElement.CurMove = dstElement.MaxMove;

        Element dest = dstElement;
        dstElement = srcElement;
        srcElement = dest;

        memcpy(&PixelWorld::GetPixelColor()[(srcX + ((srcY) * 2048)) * 4], &srcElement.Color, 4);
        memcpy(&PixelWorld::GetPixelColor()[(dstX + ((dstY) * 2048)) * 4], &dstElement.Color, 4);
        //KeepAlive(srcX, srcY);

        if (InBounds(dstX, dstY))
        {
            KeepAlive(dstX, dstY);
        }
        else
        {
            PixelWorld::GetChunk(dstX, dstY)->KeepAlive(dstX, dstY);
        }
    }

    void PixelChunk::InsertElement(size_t index, const Element& element)  // 수정해라
    {
        Element& dest = mElements[index];

        if (dest.Type == eElementType::EMPTY && element.Type != eElementType::EMPTY)
        {
            mElementCount++;
        }

        else if (dest.Type != eElementType::EMPTY && element.Type == eElementType::EMPTY)
        {
            mElementCount--;
        }

        mElements[index] = element;

        //element->SetPos(Position((int)(mStartX + index % mWidth), (int)(mStartY + index / mWidth)));
        memcpy(&PixelWorld::GetPixelColor()[(mStartX + index % mWidth + ((mStartY + index / mWidth) * 2048)) * 4], &element.Color, 4);

        KeepAlive(index);
    }

    void PixelChunk::RegisterElement(size_t index, const Element& element)
    {
        Element& dest = mElements[index];

        if (dest.Type != eElementType::EMPTY)
        {
            mElementCount--;
        }

        mElements[index] = element;

        //element->SetPos(Position((int)(mStartX + index % mWidth), (int)(mStartY + index / mWidth)));
        memcpy(&PixelWorld::GetPixelColor()[(mStartX + index % mWidth + ((mStartY + index / mWidth) * 2048)) * 4], &element.Color, 4);

    }

    void PixelChunk::DelteElement(size_t index)
    {
        mElements[index] = EMPTY;
        mElementCount--;
        memcpy(&PixelWorld::GetPixelColor()[(mStartX + index % mWidth + ((mStartY + index / mWidth) * 2048)) * 4], &mElements[index].Color, 4);

        KeepAlive(index);
    }

    bool PixelChunk::TakeElement(size_t index)
    {
        if (mElements[index].Name == L"DeleteEmpty")
        {
            return false;
        }
        else
        {
            mElements[index] = EMPTY;
            memcpy(&PixelWorld::GetPixelColor()[(mStartX + index % mWidth + ((mStartY + index / mWidth) * 2048)) * 4], &mElements[index].Color, 4);

            return true;
        }
    }

    void PixelChunk::ResiterChanges(PixelChunk* source, int x, int y, int toX, int toY)
    {
        mElementCount++;
        if (source == nullptr)
        {
            int a = 0;
        }
        std::tuple chage = { source, source->GetIndex(x, y), GetIndex(toX, toY) };
        mChanges.push_back(chage);
        //mChanges.emplace_back( source, source->GetIndex(x, y), GetIndex(toX, toY));

    }

    void PixelChunk::KeepAlive(size_t index)
    {
        std::unique_lock<std::mutex>(mDirtyBoxMutex);

        int x = (int)(index % mWidth);
        int y = (int)(index / mWidth);

        m_minXw = std::clamp(std::min(x - 2, m_minXw), 0, mWidth);
        m_minYw = std::clamp(std::min(y - 2, m_minYw), 0, mHeight);
        m_maxXw = std::clamp(std::max(x + 2, m_maxXw), 0, mWidth);
        m_maxYw = std::clamp(std::max(y + 2, m_maxYw), 0, mHeight);

        //if (x - 2 < 0 || y - 2 < 0 || x + 2 >= mWidth || y + 2 >= mHeight)
        //{

        //}

        //m_minXw = std::clamp(std::min(x, m_minXw), 0, mWidth);
        //m_minYw = std::clamp(std::min(y, m_minYw), 0, mHeight);
        //m_maxXw = std::clamp(std::max(x, m_maxXw), 0, mWidth);
        //m_maxYw = std::clamp(std::max(y, m_maxYw), 0, mHeight);
    }

    //void PixelChunk::TKA(int x, int y)
    //{
    //    std::unique_lock<std::mutex>(mDirtyBoxMutex);

    //    m_minXw = (std::min(x, m_minXw));
    //    m_minYw = (std::min(y, m_minYw));
    //    m_maxXw = (std::max(x, m_maxXw));
    //    m_maxYw = (std::max(y, m_maxYw));
    //}

    //void PixelChunk::TempKeepAlive(int x, int y)
    //{
    //    std::unique_lock<std::mutex>(mDirtyBoxMutex);

    //    m_minXw = (std::min(x - 2, m_minXw));
    //    m_minYw = (std::min(y - 2, m_minYw));
    //    m_maxXw = (std::max(x + 2, m_maxXw));
    //    m_maxYw = (std::max(y + 2, m_maxYw));

    //   
    //}

    void PixelChunk::UpdateRect()
    {
        mMinX = m_minXw;  m_minXw = mWidth;
        mMinY = m_minYw;  m_minYw = mHeight;
        mMaxX = m_maxXw;  m_maxXw = -1;
        mMaxY = m_maxYw;  m_maxYw = -1;

        if (mMaxX <= -1) return;
        DebugMesh mesh = {};
        mesh.position = math::Vector3((mMaxX + mMinX) / 2.f + mStartX, -((mMaxY + mMinY) / 2.f + mStartY), 0.0f);
        //mesh.position = math::Vector3(0,0, 0.0f);
        mesh.scale = math::Vector3((float)(mMaxX - mMinX), (float)(mMaxY - mMinY), 1.0f);
        mesh.rotation = math::Vector3::Zero;

        renderer::PushDebugMeshAttribute(mesh);
    }

    Element& PixelChunk::GetElement(int x, int y)
    {
        if (InBounds(x, y))
        {
            return mElements[GetIndex(x, y)];
        }
        else
        {
            return PixelWorld::GetElement(x, y);
        }
    }

    bool PixelChunk::MoveDownSolid(int x, int y, Element& element)
    {
        float posY = y;
        for (int i = 1; i <= element.Velocity.y; i++)
        {
            Element& target = GetElement(x, y + i);
            if (target.Type == eElementType::EMPTY || target.Type == eElementType::LIQUID)
            {
                if (target.Type == eElementType::LIQUID)
                    element.Velocity.y -= 1;
                posY = y + i;
            }
            else
            {
                break;
            }
        }

        if (posY == y) return false;

        element.Velocity.x = 2;
        SwapElement(PixelWorld::GetElement(x, posY), x, posY, element, x, y);

        return true;
    }
    bool PixelChunk::MoveDownLiquid(int x, int y, Element& element)
    {
        float posY = y;
        for (int i = 1; i <= element.Velocity.y; i++)
        {
            Element& target = GetElement(x, y + i);
            if (target.Type == eElementType::EMPTY)
            {
                posY = y + i;
            }
            else
            {
                break;
            }
        }

        if (posY == y) return false;

        SwapElement(PixelWorld::GetElement(x, posY), x, posY, element, x, y);
        return true;
    }
    bool PixelChunk::MoveDown(int x, int y, Element& element)
    {
        Element& target = GetElement(x, y + 1);
        if (target.Type == eElementType::EMPTY)
        {
            SwapElement(target, x, y + 1, element, x, y);
            return true;
        }
        return false;
    }
    bool PixelChunk::MoveSide(int x, int y, Element& element)
    {
        int dir = random() > 0.5 ? -1 : 1;
        float posX = x;
        for (int i = 1; i <= element.Velocity.x; i++)
        {
            Element& target = GetElement(x + i * dir, y);

            if (target.Type == eElementType::EMPTY)
            {
                posX = x + i * dir;
            }
            else
            {
                break;
            }
        }

        if (posX == x)
        {
            dir *= -1;
            float posX = x;
            for (int i = 1; i <= element.Velocity.x; i++)
            {
                Element& target = GetElement(x + i * dir, y);

                if (target.Type == eElementType::EMPTY)
                {
                    posX = x + i * dir;
                }
                else
                {
                    break;
                }
            }
        }
        else
        {
            SwapElement(PixelWorld::GetElement(posX, y), posX, y, element, x, y);
            return true;
        }

        if (posX == x) return false;

        SwapElement(PixelWorld::GetElement(posX, y), posX, y, element, x, y);
        return true;
    }
    bool PixelChunk::MoveDownSide(int x, int y, Element& element)
    {
        int dir = random() > 0.5 ? -1 : 1;
        Element& target = GetElement(x + dir, y + 1);

        if (target.Type == eElementType::EMPTY || target.Type == eElementType::LIQUID)
        {
            SwapElement(target, x + dir, y + 1, element, x, y);
            return true;
        }
        else
        {
            Element& target = GetElement(x - dir, y + 1);
            if (target.Type == eElementType::EMPTY || target.Type == eElementType::LIQUID)
            {
                SwapElement(target, x - dir, y + 1, element, x, y);
                return true;
            }
        }

        float posX = x;
        for (int i = 1; i <= element.Velocity.x; i++)
        {
            Element& target = GetElement(x + i * dir, y);

            if (target.Type == eElementType::EMPTY || target.Type == eElementType::LIQUID)
            {
                posX = x + i * dir;
            }
            else
            {
                break;
                element.Velocity.x--;
            }
        }

        if (posX == x)
        {
            dir *= -1;
            float posX = x;
            for (int i = 1; i <= element.Velocity.x; i++)
            {
                Element& target = GetElement(x + i * dir, y);

                if (target.Type == eElementType::EMPTY || target.Type == eElementType::LIQUID)
                {
                    posX = x + i * dir;
                }
                else
                {
                    break;
                    //cell.Velocity.x--;
                }
            }
        }
        else
        {
            element.Velocity.x--;
            SwapElement(PixelWorld::GetElement(posX, y), posX, y, element, x, y);
            return true;
        }

        if (posX == x) return false;

        SwapElement(PixelWorld::GetElement(posX, y), posX, y, element, x, y);
        return true;
    }
}