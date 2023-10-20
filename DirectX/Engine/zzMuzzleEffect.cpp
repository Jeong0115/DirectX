#include "zzMuzzleEffect.h"
#include "zzAnimator.h"
#include "zzMeshRenderer.h"
#include "zzResourceManager.h"
#include "zzMesh.h"
#include "zzMaterial.h"
#include "zzInput.h"
#include "zzRenderer.h"
#include "zzTransform.h"

namespace zz
{
    MuzzleEffect::MuzzleEffect()
        : mAnimator(nullptr)
        , mDir(0)
    {
    }

    MuzzleEffect::~MuzzleEffect()
    {
    }

    void MuzzleEffect::Initialize()
    {
        MeshRenderer* meshRender = AddComponent<MeshRenderer>();
        meshRender->SetMaterial(ResourceManager::Find<Material>(L"m_SpriteAnimation"));
        meshRender->SetMesh(ResourceManager::Find<Mesh>(L"RectMesh"));

        if(mDir != 0)
        {
            if (mDir == 1)
            {
                GetComponent<Transform>()->SetPositionXFlip();
            }
        }


        GameObject::Initialize();
    }

    void MuzzleEffect::Update()
    {
        GameObject::Update();
    }

    void MuzzleEffect::LateUpdate()
    {
        GameObject::LateUpdate();
    }

    void MuzzleEffect::Render()
    {
        renderer::FlipCB flipCB = {};

        if(mDir != 0)
        {
            flipCB.flip.x = mDir;
        }

        ConstantBuffer* cb = renderer::constantBuffer[(UINT)eCBType::Flip];
        cb->SetBufferData(&flipCB);
        cb->BindConstantBuffer(eShaderStage::PS);

        GameObject::Render();

        flipCB = {};
        cb->SetBufferData(&flipCB);
        cb->BindConstantBuffer(eShaderStage::PS);
    }

    void MuzzleEffect::SetAnimator(Animator* ani, const std::wstring name)
    {
        mAnimator = ani;
        AddComponent<Animator>(ani);
        ani->EndEvent(name) = [this]() { endAnimation(); };
    }

    void MuzzleEffect::endAnimation()
    {
        if (!IsDead())
        {
            DeleteObject(this, eLayerType::Effect);
        }
    }
}