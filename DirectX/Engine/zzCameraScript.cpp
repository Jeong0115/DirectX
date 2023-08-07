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


        if (Input::GetKey(eKeyCode::J))
        {
            pos.x -= 305.0f * (float)Time::DeltaTime();
            tr->SetPosition(pos);
        }
        else if (Input::GetKey(eKeyCode::L))
        {
            pos.x += 305.0f * (float)Time::DeltaTime();
            tr->SetPosition(pos);
        }
        else if (Input::GetKey(eKeyCode::K))
        {
            pos.y -= 305.0f * (float)Time::DeltaTime();
            tr->SetPosition(pos);
        }
        else if (Input::GetKey(eKeyCode::I))
        {
            pos.y += 305.0f * (float)Time::DeltaTime();
            tr->SetPosition(pos);
        }

    }
}