#include "zzWand.h"

namespace zz
{
    Wand::Wand()
        : mCapacity(0)
        , mManaMax(0)
        , mManaChargeSpeed(0)
        , mCurMana(0)
        , mCastDelay(0.f)
        , mReChargeTime(0.f)
        , mSpread(0.f)
        , mSpells{}
        , mTip(Vector3::Zero)
        , mInfoBox(nullptr)
    {
    }

    Wand::~Wand()
    {
    }
    void Wand::Initialize()
    {
    }
    void Wand::Update()
    {
    }
    void Wand::LateUpdate()
    {
    }
    void Wand::Render()
    {
    }
    void Wand::UseEquipment()
    {
    }
}