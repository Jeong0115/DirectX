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
        static void Initialize();
        static void Update();
        static void LateUpdate();
        static void Render();
        static void Release();

        static void LoadScene(const std::wstring& name, Scene* scene);

        static void CreateScene(std::wstring name, Scene* scene);

        static Scene* GetActiveScene() { return mActiveScene; }
    private:
        static Scene* mActiveScene;
        static std::map<std::wstring, Scene*> mScenes;
    };
}
