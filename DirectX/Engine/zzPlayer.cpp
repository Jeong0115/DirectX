#include "zzPlayer.h"
#include "zzTexture.h"
#include "zzAnimator.h"
#include "zzResourceManager.h"
#include "zzPixelWorld.h"
#include "zzInput.h"
#include "zzTime.h"
#include "zzTransform.h"
#include "zzRenderer.h"
#include "zzApplication.h"
#include "zzMeshRenderer.h"
#include "zzMaterial.h"
#include "zzInventoryManager.h"
#include "zzExplosion_128.h"

namespace zz
{
    Player::Player()
        : mEquipment(nullptr)
        , mCamera(nullptr)
        , mAnimator(nullptr)
        , vel(0.f)
        , mEquipmentInitialPos(Vector3::Zero)
        , mPlayerArm(nullptr)
    {
    }

    Player::~Player()
    {
        if (mPlayerArm != nullptr)
        {
            delete mPlayerArm;
            mPlayerArm = nullptr;
        }
    }

    void Player::Initialize()
    {
        std::shared_ptr<Texture> texture
            = ResourceManager::Load<Texture>(L"Player", L"..\\Resources\\Texture\\Player\\Player.png");

        MeshRenderer* mesh = AddComponent<MeshRenderer>();
        mesh->SetMaterial(ResourceManager::Find<Material>(L"m_SpriteAnimation"));
        mesh->SetMesh(ResourceManager::Find<Mesh>(L"RectMesh"));

        mAnimator = AddComponent<Animator>();

        mAnimator->Create(L"Player_Idle", texture, Vector2(0.0f, 1.0f), Vector2(12.0f, 19.0f), 6, Vector2::Zero, 0.1f);
        mAnimator->Create(L"Player_Walk", texture, Vector2(0.0f, 21.f), Vector2(12.0f, 19.0f), 6, Vector2::Zero, 0.105f);
        mAnimator->Create(L"Player_Jump_Up", texture, Vector2(0.0f, 41.f), Vector2(12.0f, 19.0f), 3, Vector2::Zero, 0.082f);
        mAnimator->Create(L"Player_Jump_Fall", texture, Vector2(0.0f, 61.f), Vector2(12.0f, 19.0f), 3, Vector2::Zero, 0.082f);
        mAnimator->Create(L"Player_Land", texture, Vector2(0.0f, 81.f), Vector2(12.0f, 19.0f), 3, Vector2::Zero, 0.075f);

        mAnimator->PlayAnimation(L"Player_Idle", true);     

        mPlayerArm->Initialize();
        //mEquipment->Initialize();
    }

    void Player::Update()
    {
        if (Input::GetKeyDown(eKeyCode::B))
        {
            Explosion_128* bomb = new Explosion_128();
            bomb->GetComponent<Transform>()->SetPosition(Input::GetMouseWorldPos());
            CreateObject(bomb, eLayerType::Effect);
        }
        if (Input::GetKeyDown(eKeyCode::TAB))
        {
            InventoryManager::SetOpenOrCloseInventory();
        }

        Transform* tr = GetComponent<Transform>();
        Vector3 pos = tr->GetPosition();

#pragma region MoveTemp

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
                if (vel <= -180.f)
                {
                    mAnimator->PlayAnimation(L"Player_Jump_Fall", true);

                }
            }
            if (PixelWorld::GetElement(pos.x + 6.f, -pos.y).Type == eElementType::SOLID)
            {

            }
            else tr->SetPosition(pos);
        }


#pragma endregion

        if (mCamera != nullptr)
        {
            Vector3 cPos = mCamera->GetComponent<Transform>()->GetPosition();
            cPos.x = pos.x;
            cPos.y = pos.y;
            mCamera->GetComponent<Transform>()->SetPosition(cPos);
        }

        GameObject::Update();

        mPlayerArm->Update();

        if(mEquipment != nullptr)
        {
            mEquipment->Update();
            if (Input::GetKeyDown(eKeyCode::LBUTTON))
            {
                mEquipment->UseEquipment();
            }
        }
    }

    void Player::LateUpdate()
    {
        GameObject::LateUpdate();

        mPlayerArm->LateUpdate();
        if (mEquipment != nullptr)
            mEquipment->LateUpdate();
    }

    void Player::Render()
    {
        renderer::FlipCB flipCB = {};

        float playerX = GetComponent<Transform>()->GetPosition().x;

        if (Input::GetMouseWorldPos().x - playerX >= 0)
        {
            flipCB.flip.x = 0;
        }
        else
        {
            flipCB.flip.x = 1;
        }

        ConstantBuffer* cb = renderer::constantBuffer[(UINT)eCBType::Flip];
        cb->SetBufferData(&flipCB);
        cb->BindConstantBuffer(eShaderStage::PS);

        GameObject::Render();

        mPlayerArm->Render();
        if (mEquipment != nullptr)
        {
            Transform* equipmentTr = mEquipment->GetComponent<Transform>();

            if(flipCB.flip.x == 1)
            {          
                equipmentTr->SetPosition(-mEquipmentInitialPos.x, mEquipmentInitialPos.y, mEquipmentInitialPos.z);
            }
            else
            {
                equipmentTr->SetPosition(mEquipmentInitialPos.x, mEquipmentInitialPos.y, mEquipmentInitialPos.z);
            }
            
            mEquipment->Render();
        }
    }
}