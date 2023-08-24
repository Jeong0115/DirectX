#include "zzPlayScene.h"
#include "zzTransform.h"
#include "zzMeshRenderer.h"
#include "zzResourceManager.h"
#include "zzMesh.h"
#include "zzMaterial.h"
#include "zzInput.h"
#include "zzCamera.h"
#include "zzMainCamera.h"
#include "zzUICamera.h"
#include "zzCameraScript.h"
#include "zzBGScript.h"
#include "zzAnimator.h"
#include "zzWandScript.h"
#include "zzRenderer.h"
#include "zzCursorScript.h"
#include "zzInventoryBG.h"
#include "zzInventoryBox.h"
#include "zzPlayer.h"
#include "zzPlayerArm.h"
#include "zzInventoryManager.h"
#include "zzParticleSystem.h"
#include "zzShotGunner_Weak.h"

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
            AddGameObject(bg, eLayerType::BG);

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
        AddGameObject(camera, eLayerType::Camera);
        camera->GetComponent<Transform>()->SetPosition(Vector3(0.0f, 0.0f, -10.0f));
        MainCamera* cameraComp = camera->AddComponent<MainCamera>();
        camera->AddComponent<CameraScript>();
        cameraComp->SetSize(0.25f);
        cameraComp->TurnLayerMask(eLayerType::UI, false);
        renderer::cameras.push_back(cameraComp);
        renderer::mainCamera = cameraComp;

        {
            GameObject* uiCamera = new GameObject();
        	AddGameObject(uiCamera, eLayerType::Camera);
            uiCamera->GetComponent<Transform>()->SetPosition(Vector3(400.f, 225.f, -1.0f));
        	UICamera* uiCameraComp = uiCamera->AddComponent<UICamera>();
            uiCameraComp->SetSize(0.5f);
            renderer::cameras.push_back(uiCameraComp);
            renderer::uiCamera = uiCameraComp;
        	//camera->AddComponent<CameraScript>();
        }

       //// z값을 다르게 하거나, z값을 같게 한다음 깊이 버퍼를 사용 안할지 생각
       //MakeBG(L"M_MountainBG0", Vector3(1066, 512, 1.0f), Vector3(0.f, -20.f, 1.0f)  , 0.f   , 0.f);          // 파란 배경
       //MakeBG(L"M_MountainBG1", Vector3(1066, 512, 1.0f), Vector3(0.f, 0.f, 0.9f)    , 8.f   , 1.f);         // 0번 구름
       //MakeBG(L"M_MountainBG2", Vector3(800, 512, 1.0f) , Vector3(0.f, -100.f, 0.8f) , 0.f   , 0.6f);          // 0번 산
       //MakeBG(L"M_MountainBG3", Vector3(1130, 512, 1.0f), Vector3(0.f, 0.f, 0.7f)    , 15.f  , 1.f);          // 1번 구름
       //MakeBG(L"M_MountainBG4", Vector3(1024, 512, 1.0f), Vector3(0.f, -120.f, 0.6f) , 0.f   , 0.2f);          // 1번 산
       //MakeBG(L"M_MountainBG5", Vector3(1024, 512, 1.0f), Vector3(0.f, -120.f, 0.5f) , 0.f   , 0.2f);          // 1번 산
        
        float x = 256;
        float y = -256;
        {
            //GameObject* object = new GameObject();
            //AddGameObject(object, eLayerType::Player);
            //object->GetComponent<Transform>()->SetPosition(Vector3(x, y, 0.2f));
            //object->GetComponent<Transform>()->SetScale(Vector3(512.f, 512.f, 1.0f));
            //MeshRenderer* mesh = object->AddComponent<MeshRenderer>();
            //mesh->SetMaterial(ResourceManager::Find<Material>(L"m_hall_background_0_0"));
            //mesh->SetMesh(ResourceManager::Find<Mesh>(L"RectMesh"));

            //GameObject* object2 = new GameObject();
            //AddGameObject(object2, eLayerType::Player);
            //object2->GetComponent<Transform>()->SetPosition(Vector3(x - 512, y, 0.2f));
            //object2->GetComponent<Transform>()->SetScale(Vector3(512.f, 512.f, 1.0f));
            //MeshRenderer* mesh2 = object2->AddComponent<MeshRenderer>();
            //mesh2->SetMaterial(ResourceManager::Find<Material>(L"m_left_entrance_background_-512_0"));
            //mesh2->SetMesh(ResourceManager::Find<Mesh>(L"RectMesh"));
        }

        GameObject* object = new GameObject();
        AddGameObject(object, eLayerType::Player);
        object->GetComponent<Transform>()->SetPosition(Vector3(x +1024 - 256, y -1024 + 256, 0.05f));
        object->GetComponent<Transform>()->SetScale(Vector3(2048.f, 2048.f, 1.0f));
        MeshRenderer* mesh = object->AddComponent<MeshRenderer>();
        mesh->SetMaterial(ResourceManager::Find<Material>(L"m_PixelTexture"));
        mesh->SetMesh(ResourceManager::Find<Mesh>(L"RectMesh"));

        for (int y = 0; y < 8; y++)
        {
            for (int x = 0; x < 4; x++)
            {
                GameObject* bg = new GameObject();
                AddGameObject(bg, eLayerType::BG);
                bg->GetComponent<Transform>()->SetPosition(Vector3(x + 256 - 256 + 512 * x, y - 128 + 256 - 256 * y, 1.0f));
                bg->GetComponent<Transform>()->SetScale(Vector3(512.f, 256.f, 1.0f));
                MeshRenderer* bgmesh = bg->AddComponent<MeshRenderer>();
                bgmesh->SetMaterial(ResourceManager::Find<Material>(L"m_background_coalmine"));
                bgmesh->SetMesh(ResourceManager::Find<Mesh>(L"RectMesh"));
            }
        }


        {
            //GameObject* object = new GameObject();
            ////AddGameObject(eLayerType::Player, object);
            //object->GetComponent<Transform>()->SetPosition(Vector3(x, y, 0.0f));
            //MeshRenderer* mesh = object->AddComponent<MeshRenderer>();
            //mesh->SetMaterial(ResourceManager::Find<Material>(L"m_hall_visual_0_0"));
            //mesh->SetMesh(ResourceManager::Find<Mesh>(L"RectMesh"));
        }
        //int* a = new int;
        //{
        //    GameObject* object = new GameObject();
        //    AddGameObject(eLayerType::Player, object);
        //    object->GetComponent<Transform>()->SetPosition(Vector3(x, y - 306 , 0.0f));
        //    MeshRenderer* mesh = object->AddComponent<MeshRenderer>();
        //    mesh->SetMaterial(ResourceManager::Find<Material>(L"m_hall_b_visual_0_512"));
        //    mesh->SetMesh(ResourceManager::Find<Mesh>(L"RectMesh"));
        //}
        {
            GameObject* object = new GameObject();
            AddGameObject(object, eLayerType::Player);
            object->GetComponent<Transform>()->SetPosition(Vector3(x + 300, y - 331, 0.0f));
            object->GetComponent<Transform>()->SetScale(Vector3(88.f, 150.f, 1.0f));
            MeshRenderer* mesh = object->AddComponent<MeshRenderer>();
            mesh->SetMaterial(ResourceManager::Find<Material>(L"m_hall_br_visual_512_512"));
            mesh->SetMesh(ResourceManager::Find<Mesh>(L"RectMesh"));
        }
        
        {
            GameObject* object = new GameObject();
            AddGameObject(object, eLayerType::Player);
            object->GetComponent<Transform>()->SetPosition(Vector3(x + 300, y, 0.01f));
            object->GetComponent<Transform>()->SetScale(Vector3(88.f, 512.f, 1.0f));
            MeshRenderer* mesh = object->AddComponent<MeshRenderer>();
            mesh->SetMaterial(ResourceManager::Find<Material>(L"m_hall_r_visual_512_0"));
            mesh->SetMesh(ResourceManager::Find<Mesh>(L"RectMesh"));
        }

        {
            ShotGunner_Weak* object = new ShotGunner_Weak();
            AddGameObject(object, eLayerType::Monster);
            object->GetComponent<Transform>()->SetPosition(Vector3(0, 0, 0.2f));
        }

        //Explosion_128* bomb = new Explosion_128();
        //AddGameObject(bomb, eLayerType::Effect);
        //bomb->GetComponent<Transform>()->SetPosition(Vector3(0.f,0.f, 0.1f));

