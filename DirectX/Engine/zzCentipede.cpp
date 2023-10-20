#include "zzCentipede.h"
#include "zzResourceManager.h"
#include "zzAnimator.h"
#include "zzTransform.h"
#include "zzMeshRenderer.h"
#include "zzDetectPlayer.h"
#include "zzAudioSource.h"

#include "zzLimbA.h"
#include "zzLimbB.h"
#include "zzLimbKnee.h"
#include "zzTail1.h"
#include "zzTail2.h"

#include "zzCentipedeParticle.h"
#include "zzOrbPink.h"
#include "zzGrenade.h"
#include "zzOrbBlue.h"
#include "zzHealthPoint.h"
#include "zzCollider.h"
#include "zzBug.h"

#include "zzInput.h"
#include "zzTime.h"
#include "zzRenderer.h"
#include "zzRedTeleport.h"
#include "zzBossHPBar.h"
#include "zzUIManager.h"
#include "zzEventManager.h"
#include "zzRigidBody.h"
#include "zzBox2dCollider.h"
#include "zzPixelWorld.h"

namespace zz
{
    Centipede::Centipede()
        : mState(eCentipedeState::Sleep)
        , mExplosion32Pink(nullptr)
        , mChoiceNextAction(2.0f)
        , mbEnterAction(false)
        , mActionIndex(0)
        , mActionCnt(0)
        , mTime(0.f)
        , mCircleParticle(nullptr)
        , mAttackAudio(nullptr)
        , mAudio(nullptr)
        , mHitFlashTime(0.0f)
        , mInitialPos(Vector3::Zero)
        , mRigid(nullptr)
        , mTargetPos(Vector3::Zero)
        , mVelocity(Vector3::Zero)
        , mHP(nullptr)
        , mbDead(false)
        , mTail1s{}
        , mTail2s{}
    {
    }
    Centipede::~Centipede()
    {
    }

    void Centipede::Initialize()
    {
        GetComponent<Transform>()->SetScale(96.f, 96.f, 1.0f);
        mInitialPos = GetComponent<Transform>()->GetPosition();

        mRigid = AddComponent<RigidBody>();
        mRigid->SetGravity(0.0f);

        MeshRenderer* mesh = AddComponent<MeshRenderer>();
       
        mesh->SetMaterial(ResourceManager::Find<Material>(L"m_SpriteAnimation"));
        mesh->SetMesh(ResourceManager::Find<Mesh>(L"RectMesh"));
        
        std::shared_ptr<Texture> texture = ResourceManager::Find<Texture>(L"body");

        mAnimator = AddComponent<Animator>();
        mAnimator->Create(L"body_stand", texture, Vector2(0.0f, 0.0f), Vector2(96.0f, 96.0f), 4, Vector2::Zero, 0.12f);
        mAnimator->Create(L"body_open", texture, Vector2(0.0f, 96.0f), Vector2(96.0f, 96.0f), 7, Vector2::Zero, 0.09f);
        mAnimator->Create(L"body_close", texture, Vector2(0.0f, 288.0f), Vector2(96.0f, 96.0f), 7, Vector2::Zero, 0.09f);
        mAnimator->Create(L"body_opened", texture, Vector2(0.0f, 384.0f), Vector2(96.0f, 96.0f), 10, Vector2::Zero, 0.11f);
        mAnimator->Create(L"body_aggro", texture, Vector2(0.0f, 480.0f), Vector2(96.0f, 96.0f), 7, Vector2::Zero, 0.13f);

        mAnimator->PlayAnimation(L"body_stand", true);
        mAnimator->EndEvent(L"body_open") = [this]() { NextAnimation(); };

        mDetectPlayer = AddComponent<DetectPlayer>();
        AddComponent<Collider>()->SetScale(60.f, 60.f, 1.0f);
        makeBody();
        
        mExplosion32Pink = new ExplosionEffect();
        std::shared_ptr<Texture> eff_texture = ResourceManager::Find<Texture>(L"explosion_032_pink");
        Animator* eff_animator = new Animator();
        eff_animator->Create(L"explosion_032_pink_idle", eff_texture, Vector2(0.0f, 0.0f), Vector2(32.0f, 32.0f), 8, Vector2::Zero, 0.021f);
        eff_animator->PlayAnimation(L"explosion_032_pink_idle", false);
        mExplosion32Pink->SetAnimator(eff_animator, L"explosion_032_pink_idle");
        mExplosion32Pink->SetScale(32.f, 32.f, 1.0f);
        mExplosion32Pink->KeepObjectEndAnimatiom(true);

        CreateObject(mExplosion32Pink, eLayerType::Effect);
        
        mCircleParticle = new CentipedeParticle();
        CreateObject(mCircleParticle, eLayerType::Monster);

        mAttackAudio = AddComponent<AudioSource>();
        mAudio = AddComponent<AudioSource>();

        mHealthPoint = AddComponent<HealthPoint>();
        mHealthPoint->SetMaxHP(500.f);
        mHealthPoint->SetHpZeroEvent([this]() { DeadEvent(); });
        mHealthPoint->SetHitEvent([this]() { HitEvent(); });

        GameObject::Initialize();
    }

