#include "zzSceneManger.h"
#include "zzPlayScene.h"

namespace zz
{
    SceneManger::SceneManger()
        : mActiveScene(nullptr)
    {
    }

    SceneManger::~SceneManger()
    {
    }

    void SceneManger::Initialize()
    {
        mActiveScene = new PlayScene();
        mScenes.insert(std::make_pair(L"PlayScene", mActiveScene));
        mActiveScene->Initialize();
    }
    void SceneManger::Update()
    {
        mActiveScene->Update();
    }
    void SceneManger::LateUpdate()
    {
        mActiveScene->LateUpdate();
    }
    void SceneManger::Render()
    {
        mActiveScene->Render();
    }

    void SceneManger::Release()
    {
        for (auto iter : mScenes)
        {
            delete iter.second;
            iter.second = nullptr;
        }
    }
}