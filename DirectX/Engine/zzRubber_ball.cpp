#include "zzRubber_ball.h"
#include "zzTransform.h"
#include "zzBouncingBurst.h"
#include "zzTextBox.h"

namespace zz
{
    Rubber_ball::Rubber_ball()
    {
        SetName(L"rubber_ball_ui");

        mSpell = new BouncingBurst();
    }
    Rubber_ball::~Rubber_ball()
    {
    }
    void Rubber_ball::Initialize()
    {
        GetComponent<Transform>()->SetScale(16.f, 16.f, 1.0f);

        SpellUI::Initialize();

        std::wstring textBox =
            L"\n"
            L"  BOUNCING BURST\n"
            L"\n"
            L"  A very bouncy projectile\n"
            L"\n"
            L"  \x01 Type         Projectile\n"
            L"  \x04 Mana darin   5\n"
            L"\n"
            L"  \x02 Damage       3\n"
            L"  \x07 Spread       0.6 DEG\n"
            L"  \x06 Speed        700\n"
            L"\n"
            L"  \x03 Cast delay   +0.03s\n"
            L"  \x07 Spread       -1 DEG\n";


        mTextBox = new TextBox(textBox, Vector3(180.f * 1.2f, 110.f * 1.2f, 1.0f));
        mTextBox->Initialize();
        mTextBox->GetComponent<Transform>()->SetScale(180.f, 110.f, 1.0f);
    }
    void Rubber_ball::Update()
    {
        SpellUI::Update();
    }
    void Rubber_ball::LateUpdate()
    {
        SpellUI::LateUpdate();
    }
    void Rubber_ball::Render()
    {
        SpellUI::Render();
    }
}