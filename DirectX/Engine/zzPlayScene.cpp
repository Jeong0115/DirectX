#include "zzPlayScene.h"
#include "zzTransform.h"
#include "zzMeshRenderer.h"

namespace zz
{
    PlayScene::PlayScene()
    {
    }

    PlayScene::~PlayScene()
    {
    }

    void PlayScene::Initialize()
    {
        GameObject* player = new GameObject();
        AddGameObject(eLayerType::Player, player);
        player->AddComponent<MeshRenderer>();

        GameObject* player2 = new GameObject();
        AddGameObject(eLayerType::Player, player2);
        player2->AddComponent<MeshRenderer>();

        Transform* tr = player->GetComponent<Transform>();      
        tr->SetPosition(Vector3(0.5f, 0.5f, 0.0f));

        Scene::Initialize();
    }

    void PlayScene::Update()
    {
        Scene::Update();
    }

    void PlayScene::LateUpdate()
    {
        Scene::LateUpdate();
    }

    void PlayScene::Render()
    {
        Scene::Render();
    }
}