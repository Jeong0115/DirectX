#include "zzPixelChunk.h"
#include "zzPixelWorld.h"
#include "zzRenderer.h"
#include "zzTime.h"

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
        mElements = nullptr;
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
                    UpdateElement(x, y);
                }
            }
            else
            {
                for (int x = mMaxX - 1; x >= mMinX; x--)
                {
                    UpdateElement(x, y);
                }
            }
        }
    }

    void PixelChunk::UpdateElement(int x, int y)
    {
        Element& element = mElements[y * mWidth + x];

        if (PixelWorld::FrameCount != element.ElementFrameCount)
        {
            element.ElementFrameCount = PixelWorld::FrameCount;
        }
        else
        {
            return;
        }

        if (element.Type == eElementType::EMPTY)
        {
            return;
        }

        int worldX = x + mStartX;
        int worldY = y + mStartY;


        if (!(element.Temp & eElementUpdate::NONE))
        {
            if (element.Temp & eElementUpdate::HEAT_TRANSFER)
            {
                if (element.onFire)
                {
                    HeatNeighbors(worldX, worldY, element);
                    KeepAlive(worldX, worldY);
                }
            }

            if (element.Temp & eElementUpdate::DECREASE_LIFE_TIME)
            {
                DecreaseLifeTime(worldX, worldY, element);
                KeepAlive(worldX, worldY);
            }
        }

        if (element.Move & eElementMove::MOVE_DOWN_SOLID && MoveDownSolid(worldX, worldY, element)) {}
        else if (element.Move & eElementMove::MOVE_DOWN_SIDE && MoveDownSide(worldX, worldY, element)) {}
        else if (element.Move & eElementMove::MOVE_DOWN_LIQUID && MoveDownLiquid(worldX, worldY, element)) {}
        else if (element.Move & eElementMove::MOVE_SIDE && MoveSide(worldX, worldY, element)) {}
        else if (element.Move & eElementMove::MOVE_GAS && MoveGas(worldX, worldY, element)) {}
        else
        {
            if (element.StopCount > 0)
            {
                element.StopCount--;
                KeepAlive(worldX, worldY);
            }
        }
    }

    void PixelChunk::SwapElement(Element& dstElement, int dstX, int dstY, Element& srcElement, int srcX, int srcY)
    {   
        srcElement.StopCount = srcElement.StopThreshold;
        dstElement.StopCount = dstElement.StopThreshold;

        Element dest = dstElement;
        dstElement = srcElement;
        srcElement = dest;

        memcpy(&PixelWorld::GetPixelColor()[(srcX + ((srcY) * 2048)) * 4], &srcElement.Color, 4);
        memcpy(&PixelWorld::GetPixelColor()[(dstX + ((dstY) * 2048)) * 4], &dstElement.Color, 4);
        KeepAlive(srcX, srcY);

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

    void PixelChunk::KeepAlive(int x, int y)
    {
        std::unique_lock<std::mutex>(mDirtyBoxMutex);

        int relativeX = x - mStartX;
        int relativeY = y - mStartY;

        m_minXw = std::clamp(std::min(relativeX - 2, m_minXw), 0, mWidth);
        m_minYw = std::clamp(std::min(relativeY - 2, m_minYw), 0, mHeight);
        m_maxXw = std::clamp(std::max(relativeX + 2, m_maxXw), 0, mWidth);
        m_maxYw = std::clamp(std::max(relativeY + 2, m_maxYw), 0, mHeight);

        if (relativeX < 2 || relativeY < 2 || relativeX >= mWidth - 2 || relativeY >= mHeight - 2)
        {
            if (relativeX < 2)
                x -= 2;
            else if (relativeX >= mWidth - 2)
                x += 2;

            if (relativeY < 2)
                y -= 2;
            else if (relativeY >= mHeight - 2)
                y += 2;

            if(x >=0 && y>=0 && x<1536 && y<1536)
                PixelWorld::GetChunk(x, y)->KeepAliveBoundary(x, y);
        }
    }

    void PixelChunk::KeepAliveBoundary(int x, int y)
    {
        // 나중에 멀티 쓰레드로 바꿀 때 수정
        int relativeX = x - mStartX;
        int relativeY = y - mStartY;

        m_minXw = std::clamp(std::min(relativeX - 2, m_minXw), 0, mWidth);
        m_minYw = std::clamp(std::min(relativeY - 2, m_minYw), 0, mHeight);
        m_maxXw = std::clamp(std::max(relativeX + 2, m_maxXw), 0, mWidth);
        m_maxYw = std::clamp(std::max(relativeY + 2, m_maxYw), 0, mHeight);
    }

    void PixelChunk::UpdateRect()
    {
        mMinX = m_minXw;  m_minXw = mWidth;
        mMinY = m_minYw;  m_minYw = mHeight;
        mMaxX = m_maxXw;  m_maxXw = -1;
        mMaxY = m_maxYw;  m_maxYw = -1;

        if (mMaxX <= -1) return;
        DebugMesh mesh = {};
        mesh.position = math::Vector3((mMaxX + mMinX) / 2.f + mStartX, -((mMaxY + mMinY) / 2.f + mStartY), 0.0f);
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
            if (target.Type == eElementType::EMPTY || target.Type == eElementType::GAS || target.Type == eElementType::LIQUID)
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
            if (target.Type == eElementType::EMPTY || target.Type == eElementType::GAS)
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
        if (target.Type == eElementType::EMPTY  || target.Type == eElementType::GAS)
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

            if (target.Type == eElementType::EMPTY || target.Type == eElementType::GAS)
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

                if (target.Type == eElementType::EMPTY || target.Type == eElementType::GAS)
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

        if (posX == x) 
        {
            return false;
        }

        SwapElement(PixelWorld::GetElement(posX, y), posX, y, element, x, y);
        return true;
    }
    bool PixelChunk::MoveDownSide(int x, int y, Element& element)
    {
        int dir = random() > 0.5 ? -1 : 1;
        Element& target = GetElement(x + dir, y + 1);

        if (target.Type == eElementType::EMPTY || target.Type == eElementType::GAS || target.Type == eElementType::LIQUID)
        {
            SwapElement(target, x + dir, y + 1, element, x, y);
            return true;
        }
        else
        {
            Element& target = GetElement(x - dir, y + 1);
            if (target.Type == eElementType::EMPTY || target.Type == eElementType::GAS || target.Type == eElementType::LIQUID)
            {
                SwapElement(target, x - dir, y + 1, element, x, y);
                return true;
            }
        }

        float posX = x;
        for (int i = 1; i <= element.Velocity.x; i++)
        {
            Element& target = GetElement(x + i * dir, y);

            if (target.Type == eElementType::EMPTY || target.Type == eElementType::GAS || target.Type == eElementType::LIQUID)
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

                if (target.Type == eElementType::EMPTY || target.Type == eElementType::GAS || target.Type == eElementType::LIQUID)
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
            element.Velocity.x--;
            SwapElement(PixelWorld::GetElement(posX, y), posX, y, element, x, y);
            return true;
        }

        if (posX == x) return false;

        SwapElement(PixelWorld::GetElement(posX, y), posX, y, element, x, y);
        return true;
    }
    bool PixelChunk::MoveGas(int x, int y, Element& element)
    {
        Element& target = GetElement(x, y - 1);
        if (target.Type == eElementType::EMPTY)
        {
            SwapElement(target, x, y - 1, element, x, y);
            return true;
        }
        else
        {
            int dir = random() > 0.5 ? -1 : 1;

            for (int i = 0; i < 2; i++)
            {
                Element& target = GetElement(x + dir, y - 1);

                if (target.Type == eElementType::EMPTY)
                {
                    SwapElement(target, x + dir, y - 1, element, x, y);
                    return true;
                }

                dir *= -1;
            }
        }
        return false;
    }

    void PixelChunk::HeatNeighbors(int x, int y, Element& element)
    {
        element.FireHP -= (float)Time::DeltaTime();

        if (element.FireHP <= 0.f)
        {
            element = EMPTY;
        }
        else
        {
            if (random() > 0.95f)
            {
                if (PixelWorld::GetElement(x, y - 1).Type == eElementType::EMPTY)
                {
                    PixelWorld::InsertElement(x, y - 1, SMOKE);
                }
            }
            element.Color = RandomFireColor();
        }

        memcpy(&PixelWorld::GetPixelColor()[(x + ((y) * 2048)) * 4], &element.Color, 4);

        for (int i = y - 1; i <= y + 1; i++)
        {
            if (i == y) continue;
            for (int j = x - 1; j <= x + 1; j++)
            {
                if (j == x) continue;

                Element& dstElement = PixelWorld::GetElement(j, i);

                if (dstElement.isIgnite && !dstElement.onFire)
                {
                    dstElement.Temperature += element.Temperature * (randf(1.0f) + 0.5f) * (float)Time::DeltaTime();

                    if(dstElement.Temperature >= dstElement.MaxTemperature)
                    {
                        dstElement.Temperature = dstElement.MaxTemperature;
                        dstElement.onFire = true;
                    }
                }
            }
        }
    }

    void PixelChunk::DecreaseLifeTime(int x, int y, Element& element)
    {
        element.LifeTime -= (float)Time::DeltaTime();

        if (element.LifeTime <= 0.0f)
        {
            PixelWorld::InsertElement(x, y, EMPTY);
        }
    }

}