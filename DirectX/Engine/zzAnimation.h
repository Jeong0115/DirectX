#pragma once

#include "zzResource.h"
#include "zzTexture.h"

namespace zz
{
    using namespace math;

    class Animator;
    class Animation : public Resource
    {
    public:
        struct Sprite
        {
            Vector2 leftTop;
            Vector2 size;
            Vector2 offset;
            Vector2 atlasSize;
            float   duration;

            Sprite()
                : leftTop(Vector2::Zero)
                , size(Vector2::Zero)
                , offset(Vector2::Zero)
                , atlasSize(Vector2::Zero)
                , duration(0.0f)
            {}
        };

        Animation();
        ~Animation();

        virtual HRESULT Load(const std::wstring& path) { return S_FALSE; };

        void Update();
        void LateUpdate();
        void Render();

        void Create(std::wstring name, std::shared_ptr<Texture> atlas, Vector2 leftTop, Vector2 size
            , UINT columnLength, Vector2 offset = Vector2::Zero, float duration = 0.0f);

        void SetAnimationEvent(int index, std::function<void()> func);

        void Binds();
        void Reset();

        bool IsComplete() { return mbComplete; }

    private:
        std::unordered_map<int, std::function<void()>> mAnimationEvents;

        std::shared_ptr<Texture>    mAtlas;
        Animator*                   mAnimator;
        std::vector<Sprite>         mSprites;

        float                       mTime;
        int                         mIndex;
        bool                        mbComplete;
    };
}
