#include "zzPixelCollider.h"
#include "zzPixelWorld.h"
#include "zzTransform.h"
#include "zzGameObject.h"
#include "zzRigidBody.h"
#include "zzRenderer.h"
#include "zzGraphics.h"
#include "zzTime.h"

namespace zz
{
    PixelCollider::PixelCollider()
        : Component(eComponentType::PixelCollider)
        , mTransform(nullptr)
        , mRigid(nullptr)
        , mCollisionOffset(Vector3::Zero)
        , mCollisionScale(Vector3::One)
        , mClimbOverY(0)
    {
    }

    PixelCollider::~PixelCollider()
    {
    }

    void PixelCollider::Initialize()
    {
        mTransform = GetOwner()->GetComponent<Transform>();
        mRigid = GetOwner()->GetComponent<RigidBody>();
    }

    void PixelCollider::Update()
    {
        Vector3 velocity = mRigid->GetVelocity();

        Vector3 curPos = mTransform->GetPosition();
        Vector3 predictedPos = GetOwner()->GetComponent<RigidBody>()->GetPredictedPosition();

        Vector3 collisionPos = predictedPos;
        collisionPos += mCollisionOffset;
        collisionPos.y += mCollisionScale.y / 2.f;

        DebugMesh mesh = {};
        mesh.position = predictedPos + mCollisionOffset;
        mesh.scale = mCollisionScale;
        mesh.rotation = Vector3::Zero;
        renderer::PushDebugMeshAttribute(mesh);

        // 천장 검사
        int lenX = static_cast<int>(mCollisionScale.x);

        for (int i = -lenX / 2; i <= lenX / 2; i++)
        {
            for (int j = 0; j >= -1; j--)
            {
                int x = collisionPos.x + i;
                int y = collisionPos.y + j;

                if (PixelWorld::GetElement(x, -y).Type == eElementType::SOLID)
                {
                    if(mRigid->GetVelocity().y > 0.f)
                         mRigid->SetVelocityY(0.0f);
                    break;
                }
            }
        }

        if (velocity.x != 0)
        {
            checkSidePixel(predictedPos + mCollisionOffset, velocity);
        }


        // 바닥 검사
        mRigid->SetGround(checkFloorPixel(predictedPos));
    }

    void PixelCollider::LateUpdate()
    {
    }

    void PixelCollider::Render()
    {
    }

    void PixelCollider::checkSidePixel(Vector3 collisionPos, Vector3 velocity)
    {
        Vector3 checkPos = collisionPos;

        int halfScaleX = static_cast<int>(mCollisionScale.x / 2);
        int halfScaleY = static_cast<int>(mCollisionScale.y / 2);

        if (velocity.x > 0.f)
        {
            checkPos.x += halfScaleX;

            for (int i = -halfScaleY; i <= halfScaleY - mClimbOverY; i++)
            {
                for (int j = 0; j >= -1; j--)
                {
                    int x = checkPos.x + j;
                    int y = checkPos.y - i;

                    if (PixelWorld::GetElement(x, -y).Type == eElementType::SOLID)
                    {
                        velocity.x = 0.0f;
                        break;
                    }
                }

                if (velocity.x == 0.0f)
                {
                    break;
                }
            }
        }
        else if (velocity.x < 0.f)
        {
            checkPos = collisionPos;
            checkPos.x -= halfScaleX;

            for (int i = -halfScaleY; i <= halfScaleY - mClimbOverY; i++)
            {
                for (int j = 0; j <= 1; j++)
                {
                    int x = checkPos.x + j;
                    int y = checkPos.y - i;

                    if (PixelWorld::GetElement(x, -y).Type == eElementType::SOLID)
                    {
                        velocity.x = 0.0f;
                        break;
                    }
                }

                if (velocity.x == 0.0f)
                {
                    break;
                }
            }
        }

        checkPos = collisionPos;
        if (velocity.x != 0.0f)
        {
            for (int i = halfScaleY - mClimbOverY + 1; i <= halfScaleY; i++)
            {
                for (int j = -halfScaleX / 2; j <= halfScaleX / 2; j++)
                {
                    int x = checkPos.x + j;
                    int y = checkPos.y - i;

                    if (PixelWorld::GetElement(x, -y).Type == eElementType::SOLID)
                    {
                        mTransform->ShiftPositionY(halfScaleY - i + 1);
                        break;
                    }
                }
            }
        }

        mRigid->SetVelocityX(velocity.x);
    }

    bool PixelCollider::checkFloorPixel(Vector3 predictedPos)
    {
        Vector3 collisionPos = predictedPos + mCollisionOffset;
        collisionPos.y -= mCollisionScale.y / 2 + 1;

        for (int i = -mCollisionScale.x / 2; i < mCollisionScale.x / 2; i++)
        {
            int x = collisionPos.x + i;

            if (PixelWorld::GetElement(x, -collisionPos.y).Type == eElementType::SOLID)
            {
                // 나중에 속도를 받아와서 일정 속도 이상이면 함수를 실행하거나, 함수의 반복문 범위 수정
                preventTunneling(collisionPos);
                return true;
            }
        }

        return false;
    }

    void PixelCollider::preventTunneling(Vector3 collisionPos)
    {
        int maxDistanceY = abs((int)(mRigid->GetVelocity().y * (float)Time::DeltaTime())) - 1;
        bool isEmpty = true;

        for (int i = 0; i <= maxDistanceY; i++)
        {
            for (int j = -mCollisionScale.x / 2; j < mCollisionScale.x / 2; j++)
            {
                if (PixelWorld::GetElement(collisionPos.x + j, -(collisionPos.y + i)).Type == eElementType::SOLID)
                {
                    isEmpty = false;
                    break;
                }
            }

            if (isEmpty)
            {
                mTransform->ShiftPositionY(i);
                break;
            }
        }
    }
}