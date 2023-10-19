#include "zzEndScene.h"
#include "zzTransform.h"
#include "zzPixelWorld.h"
#include "zzMeshRenderer.h"
#include "zzResourceManager.h"
#include "zzTexture.h"
#include "zzEndingEffect.h"
#include "zzInput.h"

namespace zz
{
    EndScene::EndScene()
    {
    }
    EndScene::~EndScene()
    {
    }
    void EndScene::Initialize()
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
        bg->GetComponent<Transform>()->SetPosition(Vector3(768.f, -768.f, 1.0f));
        bg->GetComponent<Transform>()->SetScale(Vector3(512.f, 512.f, 1.0f));
        MeshRenderer* bgmesh = bg->AddComponent<MeshRenderer>();
        bgmesh->SetMaterial(ResourceManager::Find<Material>(L"m_boss_victoryroom_background"));
        bgmesh->SetMesh(ResourceManager::Find<Mesh>(L"RectMesh"));


        Scene::Initialize();
    }
    void EndScene::Update()
    {
        if (Input::GetKeyDown(eKeyCode::E))
        {
            EndingEffect* effect = new EndingEffect();
            effect->GetComponent<Transform>()->SetPosition(Vector3(768.f, -744.f, 0.0f));
            AddGameObject(effect, eLayerType::Effect);
            effect->Initialize();
        }

        Scene::Update();
    }
    void EndScene::LateUpdate()
    {
        Scene::LateUpdate();
    }
    void EndScene::Render()
    {
        Scene::Render();
    }
    void EndScene::MovePlayer()
    {
        GetLayer(eLayerType::Player).GetGameObjects()[0]->GetComponent<Transform>()->SetPosition(768.f, -700.f, 0.200f);
    }
}