#include "zzExplosionEffect.h"
#include "zzAnimator.h"
#include "zzMeshRenderer.h"
#include "zzResourceManager.h"
#include "zzMesh.h"
#include "zzMaterial.h"
#include "zzTransform.h"

namespace zz
{
    ExplosionEffect::ExplosionEffect()
        : mAnimator(nullptr)
        , mbDelete(true)
        , mAnimationName{}
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

        if (!mbDelete)
        {
            SetState(eState::Sleep);
            SetActive(false);
        }

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
        mAnimationName = name;
        AddComponent<Animator>(ani);
        ani->EndEvent(name) = [this]() { endAnimation(); };
    }

    void ExplosionEffect::SetPosition(Vector3 pos)
    {
        GetComponent<Transform>()->SetPosition(pos);
    }

    void ExplosionEffect::SetPosition(float x, float y, float z)
    {
        GetComponent<Transform>()->SetPosition(x, y, z);
    }

    void ExplosionEffect::SetScale(Vector3 scale)
    {
        GetComponent<Transform>()->SetScale(scale);
    }

    void ExplosionEffect::SetScale(float x, float y, float z)
    {
        GetComponent<Transform>()->SetScale(x, y, z);
    }

    void ExplosionEffect::Play()
    {
        SetState(eState::Active);
        SetActive(true);
        GetComponent<Animator>()->PlayAnimation(mAnimationName, false);
    }

    void ExplosionEffect::endAnimation()
    {
        if(mbDelete)
        {
            if (!IsDead())
            {
                DeleteObject(this, eLayerType::Effect);
            }
        }
        else
        {
            SetActive(false);
            SetState(eState::Sleep);
        }
    }
}