#include "zzExplosionEffect.h"
#include "zzAnimator.h"
#include "zzMeshRenderer.h"
#include "zzResourceManager.h"
#include "zzMesh.h"
#include "zzMaterial.h"

namespace zz
{
    ExplosionEffect::ExplosionEffect()
        : mAnimator(nullptr)
    {
    }

    ExplosionEffect::~ExplosionEffect()
    {
    }

    void ExplosionEffect::Initialize()
    {
        MeshRenderer* meshRender = AddComponent<MeshRenderer>();
        meshRender->SetMaterial(ResourceManager::Find<Material>(L"m_SpriteAnimation"));
        meshRender->SetMesh(ResourceManager::Find<Mesh>(L"RectMesh"));

        GameObject::Initialize();
    }

    void ExplosionEffect::Update()
    {
        GameObject::Update();
    }

    void ExplosionEffect::LateUpdate()
    {
        GameObject::LateUpdate();
    }

    void ExplosionEffect::Render()
    {
        GameObject::Render();
    }

    void ExplosionEffect::SetAnimator(Animator* ani, const std::wstring name)
    {
        AddComponent<Animator>(ani);
        ani->EndEvent(name) = [this]() { endAnimation(); };
    }

    void ExplosionEffect::endAnimation()
    {
        if (!IsDead())
        {
            DeleteObject(this, eLayerType::Effect);
        }
    }
}