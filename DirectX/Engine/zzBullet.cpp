#include "zzBullet.h"
#include "zzTransform.h"
#include "zzMagicArrow.h"
#include "zzTextBox.h"

namespace zz
{
    Bullet::Bullet()
    {
        SetName(L"bullet");

        mSpell = new MagicArrow();
    }

    Bullet::~Bullet()
    {
    }

    void Bullet::Initialize()
    {
        GetComponent<Transform>()->SetScale(16.f, 16.f, 1.0f);

        SpellUI::Initialize();


        std::wstring textBox =
            L"\n"
            L"  MAGIC ARROW\n"
            L"\n"
            L"  A handy magical arrow\n"
            L"\n"
            L"  \x01 Type         Projectile\n"
            L"  \x04 Mana darin   20\n"
            L"\n"
            L"  \x02 Damage       10\n"
            L"  \x06 Speed        625\n"
            L"\n"
            L"  \x03 Cast delay   +0.07s\n"
            L"  \x07 Spread       +2 DEG\n";


        mTextBox = new TextBox(textBox, Vector3(180.f * 1.2f, 100.f * 1.2f, 1.0f));
        mTextBox->Initialize();
        mTextBox->GetComponent<Transform>()->SetScale(180.f, 100.f, 1.0f);
    }

    void Bullet::Update()
    {
        SpellUI::Update();
    }

    void Bullet::LateUpdate()
    {
        SpellUI::LateUpdate();
    }

    void Bullet::Render()
    {
        SpellUI::Render();
    }

}