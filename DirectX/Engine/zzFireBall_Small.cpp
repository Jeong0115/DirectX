#include "zzFireBall_Small.h"
#include "zzHealthPoint.h"
#include "zzRigidBody.h"
#include "zzMeshRenderer.h"
#include "zzResourceManager.h"
#include "zzTexture.h"
#include "zzAnimator.h"
#include "zzPixelCollider_Lite.h"
#include "zzTime.h"
#include "zzTransform.h"

namespace zz
{
    FireBall_Small::FireBall_Small()
        : mRigid(nullptr)
        , mDirection(Vector3::Zero)
    {
        mDamage = 20.f;
        mLimitTime = 2.0f;
        mSpeed = 100.f;
    }
    FireBall_Small::~FireBall_Small()
    {
    }

    void FireBall_Small::Initialize()
    {
        std::shared_ptr<Texture> texture = ResourceManager::Load<Texture>(L"fireball_small", L"..\\Resources\\Texture\\Monster\\fireball_small.png");

        MeshRenderer* mesh = AddComponent<MeshRenderer>();
        mesh->SetMaterial(ResourceManager::Find<Material>(L"m_SpriteAnimation"));
        mesh->SetMesh(ResourceManager::Find<Mesh>(L"RectMesh"));

        Animator* animator = AddComponent<Animator>();

        animator->Create(L"fireball_small_play", texture, Vector2(0.0f, 0.0f), Vector2(4.0f, 4.0f), 4, Vector2::Zero, 0.02f);
        animator->PlayAnimation(L"fireball_small_play", true);

        mRigid = AddComponent<RigidBody>();
        mRigid->SetGround(true);
        mRigid->SetStartVelocity(mSpeed, mDirection);

        AddComponent<PixelCollider_Lite>()->SetCollisionEvent([this](Element& element) { OnCollision(element); });

        MonsterAttack::Initialize();
    }

    void FireBall_Small::Update()
    {
        MonsterAttack::Update();
    }

    void FireBall_Small::LateUpdate()
    {
        MonsterAttack::LateUpdate();
    }

    void FireBall_Small::Render()
    {
        MonsterAttack::Render();
    }

    void FireBall_Small::OnCollisionEnter(GameObject* other)
    {
        if (other->GetLayerType() == eLayerType::Player)
        {
            other->GetComponent<HealthPoint>()->ChangeCurHP(-mDamage);

            DeleteObject(this, eLayerType::MonsterAttack);
        }
    }
    void FireBall_Small::OnCollisionStay(GameObject* other)
    {
    }
    void FireBall_Small::OnCollisionExit(GameObject* other)
    {
    }

    void FireBall_Small::SetDetectPos(Vector3 pos)
    {
        Vector3 myPos = GetComponent<Transform>()->GetPosition();
        pos -= myPos;

        pos.Normalize(mDirection);
    }

    void FireBall_Small::OnCollision(Element& element)
    {
        if (element.Type == eElementType::SOLID)
        {
            DeleteObject(this, eLayerType::MonsterAttack);
        }
        else if (element.Type == eElementType::LIQUID)
        {
            mRigid->ApplyResistance(pow((0.0001f), (float)Time::DeltaTime()));
        }
    }
}