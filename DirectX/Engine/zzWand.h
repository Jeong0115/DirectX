#pragma once

#include "zzEquipment.h"
#include "zzInfoBox.h"

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

        virtual void UseEquipment();

        UINT GetCapacity() { return mCapacity; }

        void SetInfoBox(InfoBox* infoBox) { mInfoBox = infoBox; }
        InfoBox* GetInfoBox() { return mInfoBox; }

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

    private:
        InfoBox* mInfoBox;
    };
}