    void Centipede::Update()
    {
        if(!mbDead)
        {
            mTime += (float)Time::DeltaTime();

            if (Input::GetKeyDown(eKeyCode::M))
            {
                Awake();
            }
            if (mState != eCentipedeState::Sleep)
            {
                mMoveTime -= (float)Time::DeltaTime();

                if (mMoveTime < 0.f)
                {
                    Move();
                }
                mRigid->SetVelocity(mVelocity);
            }

            switch (mState)
            {
            case eCentipedeState::Sleep: break;
            case eCentipedeState::Wait: wait(); break;
            case eCentipedeState::ChoiceNextAttack: choice(); break;
            case eCentipedeState::CircleShoot: circleShoot(); break;
            case eCentipedeState::FirePillar: firePillar(); break;
            case eCentipedeState::CleanMaterial: cleanMaterial(); break;
            case eCentipedeState::OrbBlue: orbBlue(); break;
            case eCentipedeState::BugBlue: bugBlue(); break;
            default: break;
            }
        }

        GameObject::Update();
    }

    void Centipede::LateUpdate()
    {
        GameObject::LateUpdate();
    }

    void Centipede::Render()
    {
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

        flash = {};
        color->SetBufferData(&flash);
        color->BindConstantBuffer(eShaderStage::PS);
        GameObject::Render();
    }

    void Centipede::Move()
    {
        Vector3 pos = GetComponent<Transform>()->GetPosition();

        mTargetPos.x = mInitialPos.x + randf(200.f) - 100.f;
        mTargetPos.y = mInitialPos.y + randf(200.f) - 100.f;


        mMoveTime = randf(2.0f) + 1.0f;

        mVelocity.x = (mTargetPos.x - pos.x) / mMoveTime;
        mVelocity.y = (mTargetPos.y - pos.y) / mMoveTime;
    }

    void Centipede::DeadEvent()
    {
        if (mbDead) return;
        mAnimator->PlayAnimation(L"body_aggro", true);
        EvenetData data;
        data.eventType = eEvent::Boss_Health;
        data.health = mHealthPoint->GetCurHP();
        EventManager::RegisterEvent(data);

        mAudio->SetClip(ResourceManager::LoadAudioClip(L"centipede_death_vocal_02", L"..\\Resources\\Audio\\Enemy\\centipede_death_vocal_02.wav")); 

        mAudio->SetLoop(false);
        mAudio->Play();

        Vector3 pos = GetComponent<Transform>()->GetPosition();
        CreateObject(new RedTeleport(), eLayerType::Object, pos.x, pos.y, pos.z);
        mHP->SetDead();

        for (auto limbA : limbAs)
        {
            limbA->GetComponent<Transform>()->SetParent(nullptr);
            limbA->GetComponent<Transform>()->SetPosition(pos);
            limbA->SetMoveState(LimbA::eMoveState::Sleep);
            Box2dCollider* box = limbA->AddComponent<Box2dCollider>();
            box->Initialize();
            box->Create(pos, limbA->GetComponent<Transform>()->GetScale());
        }
        int i = 10;
        for (auto tail : mTail1s)
        {
            tail->GetComponent<Transform>()->SetParent(nullptr);
            tail->GetComponent<Transform>()->SetPosition(pos.x, pos.y + i, pos.z);
            tail->SetDead();
            Box2dCollider* box = tail->AddComponent<Box2dCollider>();
            box->Initialize();
            box->Create(Vector3(pos.x, pos.y + i++, pos.z), tail->GetComponent<Transform>()->GetScale());
        }
        for (auto tail : mTail2s)
        {
            tail->GetComponent<Transform>()->SetParent(nullptr);
            tail->GetComponent<Transform>()->SetPosition(pos.x, pos.y + i, pos.z);
            tail->SetDead();
            Box2dCollider* box = tail->AddComponent<Box2dCollider>();
            box->Initialize();
            box->Create(Vector3(pos.x, pos.y + i++, pos.z), tail->GetComponent<Transform>()->GetScale());
        }

        Box2dCollider* box = AddComponent<Box2dCollider>();
        box->Initialize();
        box->Create(pos, GetComponent<Transform>()->GetScale());

        mbDead = true;
    }

