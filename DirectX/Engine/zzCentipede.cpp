#include "zzCentipede.h"
#include "zzResourceManager.h"
#include "zzAnimator.h"
#include "zzTransform.h"
#include "zzMeshRenderer.h"

#include "zzLimbA.h"
#include "zzLimbB.h"
#include "zzLimbKnee.h"
#include "zzTail1.h"
#include "zzTail2.h"

#include "zzCentipedeParticle.h"
#include "zzOrbPink.h"
#include "zzGrenade.h"

#include "zzInput.h"
#include "zzTime.h"

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
    {
    }
    Centipede::~Centipede()
    {
    }

    void Centipede::Initialize()
    {
        GetComponent<Transform>()->SetScale(96.f, 96.f, 1.0f);

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
        CreateObject(mCircleParticle, eLayerType::Effect);

        GameObject::Initialize();
    }

    void Centipede::Update()
    {
        Vector3 pos = GetComponent<Transform>()->GetPosition();

        if (Input::GetKey(eKeyCode::W))
        {
            pos.y += 50.f * (float)Time::DeltaTime();
        }
        if (Input::GetKey(eKeyCode::S))
        {
            pos.y -= 50.f * (float)Time::DeltaTime();
        }
        if (Input::GetKey(eKeyCode::A))
        {
            pos.x -= 50.f * (float)Time::DeltaTime();
        }
        if (Input::GetKey(eKeyCode::D))
        {
            pos.x += 50.f * (float)Time::DeltaTime();
        }

        if (Input::GetKeyDown(eKeyCode::C))
        {
            mState = eCentipedeState::CircleShoot;
        }
        else if (Input::GetKeyDown(eKeyCode::F))
        {
            mState = eCentipedeState::FirePillar;
        }
        else if (Input::GetKeyDown(eKeyCode::Q))
        {
            mCircleParticle->GetComponent<Transform>()->SetPosition(pos);
            mCircleParticle->Play();
        }
        //GetComponent<Transform>()->SetPosition(pos);


        switch (mState)
        {
        case eCentipedeState::Sleep: break;
        case eCentipedeState::ChoiceNextAttack: battle(); break;
        case eCentipedeState::CircleShoot: circleShoot(); break;
        case eCentipedeState::FirePillar: firePillar(); break;
        default: break;
        }

        GameObject::Update();
    }

    void Centipede::LateUpdate()
    {
        GameObject::LateUpdate();
    }

    void Centipede::Render()
    {
        GameObject::Render();
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
    }

    void Centipede::battle()
    {
        mState = eCentipedeState::ChoiceNextAttack;
        mActionCnt = 0;
        mTime = 0.f;
    }

    void Centipede::circleShoot()
    {
        mTime += (float)Time::DeltaTime();

        if (mTime >= 0.3f)
        {
            Vector3 pos = GetComponent<Transform>()->GetPosition();

            mExplosion32Pink->SetPosition(pos.x, pos.y, pos.z - 0.06f);
            mExplosion32Pink->Play();

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
    }

    void Centipede::firePillar()
    {
        Vector3 pos = GetComponent<Transform>()->GetPosition();

        for (int i = 0; i < 7; i++)
        {
            Grenade* grenade = new Grenade();
            grenade->SetPosition(pos.x, pos.y, pos.z - 0.05f);
            grenade->SetAngle(-PI / 6 * i);

            CreateObject(grenade, eLayerType::MonsterAttack);
        }
        mState = eCentipedeState::Sleep;
    }
}