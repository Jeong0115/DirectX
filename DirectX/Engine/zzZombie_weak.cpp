#include "zzZombie_weak.h"
#include "zzAnimator.h"
#include "zzMeshRenderer.h"
#include "zzResourceManager.h"
#include "zzTransform.h"
#include "zzCollider.h"
#include "zzDetectPlayer.h"
#include "zzPixelCollider.h"
#include "zzRigidBody.h"
#include "zzTime.h"
#include "zzRenderer.h"
#include "zzHealthPoint.h"
#include "zzRenderer.h"
#include "zzPixelWorld.h"
#include "zzMonsterBody.h"
#include "zzPixelWorld.h"
#include "zzZombieAttack.h"
#include "zzAudioSource.h"

namespace zz
{
    Zombie_weak::Zombie_weak()
        : mAnimator(nullptr)
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
        , mBodies{}
        , mAttack(nullptr)
        , mAudio(nullptr)
    {
    }
    Zombie_weak::~Zombie_weak()
    {
        if (mAttack)
        {
            DeleteObject(mAttack, eLayerType::MonsterAttack);
            mAttack = nullptr;
        }
    }
    void Zombie_weak::Initialize()
    {
        GetComponent<Transform>()->SetScale(17.f, 17.f, 1.0f);
        AddComponent<Collider>()->SetScale(14.f, 10.f, 1.0f);

        mPxColliderScale = Vector3(5.f, 6.f, 0.0f);
        mPxCollider = AddComponent<PixelCollider>();
        mPxCollider->SetCollision(Vector3(0.0f, -4.0f, 0.0f), mPxColliderScale);
        mPxCollider->SetClimbY(3.0f);

        mRigid = AddComponent<RigidBody>();
        mDetectPlayer = AddComponent<DetectPlayer>();

        std::shared_ptr<Texture> texture = ResourceManager::Find<Texture>(L"zombie_weak");

        MeshRenderer* mesh = AddComponent<MeshRenderer>();
        mesh->SetMaterial(ResourceManager::Find<Material>(L"m_SpriteAnimation"));
        mesh->SetMesh(ResourceManager::Find<Mesh>(L"RectMesh"));

        mAnimator = AddComponent<Animator>();

        mAnimator->Create(L"zombie_weak_stand", texture, Vector2(0.0f, 1.0f), Vector2(18.0f, 19.0f), 6, Vector2::Zero, 0.2f);
        mAnimator->Create(L"zombie_weak_walk", texture, Vector2(0.0f, 21.0f), Vector2(18.0f, 19.0f), 6, Vector2::Zero, 0.082f);
        mAnimator->Create(L"zombie_weak_jump", texture, Vector2(0.0f, 61.0f), Vector2(19.0f, 20.0f), 3, Vector2::Zero, 0.082f);
        mAnimator->Create(L"zombie_weak_fall", texture, Vector2(0.0f, 81.0f), Vector2(19.0f, 20.0f), 3, Vector2::Zero, 0.082f);
        mAnimator->Create(L"zombie_weak_attack", texture, Vector2(0.0f, 121.0f), Vector2(19.0f, 20.0f), 4, Vector2::Zero, 0.075f);

        mAnimator->PlayAnimation(L"zombie_weak_stand", true);

        mAnimator->EndEvent(L"zombie_weak_attack") = [this]() { playIdleAnimation(); };
        //mAnimator->FindAnimation(L"shotgunner_weak_attack_ranged")->SetAnimationEvent(4, [this]() { shoot(); });

        mHealPoint = AddComponent<HealthPoint>();
        mHealPoint->SetMaxHP(30.f);
        mHealPoint->SetHpZeroEvent([this]() { DeadEvent(); });
        mHealPoint->SetHitEvent([this]() { HitEvent(); });

        mAudio = AddComponent<AudioSource>();

        createBody();

        GameObject::Initialize();
    }

    void Zombie_weak::Update()
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

    void Zombie_weak::LateUpdate()
    {
        GameObject::LateUpdate();
    }

    void Zombie_weak::Render()
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

    void Zombie_weak::OnCollisionEnter(GameObject* other)
    {
        
    }
    void Zombie_weak::OnCollisionStay(GameObject* other)
    {
    }
    void Zombie_weak::OnCollisionExit(GameObject* other)
    {
    }

    void Zombie_weak::playIdleAnimation()
    {
        mbEnterAction = true;
        mState = eMonsterState::Freedom;

        if (mAttack)
        {
            DeleteObject(mAttack, eLayerType::MonsterAttack);
            mAttack = nullptr;
        }
    }

