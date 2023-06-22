#include "zzPlayScene.h"
#include "zzTransform.h"
#include "zzMeshRenderer.h"
#include "zzResourceManager.h"
#include "zzMesh.h"
#include "zzMaterial.h"

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
        MeshRenderer* mr = player->AddComponent<MeshRenderer>();
        mr->SetMesh(ResourceManager::Find<Mesh>(L"RectMesh"));
        mr->SetMaterial(ResourceManager::Find<Material>(L"SpriteMaterial"));

        Transform* tr = player->GetComponent<Transform>();
        tr->SetPosition(Vector3(-0.5f, 0.0f, 0.0f));
        tr->SetScale(Vector3(1.0f, 2.0f, 1.0f));

        //player->AddComponent<CameraScript>();

       /* GameObject* player2 = new GameObject();
        AddGameObject(eLayerType::Player, player2);
        player2->AddComponent<MeshRenderer>();

        Transform* tr = player->GetComponent<Transform>();      
        tr->SetPosition(Vector3(0.5f, 0.5f, 0.0f));*/

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