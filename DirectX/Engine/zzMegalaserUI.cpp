#include "zzMegalaserUI.h"
#include "zzTransform.h"
#include "zzMegalaser.h"
#include "zzTextBox.h"

namespace zz
{
    MegalaserUI::MegalaserUI()
    {
        SetName(L"megalaser_ui");

        mSpell = new Megalaser();
    }
    MegalaserUI::~MegalaserUI()
    {
    }
    void MegalaserUI::Initialize()
    {
        GetComponent<Transform>()->SetScale(16.f, 16.f, 1.0f);

        SpellUI::Initialize();

        std::wstring textBox =
            L"\n"
            L"  INTENSE CONCENTRATED LIGHT\n"
            L"\n"
            L"  A sprectral wand is summoned that casts a huge beam of light\n"
            L"\n"
            L"  \x01 Type         Projectile\n"
            L"  \x04 Mana darin   110\n"
            L"\n"
            L"  \x02 Damage       125\n"
            L"  \x07 Spread       2.9 DEG\n"
            L"  \x06 Speed        1\n"
            L"\n"
            L"  \x03 Cast delay   +1.50s\n";



        mTextBox = new TextBox(textBox, Vector3(350.f * 1.2f, 100.f * 1.2f, 1.0f));
        mTextBox->Initialize();
        mTextBox->GetComponent<Transform>()->SetScale(350.f, 100.f, 1.0f);
    }
    void MegalaserUI::Update()
    {
        SpellUI::Update();
    }
    void MegalaserUI::LateUpdate()
    {
        SpellUI::LateUpdate();
    }
    void MegalaserUI::Render()
    {
        SpellUI::Render();
    }
}