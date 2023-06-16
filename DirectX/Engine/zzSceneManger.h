#pragma once

#include "zzScene.h"

namespace zz
{
    class SceneManger
    {
    private:
        SceneManger();
        ~SceneManger();

    public:
        SINGLETON(SceneManger);

        void Initialize();
        void Update();
        void LateUpdate();
        void Render();

    private:
        Scene* mActiveScene;
    };
}
