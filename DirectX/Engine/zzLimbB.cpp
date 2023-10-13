#include "zzLimbB.h"
#include "zzResourceManager.h"
#include "zzAnimator.h"
#include "zzTransform.h"
#include "zzMeshRenderer.h"

#include "zzTime.h"
#include "zzInput.h"

namespace zz
{
    LimbB::LimbB()
        : mTime(0.0f)
        , mbBend(false)
        , mDuration(0.1f)
        , mMoveState(eMoveState::Stay)
        , dir(-1)
        , mCurAngle(0.f)
        , mNextAngle(0.f)
    {
    }
    LimbB::~LimbB()
    {
    }

    void LimbB::Initialize()
    {
        GetComponent<Transform>()->SetScale(80.f, 16.f, 1.0f);

        MeshRenderer* mesh = AddComponent<MeshRenderer>();
        mesh->SetMaterial(ResourceManager::Find<Material>(L"m_SpriteAnimation"));
        mesh->SetMesh(ResourceManager::Find<Mesh>(L"RectMesh"));

        std::shared_ptr<Texture> texture = ResourceManager::Find<Texture>(L"limb_long_b");

        Animator* animator = AddComponent<Animator>();

        animator->Create(L"limb_long_b_idle", texture, Vector2(0.0f, 0.0f), Vector2(80.0f, 16.0f), 4, Vector2::Zero, 0.17f);

        animator->PlayAnimation(L"limb_long_b_idle", true);

        GetComponent<Transform>()->SetRotationZ(PI / 5.f * dir);
        GameObject::Initialize();
    }

    void LimbB::Update()
    {
        

        switch (mMoveState)
        {
        case eMoveState::Bend: bend(); break;
        //case eMoveState::Search: search();  break;
        case eMoveState::Move: move();  break;
        case eMoveState::Land: land();  break;
        case eMoveState::Stay: break;
       
        default: break;
        }

        GameObject::Update();
    }

    void LimbB::LateUpdate()
    {
        GameObject::LateUpdate();
    }

    void LimbB::Render()
    {
        GameObject::Render();
    }

    void LimbB::SetBend()
    {
        mCurAngle = GetComponent<Transform>()->GetRotation().z;
        mMoveState = eMoveState::Bend;
        mDuration = 1.0f;
        mTime = 0.f;

    }

    void LimbB::SetMove(float revolutionAngle)
    {
        mCurAngle = GetComponent<Transform>()->GetRotation().z;
        mNextAngle = revolutionAngle;
        mMoveState = eMoveState::Move;
        mTime = 0.f;
    }


    void LimbB::bend()
    {
        mTime += (float)Time::DeltaTime();

        if (mTime >= mDuration)
        {
            GetComponent<Transform>()->SetRotationZ(PI * dir);
            mMoveState = eMoveState::Stay;
        }
        else
        {  
            GetComponent<Transform>()->SetRotationZ((PI * dir - mCurAngle) * (mTime / mDuration) + mCurAngle);
        }
    }

    void LimbB::move()
    {
        mTime += (float)Time::DeltaTime();

        if (mTime >= mDuration)
        {
            GetComponent<Transform>()->SetRotationZ(mNextAngle * dir);
            mMoveState = eMoveState::Land;
        }
        else
        {
            GetComponent<Transform>()->SetRotationZ((mNextAngle * dir - mCurAngle) * (mTime / mDuration) + mCurAngle);
        }
    }

    void LimbB::land()
    {
        GetComponent<Transform>()->SetRotationZ(mCurAngle * dir);
    }
}