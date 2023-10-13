#include "zzLimbA.h"
#include "zzResourceManager.h"
#include "zzAnimator.h"
#include "zzTransform.h"
#include "zzMeshRenderer.h"
#include "zzLimbKnee.h"
#include "zzLimbB.h"
#include "zzPixelWorld.h"

#include "zzInput.h"
#include "zzTime.h"

#define RAY_LEN 160.0f

namespace zz
{
    LimbA::LimbA(GameObject* body)
        : mTime(0.f)
        , mDuration(0.1f)
        , dir(1)
        , limbB(nullptr)
        , mCurAngle(0.0f)
        , mRotationAngle(0.f)
        , mbBend(false)
        , mMoveState(eMoveState::Stay)
    {
        mBody = body;
    }
    LimbA::~LimbA()
    {
    }

    void LimbA::Initialize()
    {
        GetComponent<Transform>()->SetScale(80.f, 16.f, 1.0f);

        MeshRenderer* mesh = AddComponent<MeshRenderer>();
        mesh->SetMaterial(ResourceManager::Find<Material>(L"m_SpriteAnimation"));
        mesh->SetMesh(ResourceManager::Find<Mesh>(L"RectMesh"));

        std::shared_ptr<Texture> texture = ResourceManager::Find<Texture>(L"limb_long_a");

        Animator* animator = AddComponent<Animator>();
        animator->Create(L"limb_long_a_idle", texture, Vector2(0.0f, 0.0f), Vector2(80.0f, 16.0f), 8, Vector2::Zero, 0.17f);
        animator->PlayAnimation(L"limb_long_a_idle", true);

        LimbKnee* limbKnee = new LimbKnee();
        CreateObject(limbKnee, eLayerType::Monster);
        Transform* kTr = limbKnee->GetComponent<Transform>();
        kTr->SetPosition(40.f, 0.f, 0.21f);
        kTr->SetParent(GetComponent<Transform>());

        limbB = new LimbB();
        CreateObject(limbB, eLayerType::Monster);
        Transform* bTr = limbB->GetComponent<Transform>();
        bTr->SetPosition(40.f, 0.f, 0.22f);
        bTr->SetParent(kTr);
        bTr->SetRevolution(Vector3(-40.f, 0.f, 0.0f));

        float angle = GetComponent<Transform>()->GetRotation().z;
        if (angle > PI * 1.5f || angle < PI * 0.5f)
        {
            limbB->SetDir(-1);
        }
        else
        {
            limbB->SetDir(1);
        }


        GameObject::Initialize();
    }

    void LimbA::Update()
    {
        switch (mMoveState)
        {
        case eMoveState::Bend: bend(); break;
        case eMoveState::Search: search();  break;
        case eMoveState::Move: move();  break;
        case eMoveState::Stay: stay();  break;
        default: break;
        }

        float angle = GetComponent<Transform>()->GetRotation().z;
        if (angle > PI * 1.5f || angle < PI * 0.5f)
        {
            limbB->SetDir(-1);
        }
        else
        {
            limbB->SetDir(1);
        }


        GameObject::Update();
    }

    void LimbA::LateUpdate()
    {
        GameObject::LateUpdate();
    }

    void LimbA::Render()
    {
        GameObject::Render();
    }

    void LimbA::SetMoveState(eMoveState state)
    {
        switch (state)
        {
        case eMoveState::Bend: 
        {
            mMoveState = state;
            mTime = 0.f;
            limbB->SetBend();
        }
        break;

        case eMoveState::Move:
        {
            mCurAngle = GetComponent<Transform>()->GetRotation().z;
            mMoveState = state;
            mTime = 0.f;
        }
        break;
        //case eMoveState::Search: search();  break;
        //case eMoveState::Move: break;
        //case eMoveState::Stay: break;
        default: break;
        }
    }

    void LimbA::bend()
    {
        mTime += (float)Time::DeltaTime();

        if (mTime >= mDuration)
        {
            GetComponent<Transform>()->SetPositionX(0.f);
            GetComponent<Transform>()->SetRevolution(Vector3(0.f, 0.f, 0.f));

            mMoveState = eMoveState::Search;
        }
        else
        {
            GetComponent<Transform>()->SetPositionX(40.f - 40.f * (mTime / mDuration));
            GetComponent<Transform>()->SetRevolution(Vector3(-40.f + 40.f * (mTime / mDuration), 0.f, 0.f));
        }
    }

