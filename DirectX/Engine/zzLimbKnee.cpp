#include "zzLimbKnee.h"
#include "zzResourceManager.h"
#include "zzAnimator.h"
#include "zzTransform.h"
#include "zzMeshRenderer.h"

namespace zz
{
    LimbKnee::LimbKnee()
    {
    }
    LimbKnee::~LimbKnee()
    {
    }

    void LimbKnee::Initialize()
    {
        GetComponent<Transform>()->SetScale(16.f, 16.f, 1.0f);

        MeshRenderer* mesh = AddComponent<MeshRenderer>();
        mesh->SetMaterial(ResourceManager::Find<Material>(L"m_SpriteAnimation"));
        mesh->SetMesh(ResourceManager::Find<Mesh>(L"RectMesh"));

        std::shared_ptr<Texture> texture = ResourceManager::Find<Texture>(L"limb_long_knee");

        Animator* animator = AddComponent<Animator>();

        animator->Create(L"limb_long_knee_idle", texture, Vector2(0.0f, 0.0f), Vector2(16.0f, 16.0f), 4, Vector2::Zero, 0.14f);

        animator->PlayAnimation(L"limb_long_knee_idle", true);

        GameObject::Initialize();
    }

    void LimbKnee::Update()
    {
        GameObject::Update();
    }

    void LimbKnee::LateUpdate()
    {
        GameObject::LateUpdate();
    }

    void LimbKnee::Render()
    {
        GameObject::Render();
    }
}