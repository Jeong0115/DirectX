#include "zzCameraScript.h"
#include "zzTransform.h"
#include "zzGameObject.h"
#include "zzTime.h"
#include "zzInput.h"

namespace zz
{
    void CameraScript::Update()
    {
        Transform* tr = GetOwner()->GetComponent<Transform>();
        Vector3 pos = tr->GetPosition();


        if (Input::GetKey(eKeyCode::W))
        {
            pos.z += 105.0f * (float)Time::DeltaTime();
            tr->SetPosition(pos);
        }
        else if (Input::GetKey(eKeyCode::S))
        {
            pos.z -= 105.0f * (float)Time::DeltaTime();
            tr->SetPosition(pos);
        }
        else if (Input::GetKey(eKeyCode::A))
        {
            pos.x -= 105.0f * (float)Time::DeltaTime();
            tr->SetPosition(pos);
        }
        else if (Input::GetKey(eKeyCode::D))
        {
            pos.x += 305.0f * (float)Time::DeltaTime();
            tr->SetPosition(pos);
        }
        else if (Input::GetKey(eKeyCode::Q))
        {
            pos.y -= 105.0f * (float)Time::DeltaTime();
            tr->SetPosition(pos);
        }
        else if (Input::GetKey(eKeyCode::E))
        {
            pos.y += 105.0f * (float)Time::DeltaTime();
            tr->SetPosition(pos);
        }

    }
}