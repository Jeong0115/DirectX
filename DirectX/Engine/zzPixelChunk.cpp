#include "zzPixelChunk.h"
#include "zzPixelWorld.h"
#include "zzEmptyElement.h"
#include "zzRenderer.h"
#define _DEST(x) std::get<2>(x)

namespace zz
{
    std::mutex PixelChunk::tempMutex;
    PixelChunk::PixelChunk(size_t width, size_t height, int x, int y)
        : mWidth((int)width)
        , mHeight((int)height)
        , mStartX(x )
        , mStartY(y )
        , mMinX((int)width)
        , mMinY((int)height)
        , mMaxX(-1)
        , mMaxY(-1)
        , m_minXw((int)width), m_minYw((int)height)
        , m_maxXw(-1), m_maxYw(-1)
        , mElementCount(0)
    {
        mElements = new Element * [height * width];

        for (int i = 0; i < height * width; i++)
        {
            mElements[i] = new EmptyElement();
            mElements[i]->SetPos(Position(mStartX + i % mWidth, mStartY + i / mWidth));
        }
    }

    PixelChunk::~PixelChunk()
    {
        for (int i = 0; i < mHeight * mWidth; i++)
            delete mElements[i];

        delete[] mElements;
    }

    std::mutex asd;
    void PixelChunk::MoveElements()
    {
        //if (mElementCount == 0) return;
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

        int c = mChanges.size();
        std::sort(mChanges.begin(), mChanges.end(), [](auto& a, auto& b) { return _DEST(a) < _DEST(b); });

        size_t iprev = 0;

        mChanges.emplace_back(nullptr, -1, -1); // to catch final move

        for (size_t i = 0; i < mChanges.size() - 1; i++) 
        {
            if (_DEST(mChanges[i + 1]) != _DEST(mChanges[i])) 
            {
                if (std::get<0>(mChanges[i]) == nullptr)
                {
                    
                    return;
                }
                size_t rand = iprev + randi((int)(i - iprev));

                auto [chunk, src, dst] = mChanges[rand];

                if (chunk == nullptr)
                    int a = 0;

                
                Element* dstEle = GetElement(dst);
                SwapElement(dst, chunk->GetElement(src));
                chunk->SwapElement(src, dstEle);

                //InsertElement(dst, chunk->GetElement(src));
                //chunk->InsertElement(src, new EmptyElement());

                iprev = i + 1;
            }
        }

        mChanges.clear();
       

    }

    Element* PixelChunk::GetElement(size_t index)
    {
        return mElements[index];

    }

    void PixelChunk::SwapElement(size_t index, Element* cell)
    {
        Element* dest = mElements[index];

        if (dest->GetType() == eElementType::Empty && cell->GetType() != eElementType::Empty) 
        {
            mElementCount++;
        }

        else if (dest->GetType() != eElementType::Empty && cell->GetType() == eElementType::Empty)
        {
            mElementCount--;
        }

        mElements[index] = cell;
        cell->SetPos(Position((int)(mStartX + index % mWidth), (int)(mStartY + index / mWidth)));

        memcpy(&PixelWorld::GetPixelColor()[(mStartX + index % mWidth + ((mStartY + index / mWidth) * 2048)) * 4], &cell->mColor, 4);

        KeepAlive(index);
    }



    void PixelChunk::InsertElement(size_t index, Element* element)  // 수정해라
    {
        Element* dest = mElements[index];

        if (dest->GetType() == eElementType::Empty && element->GetType() != eElementType::Empty)
        {
            mElementCount++;
        }

        else if (dest->GetType() != eElementType::Empty && element->GetType() == eElementType::Empty)
        {
            mElementCount--;
        }

        delete mElements[index];
        mElements[index] = element;

        element->SetPos(Position((int)(mStartX + index % mWidth), (int)(mStartY + index / mWidth)));

        memcpy(&PixelWorld::GetPixelColor()[(mStartX + index % mWidth + ((mStartY + index / mWidth) * 2048)) * 4], &element->mColor, 4);

        KeepAlive(index);
    }

    void PixelChunk::ResiterChanges(PixelChunk* source, int x, int y, int toX, int toY)
    {     
        //std::lock_guard<std::mutex> lock(resiterMutex);

        mElementCount++;
        if (source == nullptr)
        {
            int a = 0;
        }
        std::tuple chage = { source, source->GetIndex(x, y), GetIndex(toX, toY) };
        mChanges.push_back(chage);
        //mChanges.emplace_back( source, source->GetIndex(x, y), GetIndex(toX, toY));
        
    }

    std::mutex mu;
    void PixelChunk::KeepAlive(size_t index)
    {
        int x = (int)(index % mWidth);
        int y = (int)(index / mWidth);

        //mu.lock();
        m_minXw = std::clamp(min(x - 2, m_minXw), 0, mWidth);
        m_minYw = std::clamp(min(y - 2, m_minYw), 0, mHeight);
        m_maxXw = std::clamp(max(x + 2, m_maxXw), 0, mWidth);
        m_maxYw = std::clamp(max(y + 2, m_maxYw), 0, mHeight);
        //mu.unlock();
    }

    void PixelChunk::UpdateRect()
    {      
        mMinX = m_minXw;  m_minXw = mWidth;
        mMinY = m_minYw;  m_minYw = mHeight;
        mMaxX = m_maxXw;  m_maxXw = -1;
        mMaxY = m_maxYw;  m_maxYw = -1;

        if (mMaxX <= -1) return;
        DebugMesh mesh = {};
        mesh.position = math::Vector3((mMaxX + mMinX) / 2.f + mStartX - 256.f, -((mMaxY + mMinY) / 2.f + mStartY - 256.f), 0.0f);
        //mesh.position = math::Vector3(0,0, 0.0f);
        mesh.scale = math::Vector3((float)(mMaxX - mMinX), (float)(mMaxY - mMinY), 1.0f);
        mesh.rotation = math::Vector3::Zero;

        renderer::PushDebugMeshAttribute(mesh);
    }
}