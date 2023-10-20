#include "zzBloodTrail_UI.h"
#include "zzTransform.h"
#include "zzBloodTrail.h"
#include "zzTextBox.h"

namespace zz
{
    BloodTrail_UI::BloodTrail_UI()
    {
        SetName(L"blood_trail");

        mSpell = new BloodTrail();
    }
    BloodTrail_UI::~BloodTrail_UI()
    {
    }
    void BloodTrail_UI::Initialize()
    {
        GetComponent<Transform>()->SetScale(16.f, 16.f, 1.0f);

        SpellUI::Initialize();

        std::wstring textBox =
            L"\n"
            L"  WATER TRAIL\n"
            L"\n"
            L"  Gives a projectile a trail of blood\n"
            L"\n"
            L"  \x01 Type         Proj.modifier\n"
            L"  \x04 Mana darin   10\n";


        mTextBox = new TextBox(textBox, Vector3(205.f * 1.2f, 60.f * 1.2f, 1.0f));
        mTextBox->Initialize();
        mTextBox->GetComponent<Transform>()->SetScale(205.f, 60.f, 1.0f);
    }
    void BloodTrail_UI::Update()
    {
        SpellUI::Update();
    }
    void BloodTrail_UI::LateUpdate()
    {
        SpellUI::LateUpdate();
    }
    void BloodTrail_UI::Render()
    {
        SpellUI::Render();
    }
}