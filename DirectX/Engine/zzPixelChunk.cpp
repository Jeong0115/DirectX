#include "zzPixelChunk.h"
#include "zzPixelWorld.h"
#include "zzRenderer.h"
#include "zzTime.h"

namespace zz
{

    PixelChunk::PixelChunk(size_t width, size_t height, int x, int y, int worldWidth, int worldHeight)
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
        , mStaticCount{}
        , mBodies{}
        , mbChange(false)
        , mWorldWidth(worldWidth)
        , mWorldHeight(worldHeight)
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

        if (element.StopCount > 0)
        {
            element.StopCount--;
            KeepAlive(worldX, worldY);
        }


        switch (element.Id)
        {
        case eElementID::EMPTY: break;
        case eElementID::WOOD:  break;
        case eElementID::SMOKE: UpdateSmoke(worldX, worldY, element);   break;
        case eElementID::FIRE:  UpdateFire(worldX, worldY, element);    break;
        case eElementID::SAND:  UpdateSand(worldX, worldY, element);    break;
        case eElementID::WATER: UpdateWater(worldX, worldY, element);   break;
        case eElementID::OIL:   UpdateWater(worldX, worldY, element);   break;
        case eElementID::BLOOD: UpdateWater(worldX, worldY, element);   break;
        case eElementID::GRASS: UpdateGrass(worldX, worldY, element);   break;
        case eElementID::ROCK:  break;
        default: break;
        }        
    }

    void PixelChunk::SwapElement(Element& dstElement, int dstX, int dstY, Element& srcElement, int srcX, int srcY)
    {
        srcElement.StopCount = srcElement.StopThreshold;
        dstElement.StopCount = dstElement.StopThreshold;

        Element dest = dstElement;
        dstElement = srcElement;
        srcElement = dest;

        //PixelWorld::GetPixelColor((srcX + ((srcY) * 2048))) = srcElement.Color;
        //PixelWorld::GetPixelColor((dstX + ((dstY) * 2048))) = dstElement.Color;

        memcpy(&PixelWorld::GetPixelColor((srcX + ((srcY) *mWorldWidth))), &srcElement.Color, 4);
        memcpy(&PixelWorld::GetPixelColor((dstX + ((dstY) *mWorldWidth))), &dstElement.Color, 4);


        if (InBounds(dstX, dstY))
        {
            KeepAlive(dstX, dstY);
        }
        else
        {
            PixelWorld::GetChunk(dstX, dstY)->KeepAlive(dstX, dstY);
        }
    }

    void PixelChunk::InsertElement(int x, int y, const Element& element)
    {
        int index = GetIndex(x, y);
        Element& dest = mElements[index];

        if (dest.Type == eElementType::SOLID && element.Type != eElementType::SOLID)
        {
            mStaticSolidElements[y - mStartY][x - mStartX] = 0;
            mbChange = true;

            if (dest.SolidType == eSolidType::DYNAMIC)
            {
                bool* body = mElements[index].destoryBody;
                mElements[index] = element;
                mElements[index].destoryBody = body;
                *body = true;
                PixelWorld::GetPixelColor(mStartX + index % mWidth + ((mStartY + index / mWidth) * mWorldWidth)) = element.Color;
                KeepAlive(index);

                return;
            }
        }
        else if (dest.Type != eElementType::SOLID && element.Type == eElementType::SOLID)
        {
            mStaticSolidElements[y - mStartY][x - mStartX] = 1;
            mbChange = true;
        }

        mElements[index] = element;
        PixelWorld::GetPixelColor(mStartX + index % mWidth + ((mStartY + index / mWidth) * mWorldWidth)) = element.Color;
        KeepAlive(index);
    }

    void PixelChunk::InsertElementInOrOut(int x, int y, const Element& element)
    {
        if (InBounds(x, y))
        {
            InsertElement(x, y, element);
        }
        else
        {
            PixelWorld::InsertElement(x, y, element);
        }
    }

    void PixelChunk::InsertElementIfEmpty(int x, int y, const Element& element)
    {
        int index = GetIndex(x, y);
        Element& dest = mElements[index];

        if (dest.Type == eElementType::EMPTY)
        {
            mElements[index] = element;
            mElementCount++;

            PixelWorld::GetPixelColor(mStartX + index % mWidth + ((mStartY + index / mWidth) * mWorldWidth)) = element.Color;

            KeepAlive(index);
        }
    }

    void PixelChunk::RegisterElement(size_t index, const Element& element)
    {
        Element& dest = mElements[index];

        mStaticCount[index]++;

        if (dest.Type != eElementType::EMPTY)
        {
            mElementCount--;
        }

        mElements[index] = element;
        PixelWorld::GetPixelColor(mStartX + index % mWidth + ((mStartY + index / mWidth) * mWorldWidth)) = mElements[index].Color;
    }

    void PixelChunk::DelteElement(size_t index)
    {
        mElements[index] = EMPTY;
        mElementCount--;
        
        PixelWorld::GetPixelColor(mStartX + index % mWidth + ((mStartY + index / mWidth) * mWorldWidth)) = mElements[index].Color;
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
            PixelWorld::GetPixelColor(mStartX + index % mWidth + ((mStartY + index / mWidth) * mWorldWidth)) = mElements[index].Color;
            return true;
        }
    }

    void PixelChunk::KeepAlive(int x, int y)
    {
        std::unique_lock<std::mutex>(mDirtyBoxMutex);

        int relativeX = x - mStartX;
        int relativeY = y - mStartY;

        m_minXw = std::clamp(std::min(relativeX - 3, m_minXw), 0, mWidth);
        m_minYw = std::clamp(std::min(relativeY - 3, m_minYw), 0, mHeight);
        m_maxXw = std::clamp(std::max(relativeX + 3, m_maxXw), 0, mWidth);
        m_maxYw = std::clamp(std::max(relativeY + 3, m_maxYw), 0, mHeight);

        if (relativeX < 3 || relativeY < 3 || relativeX >= mWidth - 3 || relativeY >= mHeight - 3)
        {
            if (relativeX < 3)
                x -= 3;
            else if (relativeX >= mWidth - 3)
                x += 3;

            if (relativeY < 3)
                y -= 3;
            else if (relativeY >= mHeight - 3)
                y += 3;

            if (x >= 0 && y >= 0 && x < mWorldWidth && y < mWorldHeight)
                PixelWorld::GetChunk(x, y)->KeepAliveBoundary(x, y);
        }
    }

    void PixelChunk::KeepAliveBoundary(int x, int y)
    {
        std::unique_lock<std::mutex>(mDirtyBoxMutex);

        int relativeX = x - mStartX;
        int relativeY = y - mStartY;

        m_minXw = std::clamp(std::min(relativeX - 3, m_minXw), 0, mWidth);
        m_minYw = std::clamp(std::min(relativeY - 3, m_minYw), 0, mHeight);
        m_maxXw = std::clamp(std::max(relativeX + 3, m_maxXw), 0, mWidth);
        m_maxYw = std::clamp(std::max(relativeY + 3, m_maxYw), 0, mHeight);
    }

    void PixelChunk::UpdateRect()
    {
        mMinX = m_minXw;  m_minXw = mWidth;
        mMinY = m_minYw;  m_minYw = mHeight;
        mMaxX = m_maxXw;  m_maxXw = -1;
        mMaxY = m_maxYw;  m_maxYw = -1;

        if (mMaxX <= -1 || mMaxY <= -1) return;
        DebugMesh mesh = {};
        mesh.position = math::Vector3((mMaxX + mMinX) / 2.f + mStartX, -((mMaxY + mMinY) / 2.f + mStartY), 0.0f);
        mesh.scale = math::Vector3((float)(mMaxX - mMinX), (float)(mMaxY - mMinY), 1.0f);
        mesh.rotation = math::Vector3::Zero;

        renderer::PushDebugMeshAttribute(mesh);
    }

    Element& PixelChunk::GetElement(int x, int y)
    {
        return mElements[GetIndex(x, y)];
    }

    Element& PixelChunk::GetElementInOrOut(int x, int y)
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

    void PixelChunk::UpdateSand(int x, int y, Element& element)
    {
        int velX = randi(2) + 1;

        int lastY = y;

        for (int i = 1; i <= velX; i++)
        {
            Element& target = GetElementInOrOut(x, y + i);
            
            if (target.Type == eElementType::EMPTY || target.Type == eElementType::LIQUID || target.Type == eElementType::GAS)
            {
                lastY = y + i;
            }
            else
            {
                break;
            }
        }

        if (lastY != y)
        {
            SwapElement(GetElementInOrOut(x, lastY), x , lastY, element, x, y);
            return;
        }       
        else
        { 
            int rand = random() > 0.5f ? 1 : -1;

            Element& lTarget = GetElementInOrOut(x + rand, y + 1);
            if (lTarget.Type == eElementType::EMPTY || lTarget.Type == eElementType::LIQUID || lTarget.Type == eElementType::GAS)
            {
                SwapElement(lTarget, x + rand, y + 1, element, x, y);
            }
            else
            {
                Element& rTarget = GetElementInOrOut(x - rand, y + 1);
                if (rTarget.Type == eElementType::EMPTY || rTarget.Type == eElementType::LIQUID || rTarget.Type == eElementType::GAS)
                {
                    SwapElement(rTarget, x - rand, y + 1, element, x, y);
                }
            }
        }
    }

    void PixelChunk::UpdateWater(int x, int y, Element& element)
    {
        Element& target = GetElementInOrOut(x, y + 1);

        if (target.Type == eElementType::EMPTY || target.Type == eElementType::GAS
            || (target.Type == eElementType::LIQUID && target.Density < element.Density))
        {
            element.Velocity.x *= 0.8f;
            element.Velocity.y += 0.1f;
            
            int floorVelX = (int)fabs(floor(element.Velocity.x));
            int floorVelY = (int)floor(element.Velocity.y);

            int dirX = element.Velocity.x >= 0 ? 1 : -1;

            int largeVel;
            int smallVel;

            bool isVelXLarger;

            floorVelX > floorVelY ? (largeVel = floorVelX, smallVel = floorVelY, isVelXLarger = true)
                : (largeVel = floorVelY, smallVel = floorVelX, isVelXLarger = false);

            float slope = (smallVel == 0 || largeVel == 0) ? 0 : ((float)(smallVel + 1) / (largeVel + 1));

            int lastX = x;
            int lastY = y + 1;

            for (int i = 1; i < largeVel; i++)
            {
                int smallerCount = (int)floor(i * slope);

                int yIncrease, xIncrease;
                if (isVelXLarger)
                {
                    xIncrease = i;
                    yIncrease = smallerCount;
                }
                else
                {
                    yIncrease = i;
                    xIncrease = smallerCount;
                }

                Element& nextTarget = GetElementInOrOut(x + (xIncrease * dirX), y + yIncrease);

                if (nextTarget.Type == eElementType::EMPTY || nextTarget.Type == eElementType::GAS
                    || (nextTarget.Type == eElementType::LIQUID && nextTarget.Density < element.Density))
                {
                    lastX = x + (xIncrease * dirX);
                    lastY = y + yIncrease;
                }
                else if (nextTarget.Type == eElementType::LIQUID || nextTarget.Type == eElementType::SOLID)
                {
                    Element& sideElement = GetElementInOrOut(x + (xIncrease * dirX) + dirX, y + yIncrease);

                    if (sideElement.Type == eElementType::SOLID 
                        || (sideElement.Type == eElementType::LIQUID && sideElement.Density >= element.Density))
                    {
                        dirX *= -1;
                    }

                    element.Velocity.x = (element.Velocity.y + random() * 3.0f - 1.0f) * dirX;
                    break;
                }
            }

            if (lastX != x || lastY != y)
            {
                SwapElement(GetElementInOrOut(lastX, lastY), lastX, lastY, element, x, y);
                return;
            }
        }
        else
        {
            element.Velocity.y = 2.f;

            int dirX = element.Velocity.x >= 0 ? 1 : -1;

            if (element.Velocity.x <= 3.f)
            {
                element.Velocity.x = (3.0f + randi(3)) * dirX;
            }

            int lastX = x;
            int lastY = y;

            float absVelocityX = fabs(element.Velocity.x);
            for (int j = 1; j < (int)absVelocityX; j++)
            {
                Element& target = GetElementInOrOut(x + j * dirX, y);

                if (target.Type == eElementType::EMPTY || target.Type == eElementType::GAS
                    || (target.Type == eElementType::LIQUID && target.Density < element.Density))
                {
                    lastX = x + j * dirX;
                }
                else //if (target.Type == eElementType::SOLID || target.Type == eElementType::LIQUID)
                {
                    break;
                }
            }

            if (lastX != x)
            {
                SwapElement(GetElementInOrOut(lastX, lastY), lastX, lastY, element, x, y);
                return;
            }
            else
            {
                element.Velocity.x *= -1;
            }
        }
    }

    void PixelChunk::UpdateFire(int x, int y, Element& element)
    {
        Element& target = GetElementInOrOut(x, y - 1);

        if (target.Type == eElementType::EMPTY)
        {
            SwapElement(target, x, y - 1, element, x, y);
        }
        else
        {
            int rand = random() > 0.5f ? 1 : -1;

            Element& lTarget = GetElementInOrOut(x + rand, y - 1);
            if (lTarget.Type == eElementType::EMPTY)
            {
                SwapElement(lTarget, x + rand, y - 1, element, x, y);
            }
            else
            {
                Element& rTarget = GetElementInOrOut(x - rand, y - 1);
                if (rTarget.Type == eElementType::EMPTY)
                {
                    SwapElement(rTarget, x - rand, y - 1, element, x, y);
                }
            }
        }
    }

    void PixelChunk::UpdateSmoke(int x, int y, Element& element)
    {
        int velX = randi(3);

        int lastY = y;

        for (int i = 1; i <= velX; i++)
        {
            Element& target = GetElementInOrOut(x, y - i);

            if (target.Type == eElementType::EMPTY)
            {
                lastY = y - i;
            }
            else
            {
                break;
            }
        }

        if (lastY != y)
        {
            SwapElement(GetElementInOrOut(x, lastY), x, lastY, element, x, y);
            return;
        }
        else
        {
            int dirX = element.Velocity.x >= 0 ? 1 : -1;

            int lastX = x;
            int lastY = y;

            float absVelocityX = fabs(element.Velocity.x);
            for (int j = 1; j < (int)absVelocityX; j++)
            {
                Element& target = GetElementInOrOut(x + j * dirX, y);

                if (target.Type == eElementType::EMPTY)
                {
                    lastX = x + j * dirX;
                }
                else
                {
                    break;
                }
            }

            if (lastX != x)
            {
                SwapElement(GetElementInOrOut(lastX, lastY), lastX, lastY, element, x, y);
                return;
            }
            else
            {
                element.Velocity.x *= -1;
            }
        }
    }

    void PixelChunk::UpdateGrass(int x, int y, Element& element)
    {
        if (element.StopCount > 0)
        {
            InsertElement(x, y, EMPTY);
        }
    }

    void PixelChunk::HeatNeighbors(int x, int y, Element& element)
    {
        element.FireHP -= (float)Time::DeltaTime();

        if (element.FireHP <= 0.f)
        {
            InsertElement(x, y, EMPTY);
            return;
        }
        else
        {
            if (random() > 0.95f)
            {
                if (GetElementInOrOut(x, y - 1).Type == eElementType::EMPTY)
                {
                    Element smoke = SMOKE;
                    //smoke.Color = element.Color;

                    InsertElementInOrOut(x, y - 1, smoke);
                }
            }   

/*            if (element.Temperature < element.FlashPoint)
            {
                element.onFire = false;
                element.Color = getElementColor(element.Id);
                PixelWorld::GetPixelColor((x + ((y) * 1536))) = element.Color;
                return;
            }
            else */if(element.Id != eElementID::FIRE)
            //{
                element.Color = RandomFireColor();
            //}
        }

        PixelWorld::GetPixelColor((x + ((y) *mWorldWidth))) = element.Color;

        for (int i = y - 1; i <= y + 1; i++)
        {
            if (i == y) continue;
            for (int j = x - 1; j <= x + 1; j++)
            {
                if (j == x) continue;

                Element& dstElement = GetElementInOrOut(j, i);

                //if (dstElement.Id == eElementID::WATER)
                //{
                //    element.Temperature -= 500.f * (float)Time::DeltaTime();
                //}
                //else
                /*else
                {*/
                    if (dstElement.isIgnite)
                    {
                        if (dstElement.Temperature < dstElement.IgnitionPoint)
                        {
                            dstElement.Temperature += element.Temperature * (randf(1.0f) + 0.5f) * (float)Time::DeltaTime();
                        }
                    }
                    if (dstElement.Temperature >= dstElement.FlashPoint)
                    {
                        dstElement.onFire = true;
                    }
                //}


            }
        }
    }

    void PixelChunk::DecreaseLifeTime(int x, int y, Element& element)
    {
        element.LifeTime -= (float)Time::DeltaTime() * (random() > 0.5f ? 1.3f : 0.7f);

        if (element.LifeTime <= 0.0f)
        {
            InsertElementInOrOut(x, y, EMPTY);
        }
    }

    void PixelChunk::RenewalBody()
    {
        if (mbChange)
        {
            Box2dWorld::RenewalChunkBody(mStaticSolidElements, mStartX, mStartY, mBodies);
            mbChange = false;
        }
    }

}