#include "zzLightBullet.h"
#include "zzTransform.h"
#include "zzSparkBolt.h"
#include "zzTextBox.h"

namespace zz
{
    LightBullet::LightBullet()
    {
        SetName(L"light_bullet");

        mSpell = new SparkBolt();
    }

    LightBullet::~LightBullet()
    {
    }

    void LightBullet::Initialize()
    {
        GetComponent<Transform>()->SetScale(16.f, 16.f, 1.0f);

        SpellUI::Initialize();

        std::wstring textBox =
            L"\n"
            L"  SPARK BOLT\n"
            L"\n"
            L"  A weak but enchanting sparkling projectile\n"
            L"\n"
            L"  \x01 Type         Projectile\n"
            L"  \x04 Mana darin   5\n"
            L"\n"
            L"  \x02 Damage       3\n"
            L"  \x06 Speed        800\n"
            L"\n"
            L"  \x03 Cast delay   +0.05s\n"
            L"  \x07 Spread       -1 DEG\n";


        mTextBox = new TextBox(textBox, Vector3(250.f * 1.2f, 100.f * 1.2f, 1.0f));
        mTextBox->Initialize();
        mTextBox->GetComponent<Transform>()->SetScale(250.f, 100.f, 1.0f);
    }

    void LightBullet::Update()
    {
        SpellUI::Update();
    }

    void LightBullet::LateUpdate()
    {
        SpellUI::LateUpdate();
    }

    void LightBullet::Render()
    {
        SpellUI::Render();
    }

}