    void Centipede::HitEvent()
    {
        if (mbDead) return;


        mHitFlashTime = 0.15f;

        switch (randi(1))
        {
        case 0: mAudio->SetClip(ResourceManager::LoadAudioClip(L"centipede_damage_ver2_03", L"..\\Resources\\Audio\\Enemy\\centipede_damage_ver2_03.wav")); break;
        case 1: mAudio->SetClip(ResourceManager::LoadAudioClip(L"centipede_damage_ver2_02", L"..\\Resources\\Audio\\Enemy\\centipede_damage_ver2_02.wav")); break;
        }

        mAudio->SetLoop(false);
        mAudio->Play();

        EvenetData data;
        data.eventType = eEvent::Boss_Health;
        data.health = mHealthPoint->GetCurHP();

        EventManager::RegisterEvent(data);
    }

    void Centipede::Awake()
    {
        mAnimator->PlayAnimation(L"body_open", false);

        mHP = new BossHPBar();
        UIManager::AddUIObject(mHP, eUIType::HUD);
        mHP->Initialize();
        
        for (auto limbA : limbAs)
        {
            limbA->Awake();
        }

        mAudio->SetClip(ResourceManager::LoadAudioClip(L"centipede_pickup_sampo_vocal", L"..\\Resources\\Audio\\Enemy\\centipede_pickup_sampo_vocal.wav"));
        mAudio->SetLoop(false);
        mAudio->Play();

        mState = eCentipedeState::Wait;
    }

    void Centipede::NextAnimation()
    {
        mAnimator->PlayAnimation(L"body_opened", true);
    }
    
    void Centipede::makeBody()
    {
        for (int i = 0; i < 10; i++)
        {
            limbAs[i] = new LimbA(this);
            CreateObject(limbAs[i], eLayerType::Monster);
            Transform* aTr = limbAs[i]->GetComponent<Transform>();
            aTr->SetPosition(40.f, 0.f, 0.22f);
            aTr->SetParent(GetComponent<Transform>());
            aTr->SetRevolution(Vector3(-40.f, 0.f, 0.0f));
            aTr->SetRotationZ(2 * PI * i / 10.f);
        }
        
        Tail2* tail2_1 = new Tail2();
        CreateObject(tail2_1, eLayerType::Monster);
        Transform* tail2_1_tr = tail2_1->GetComponent<Transform>();
        tail2_1_tr->SetParent(GetComponent<Transform>());
        tail2_1_tr->SetRevolution(Vector3(-10.f, 0.f, 0.0f));

        Tail2* tail2_2 = new Tail2();
        CreateObject(tail2_2, eLayerType::Monster);
        Transform* tail2_2_tr = tail2_2->GetComponent<Transform>();
        tail2_2_tr->SetParent(tail2_1_tr);
        
        mTail2s.push_back(tail2_1);
        mTail2s.push_back(tail2_2);

        Tail1* tail1_1 = new Tail1();
        CreateObject(tail1_1, eLayerType::Monster);
        Transform* tail1_1_tr = tail1_1->GetComponent<Transform>();
        tail1_1_tr->SetParent(tail2_2_tr);
        
        Tail1* tail1_2 = new Tail1();
        CreateObject(tail1_2, eLayerType::Monster);
        Transform* tail1_2_tr = tail1_2->GetComponent<Transform>();
        tail1_2_tr->SetParent(tail1_1_tr);
        
        Tail1* tail1_3 = new Tail1();
        CreateObject(tail1_3, eLayerType::Monster);
        Transform* tail1_3_tr = tail1_3->GetComponent<Transform>();
        tail1_3_tr->SetParent(tail1_2_tr);
        
        Tail1* tail1_4 = new Tail1();
        CreateObject(tail1_4, eLayerType::Monster);
        Transform* tail1_4_tr = tail1_4->GetComponent<Transform>();
        tail1_4_tr->SetParent(tail1_3_tr);
        
        Tail1* tail1_5 = new Tail1();
        CreateObject(tail1_5, eLayerType::Monster);
        Transform* tail1_5_tr = tail1_5->GetComponent<Transform>();
        tail1_5_tr->SetParent(tail1_4_tr);

        mTail1s.push_back(tail1_1);
        mTail1s.push_back(tail1_2);
        mTail1s.push_back(tail1_3);
        mTail1s.push_back(tail1_4);
        mTail1s.push_back(tail1_5);
    }

    void Centipede::wait()
    {
        mTime += random() * (float)Time::DeltaTime();
        if (mTime >= 4.5f)
        {
            mState = eCentipedeState::ChoiceNextAttack;
        }
        mActionCnt = 0;
    }

