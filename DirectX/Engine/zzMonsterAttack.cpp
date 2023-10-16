#include "zzMonsterAttack.h"
#include "zzTime.h"
#include "zzHealthPoint.h"
#include "zzTransform.h"
#include "zzCollider.h"

namespace zz
{
    MonsterAttack::MonsterAttack()
        : mDamage(0.0f)
        , mTime(0.0f)
        , mLimitTime(10.f)
    {
        mCollider = AddComponent<Collider>();
    }
    MonsterAttack::~MonsterAttack()
    {
    }

    void MonsterAttack::Initialize()
    {
        GameObject::Initialize();
    }

    void MonsterAttack::Update()
    {
        mTime += (float)Time::DeltaTime();

        if (mTime >= mLimitTime)
        {
            dead();
        }

        GameObject::Update();
    }

    void MonsterAttack::LateUpdate()
    {
        GameObject::LateUpdate();
    }

    void MonsterAttack::Render()
    {
        GameObject::Render();
    }

    void MonsterAttack::OnCollisionEnter(GameObject* other)
    {
        if (other->GetLayerType() == eLayerType::Player)
        {
            other->GetComponent<HealthPoint>()->Hit(mDamage);
            dead();
        }
    }

    void MonsterAttack::OnCollisionStay(GameObject* other)
    {
    }
    void MonsterAttack::OnCollisionExit(GameObject* other)
    {
    }

    void MonsterAttack::SetPosition(Vector3 pos)
    {
        GetComponent<Transform>()->SetPosition(pos);
    }
    void MonsterAttack::SetPosition(float x, float y, float z)
    {
        GetComponent<Transform>()->SetPosition(x, y, z);
    }
    void MonsterAttack::dead()
    {
    }
}