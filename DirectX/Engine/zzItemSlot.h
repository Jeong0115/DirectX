#pragma once

#include "zzUI.h"
#include "zzEquipment.h"
#include "zzItemTexture.h"
#include "zzTextBox.h"

namespace zz
{
    class ItemSlot : public UI
    {
    public:
        ItemSlot(eUIType type);
        virtual ~ItemSlot();

        virtual void Initialize()   override;
        virtual void Update()       override;
        virtual void LateUpdate()   override;
        virtual void Render()       override;

        virtual void OnCollisionEnter(GameObject* other)    override;
        virtual void OnCollisionStay(GameObject* other)     override;
        virtual void OnCollisionExit(GameObject* other)     override;

        void SetSlotIndex(UINT index)   { mSlotIndex = index; }
        UINT GetSlotIndex()             { return mSlotIndex; }

        Equipment* GetItem()            { return mItem; }
        ItemTexture* GetItemTexture()   { return mItemTexture; }
        void SetItem(Equipment* item, ItemTexture* tex);

    private:
        Equipment*      mItem;
        ItemTexture*    mItemTexture;
        TextBox*        mTextBox;

        UINT            mSlotIndex;
        bool            mbMouseOn;
    };
}
