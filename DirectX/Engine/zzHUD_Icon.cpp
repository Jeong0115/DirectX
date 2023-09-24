#include "zzHUD_Icon.h"
#include "zzTransform.h"
#include "zzMeshRenderer.h"
#include "zzResourceManager.h"

namespace zz
{
    HUD_Icon::HUD_Icon()
        : UI(eUIType::HUD)
    {
    }
    HUD_Icon::~HUD_Icon()
    {
    }

    void HUD_Icon::Initialize()
    {
        GameObject::Initialize();
    }

    void HUD_Icon::Update()
    {
        GameObject::Update();
    }

    void HUD_Icon::LateUpdate()
    {
        GameObject::LateUpdate();
    }

    void HUD_Icon::Render()
    {
        GameObject::Render();
    }

    void HUD_Icon::OnCollisionEnter(GameObject* other)
    {
    }
    void HUD_Icon::OnCollisionStay(GameObject* other)
    {
    }
    void HUD_Icon::OnCollisionExit(GameObject* other)
    {
    }
}