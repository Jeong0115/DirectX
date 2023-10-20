#include "zzDetectPlayer.h"
#include "zzGameObject.h"
#include "zzTransform.h"
#include "zzPixelWorld.h"

namespace zz
{
    Vector3 DetectPlayer::PlayerPos = Vector3(0.0f, 0.0f, 0.0f);

    DetectPlayer::DetectPlayer()
        : Component(eComponentType::DetectPlayer)
        , mDetectRangeX(100.0f)
        , mDetectRangeYUp(20.f)
        , mDetectRangeYDown(-100.f)
        , mbPlayerInRange(false)
        , mDirection(1)
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
        mDirection = myPos.x > PlayerPos.x ? -1 : 1;
    }

    void DetectPlayer::LateUpdate()
    {
    }

    void DetectPlayer::Render()
    {
    }


    bool DetectPlayer::IsPlayerInRange()
    {
        Vector3 myPos = GetOwner()->GetComponent<Transform>()->GetPosition();
        float distanceX = fabs(myPos.x - PlayerPos.x);
        float distanceY = PlayerPos.y - myPos.y;

        if (distanceX <= mDetectRangeX && distanceY >= mDetectRangeYDown && distanceY <= mDetectRangeYUp)
        {
            mbPlayerInRange = true;
        }
        else
        {
            mbPlayerInRange = false;
        }

        return mbPlayerInRange;
    }

    void DetectPlayer::rayCast(Vector3 pos)
    {
       
    }
}