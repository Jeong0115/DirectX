#pragma once

#include "zzScene.h"

namespace zz
{
    class OpeningScene : public Scene
    {
    public:
        OpeningScene();
        virtual ~OpeningScene();

        virtual void Initialize()   override;
        virtual void Update()       override;
        virtual void LateUpdate()   override;
        virtual void Render()       override;

        void EndLoading();
        bool Run();

    private:
        void playNextAnimation();

        class Transform* trans;
        class MeshRenderer* mesh;
        class Animator* animator;

        GameObject* camera;

        float time;
        float fadeTime;

        float sign;
        float duration;

        bool mbEnd;
    };
}
