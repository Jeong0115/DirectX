#include "zzBGScript.h"
#include "zzTransform.h"
#include "zzGameObject.h"
#include "zzTime.h"

namespace zz
{
    BGScript::BGScript()
        : mParallaxScale(0.0f)
        , mMoveSpeed(0.f)
        , mainCamera(nullptr)
        , mWidth(0.f)
    {
    }
    BGScript::~BGScript()
    {
    }

    void BGScript::Initialize()
    {
        mLastCameraPos = mainCamera->GetComponent<Transform>()->GetPosition();
        mWidth = GetOwner()->GetComponent<Transform>()->GetScale().x;
    }

    void BGScript::Update()
    {
        Vector3 curCameraPos = mainCamera->GetComponent<Transform>()->GetPosition();
        Vector3 cameraMovement = curCameraPos - mLastCameraPos;
        Vector3 position = GetOwner()->GetComponent<Transform>()->GetPosition();
    
        Vector3 curPos = position + cameraMovement * mParallaxScale;

        if (mMoveSpeed != 0.f)
        {
            curPos.x += (float)(Time::DeltaTime() * mMoveSpeed);
        }
        mLastCameraPos = curCameraPos;

        if (curCameraPos.x - curPos.x > mWidth)
        {
            curPos.x += mWidth * 2.f;
        }
        else if (curPos.x - curCameraPos.x > mWidth)
        {
            curPos.x -= mWidth * 2.f;
        }
        GetOwner()->GetComponent<Transform>()->SetPosition(curPos);
    }

    void BGScript::LateUpdate()
    {
    }

    void BGScript::Render()
    {
    }
}