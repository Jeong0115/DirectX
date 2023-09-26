#include "zzWand.h"
#include "zzTime.h"

namespace zz
{
    Wand::Wand()
        : mCapacity(0)
        , mManaMax(0.f)
        , mManaChargeSpeed(0.f)
        , mCurMana(0.f)
        , mCurSpellIndex(0)
        , mCastDelay(0.f)
        , mReChargeTime(0.f)
        , mSpread(0.f)
        , mSpells{}
        , mTip(Vector3::Zero)
        , mInfoBox(nullptr)
        , mInformation{}
    {
    }

    Wand::~Wand()
    {
    }

    void Wand::Initialize()
    {
        GameObject::Initialize();
    }

    void Wand::Update()
    {
        if (mCurMana > mManaMax)
        {
            mCurMana = mManaMax;
        }
        else if (mCurMana < mManaMax)
        {
            mCurMana += static_cast<float>(Time::DeltaTime() * mManaChargeSpeed);
        }
        GameObject::Update();
    }

    void Wand::LateUpdate()
    {
        GameObject::LateUpdate();
    }

    void Wand::Render()
    {
        GameObject::Render();
    }

    void Wand::UseEquipment()
    {
    }

    float Wand::GetEquipmentRate()
    {
        return mCurMana / mManaMax;
    }
}