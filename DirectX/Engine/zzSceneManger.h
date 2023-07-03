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
        void Release();

        void CreateScene(std::wstring name, Scene* scene);
    private:
        Scene* mActiveScene;
        std::map<std::wstring, Scene*> mScenes;
    };
}
