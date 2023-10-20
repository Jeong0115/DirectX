#include "zzOilTrail_UI.h"
#include "zzTransform.h"
#include "zzOilTrail.h"
#include "zzTextBox.h"

namespace zz
{
    OilTrail_UI::OilTrail_UI()
    {
        SetName(L"oil_trail");

        mSpell = new OilTrail();
    }
    OilTrail_UI::~OilTrail_UI()
    {
    }
    void OilTrail_UI::Initialize()
    {
        GetComponent<Transform>()->SetScale(16.f, 16.f, 1.0f);

        SpellUI::Initialize();

        std::wstring textBox =
            L"\n"
            L"  WATER TRAIL\n"
            L"\n"
            L"  Gives a projectile a trail of oil\n"
            L"\n"
            L"  \x01 Type         Proj.modifier\n"
            L"  \x04 Mana darin   10\n";


        mTextBox = new TextBox(textBox, Vector3(205.f * 1.2f, 60.f * 1.2f, 1.0f));
        mTextBox->Initialize();
        mTextBox->GetComponent<Transform>()->SetScale(205.f, 60.f, 1.0f);
    }
    void OilTrail_UI::Update()
    {
        SpellUI::Update();
    }
    void OilTrail_UI::LateUpdate()
    {
        SpellUI::LateUpdate();
    }
    void OilTrail_UI::Render()
    {
        SpellUI::Render();
    }
}