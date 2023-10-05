#include "zzBomb_UI.h"
#include "zzTransform.h"
#include "zzBomb.h"
#include "zzTextBox.h"

namespace zz
{
    Bomb_UI::Bomb_UI()
    {
        SetName(L"bomb_ui");

        mSpell = new Bomb();
    }

    Bomb_UI::~Bomb_UI()
    {
    }

    void Bomb_UI::Initialize()
    {
        GetComponent<Transform>()->SetScale(16.f, 16.f, 1.0f);

        SpellUI::Initialize();


        std::wstring textBox =
            L"\n"
            L"  BOMB\n"
            L"\n"
            L"  Summons a bomb that destroys ground very efficiently\n"
            L"\n"
            L"  \x01 Type         Projectile\n"
            L"  \x04 Mana darin   25\n"
            L"\n"
            L"  \x02 Dmg.Expl     125\n"
            L"  \x02 Expl.Radius  60\n"
            L"  \x06 Speed        60\n"
            L"\n"
            L"  \x03 Cast delay   +1.67s\n";


        mTextBox = new TextBox(textBox, Vector3(280.f * 1.2f, 120.f * 1.2f, 1.0f));
        mTextBox->Initialize();
        mTextBox->GetComponent<Transform>()->SetScale(280.f, 120.f, 1.0f);
    }

    void Bomb_UI::Update()
    {
        SpellUI::Update();
    }

    void Bomb_UI::LateUpdate()
    {
        SpellUI::LateUpdate();
    }

    void Bomb_UI::Render()
    {
        SpellUI::Render();
    }

}