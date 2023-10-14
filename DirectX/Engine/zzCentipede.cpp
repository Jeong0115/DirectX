#include "zzCentipede.h"
#include "zzResourceManager.h"
#include "zzAnimator.h"
#include "zzTransform.h"
#include "zzMeshRenderer.h"
#include "zzDetectPlayer.h"

#include "zzLimbA.h"
#include "zzLimbB.h"
#include "zzLimbKnee.h"
#include "zzTail1.h"
#include "zzTail2.h"

#include "zzCentipedeParticle.h"
#include "zzOrbPink.h"
#include "zzGrenade.h"
#include "zzOrbBlue.h"

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

        mDetectPlayer = AddComponent<DetectPlayer>();

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
        mTime += (float)Time::DeltaTime();

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

        static int i = 0;

        if (Input::GetKeyDown(eKeyCode::O))
        {
            limbAs[i++]->SetMoveState(LimbA::eMoveState::Bend);
            if (i > 9) i = 0;
        }

        if (Input::GetKeyDown(eKeyCode::C))
        {
            mState = eCentipedeState::CircleShoot;
            mTime = 0.f;
        }
        else if (Input::GetKeyDown(eKeyCode::F))
        {
            mState = eCentipedeState::FirePillar;
            mTime = 0.f;
        }
        else if (Input::GetKeyDown(eKeyCode::Q))
        {
            mCircleParticle->GetComponent<Transform>()->SetPosition(pos);
            mCircleParticle->Play();
            mTime = 0.f;
        }
        else if (Input::GetKeyDown(eKeyCode::B))
        {
            mState = eCentipedeState::OrbBlue;
            mTime = 0.f;
        }
        GetComponent<Transform>()->SetPosition(pos);


        switch (mState)
        {
        case eCentipedeState::Sleep: break;
        case eCentipedeState::Wait: wait(); break;
        case eCentipedeState::ChoiceNextAttack: choice(); break;
        case eCentipedeState::CircleShoot: circleShoot(); break;
        case eCentipedeState::FirePillar: firePillar(); break;
        case eCentipedeState::CleanMaterial: cleanMaterial(); break;
        case eCentipedeState::OrbBlue: orbBlue(); break;
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

    void Centipede::wait()
    {
        mTime += rand() * 2.0f;
        if (mTime >= 5.0f)
        {
           // mState = eCentipedeState::ChoiceNextAttack;
        }
        mActionCnt = 0;
    }

    void Centipede::choice()
    {
        int random = randi(3);

        switch (random)
        {
        case 0: mState = eCentipedeState::CircleShoot; break;
        case 1: mState = eCentipedeState::FirePillar; break;
        case 2: mState = eCentipedeState::CleanMaterial; break;
        case 3: mState = eCentipedeState::OrbBlue; break;
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
        if (mActionCnt >= 6)
        {
            mState = eCentipedeState::Wait;
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
        mState = eCentipedeState::Wait;
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
                mState = eCentipedeState::Wait;
            }
        }
   
    }

    void Centipede::cleanMaterial()
    {
        Vector3 pos = GetComponent<Transform>()->GetPosition();

        mCircleParticle->GetComponent<Transform>()->SetPosition(pos);
        mCircleParticle->Play();

        mState = eCentipedeState::Wait;
    }
}