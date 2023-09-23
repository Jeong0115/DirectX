#include "zzLightBullet.h"
#include "zzTransform.h"
#include "zzSparkBolt.h"

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