    void Zombie_weak::DeadEvent()
    {
        if (!IsDead())
        {
            DeleteObject(this, eLayerType::Monster);

            std::random_device rd;
            std::mt19937 g(rd());
            std::shuffle(mBodies.begin(), mBodies.end(), g);

            Vector3 pos = GetComponent<Transform>()->GetPosition();
            Vector2 impulse = Vector2(4, 0);
            int size = mBodies.size();
            for (int i = 0; i < size; i++)
            {
                float angle = PI * ((float)i / size);

                Vector2 vel;
                vel.x = impulse.x * cos(angle);
                vel.y = impulse.x * sin(angle);

                mBodies[i]->Impulse(vel * 30.f);

                mBodies[i]->GetComponent<Transform>()->SetPosition(Vector3(pos.x + vel.x, pos.y + vel.y, pos.z));
                mBodies[i]->SetBox2dScale(Vector3(3.0f, 3.0f, 1.0f));

                CreateObject(mBodies[i], eLayerType::Effect);
            }
            if (mAttack)
            {
                DeleteObject(mAttack, eLayerType::MonsterAttack);
                mAttack = nullptr;
            }

            mAudio->SetClip(ResourceManager::LoadAudioClip(L"zombie_death_01", L"..\\Resources\\Audio\\Enemy\\zombie_death_01.wav"));
            mAudio->SetLoop(false);
            mAudio->Play();
        }
    }
    void Zombie_weak::HitEvent()
    {
        mHitFlashTime = 0.15f;

        switch (randi(2))
        {
        case 0: mAudio->SetClip(ResourceManager::LoadAudioClip(L"zombie_damage_01", L"..\\Resources\\Audio\\Enemy\\zombie_damage_01.wav")); break;
        case 1: mAudio->SetClip(ResourceManager::LoadAudioClip(L"zombie_damage_02", L"..\\Resources\\Audio\\Enemy\\zombie_damage_02.wav")); break;
        case 2: mAudio->SetClip(ResourceManager::LoadAudioClip(L"zombie_damage_03", L"..\\Resources\\Audio\\Enemy\\zombie_damage_03.wav")); break;
        }

        mAudio->SetLoop(false);
        mAudio->Play();

        Vector3 pos = GetComponent<Transform>()->GetPosition();

        for (int i = -1; i <= 1; i++)
        {
            for (int j = -1; j <= 1; j++)
            {
                PixelWorld::InsertElementIfEmpty((int)pos.x + i, (int)-pos.y + j, BLOOD);
            }
        }
    }

