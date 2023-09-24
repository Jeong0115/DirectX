#include "zzUICamera.h"
#include "zzUIManager.h"
#include "zzUI.h"

namespace zz
{
    UICamera::UICamera()
    {
        EnableUIMasks();
    }

    UICamera::~UICamera()
    {
    }

    void UICamera::Initialize()
    {
    }

    void UICamera::Update()
    {
    }

    void UICamera::LateUpdate()
    {
        Camera::CreateViewMatrix();
        Camera::CreateProjectionMatrix((Camera::eProjectionType)mType);
        Camera::RegisterCameraInRenderer();
    }

    void UICamera::Render()
    {
        View = mView;
        Projection = mProjection;

        DisableDepthStencilState();
        RenderUI();
        EnableDepthStencilState();
    }

    void UICamera::RenderUI()
    {
        const auto& objects = UIManager::GetInventoryUI();

        for (UINT i = 0; i < (UINT)eUIType::End; i++)
        {
            if (mUIMask[i] == true)
            {
                for (auto object : objects[i])
                {
                    object->Render();
                }
            }
        }
    }

    void UICamera::TurnLayerMask(eUIType type, bool enable)
    {
        mUIMask.set((UINT)type, enable);
    }
}