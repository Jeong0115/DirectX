#include "zzAnimation.h"
#include "zzTime.h"
#include "zzAnimator.h"
#include "zzRenderer.h"
#include "zzConstantBuffer.h"

namespace zz
{
    Animation::Animation()
        : Resource(eResourceType::Animation)
        , mAtlas(nullptr)
        , mAnimator(nullptr)
        , mSprites{}
        , mIndex(-1)
        , mTime(0.0f)
        , mbComplete(false) 
    {
    }
    Animation::~Animation()
    {
    }
    void Animation::Update()
    {
    }
    void Animation::LateUpdate()
    {
        if (mbComplete)
            return;

        mTime += (float)Time::DeltaTime();

        if (mSprites[mIndex].duration <= mTime)
        {
            mIndex++;
            mTime = 0.0f;

            if (mSprites.size() <= mIndex)
            {
                mIndex = (int)(mSprites.size() - 1);
                mbComplete = true;
            }
        }
    }
    void Animation::Render()
    {
    }

    void Animation::Create(std::wstring name, std::shared_ptr<Texture> atlas, Vector2 leftTop, Vector2 size, UINT columnLength, Vector2 offset, float duration)
    {
        SetKey(name);
        mAtlas = atlas;

        float width = (float)atlas->GetImageWidth();
        float height = (float)atlas->GetImageHeight();

        for (size_t i = 0; i < columnLength; i++)
        {
            Sprite sprite = {};
            sprite.leftTop.x = leftTop.x + (i * size.x);
            sprite.leftTop.y = leftTop.y;
            sprite.size = size;
            sprite.offset = offset;
            sprite.atlasSize = Vector2(width, height);
            sprite.duration = duration;

            mSprites.push_back(sprite);
        }
    }
    void Animation::Binds()
    {
        // texture bind
        mAtlas->BindShader(eShaderStage::PS, 12);

        // AnimationCB
        renderer::AnimatorCB data = {};

        data.spriteLeftTop = mSprites[mIndex].leftTop;
        data.spriteSize = mSprites[mIndex].size;
        data.spriteOffset = mSprites[mIndex].offset;
        data.atlasSize = mSprites[mIndex].atlasSize;
        data.animationType = 1;

        ConstantBuffer* cb = renderer::constantBuffer[(UINT)eCBType::Animator];
        cb->SetBufferData(&data);

        cb->BindConstantBuffer(eShaderStage::VS);

        cb->BindConstantBuffer(eShaderStage::PS);
    }
    void Animation::Reset()
    {
        mTime = 0.0f;
        mbComplete = false;
        mIndex = 0;
    }
}