#include "zzBlastWand_0585.h"
#include "zzTransform.h"
#include "zzTexture.h"
#include "zzResourceManager.h"
#include "zzAnimator.h"
#include "zzMeshRenderer.h"
#include "zzMaterial.h"

namespace zz
{
    BlastWand_0585::BlastWand_0585()
    {
        mCapacity = 3;
        mManaMax = 113;
        mManaChargeSpeed = 30;
        mCurMana = mManaMax;
        mCastDelay = 0.f;
        mReChargeTime = 2.f;
        mSpread = 2.f;
        mSpells.resize(mCapacity);
        mTip = Vector3(15.f, 3.f, 1.0f);

        SetName(L"BlastWand_0585");
    }

    BlastWand_0585::~BlastWand_0585()
    {
    }
    void BlastWand_0585::Initialize()
    {
        GetComponent<Transform>()->SetPosition(6.f, -2.f, 0.0f);
        GetComponent<Transform>()->SetScale(15.f, 7.f, 1.0f);
            
        mInitialPosition = Vector3(6.f, -2.f, 0.0f);

        std::shared_ptr<Texture> texture
            = ResourceManager::Load<Texture>(L"BlastWand_0585", L"..\\Resources\\Texture\\Wand\\BlastWand_0585.png");

        MeshRenderer* mesh = AddComponent<MeshRenderer>();
        mesh->SetMaterial(ResourceManager::Find<Material>(L"m_SpriteAnimation"));
        mesh->SetMesh(ResourceManager::Find<Mesh>(L"RectMesh"));

        Animator* animator = AddComponent<Animator>();
        animator->Create(L"BlastWand_0585", texture, Vector2(0.0f, 0.0f), Vector2(15.0f, 7.0f), 1, Vector2::Zero, 1.f);
        animator->PlayAnimation(L"BlastWand_0585", true);

        GameObject::Initialize();
    }

    void BlastWand_0585::Update()
    {
        GameObject::Update();
    }

    void BlastWand_0585::LateUpdate()
    {
        GameObject::LateUpdate();
    }

    void BlastWand_0585::Render()
    {
        GameObject::Render();
    }
    void BlastWand_0585::UseEquipment()
    {
    }
}