#include "zzWaterTrail_UI.h"
#include "zzTransform.h"
#include "zzWaterTrail.h"
#include "zzTextBox.h"

namespace zz
{
    WaterTrail_UI::WaterTrail_UI()
    {
        SetName(L"water_trail");

        mSpell = new WaterTrail();
    }

    WaterTrail_UI::~WaterTrail_UI()
    {
    }

    void WaterTrail_UI::Initialize()
    {
        GetComponent<Transform>()->SetScale(16.f, 16.f, 1.0f);

        SpellUI::Initialize();

        std::wstring textBox =
            L"\n"
            L"  WATER TRAIL\n"
            L"\n"
            L"  Gives a projectile a trail of water\n"
            L"\n"
            L"  \x01 Type         Proj.modifier\n"
            L"  \x04 Mana darin   10\n";


        mTextBox = new TextBox(textBox, Vector3(205.f * 1.2f, 60.f * 1.2f, 1.0f));
        mTextBox->Initialize();
        mTextBox->GetComponent<Transform>()->SetScale(205.f, 60.f, 1.0f);
    }

    void WaterTrail_UI::Update()
    {
        SpellUI::Update();
    }

    void WaterTrail_UI::LateUpdate()
    {
        SpellUI::LateUpdate();
    }

    void WaterTrail_UI::Render()
    {
        SpellUI::Render();
    }

}