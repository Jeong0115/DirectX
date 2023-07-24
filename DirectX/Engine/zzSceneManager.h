#pragma once

#include "zzScene.h"

namespace zz
{
    class SceneManager
    {
    private:
        SceneManager();
        ~SceneManager();

    public:
        SINGLETON(SceneManager);

        void Initialize();
        void Update();
        void LateUpdate();
        void Render();
        void Release();

        void CreateScene(std::wstring name, Scene* scene);

        Scene* GetActiveScene() { return mActiveScene; }
    private:
        Scene* mActiveScene;
        std::map<std::wstring, Scene*> mScenes;
    };
}
