#pragma once

#include "zzEquipment.h"

namespace zz
{
    class Wand : public Equipment
    {
    public:
        Wand();
        virtual ~Wand();

        virtual void Initialize();
        virtual void Update();
        virtual void LateUpdate();
        virtual void Render();

        UINT GetCapacity() { return mCapacity; }

        void SetSpell(GameObject* spell, UINT index) { mSpells[index] = spell; }

    protected:
        UINT    mCapacity;
        UINT    mManaMax;
        UINT    mManaChargeSpeed;
        UINT    mCurMana;

        float   mCastDelay;
        float   mReChargeTime;
        float   mSpread;

        Vector3 mTip;

        std::vector<GameObject*> mSpells;

        
    };
}
