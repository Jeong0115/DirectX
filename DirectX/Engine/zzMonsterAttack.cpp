#include "zzMonsterAttack.h"
#include "zzTime.h"

namespace zz
{
    MonsterAttack::MonsterAttack()
        : mDamage(0.0f)
        , mTime(0.0f)
        , mLimitTime(10.f)
    {
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
            DeleteObject(this, eLayerType::MonsterAttack);
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

    }

    void MonsterAttack::OnCollisionStay(GameObject* other)
    {
    }
    void MonsterAttack::OnCollisionExit(GameObject* other)
    {
    }
}