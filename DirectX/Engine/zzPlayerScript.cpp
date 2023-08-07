#include "zzPlayerScript.h"
#include "zzTime.h"
#include "zzInput.h"
#include "zzTransform.h"
#include "zzGameObject.h"
#include "zzPixelWorld.h"
#include "zzTexture.h"
#include "zzResourceManager.h"
#include "zzAnimator.h"
#include "zzRenderer.h"
#include "zzApplication.h"

namespace zz
{
    PlayerScript::PlayerScript()
        : vel(0.f)
    {
    }

    PlayerScript::~PlayerScript()
    {
    }

    void PlayerScript::Initialize()
    {
        std::shared_ptr<Texture> texture
            = ResourceManager::Load<Texture>(L"Player", L"..\\Resources\\Texture\\Player\\Player.png");

        mAnimator = GetOwner()->AddComponent<Animator>();

        mAnimator->Create(L"Player_Idle", texture, Vector2(0.0f, 1.0f), Vector2(12.0f, 19.0f), 6, Vector2::Zero, 0.1f);
        mAnimator->Create(L"Player_Walk", texture, Vector2(0.0f, 21.f), Vector2(12.0f, 19.0f), 6, Vector2::Zero, 0.105f);
        mAnimator->Create(L"Player_Jump_Up", texture, Vector2(0.0f, 41.f), Vector2(12.0f, 19.0f), 3, Vector2::Zero, 0.082f);
        mAnimator->Create(L"Player_Jump_Fall", texture, Vector2(0.0f, 61.f), Vector2(12.0f, 19.0f), 3, Vector2::Zero, 0.082f);
        mAnimator->Create(L"Player_Land", texture, Vector2(0.0f, 81.f), Vector2(12.0f, 19.0f), 3, Vector2::Zero, 0.075f);

        mAnimator->PlayAnimation(L"Player_Idle", true);
    }

    void PlayerScript::Update()
    {
        Transform* tr = GetOwner()->GetComponent<Transform>();
        Vector3 pos = tr->GetPosition();

        if (Input::GetKey(eKeyCode::LEFT) || Input::GetKey(eKeyCode::RIGHT))
        {
            if (Input::GetKey(eKeyCode::LEFT))
            {
                pos.x -= 50.0f * Time::DeltaTime();
            }
            if (Input::GetKey(eKeyCode::RIGHT))
            {
                pos.x += 50.0f * Time::DeltaTime();
            }        
        }

        if (!jump && !fall && Input::GetKeyDown(eKeyCode::LEFT) || Input::GetKeyDown(eKeyCode::RIGHT))
        {
            mAnimator->PlayAnimation(L"Player_Walk", true);
        }
        else if (!jump && !fall && Input::GetKeyUp(eKeyCode::LEFT) || Input::GetKeyUp(eKeyCode::RIGHT))
        {
            mAnimator->PlayAnimation(L"Player_Idle", true);
        }

        if (!jump && Input::GetKeyDown(eKeyCode::UP))
        {
            vel += 120.f;
            jump = true;
            mAnimator->PlayAnimation(L"Player_Jump_Up", true);
        }

        if (jump || fall)
        {
            vel -= 180.0f * Time::DeltaTime();
        }
        if (jump && vel < 0)
        {
            fall = true;
            mAnimator->PlayAnimation(L"Player_Jump_Fall", true);
        }

        pos.y += vel * Time::DeltaTime();

        if (pos.x >= 0 && -pos.y + 7 >= 0)
        {
            if (PixelWorld::GetElement(pos.x, -pos.y + 7).Type == eElementType::SOLID)
            {
                pos.y += 1;
                if (jump || fall)
                {
                    mAnimator->PlayAnimation(L"Player_Idle", true);
                    vel = 0.f;
                }
                jump = fall = false;
            }
            else if (PixelWorld::GetElement(pos.x, -pos.y + 9).Type != eElementType::SOLID)
            {
                fall = true;
                if(vel <= -180.f)
                {
                    mAnimator->PlayAnimation(L"Player_Jump_Fall", true);
                    
                }
            }
            if (PixelWorld::GetElement(pos.x + 6.f, -pos.y).Type == eElementType::SOLID)
            {

            }
            else tr->SetPosition(pos);
        }

        
        renderer::FlipCB flipCB = {};
   
        HWND Hwnd = Application::GetInst().GetHwnd();
        POINT mousePos = {};
        GetCursorPos(&mousePos);
        ScreenToClient(Hwnd, &mousePos);

        if (mousePos.x >= 800)
        {
            flipCB.flip.x = 0;
        }
        else
        {
            flipCB.flip.x = 1;
        }


        graphics::ConstantBuffer* cb = renderer::constantBuffer[(UINT)eCBType::Flip];
        cb->SetBufferData(&flipCB);
        cb->BindConstantBuffer(eShaderStage::PS);



        if(mCamera != nullptr)
        {
            Vector3 cPos = mCamera->GetComponent<Transform>()->GetPosition();
            cPos.x = pos.x;
            cPos.y = pos.y;
            mCamera->GetComponent<Transform>()->SetPosition(cPos);
        }
    }
}