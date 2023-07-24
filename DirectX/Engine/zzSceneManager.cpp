#include "zzSceneManager.h"
#include "zzPlayScene.h"

namespace zz
{
    SceneManager::SceneManager()
        : mActiveScene(nullptr)
    {
    }

    SceneManager::~SceneManager()
    {
    }

    void SceneManager::Initialize()
    {
        CreateScene(L"PlayScene", new PlayScene());
    }
    void SceneManager::Update()
    {
        mActiveScene->Update();
    }
    void SceneManager::LateUpdate()
    {
        mActiveScene->LateUpdate();
    }
    void SceneManager::Render()
    {
        mActiveScene->Render();
    }

    void SceneManager::Release()
    {
        for (auto iter : mScenes)
        {
            delete iter.second;
            iter.second = nullptr;
        }
    }

    void SceneManager::CreateScene(std::wstring name, Scene* scene)
    {
        mScenes.insert(std::make_pair(name, scene));
        mActiveScene = scene;
        scene->Initialize();
    }
}