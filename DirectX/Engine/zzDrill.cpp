#include "zzDrill.h"
#include "zzTransform.h"
#include "zzLuminous_drill.h"
#include "zzTextBox.h"

namespace zz
{
    Drill::Drill()
    {
        SetName(L"luminous_drill");

        mSpell = new Luminous_drill();
    }
    Drill::~Drill()
    {
    }

    void Drill::Initialize()
    {
        GetComponent<Transform>()->SetScale(16.f, 16.f, 1.0f);

        SpellUI::Initialize();

        std::wstring textBox =
            L"\n"
            L"  LUMINOUSE DRILL\n"
            L"\n"
            L"  A pinpointed, short-ranged beam of concentrated light\n"
            L"\n"
            L"  \x01 Type           Projectile\n"
            L"  \x04 Mana darin     10\n"
            L"\n"
            L"  \x02 Damage         10\n"
            L"  \x06 Speed          1400\n"
            L"\n"
            L"  \x03 Cast delay     -0.58s\n"
            L"  \x05 Recharge time  -0.17s\n";


        mTextBox = new TextBox(textBox, Vector3(300.f * 1.2f, 100.f * 1.2f, 1.0f));
        mTextBox->Initialize();
        mTextBox->GetComponent<Transform>()->SetScale(300.f, 100.f, 1.0f);
    }

    void Drill::Update()
    {
        SpellUI::Update();
    }
    void Drill::LateUpdate()
    {
        SpellUI::Update();
    }
    void Drill::Render()
    {
        SpellUI::Update();
    }
}