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
#include "zzHealthPoint.h"
#include "zzRenderer.h"
#include "zzPixelWorld.h"

namespace zz
{
    ShotGunner_Weak::ShotGunner_Weak()
        : mAnimator(nullptr)
        , mMuzzleEffect(nullptr)
        , mTip(Vector3(10.f, 1.0f, 0.0f))
        , mPxColliderScale(Vector3::One)
        , mState(eMonsterState::Freedom)
        , mChoiceNextAction(2.0f)
        , mActionIndex(0)
        , mbEnterAction(true)
        , mRigid(nullptr)
        , mDirection(1)
        , mPxCollider(nullptr)
        , mDetectPlayer(nullptr)
        , mDetectPlayerTime(1.5f)
        , mReloadTime(0.0f)
        , mbJump(false)
    {
    }

    ShotGunner_Weak::~ShotGunner_Weak()
    {
    }

    void ShotGunner_Weak::Initialize()
    {
        GetComponent<Transform>()->SetScale(17.f, 17.f, 1.0f);
        AddComponent<Collider>()->SetScale(12.f, 14.f, 1.0f);

        mPxColliderScale = Vector3(6.f, 8.f, 0.0f);
        mPxCollider = AddComponent<PixelCollider>();
        mPxCollider->SetCollision(Vector3(0.0f, -4.0f, 0.0f), mPxColliderScale);
        mPxCollider->SetClimbY(4.0f);

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
        mAnimator->Create(L"shotgunner_weak_jump", texture, Vector2(0.0f, 34.f), Vector2(17.0f, 17.0f), 3, Vector2::Zero, 0.09f);
        mAnimator->Create(L"shotgunner_weak_fall", texture, Vector2(0.0f, 51.f), Vector2(17.0f, 17.0f), 3, Vector2::Zero, 0.09f);
        mAnimator->Create(L"shotgunner_weak_jump_prepare", texture, Vector2(0.0f, 136.f), Vector2(17.0f, 17.0f), 3, Vector2::Zero, 0.07f);

        //mAnimator->FindAnimation(L"shotgunner_weak_stand")->SetAnimationEvent(3, std::bind(&ShotGunner_Weak::Test, this));
        mAnimator->PlayAnimation(L"shotgunner_weak_stand", true);

        mAnimator->EndEvent(L"shotgunner_weak_attack_ranged") = [this]() { shootAnimationEnd(); };
        mAnimator->FindAnimation(L"shotgunner_weak_attack_ranged")->SetAnimationEvent(4, [this]() { shoot(); });

        mHealPoint = AddComponent<HealthPoint>();
        mHealPoint->SetMaxHP(25.f);
        mHealPoint->SetHpZeroEvent([this]() { DeadEvent(); });
        mHealPoint->SetHitEvent([this]() { HitEvent(); });

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
        cb->BindConstantBuffer(eShaderStage::PS);

        renderer::ColorCB flash = {};
        ConstantBuffer* color = renderer::constantBuffer[(UINT)eCBType::Color];

        if (mHitFlashTime > 0)
        {
            mHitFlashTime -= (float)Time::DeltaTime();

            renderer::ColorCB flash;
            flash.color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
         
            color->SetBufferData(&flash);
            color->BindConstantBuffer(eShaderStage::PS);
        }

        GameObject::Render();

        flipCB = {};
        cb->SetBufferData(&flipCB);
        cb->BindConstantBuffer(eShaderStage::PS);

        flash = {};
        color->SetBufferData(&flash);
        color->BindConstantBuffer(eShaderStage::PS);
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

    void ShotGunner_Weak::DeadEvent()
    {
        if (!IsDead())
        {
            DeleteObject(this, eLayerType::Monster);
        }
    }

    void ShotGunner_Weak::HitEvent()
    {
        mHitFlashTime = 0.15f;
    }

    void ShotGunner_Weak::playIdleAnimation()
    {
        mAnimator->PlayAnimation(L"shotgunner_weak_stand", true);
    }

    void ShotGunner_Weak::shootAnimationEnd()
    {
        mState = eMonsterState::FollowPlayer;
        mbEnterAction = true;
        mDetectPlayerTime = 1.0f;
        mReloadTime = 2.0f;
    }

    void ShotGunner_Weak::shoot()
    {
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

    void ShotGunner_Weak::freedom()
    {
        mDetectPlayerTime -= (float)Time::DeltaTime();

        if (mDetectPlayerTime <= 0)
        {
            mDetectPlayerTime = 1.5f;

            if (mDetectPlayer->IsPlayerInRange())
            {
                mState = eMonsterState::FollowPlayer;
                mbEnterAction = true;
                return;
            }
        }

        if (!mbJump)
        {
            Vector3 pos = GetComponent<Transform>()->GetPosition();
            Vector3 nextPos = mRigid->GetPredictedPosition();
            pos.y -= (int)(mPxColliderScale.y / 2.f) + 1;
            for (int j = 1; j <= (int)fabs(nextPos.x - pos.x) + 1; j++)
            {
                bool isCliff = true;
                for (int i = 5; i <= 13; i++)
                {
                    if (PixelWorld::GetElement((int)pos.x + j * mDirection, -((int)pos.y - i)).Type == eElementType::SOLID)
                    {
                        isCliff = false;
                        break;
                    }
                }

                if (isCliff)
                {
                    mDirection *= -1;
                    mbEnterAction = true;
                }
            }
        }

        if (mDirection == 1)
        {
            if (mbEnterAction)
            {
                mAnimator->PlayAnimation(L"shotgunner_weak_walk", true);
                mbEnterAction = false;
            }
            mRigid->SetVelocityX(50.f);
        }
        else if(mDirection == -1)
        {
            if (mbEnterAction)
            {
                mAnimator->PlayAnimation(L"shotgunner_weak_walk", true);
                mbEnterAction = false;
            }
            mRigid->SetVelocityX(-50.f);
        }
        else
        {
            if (mbEnterAction)
            {
                mAnimator->PlayAnimation(L"shotgunner_weak_stand", true);
                mbEnterAction = false;
            }
            mRigid->SetVelocityX(0.f);
        }

        if (mPxCollider->IsSideBlock())
        {
            if (mRigid->GetVelocityY() == 0)
            {
                mAnimator->PlayAnimation(L"shotgunner_weak_jump", false);
                mRigid->SetVelocityY(150.f);
                mbEnterAction = false;
                mbJump = true;
            }
        }
        if (mbJump)
        {
            if (mRigid->GetVelocityY() == 0)
            {
                mbJump = false;
                mbEnterAction = true;
            }
        }

        mChoiceNextAction -= (float)Time::DeltaTime();
        if (mChoiceNextAction <= 0)
        {
            mChoiceNextAction = 2.0f;
            mDirection = random() > 0.5 ? 1 : -1;
            mbEnterAction = true;
        }
    }

    void ShotGunner_Weak::followPlayer()
    {
        if (!mDetectPlayer->IsPlayerInRange())
        {
            mState = eMonsterState::Freedom;
            return;
        }

        if (mDirection != mDetectPlayer->GetDirection())
        {
            mbEnterAction = true;
            mDirection *= -1;
        }

        mReloadTime -= (float)Time::DeltaTime();

        if (mReloadTime <= 0.f && shootRay())
        {
            mState = eMonsterState::Battle;
            mbEnterAction = true;
            return;
        }

        if (mDirection == 1)
        {
            if (mbEnterAction)
            {
                mAnimator->PlayAnimation(L"shotgunner_weak_walk", true);
                mbEnterAction = false;
            }
            mRigid->SetVelocityX(50.f);
        }
        else
        {
            if (mbEnterAction)
            {
                mAnimator->PlayAnimation(L"shotgunner_weak_walk", true);
                mbEnterAction = false;
            }
            mRigid->SetVelocityX(-50.f);
        }

        if (mPxCollider->IsSideBlock())
        {
            if(mRigid->GetVelocityY() == 0)
            {
                mAnimator->PlayAnimation(L"shotgunner_weak_jump", true);
                mRigid->SetVelocityY(150.f);
                mbEnterAction = false;
            }
        }
        if (mbJump)
        {
            if (mRigid->GetVelocityY() == 0)
            {
                mbJump = false;
                mbEnterAction = true;
            }
        }
    }

    void ShotGunner_Weak::battle()
    {
        if (mbEnterAction)
        {
            mAnimator->PlayAnimation(L"shotgunner_weak_attack_ranged", false);
            mRigid->SetVelocityX(0.f);
            mbEnterAction = false;
        }

    }

    bool ShotGunner_Weak::shootRay()
    {
        Vector3 pos = GetComponent<Transform>()->GetPosition();
        pos += Vector3(mTip.x * mDirection, mTip.y, mTip.z);

        Vector3 playerPos = mDetectPlayer->PlayerPos;

        int dx = playerPos.x - pos.x;
        int dy = playerPos.y - pos.y;

        int steps = std::max(std::abs(dx), std::abs(dy));

        float xIncrement = dx / (float)steps;
        float yIncrement = dy / (float)steps;

        float x = pos.x;
        float y = pos.y;

        for (int i = 0; i < steps + 1; i++)
        {
            if (PixelWorld::GetElement(x, -y).Type == eElementType::SOLID)
            {
                return false;
            }
            x += xIncrement;
            y += yIncrement;
        }
        return true;
    }

}