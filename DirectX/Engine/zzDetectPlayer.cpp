#include "zzDetectPlayer.h"
#include "zzGameObject.h"
#include "zzTransform.h"

namespace zz
{
    Vector3 DetectPlayer::PlayerPos = Vector3(0.0f, 0.0f, 0.0f);

    DetectPlayer::DetectPlayer()
        : Component(eComponentType::DetectPlayer)
        , mDetectRange(100.0f)
    {
    }

    DetectPlayer::~DetectPlayer()
    {
    }

    void DetectPlayer::Initialize()
    {
    }

    void DetectPlayer::Update()
    {
        Vector3 myPos = GetOwner() -> GetComponent<Transform>()->GetPosition();

        float distance = sqrt((myPos.x - PlayerPos.x) * (myPos.x - PlayerPos.x) + (myPos.y - PlayerPos.y) * (myPos.y - PlayerPos.y));
        
        if (distance <= mDetectRange)
        {
            rayCast();
        }
    }

    void DetectPlayer::LateUpdate()
    {
    }

    void DetectPlayer::Render()
    {
    }

    void DetectPlayer::rayCast()
    {

    }
}