    void Zombie_weak::freedom()
    {
        mDetectPlayerTime -= (float)Time::DeltaTime();

        if (mDetectPlayerTime <= 0)
        {
            mDetectPlayerTime = 1.5f;

            if (mDetectPlayer->IsPlayerInRange())
            {
                mActionIndex = 1;
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
                mAnimator->PlayAnimation(L"zombie_weak_walk", true);
                mbEnterAction = false;
            }
            mRigid->SetVelocityX(50.f);
        }
        else if (mDirection == -1)
        {
            if (mbEnterAction)
            {
                mAnimator->PlayAnimation(L"zombie_weak_walk", true);
                mbEnterAction = false;
            }
            mRigid->SetVelocityX(-50.f);
        }
        else
        {
            if (mbEnterAction)
            {
                mAnimator->PlayAnimation(L"zombie_weak_stand", true);
                mbEnterAction = false;
            }
            mRigid->SetVelocityX(0.f);
        }

        if (mPxCollider->IsSideBlock())
        {
            if (mRigid->GetVelocityY() == 0)
            {
                mAnimator->PlayAnimation(L"zombie_weak_jump", false);
                mRigid->SetVelocityY(130.f);
                mRigid->SetVelocityX(mDirection * -50.f);
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

    void Zombie_weak::followPlayer()
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

        if (mReloadTime <= 0.f)
        {
            mState = eMonsterState::Battle;
            mbEnterAction = true;
            return;
        }

        if (fabs(mDetectPlayer->GetPlayerPos().x - GetComponent<Transform>()->GetPosition().x) < 25.f)
        {
            if (mActionIndex == 1)
            {
                mAnimator->PlayAnimation(L"zombie_weak_stand", true);
                mbEnterAction = true;
                mActionIndex = 0;
            }
            mRigid->SetVelocityX(0.f);
        }
        else if (mDirection == 1)
        {
            mActionIndex = 1;
            if (mbEnterAction)
            {
                mAnimator->PlayAnimation(L"zombie_weak_walk", true);
                mbEnterAction = false;
            }
            mRigid->SetVelocityX(50.f);
        }
        else
        {
            mActionIndex = 1;
            if (mbEnterAction)
            {
                mAnimator->PlayAnimation(L"zombie_weak_walk", true);
                mbEnterAction = false;
            }
            mRigid->SetVelocityX(-50.f);
        }

        if (mPxCollider->IsSideBlock())
        {
            if (mRigid->GetVelocityY() == 0)
            {
                mAnimator->PlayAnimation(L"zombie_weak_jump", true);
                mRigid->SetVelocityY(120.f);
                mRigid->SetVelocityX(mDirection * -50.f);
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

    void Zombie_weak::battle()
    {
        if (mbEnterAction)
        {
            mAnimator->PlayAnimation(L"zombie_weak_attack", false);
            mRigid->Impulse(Vector3(150.f * mDirection, 50.f, 0.0f), 0.02f);
            mbEnterAction = false;

            switch (randi(2))
            {
            case 0: mAudio->SetClip(ResourceManager::LoadAudioClip(L"zombie_attack_01", L"..\\Resources\\Audio\\Enemy\\zombie_attack_01.wav")); break;
            case 1: mAudio->SetClip(ResourceManager::LoadAudioClip(L"zombie_attack_02", L"..\\Resources\\Audio\\Enemy\\zombie_attack_02.wav")); break;
            case 2: mAudio->SetClip(ResourceManager::LoadAudioClip(L"zombie_attack_03", L"..\\Resources\\Audio\\Enemy\\zombie_attack_03.wav")); break;
            }
           
            mAudio->SetLoop(false);
            mAudio->Play();

            if(mAttack == nullptr)
            {
                mAttack = new ZombieAttack();
                mAttack->GetComponent<Transform>()->SetParent(GetComponent<Transform>());

                CreateObject(mAttack, eLayerType::MonsterAttack);
            }
        }
    }

    void Zombie_weak::createBody()
    {
        MonsterBody* head = new MonsterBody();
        head->SetTexture(ResourceManager::Load<Texture>(L"zombie_weak_head", L"..\\Resources\\Texture\\Monster\\zombie_weak\\head.png"));
        mBodies.push_back(head);

        MonsterBody* left_arm = new MonsterBody();
        left_arm->SetTexture(ResourceManager::Load<Texture>(L"zombie_weak_left_arm", L"..\\Resources\\Texture\\Monster\\zombie_weak\\left_arm.png"));
        mBodies.push_back(left_arm);

        MonsterBody* left_foot = new MonsterBody();
        left_foot->SetTexture(ResourceManager::Load<Texture>(L"zombie_weak_left_foot", L"..\\Resources\\Texture\\Monster\\zombie_weak\\left_foot.png"));
        mBodies.push_back(left_foot);

        MonsterBody* left_hand = new MonsterBody();
        left_hand->SetTexture(ResourceManager::Load<Texture>(L"zombie_weak_left_hand", L"..\\Resources\\Texture\\Monster\\zombie_weak\\left_hand.png"));
        mBodies.push_back(left_hand);

        MonsterBody* left_thigh = new MonsterBody();
        left_thigh->SetTexture(ResourceManager::Load<Texture>(L"zombie_weak_left_thigh", L"..\\Resources\\Texture\\Monster\\zombie_weak\\left_thigh.png"));
        mBodies.push_back(left_thigh);

        MonsterBody* lower_torso = new MonsterBody();
        lower_torso->SetTexture(ResourceManager::Load<Texture>(L"zombie_weak_lower_torso", L"..\\Resources\\Texture\\Monster\\zombie_weak\\lower_torso.png"));
        mBodies.push_back(lower_torso);

        MonsterBody* right_arm = new MonsterBody();
        right_arm->SetTexture(ResourceManager::Load<Texture>(L"zombie_weak_right_arm", L"..\\Resources\\Texture\\Monster\\zombie_weak\\right_arm.png"));
        mBodies.push_back(right_arm);

        MonsterBody* right_foot = new MonsterBody();
        right_foot->SetTexture(ResourceManager::Load<Texture>(L"zombie_weak_right_foot", L"..\\Resources\\Texture\\Monster\\zombie_weak\\right_foot.png"));
        mBodies.push_back(right_foot);

        MonsterBody* right_hand = new MonsterBody();
        right_hand->SetTexture(ResourceManager::Load<Texture>(L"zombie_weak_right_hand", L"..\\Resources\\Texture\\Monster\\zombie_weak\\right_hand.png"));
        mBodies.push_back(right_hand);

        MonsterBody* right_thigh = new MonsterBody();
        right_thigh->SetTexture(ResourceManager::Load<Texture>(L"zombie_weak_right_thigh", L"..\\Resources\\Texture\\Monster\\zombie_weak\\right_thigh.png"));
        mBodies.push_back(right_thigh);

        MonsterBody* tail = new MonsterBody();
        tail->SetTexture(ResourceManager::Load<Texture>(L"zombie_weak_tail", L"..\\Resources\\Texture\\Monster\\zombie_weak\\tail.png"));
        mBodies.push_back(tail);

        MonsterBody* upper_torso = new MonsterBody();
        upper_torso->SetTexture(ResourceManager::Load<Texture>(L"zombie_weak_upper_torso", L"..\\Resources\\Texture\\Monster\\zombie_weak\\upper_torso.png"));
        mBodies.push_back(upper_torso);
    }
}