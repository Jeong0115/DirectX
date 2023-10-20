#include "zzBossHPIcon.h"
#include "zzMeshRenderer.h"
#include "zzTexture.h"
#include "zzTransform.h"
#include "zzResourceManager.h"

namespace zz
{
    BossHPIcon::BossHPIcon()
        : UI(eUIType::HUD)
    {
    }
    BossHPIcon::~BossHPIcon()
    {
    }
    void BossHPIcon::Initialize()
    {
        Transform* tr = GetComponent<Transform>();
        tr->SetPosition(352.f, 60.f, 0.01f);
        tr->SetScale(8.f, 8.0f, 1.0f);

        MeshRenderer* mesh = AddComponent<MeshRenderer>();
        std::shared_ptr<Material> material = std::make_shared<Material>();
        material->SetTexture(ResourceManager::Load<Texture>(L"boss_health", L"..\\Resources\\Texture\\Centipede\\health.png"));
        material->SetShader(ResourceManager::Find<Shader>(L"SpriteShader"));
        mesh->SetMesh(ResourceManager::Find<Mesh>(L"RectMesh"));
        mesh->SetMaterial(material);

        GameObject::Initialize();
    }
    void BossHPIcon::Update()
    {
        if (IsActive())
        {
            GameObject::Update();
        }
    }
    void BossHPIcon::LateUpdate()
    {
        if (IsActive())
        {
            GameObject::LateUpdate();
        }
    }
    void BossHPIcon::Render()
    {
        if (IsActive())
        {
            GameObject::Render();
        }
    }
}