    void Centipede::choice()
    {
        int random = randi(4);

        switch (random)
        {
        case 0: mState = eCentipedeState::CircleShoot; break;
        case 1: mState = eCentipedeState::FirePillar; break;
        case 2: mState = eCentipedeState::CleanMaterial; break;
        case 3: mState = eCentipedeState::OrbBlue; break;
        case 4: mState = eCentipedeState::BugBlue; break;
        }
        mTime = 0.0f;
    }

    void Centipede::circleShoot()
    {
        if (mTime >= 0.3f)
        {
            Vector3 pos = GetComponent<Transform>()->GetPosition();

            mExplosion32Pink->SetPosition(pos.x, pos.y, pos.z - 0.06f);
            mExplosion32Pink->Play();

            switch (randi(2))
            {
            case 0: mAttackAudio->SetClip(ResourceManager::LoadAudioClip(L"summon_pebble_01", L"..\\Resources\\Audio\\Enemy\\summon_pebble_01.wav"));
            case 1: mAttackAudio->SetClip(ResourceManager::LoadAudioClip(L"summon_pebble_02", L"..\\Resources\\Audio\\Enemy\\summon_pebble_02.wav"));
            case 2: mAttackAudio->SetClip(ResourceManager::LoadAudioClip(L"summon_pebble_03", L"..\\Resources\\Audio\\Enemy\\summon_pebble_03.wav"));
            }
            
            mAttackAudio->SetLoop(false);
            mAttackAudio->Play();

            for (int i = 0; i < 6; i++)
            {
                OrbPink* orb = new OrbPink();
                orb->SetPosition(pos.x, pos.y, pos.z - 0.05f);
                orb->SetAngle(PI * 2. / 6 * i + mActionCnt * PI / 12.);

                CreateObject(orb, eLayerType::MonsterAttack);
            }
            mTime = 0.f;
            mActionCnt++;
        }
        if (mActionCnt >= 8)
        {
            mTime = 0.f;
            mState = eCentipedeState::Wait;
        }
    }

    void Centipede::firePillar()
    {
        Vector3 pos = GetComponent<Transform>()->GetPosition();

        mAttackAudio->SetClip(ResourceManager::LoadAudioClip(L"meteor_create_01", L"..\\Resources\\Audio\\Enemy\\meteor_create_01.wav"));
        mAttackAudio->SetLoop(false);
        mAttackAudio->Play();

        for (int i = 0; i < 7; i++)
        {
            Grenade* grenade = new Grenade();
            grenade->SetPosition(pos.x, pos.y, pos.z - 0.05f);
            grenade->SetAngle(-PI / 6 * i);

            CreateObject(grenade, eLayerType::MonsterAttack);
        }
        mTime = 0.f;
        mState = eCentipedeState::Wait;
    }

    void Centipede::bugBlue()
    {
        if (mTime >= 0.3f)
        {
            Vector3 pos = GetComponent<Transform>()->GetPosition();
            CreateObject(new Bug(), eLayerType::MonsterAttack, pos.x, pos.y, FRONT_PIXEL_WORLD_Z);
            mActionCnt++;
            mTime = 0;

            if (mActionCnt >= 2)
            {
                mTime = 0.f;
                mState = eCentipedeState::Wait;
            }
        }

    }

    void Centipede::orbBlue()
    {
        Vector3 pos = GetComponent<Transform>()->GetPosition();

        int rand = randi(1);
        if (rand == 0)
        {
            if(mActionCnt == 0)
            {
                for (int i = 0; i < 4; i++)
                {
                    OrbBlue* orb = new OrbBlue(rand);
                    orb->SetPosition(pos.x, pos.y, pos.z - 0.05f);
                    orb->SetAngle(PI * 2 * i / 4);

                    CreateObject(orb, eLayerType::MonsterAttack);
                }
                mState = eCentipedeState::Wait;
                mTime = 0.f;
            }
        }
        else
        {
            if (mActionCnt == 0) mActionCnt++;
        }
        
        if (mTime >= 0.3f)
        {
            Vector3 direction = DetectPlayer::PlayerPos - pos;
            direction.Normalize();

            OrbBlue* orb = new OrbBlue(1);
            orb->SetPosition(pos.x, pos.y, pos.z - 0.05f);
            orb->SetDirection(direction);

            CreateObject(orb, eLayerType::MonsterAttack);

            mTime = 0.f;
            mActionCnt++;

            if (mActionCnt >= 3)
            {
                mTime = 0.f;
                mState = eCentipedeState::Wait;
            }
        }
   
    }

    void Centipede::cleanMaterial()
    {
        Vector3 pos = GetComponent<Transform>()->GetPosition();

        mCircleParticle->GetComponent<Transform>()->SetPosition(pos);
        mCircleParticle->Play();

        mTime = 0.f;
        mState = eCentipedeState::Wait;
    }
}