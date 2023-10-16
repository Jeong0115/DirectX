#include "zzHealthPoint.h"
#include "zzWrite.h"
#include "zzTextObject.h"
#include "zzTransform.h"

namespace zz
{
    HealthPoint::HealthPoint()
        : Component(eComponentType::HealPoint)
        , mMaxHP(0.0f)
        , mCurHP(0.0f)
        , mbDead(false)
        , mDeadEvent{}
        , mHitEvent{}
    {
    }

    HealthPoint::~HealthPoint()
    {
    }

    void HealthPoint::Initialize()
    {
    }

    void HealthPoint::Update()
    {
    }

    void HealthPoint::LateUpdate()
    {
    }

    void HealthPoint::Render()
    {
    }

    void HealthPoint::Hit(float damage)
    {
        mCurHP -= damage;

        Vector3 pos = GetOwner()->GetComponent<Transform>()->GetWorldPosition();

        TextObject* text = new TextObject();
        text->WriteDamage(std::to_wstring((int)damage));
        text->SetLifeTime(1.0f);
        text->SetSpeed(40.f);

        CreateObject(text, eLayerType::UI, pos.x, pos.y, pos.z - 0.2f);

        if (mCurHP <= 0 && !mbDead)
        {
            mDeadEvent();
        }
        else if (!mbDead)
        {
            mHitEvent();
        }
    }
}