#include "zzHealthPoint.h"

namespace zz
{
    HealthPoint::HealthPoint()
        : Component(eComponentType::HealPoint)
        , mMaxHP(0.0f)
        , mCurHP(0.0f)
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
}