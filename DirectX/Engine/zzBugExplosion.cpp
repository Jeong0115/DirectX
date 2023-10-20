#include "zzBugExplosion.h"
#include "zzAnimator.h"
#include "zzMeshRenderer.h"
#include "zzResourceManager.h"
#include "zzTexture.h"
#include "zzTime.h"
#include "zzTransform.h"
#include "zzRigidBody.h"
#include "zzDetectPlayer.h"
#include "zzLight.h"
#include "zzCollider.h"
#include "zzHealthPoint.h"
namespace zz
{
    BugExplosion::BugExplosion()
    {
    }
    BugExplosion::~BugExplosion()
    {
    }
    void BugExplosion::Initialize()
    {
        GetComponent<Transform>()->SetScale(40.f, 40.f, 1.0f);
        AddComponent<Collider>()->SetScale(25.f, 25.f, 1.0f);

        MeshRenderer* mesh = AddComponent<MeshRenderer>();
        mesh->SetMaterial(ResourceManager::Find<Material>(L"m_SpriteAnimation"));
        mesh->SetMesh(ResourceManager::Find<Mesh>(L"RectMesh"));

        std::shared_ptr<Texture> texture = ResourceManager::Load<Texture>(L"explosion_040_poof_blue", L"..\\Resources\\Texture\\Centipede\\explosion_040_poof_blue.png");

        Animator* animator = AddComponent<Animator>();
        animator->Create(L"explosion_040_poof_blue_idle", texture, Vector2(0.0f, 0.0f), Vector2(40.0f, 40.0f), 5, Vector2::Zero, 0.04f);
        animator->PlayAnimation(L"explosion_040_poof_blue_idle", false);

        Light* light = AddComponent<Light>();
        light->SetLightColor(0.2f, 0.3f, 0.7f, 0.8f);
        light->SetLightScale(45.f, 45.f, 45.f);

        animator->EndEvent(L"explosion_040_poof_blue_idle") = [this]() { Dead(); };

        GameObject::Initialize();
    }
    void BugExplosion::Update()
    {
        GameObject::Update();
    }
    void BugExplosion::LateUpdate()
    {
        GameObject::LateUpdate();
    }
    void BugExplosion::Render()
    {
        GameObject::Render();
    }
    void BugExplosion::OnCollisionEnter(GameObject* other)
    {
        if (other->GetLayerType() == eLayerType::Player)
        {
            other->GetComponent<HealthPoint>()->Hit(23.f);
        }
    }
    void BugExplosion::OnCollisionStay(GameObject* other)
    {
    }
    void BugExplosion::OnCollisionExit(GameObject* other)
    {
    }
    void BugExplosion::Dead()
    {
        DeleteObject(this, eLayerType::MonsterAttack);
    }
}