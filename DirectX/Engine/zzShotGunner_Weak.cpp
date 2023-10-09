#include "zzShotGunner_Weak.h"
#include "zzAnimator.h"
#include "zzMeshRenderer.h"
#include "zzResourceManager.h"
#include "zzTransform.h"
#include "zzCollider.h"
#include "zzDetectPlayer.h"
#include "zzPixelCollider.h"
#include "zzRigidBody.h"
#include "zzFireBall_Small.h"
#include "zzMuzzleEffect.h"
#include "zzTime.h"
#include "zzRenderer.h"

namespace zz
{
    ShotGunner_Weak::ShotGunner_Weak()
        : mAnimator(nullptr)
        , mMuzzleEffect(nullptr)
        , mTip(Vector3(10.f, 1.0f, 0.0f))
        , mState(eMonsterState::Freedom)
        , mChoiceNextAction(2.0f)
        , mActionIndex(0)
        , mbEnterAction(true)
        , mRigid(nullptr)
        , mDirection(1)
        , mPxCollider(nullptr)
        , mDetectPlayer(nullptr)
    {
    }

    ShotGunner_Weak::~ShotGunner_Weak()
    {
    }

    void ShotGunner_Weak::Initialize()
    {
        GetComponent<Transform>()->SetScale(17.f, 17.f, 1.0f);
        AddComponent<Collider>()->SetScale(5.f, 10.f, 1.0f); 

        mPxCollider = AddComponent<PixelCollider>();
        mPxCollider->SetCollision(Vector3(0.0f, -4.0f, 0.0f), Vector3(6.f, 8.f, 0.0f));
        mPxCollider->SetClimbY(3.0f);

        mRigid = AddComponent<RigidBody>();
        mDetectPlayer = AddComponent<DetectPlayer>();

        std::shared_ptr<Texture> texture = ResourceManager::Find<Texture>(L"shotgunner_weak");

        MeshRenderer* mesh = AddComponent<MeshRenderer>();
        mesh->SetMaterial(ResourceManager::Find<Material>(L"m_SpriteAnimation"));
        mesh->SetMesh(ResourceManager::Find<Mesh>(L"RectMesh"));

        mAnimator = AddComponent<Animator>();

        mAnimator->Create(L"shotgunner_weak_stand", texture, Vector2(0.0f, 0.0f), Vector2(17.0f, 17.0f), 6, Vector2::Zero, 0.16f);
        mAnimator->Create(L"shotgunner_weak_walk", texture, Vector2(0.0f, 17.f), Vector2(17.0f, 17.0f), 6, Vector2::Zero, 0.11f);
        mAnimator->Create(L"shotgunner_weak_attack", texture, Vector2(0.0f, 68.f), Vector2(17.0f, 17.0f), 6, Vector2::Zero, 0.07f);
        mAnimator->Create(L"shotgunner_weak_attack_ranged", texture, Vector2(0.0f, 85.f), Vector2(17.0f, 17.0f), 7, Vector2::Zero, 0.15f);

        //mAnimator->FindAnimation(L"shotgunner_weak_stand")->SetAnimationEvent(3, std::bind(&ShotGunner_Weak::Test, this));
        mAnimator->PlayAnimation(L"shotgunner_weak_stand", true);

        mAnimator->EndEvent(L"shotgunner_weak_attack_ranged") = [this]() { playIdleAnimation(); };


        GameObject::Initialize();
    }

    void ShotGunner_Weak::Update()
    {
        switch (mState)
        {
        case eMonsterState::Freedom:        freedom();      break;
        case eMonsterState::FollowPlayer:   followPlayer(); break;
        case eMonsterState::Battle:         battle();       break;
        default: break;
        }
        GameObject::Update();
    }

    void ShotGunner_Weak::LateUpdate()
    {
        GameObject::LateUpdate();
    }

    void ShotGunner_Weak::Render()
    {
        renderer::FlipCB flipCB = {};
        flipCB.flip.x = mDirection * -1;

        ConstantBuffer* cb = renderer::constantBuffer[(UINT)eCBType::Flip];
        cb->SetBufferData(&flipCB);
        cb->BindConstantBuffer(eShaderStage::HS);

        GameObject::Render();

        flipCB = {};
        cb->SetBufferData(&flipCB);
        cb->BindConstantBuffer(eShaderStage::PS);
    }

