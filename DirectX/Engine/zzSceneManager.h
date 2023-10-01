#pragma once

#include "zzScene.h"

#ifdef _DEBUG
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#else
#define DBG_NEW new
#endif

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

        static void CreateScene(std::wstring name, Scene* scene);

        static Scene* GetActiveScene() { return mActiveScene; }
    private:
        static Scene* mActiveScene;
        static std::map<std::wstring, Scene*> mScenes;
    };
}
