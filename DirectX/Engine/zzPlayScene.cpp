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
#include "zzRenderer.h"
#include "zzPlayer.h"
#include "zzPlayerArm.h"
#include "zzUIManager.h"
#include "zzShotGunner_Weak.h"
#include "zzCollisionManger.h"
#include "zzTeleport.h"

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
        CollisionManger::SetCollision(eLayerType::Monster, eLayerType::PlayerAttack, true);
        CollisionManger::SetCollision(eLayerType::Object, eLayerType::Player, true);

        camera = new GameObject();
        AddGameObject(camera, eLayerType::Camera);
        camera->GetComponent<Transform>()->SetPosition(Vector3(0.0f, 0.0f, -10.0f));
        MainCamera* cameraComp = camera->AddComponent<MainCamera>();
        camera->AddComponent<CameraScript>();
        cameraComp->SetSize(0.25f);

        renderer::cameras.push_back(cameraComp);
        renderer::mainCamera = cameraComp;

        {
            GameObject* uiCamera = new GameObject();
            AddGameObject(uiCamera, eLayerType::Camera);
            uiCamera->GetComponent<Transform>()->SetPosition(Vector3(320.f, 180.f, -100.0f));
            UICamera* uiCameraComp = uiCamera->AddComponent<UICamera>();
            uiCameraComp->SetSize(0.4f);
            renderer::cameras.push_back(uiCameraComp);
            renderer::uiCamera = uiCameraComp;
            //camera->AddComponent<CameraScript>();
        }


        float x = 256;
        float y = -256;


        GameObject* object = new GameObject();
        AddGameObject(object, eLayerType::PixelWorld);
        object->GetComponent<Transform>()->SetPosition(Vector3(x + 768 - 256, y - 1024 + 256, PIXEL_WORLD_Z));
        object->GetComponent<Transform>()->SetScale(Vector3(1536.f, 2048.f, 1.0f));
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

        for (int y = 0; y < 8; y++)
        {
            for (int x = 0; x < 2; x++)
            {
                GameObject* bg = new GameObject();
                AddGameObject(bg, eLayerType::PixelWorld);
                bg->GetComponent<Transform>()->SetPosition(Vector3(-48.f - (x * 96.f), -48.f - (y * 96.f), 1.0f));
                bg->GetComponent<Transform>()->SetScale(Vector3(96.f, 96.f, 1.0f));
                MeshRenderer* bgmesh = bg->AddComponent<MeshRenderer>();
                bgmesh->SetMaterial(ResourceManager::Find<Material>(L"m_rock_hard_alt"));
                bgmesh->SetMesh(ResourceManager::Find<Mesh>(L"RectMesh"));
            }
        }

        std::shared_ptr<Texture> newImage = ResourceManager::Find<Texture>(L"rock_hard_alt");
        std::shared_ptr<Texture> texture = std::make_shared<Texture>();

        ScratchImage& srcImage = newImage->GetScratchImage();
        ScratchImage& dstImage = texture->GetScratchImage();


        dstImage.Initialize2D(DXGI_FORMAT_B8G8R8A8_UNORM, 960, 192, 1, 1);


        for (int j = 0; j < 2; j++)
        {
            for (int i = 0; i < 10; i++)
            {
                Rect rect(0, 0, 96, 96);

                HRESULT hr = CopyRectangle(*srcImage.GetImage(0, 0, 0), rect
                    , *dstImage.GetImage(0, 0, 0), TEX_FILTER_DEFAULT, i * 96, j * 96);

                if (FAILED(hr))
                {
                    assert(false);
                }
            }
        }

        texture->UpdateImage();

        std::shared_ptr<Material> material = std::make_shared<Material>();
        material->SetShader(ResourceManager::Find<Shader>(L"SpriteShader"));
        material->SetTexture(texture);

        {
            GameObject* bg = new GameObject();
            AddGameObject(bg, eLayerType::PixelWorld);
            bg->GetComponent<Transform>()->SetPosition(Vector3(480.f, 96.f, 1.0f));
            bg->GetComponent<Transform>()->SetScale(Vector3(960.f, 192.f, 1.0f));
            MeshRenderer* bgmesh = bg->AddComponent<MeshRenderer>();
            bgmesh->SetMaterial(material);
            bgmesh->SetMesh(ResourceManager::Find<Mesh>(L"RectMesh"));
        }



        for (int x = 0; x < 3; x++)
        {
            GameObject* bg = new GameObject();
            AddGameObject(bg, eLayerType::BG);
            bg->GetComponent<Transform>()->SetPosition(Vector3(256.f + x * 512.f, -1747.f - 86.f, 1.0f));
            bg->GetComponent<Transform>()->SetScale(Vector3(514.f, 171.f, 0.0f));
            MeshRenderer* bgmesh = bg->AddComponent<MeshRenderer>();
            bgmesh->SetMaterial(ResourceManager::Find<Material>(L"m_wall_background"));
            bgmesh->SetMesh(ResourceManager::Find<Mesh>(L"RectMesh"));
        }

        Teleport* teleport = new Teleport();
        teleport->GetComponent<Transform>()->SetPosition(256.f, -1747.f - 86.f, 0.00f);
        CreateObject(teleport, eLayerType::Object);


        {
            ShotGunner_Weak* object = new ShotGunner_Weak();
            AddGameObject(object, eLayerType::Monster);
            object->GetComponent<Transform>()->SetPosition(Vector3(1536.f / 2.f, -30.f, 0.2f));
        }

        Scene::Initialize();
        {
            Player* player;
            player = new Player();
            AddGameObject(player, eLayerType::Player);
            player->GetComponent<Transform>()->SetPosition(Vector3(1536.f / 2.f, -10.f, 0.200f));
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

            UIManager::SetPlayer(player);
            UIManager::CreateStartItems();
            UIManager::Test();
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