#pragma once

#include "zzEquipment.h"
#include "zzInfoBox.h"
#include "zzSpell.h"

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

        int GetCapacity() { return mCapacity; }

        void SetInfoBox(InfoBox* infoBox) { mInfoBox = infoBox; }
        InfoBox* GetInfoBox() { return mInfoBox; }

        void SetSpell(Spell* spell, int index) { mSpells[index] = spell; }

    protected:
        int     mCapacity;
        int     mCurSpellIndex;

        float   mManaMax;
        float   mManaChargeSpeed;
        float   mCurMana;
        float   mCastDelay;
        float   mReChargeTime;
        float   mSpread;

        Vector3 mTip;

        std::vector<Spell*> mSpells;

    private:
        InfoBox* mInfoBox;
    };
}
