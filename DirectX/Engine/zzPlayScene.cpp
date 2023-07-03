#include "zzPlayScene.h"
#include "zzTransform.h"
#include "zzMeshRenderer.h"
#include "zzResourceManager.h"
#include "zzMesh.h"
#include "zzMaterial.h"
#include "zzInput.h"
#include "zzCamera.h"
#include "zzCameraScript.h"
#include "zzBGScript.h"

namespace zz
{
    GameObject* camera;

    PlayScene::PlayScene()
    {
    }

    PlayScene::~PlayScene()
    {
    }

    void PlayScene::MakeBG(std::wstring material, Vector3 scale, Vector3 pos, float moveSpeed, float parallaxScale)
    {
        for (int i = 0; i < 2; i++)
        {
            GameObject* bg = new GameObject();
            AddGameObject(eLayerType::BG, bg);

            MeshRenderer* meshRenderer = bg->AddComponent<MeshRenderer>();
            meshRenderer->SetMesh(ResourceManager::Find<Mesh>(L"RectMesh"));
            meshRenderer->SetMaterial(ResourceManager::Find<Material>(material));

            Transform* tr = bg->GetComponent<Transform>();
            tr->SetScale(scale);
            tr->SetPosition(Vector3(pos.x + scale.x * i, pos.y, pos.z));

            BGScript* script = bg->AddComponent<BGScript>();
            script->SetCamera(camera);
            script->SetMoveSpeed(moveSpeed);
            script->SetParallaxScale(parallaxScale);
        }
    }   

    void PlayScene::Initialize()
    {
        camera = new GameObject();
        AddGameObject(eLayerType::Camera, camera);
        camera->GetComponent<Transform>()->SetPosition(Vector3(0.0f, 0.0f, -10.0f));
        Camera* cameraComp = camera->AddComponent<Camera>();
        camera->AddComponent<CameraScript>();

        // z값을 다르게 하거나, z값을 같게 한다음 깊이 버퍼를 사용 안할지 생각
        MakeBG(L"M_MountainBG0", Vector3(1066, 512, 1.0f), Vector3(0.f, -20.f, 1.0f)  , 0.f   , 0.f);          // 파란 배경
        MakeBG(L"M_MountainBG1", Vector3(1066, 512, 1.0f), Vector3(0.f, 0.f, 0.9f)    , 8.f   , 1.f);         // 0번 구름
        MakeBG(L"M_MountainBG2", Vector3(800, 512, 1.0f) , Vector3(0.f, -100.f, 0.8f) , 0.f   , 0.6f);          // 0번 산
        MakeBG(L"M_MountainBG3", Vector3(1130, 512, 1.0f), Vector3(0.f, 0.f, 0.7f)    , 15.f  , 1.f);          // 1번 구름
        MakeBG(L"M_MountainBG4", Vector3(1024, 512, 1.0f), Vector3(0.f, -120.f, 0.6f) , 0.f   , 0.2f);          // 1번 산
        MakeBG(L"M_MountainBG5", Vector3(1024, 512, 1.0f), Vector3(0.f, -120.f, 0.5f) , 0.f   , 0.2f);          // 1번 산
        
        float x = 0.f;
        float y = 0.f;
        {
            GameObject* object = new GameObject();
            AddGameObject(eLayerType::Player, object);
            object->GetComponent<Transform>()->SetPosition(Vector3(x, y, 0.2f));
            MeshRenderer* mesh = object->AddComponent<MeshRenderer>();
            mesh->SetMaterial(ResourceManager::Find<Material>(L"m_hall_background_0_0"));
            mesh->SetMesh(ResourceManager::Find<Mesh>(L"RectMesh"));
        }
        {
            GameObject* object = new GameObject();
            AddGameObject(eLayerType::Player, object);
            object->GetComponent<Transform>()->SetPosition(Vector3(x, y, 0.1f));
            MeshRenderer* mesh = object->AddComponent<MeshRenderer>();
            mesh->SetMaterial(ResourceManager::Find<Material>(L"m_hall_visual_0_0"));
            mesh->SetMesh(ResourceManager::Find<Mesh>(L"RectMesh"));
        }
        {
            GameObject* object = new GameObject();
            AddGameObject(eLayerType::Player, object);
            object->GetComponent<Transform>()->SetPosition(Vector3(x, y - 306 , 0.0f));
            MeshRenderer* mesh = object->AddComponent<MeshRenderer>();
            mesh->SetMaterial(ResourceManager::Find<Material>(L"m_hall_b_visual_0_512"));
            mesh->SetMesh(ResourceManager::Find<Mesh>(L"RectMesh"));
        }
        {
            GameObject* object = new GameObject();
            AddGameObject(eLayerType::Player, object);
            object->GetComponent<Transform>()->SetPosition(Vector3(x + 300, y - 331, 0.1f));
            MeshRenderer* mesh = object->AddComponent<MeshRenderer>();
            mesh->SetMaterial(ResourceManager::Find<Material>(L"m_hall_br_visual_512_512"));
            mesh->SetMesh(ResourceManager::Find<Mesh>(L"RectMesh"));
        }
        
        {
            GameObject* object = new GameObject();
            AddGameObject(eLayerType::Player, object);
            object->GetComponent<Transform>()->SetPosition(Vector3(x + 300, y, 0.1f));
            MeshRenderer* mesh = object->AddComponent<MeshRenderer>();
            mesh->SetMaterial(ResourceManager::Find<Material>(L"m_hall_r_visual_512_0"));
            mesh->SetMesh(ResourceManager::Find<Mesh>(L"RectMesh"));
        }
        

        Scene::Initialize();

        //{
        //    GameObject* object = new GameObject();
        //    AddGameObject(eLayerType::Player, object);
        //    object->GetComponent<Transform>()->SetPosition(Vector3(x , y , 0.0f));
        //    MeshRenderer* mesh = object->AddComponent<MeshRenderer>();
        //    mesh->SetMaterial(ResourceManager::Find<Material>(L"m_rust"));
        //    mesh->SetMesh(ResourceManager::Find<Mesh>(L"RectMesh"));
        //}
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