    void ShotGunner_Weak::OnCollisionEnter(GameObject* other)
    {
    }
    void ShotGunner_Weak::OnCollisionStay(GameObject* other)
    {
    }
    void ShotGunner_Weak::OnCollisionExit(GameObject* other)
    {
    }

    void ShotGunner_Weak::playIdleAnimation()
    {
        mAnimator->PlayAnimation(L"shotgunner_weak_stand", true);
    }

    void ShotGunner_Weak::freedom()
    {
        mChoiceNextAction -= (float)Time::DeltaTime();
        if (mChoiceNextAction <= 0)
        {
            mChoiceNextAction = 2.0f;
            mActionIndex = randi(3);
            mbEnterAction = true;
        }

        switch (mActionIndex)
        {
        case 0: 
        {
            if (mbEnterAction)
            {
                mAnimator->PlayAnimation(L"shotgunner_weak_stand", true);
                mRigid->SetVelocityX(0.f);
                mbEnterAction = false;
            }
            break;
        }
        case 1:
        {
            if (mbEnterAction)
            {
                mAnimator->PlayAnimation(L"shotgunner_weak_walk", true);
                mRigid->SetVelocityX(50.f);
                mDirection = 1;
                mbEnterAction = false;
            }
            break;
            
        }
        case 2:
        {
            if (mbEnterAction)
            {
                mAnimator->PlayAnimation(L"shotgunner_weak_walk", true);
                mRigid->SetVelocityX(-50.f);
                mDirection = -1;
                mbEnterAction = false;
            }
            break;
        }
        case 3:
        {
            mAnimator->PlayAnimation(L"shotgunner_weak_stand", true);
            mRigid->SetVelocityX(0.f);
            mbEnterAction = false;
            mState = eMonsterState::Battle;
            break;
        }

        default: break;
        }
    }

    void ShotGunner_Weak::followPlayer()
    {
    }

    void ShotGunner_Weak::battle()
    {
        mChoiceNextAction -= (float)Time::DeltaTime();
        if (mChoiceNextAction <= 0)
        {
            mChoiceNextAction = 2.0f;
            mActionIndex = randi(1);
            mbEnterAction = true;
        }

        switch (mActionIndex)
        {
        case 0:
        {
            if (mbEnterAction)
            {
                mAnimator->PlayAnimation(L"shotgunner_weak_attack_ranged", false);
                //mRigid->SetVelocityX(0.f);
                mbEnterAction = false;

                std::shared_ptr<Texture> muzzleTexture = ResourceManager::Load<Texture>(L"muzzle_medium_05", L"..\\Resources\\Texture\\Monster\\muzzle_medium_05.png");

                Animator* manimator = new Animator();
                manimator->Create(L"muzzle_medium_05_play", muzzleTexture, Vector2(0.0f, 0.0f), Vector2(16.0f, 16.0f), 1, Vector2::Zero, 0.1f);
                manimator->PlayAnimation(L"muzzle_medium_05_play", false);

                MuzzleEffect* muzzleEffect = new MuzzleEffect();
                muzzleEffect->SetAnimator(manimator, L"muzzle_medium_05_play");
                muzzleEffect->GetComponent<Transform>()->SetScale(16.0f, 16.0f, 1.0f);
                muzzleEffect->GetComponent<Transform>()->SetParent(GetComponent<Transform>());
                muzzleEffect->GetComponent<Transform>()->SetPositionX(mTip.x + 5.f);
                muzzleEffect->SetDir(mDirection);
                CreateObject(muzzleEffect, eLayerType::Effect);


                Vector3 pPos = mDetectPlayer->GetPlayerPos();

                FireBall_Small* fire = new FireBall_Small();
                fire->GetComponent<Transform>()->SetPosition(GetComponent<Transform>()->GetPosition() + mTip * mDirection);
                fire->GetComponent<Transform>()->SetScale(4.0f, 4.0f, 1.0f);
                fire->SetDetectPos(pPos);

                CreateObject(fire, eLayerType::MonsterAttack);
            }
            break;
        }
        case 1:
        {
            if (mbEnterAction)
            {
                mAnimator->PlayAnimation(L"shotgunner_weak_stand", true);
                mRigid->SetVelocityX(0.f);
                mbEnterAction = false;
            }
            break;
        }
        default: break;
        }
    }

}