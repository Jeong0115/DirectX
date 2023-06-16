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
}