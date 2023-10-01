#include "zzHUD_Icon.h"
#include "zzTransform.h"
#include "zzMeshRenderer.h"
#include "zzResourceManager.h"

namespace zz
{
    HUD_Icon::HUD_Icon()
        : UI(eUIType::HUD)
        , mbActive(true)
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
        if (mbActive)
        {
            GameObject::Update();
        }
    }

    void HUD_Icon::LateUpdate()
    {
        if (mbActive)
        {
            GameObject::LateUpdate();
        }
    }

    void HUD_Icon::Render()
    {
        if (mbActive)
        {
            GameObject::Render();
        }
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