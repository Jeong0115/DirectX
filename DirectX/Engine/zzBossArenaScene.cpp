#include "zzBossArenaScene.h"
#include "zzTransform.h"
#include "zzPixelWorld.h"
#include "zzMeshRenderer.h"
#include "zzResourceManager.h"
#include "zzTexture.h"

namespace zz
{
    BossArenaScene::BossArenaScene()
    {
    }

    BossArenaScene::~BossArenaScene()
    {
    }

    void BossArenaScene::Initialize()
    {
        GameObject* object = new GameObject();
        AddGameObject(object, eLayerType::PixelWorld);
        object->GetComponent<Transform>()->SetPosition(Vector3(PixelWorld::mWorldWidth / 2 /*- 20*/, -PixelWorld::mWorldHeight / 2 /*+ 32*/, PIXEL_WORLD_Z));
        object->GetComponent<Transform>()->SetScale(Vector3(PixelWorld::mWorldWidth, PixelWorld::mWorldHeight, 1.0f));
        MeshRenderer* mesh = object->AddComponent<MeshRenderer>();

        std::shared_ptr<Material> material = ResourceManager::Find<Material>(L"m_PixelTexture");
        std::shared_ptr<Texture> texture = std::make_shared<PixelTexture>(PixelWorld::mWorldWidth, PixelWorld::mWorldHeight);
        material->SetTexture(texture);
        mesh->SetMaterial(material);
        mesh->SetMesh(ResourceManager::Find<Mesh>(L"RectMesh"));

        GameObject* bg = new GameObject();
        AddGameObject(bg, eLayerType::BG);
        bg->GetComponent<Transform>()->SetPosition(Vector3(1300.f, -800.f, 1.0f));
        bg->GetComponent<Transform>()->SetScale(Vector3(2600.f, 1600.f, 1.0f));
        MeshRenderer* bgmesh = bg->AddComponent<MeshRenderer>();
        bgmesh->SetMaterial(ResourceManager::Find<Material>(L"m_boss_arena_background"));
        bgmesh->SetMesh(ResourceManager::Find<Mesh>(L"RectMesh"));

        Scene::Initialize();
    }

    void BossArenaScene::Update()
    {
        Scene::Update();
    }

    void BossArenaScene::LateUpdate()
    {
        Scene::LateUpdate();
    }

    void BossArenaScene::Render()
    {
        Scene::Render();
    }

    void BossArenaScene::MovePlayer()
    {
        GetLayer(eLayerType::Player).GetGameObjects()[0]->GetComponent<Transform>()->SetPosition(512.f, -700.f, 0.200f);

        //GetLayer(eLayerType::Player).GetGameObjects()[0]->GetComponent<Transform>()->SetPosition(356.f, -889.f, 0.200f);
    }
}