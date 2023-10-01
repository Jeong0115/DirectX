#include "zzSpeedUp_UI.h"
#include "zzTransform.h"
#include "zzSpeedUp.h"
#include "zzTextBox.h"

namespace zz
{
    SpeedUp_UI::SpeedUp_UI()
    {
        SetName(L"speed");

        mSpell = new SpeedUp();
    }

    SpeedUp_UI::~SpeedUp_UI()
    {
    }

    void SpeedUp_UI::Initialize()
    {
        GetComponent<Transform>()->SetScale(16.f, 16.f, 1.0f);

        SpellUI::Initialize();

        std::wstring textBox =
            L"\n"
            L"  SPEED UP\n"
            L"\n"
            L"  Increases the speed at which a projectile files through the air\n"
            L"\n"
            L"  \x01 Type         Proj.modifier\n"
            L"  \x04 Mana darin   3\n"
            L"\n"
            L"  \x06 Proj.Speed   x 2.50\n";


        mTextBox = new TextBox(textBox, Vector3(330.f * 1.2f, 75.f * 1.2f, 1.0f));
        mTextBox->Initialize();
        mTextBox->GetComponent<Transform>()->SetScale(330.f, 75.f, 1.0f);
    }

    void SpeedUp_UI::Update()
    {
        SpellUI::Update();
    }

    void SpeedUp_UI::LateUpdate()
    {
        SpellUI::LateUpdate();
    }

    void SpeedUp_UI::Render()
    {
        SpellUI::Render();
    }
}