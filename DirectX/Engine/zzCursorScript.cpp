#include "zzCursorScript.h"
#include "zzApplication.h"
#include "zzCamera.h"
#include "zzGameObject.h"
#include "zzTransform.h"
#include "zzInput.h"


namespace zz
{
    CursorScript::CursorScript()
    {
    }
    CursorScript::~CursorScript()
    {
    }
    void CursorScript::Initialize()
    {
    }
    void CursorScript::Update()
    {

        Vector3 pos = Input::GetMouseWorldPos();

        GetOwner()->GetComponent<Transform>()->SetPosition(pos.x, pos.y, 0.0f);
    }
}