#pragma region UI




#pragma endregion
        // Transform * tr = object->GetComponent<Transform>();
        //tr->SetScale(Vector3(2048.f, 2048.f, 1.0f));
        //{
        //    GameObject* object = new GameObject();
        //    AddGameObject(eLayerType::Player, object);
        //    object->GetComponent<Transform>()->SetPosition(Vector3(x , y , 0.0f));
        //    MeshRenderer* mesh = object->AddComponent<MeshRenderer>();
        //    mesh->SetMaterial(ResourceManager::Find<Material>(L"m_rust"));
        //    mesh->SetMesh(ResourceManager::Find<Mesh>(L"RectMesh"));
        //}
        Scene::Initialize();
        {
            Player* player;
            player = new Player();
            AddGameObject(player, eLayerType::Player);
            player->GetComponent<Transform>()->SetPosition(Vector3(10.f, -300.f, 0.000f));
            player->GetComponent<Transform>()->SetScale(Vector3(12.f, 19.f, 1.0f));    
            //player->SetCamera(camera);
            //player->AddComponent<ParticleSystem>();
           
            
            //GameObject* aa = new GameObject();;
            //AddGameObject(aa, eLayerType::Player);
            //aa->GetComponent<Transform>()->SetPosition(Vector3(0.f, 0.f, 0.000f));
            //aa->GetComponent<Transform>()->SetScale(Vector3(12.f, 12.f, 1.0f));
            //aa->AddComponent<ParticleSystem>();

            PlayerArm* player_arm = new PlayerArm();

            player_arm->SetPlayer(player);
            player->SetArm(player_arm);

            player->Initialize();

            InventoryManager::SetPlayer(player);
            InventoryManager::CreateStartItems();
            InventoryManager::Test();

        }
       
        
    }

    void PlayScene::Update()
    {
        
        Scene::Update();
    }

    void PlayScene::LateUpdate()
    {
        Vector3 pos(800, 450, 0.0f);
        Vector3 pos2(800, 450, 1000.0f);
        Viewport viewport;
        viewport.width = 1600.0f;
        viewport.height = 900.0f;
        viewport.x = 0;
        viewport.y = 0;
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;

        pos = viewport.Unproject(pos, Camera::GetGpuProjectionMatrix(), Camera::GetGpuViewMatrix(), Matrix::Identity);
        pos2 = viewport.Unproject(pos2, Camera::GetGpuProjectionMatrix(), Camera::GetGpuViewMatrix(), Matrix::Identity);

        Scene::LateUpdate();
    }

    void PlayScene::Render()
    {
        Scene::Render();
    }
}