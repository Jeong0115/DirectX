#include "zzSceneManager.h"
#include "zzPlayScene.h"

namespace zz
{
    Scene* SceneManager::mActiveScene = {};
    std::map<std::wstring, Scene*> SceneManager::mScenes = {};

    SceneManager::SceneManager()
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

    void SceneManager::LoadScene(const std::wstring& name, Scene* scene)
    {
        mActiveScene->Exit();

        mScenes.insert(std::make_pair(name, scene));
        
        Layer& cameraLayer = mActiveScene->GetLayer(eLayerType::Camera);

        for (GameObject* obj : cameraLayer.GetGameObjects())
        {
            scene->AddGameObject(obj, eLayerType::Camera);
        }
        cameraLayer = {};

        Layer& playerLayer = mActiveScene->GetLayer(eLayerType::Player);
        for (GameObject* obj : playerLayer.GetGameObjects())
        {
            scene->AddGameObject(obj, eLayerType::Player);
        }
        playerLayer = {};


        mActiveScene = scene;
    }

    void SceneManager::CreateScene(std::wstring name, Scene* scene)
    {
        mScenes.insert(std::make_pair(name, scene));
        mActiveScene = scene;
        scene->Initialize();
    }
}