    void LimbA::search()
    {
        float randomAngle = random() * PI * 2;

        float lenX = RAY_LEN * cos(randomAngle);
        float lenY = RAY_LEN * sin(randomAngle);

        int steps = std::max(std::fabs(lenX), std::fabs(lenY));

        float xIncrement = lenX / steps;
        float yIncrement = lenY / steps;
        
        Vector3 bodyPos = mBody->GetComponent<Transform>()->GetPosition();
        float dx = bodyPos.x;
        float dy = bodyPos.y;

        for (int i = 0; i <= steps; i++)
        {
            if (PixelWorld::GetElement((int)dx, (int)-dy).Type == eElementType::SOLID)
            {
                float distance = sqrt(pow(dx - bodyPos.x, 2) + pow(dy - bodyPos.y, 2));
                float cos = (distance / 2.f) / (RAY_LEN / 2.f);
                cos = std::max(-1.0f, std::min(1.0f, cos));
                float angle = std::acos(cos);
                
                if (randomAngle <= PI / 2)
                {
                    limbB->SetMove(2 * angle);
                    mRotationAngle = randomAngle + angle;
                }
                else if (randomAngle <= PI / 2 * 3)
                {
                    mRotationAngle = (fabs((-angle) + randomAngle));
                    if (mRotationAngle <= PI / 2)
                    {
                        limbB->SetMove(-2 * angle);
                    }
                    else
                    {
                        limbB->SetMove(2 * angle);
                    }
                    
                }
                else
                {
                    limbB->SetMove(2 * angle);
                    mRotationAngle = randomAngle + angle;
                }

                landPos = Vector2(dx, dy);
                SetMoveState(eMoveState::Move);
                break;

            }
            else
            {
                dx += xIncrement;
                dy += yIncrement;
            }
        }

    }
    void LimbA::move()
    {
        mTime += (float)Time::DeltaTime();

        if (mTime >= mDuration)
        {
            GetComponent<Transform>()->SetPositionX(40.f);
            GetComponent<Transform>()->SetRevolution(Vector3(-40.f, 0.f, 0.f));


            GetComponent<Transform>()->SetRotationZ(mRotationAngle);
            mMoveState = eMoveState::Stay;
            mTime = 0.f;
        }
        else
        {
            GetComponent<Transform>()->SetPositionX(40.f * (mTime / mDuration));
            GetComponent<Transform>()->SetRevolution(Vector3(-40.f * (mTime / mDuration), 0.f, 0.f));

            GetComponent<Transform>()->SetRotationZ((mRotationAngle - mCurAngle) * (mTime / mDuration) + mCurAngle);
        }
    }
    void LimbA::stay()
    {
        Vector3 bodyPos = mBody->GetComponent<Transform>()->GetPosition();

        float randomAngle = atan2(landPos.y - bodyPos.y, landPos.x - bodyPos.x);

        float distance = sqrt(pow(landPos.x - bodyPos.x, 2) + pow(landPos.y - bodyPos.y, 2));

        if (distance > RAY_LEN)
        {
            SetMoveState(LimbA::eMoveState::Bend);
        }
        float cos = (distance / 2.f) / (RAY_LEN / 2.f);
        cos = std::max(-1.0f, std::min(1.0f, cos));
        float angle = std::acos(cos);

        if (randomAngle <= PI / 2)
        {
            limbB->SetAngle(2 * angle);
            mRotationAngle = randomAngle + angle;
            GetComponent<Transform>()->SetRotationZ(mRotationAngle);
        }
        else if (randomAngle <= PI / 2 * 3)
        {
            mRotationAngle = (fabs((-angle) + randomAngle));
            GetComponent<Transform>()->SetRotationZ(mRotationAngle);
            if (mRotationAngle <= PI / 2)
            {
                limbB->SetAngle(-2 * angle);
            }
            else
            {
                limbB->SetAngle(2 * angle);
            }

        }
        else
        {
            limbB->SetAngle(2 * angle);
            mRotationAngle = randomAngle + angle;
            GetComponent<Transform>()->SetRotationZ(mRotationAngle);
        }

       // landPos = Vector2(dx, dy);

       /* mTime += (float)Time::DeltaTime();
        if (mTime > 1.0f)
        {
            SetMoveState(LimbA::eMoveState::Bend);
        }*/
    }
}