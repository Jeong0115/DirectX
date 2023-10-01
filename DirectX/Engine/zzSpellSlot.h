#pragma once

#include "zzUI.h"
#include "zzSpellUI.h"
#include "zzItemTexture.h"
#include "zzTextBox.h"

namespace zz
{
    class SpellSlot : public UI
    {
    public:
        SpellSlot(eUIType type);
        virtual ~SpellSlot();

        virtual void Initialize()   override;
        virtual void Update()       override;
        virtual void LateUpdate()   override;
        virtual void Render()       override;

        virtual void OnCollisionEnter(GameObject* other)    override;
        virtual void OnCollisionStay(GameObject* other)     override;
        virtual void OnCollisionExit(GameObject* other)     override;

        void SetSlotIndex(UINT index) { mSlotIndex = index; }
        UINT GetSlotIndex() { return mSlotIndex; }

        SpellUI* GetSpell() { return mSpell; }
        ItemTexture* GetItemTexture() { return mItemTexture; }
        void SetSpell(SpellUI* item, ItemTexture* tex);

        bool IsInWand() { return mbSlotInWand; }
        void SetInWand(bool isInWand) { mbSlotInWand = isInWand; }

        void SetOwnerWand(class Wand* wand);

    private:
        SpellUI*        mSpell;
        ItemTexture*    mItemTexture;
        TextBox*        mTextBox;
        UINT            mSlotIndex;
        bool            mbSlotInWand;

        class Wand*     mOwnerWand;
        bool            mbMouseOn;
    };
}

