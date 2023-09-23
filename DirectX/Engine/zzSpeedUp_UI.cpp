#include "zzSpeedUp_UI.h"
#include "zzTransform.h"
#include "zzSpeedUp.h"

namespace zz
{
    SpeedUp_UI::SpeedUp_UI()
    {
        SetName(L"speed");

        mSpell = new SpeedUp();
    }

    SpeedUp_UI::~SpeedUp_UI()
    {
    }

    void SpeedUp_UI::Initialize()
    {
        GetComponent<Transform>()->SetScale(16.f, 16.f, 1.0f);

        SpellUI::Initialize();
    }

    void SpeedUp_UI::Update()
    {
        SpellUI::Update();
    }

    void SpeedUp_UI::LateUpdate()
    {
        SpellUI::LateUpdate();
    }

    void SpeedUp_UI::Render()
    {
        SpellUI::Render();
    }
}