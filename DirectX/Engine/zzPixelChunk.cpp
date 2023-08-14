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

    void PixelChunk::MoveElements()
    {
        if (mElementCount == 0) return;
        /*for (size_t i = 0; i < mChanges.size(); i++) 
        {
            if (!IsEmpty(_DEST(mChanges[i]))) 
            {
                mChanges[i] = mChanges.back(); mChanges.pop_back();
                i--;
            }
        }*/
        //std::lock_guard<std::mutex> lock(tempMutex);
        
        mElementCount -= mChanges.size();
        if (mChanges.size() == 0) 
        {
            
            return;
        }
        std::sort(mChanges.begin(), mChanges.end(), [](auto& a, auto& b) { return _DEST(a) < _DEST(b); });

        size_t iprev = 0;

        mChanges.emplace_back(nullptr, -1, -1); // to catch final move

        for (size_t i = 0; i < mChanges.size() - 1; i++) 
        {
            if (_DEST(mChanges[i + 1]) != _DEST(mChanges[i])) 
            {
                size_t rand = iprev + randi((int)(i - iprev));

                auto [chunk, src, dst] = mChanges[rand];

                
                Element dstEle = GetElement(dst);
                SwapElement(dst, chunk->GetElement(src));
                chunk->SwapElement(src, dstEle);

                //InsertElement(dst, chunk->GetElement(src));
                //chunk->InsertElement(src, new Empty   Element());

                iprev = i + 1;
            }
        }

        mChanges.clear();
    }

    Element& PixelChunk::GetElement(size_t index)
    {
        return mElements[index];

    }

    void PixelChunk::SwapElement(size_t index, const Element& element)
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

        //cell.x = mStartX + index % mWidth;
        //cell.y = mStartY + index / mWidth;

        memcpy(&PixelWorld::GetPixelColor()[(mStartX + index % mWidth + ((mStartY + index / mWidth) * 2048)) * 4], &element.Color, 4);

        KeepAlive(index);
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
        int x = (int)(index % mWidth);
        int y = (int)(index / mWidth);

        m_minXw = std::clamp(std::min(x - 2, m_minXw), 0, mWidth);
        m_minYw = std::clamp(std::min(y - 2, m_minYw), 0, mHeight);
        m_maxXw = std::clamp(std::max(x + 2, m_maxXw), 0, mWidth);
        m_maxYw = std::clamp(std::max(y + 2, m_maxYw), 0, mHeight);
    }
        
    void PixelChunk::UpdateRect()
    {      
        mMinX = m_minXw;  m_minXw = mWidth;
        mMinY = m_minYw;  m_minYw = mHeight;
        mMaxX = m_maxXw;  m_maxXw = -1;
        mMaxY = m_maxYw;  m_maxYw = -1;

        if (mMaxX <= -1) return;
        DebugMesh mesh = {};
        mesh.position = math::Vector3((mMaxX + mMinX) / 2.f + mStartX, -((mMaxY + mMinY) / 2.f + mStartY ), 0.0f);
        //mesh.position = math::Vector3(0,0, 0.0f);
        mesh.scale = math::Vector3((float)(mMaxX - mMinX), (float)(mMaxY - mMinY), 1.0f);
        mesh.rotation = math::Vector3::Zero;

        renderer::PushDebugMeshAttribute(mesh);
    }
}