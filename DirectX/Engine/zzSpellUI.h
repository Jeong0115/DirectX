#pragma once

#include "zzUI.h"

namespace zz
{
    class SpellUI : public UI
    {
    public:
        SpellUI();
        virtual ~SpellUI();

        virtual void Initialize();
        virtual void Update();
        virtual void LateUpdate();
        virtual void Render();

        virtual void OnCollisionEnter(GameObject* other)    override;
        virtual void OnCollisionStay(GameObject* other)     override;
        virtual void OnCollisionExit(GameObject* other)     override;

        class Spell* GetSpell() { return mSpell; }
        class TextBox* GetTextBox() { return mTextBox; }

    protected:
        class Spell*    mSpell;
        class TextBox*  mTextBox;
    };
}

