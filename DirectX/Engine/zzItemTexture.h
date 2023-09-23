#pragma once

#include "zzUI.h"

namespace zz
{
    class Texture;
    class SpellSlot;
    enum class eTextureType
    {
        Wand,
        Item,
        Spell
    };

    class ItemTexture : public UI
    {
    public:
        ItemTexture(eUIType type);
        virtual ~ItemTexture();

        virtual void Initialize()   override;
        virtual void Update()       override;
        virtual void LateUpdate()   override;
        virtual void Render()       override;

        void CreateItemTexture(std::wstring material_name, eTextureType type, Vector3 pos, Vector3 scale);
        void MoveSlot(Vector3 pos);
        void MoveOriginSlot();

        void SetSlotIndex(UINT index) { mSlotIndex = index; }
        UINT GetSlotIndex() { return mSlotIndex; }

        virtual void OnCollisionEnter(GameObject* other)    override;
        virtual void OnCollisionStay(GameObject* other)     override;
        virtual void OnCollisionExit(GameObject* other)     override;

        void SetTexture(std::shared_ptr<Texture> tex) { mTex = tex; }
        eTextureType GetTextureType() { return mTextureType; }
        SpellSlot* GetOwner() { return mOwner; }
        void SetOwner(SpellSlot* owner) { mOwner = owner; }

    private:
        std::shared_ptr<Texture>    mTex;
        Vector3                     mSlotPos;
        UINT                        mSlotIndex;
        SpellSlot*                  mOwner;
        eTextureType                mTextureType;
        bool                        mbMove;
    };
}

