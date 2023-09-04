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
#include "zzCollider.h"
#include "zzPixelCollider.h"
#include "zzLevitation.h"
#include "zzRigidBody.h"
#include "zzLight.h"

namespace zz
{
    Player::Player()
        : mEquipment(nullptr)
        , mCamera(nullptr)
        , mAnimator(nullptr)
        , vel(0.f)
        , mEquipmentInitialPos(Vector3::Zero)
        , mPlayerArm(nullptr)
        , mRigid(nullptr)
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

        AddComponent<Levitation>()->SetMaxEnergy(3.0f);

        PixelCollider* pxCollier = AddComponent<PixelCollider>();
        pxCollier->SetCollision(Vector3(0.0f, -4.0f, 0.0f), Vector3(6.f, 8.f, 0.0f));
        pxCollier->SetClimbY(3.0f);

        mRigid = AddComponent<RigidBody>();
        AddComponent<Collider>()->SetScale(6.0f, 10.f, 1.0f);
        AddComponent<Light>()->SetLightScale(300.f, 300.f, 1.0f);

        mAnimator = AddComponent<Animator>();

        mAnimator->Create(L"Player_Idle", texture, Vector2(0.0f, 1.0f), Vector2(12.0f, 19.0f), 6, Vector2::Zero, 0.1f);
        mAnimator->Create(L"Player_Walk", texture, Vector2(0.0f, 21.f), Vector2(12.0f, 19.0f), 6, Vector2::Zero, 0.105f);
        mAnimator->Create(L"Player_Jump_Up", texture, Vector2(0.0f, 41.f), Vector2(12.0f, 19.0f), 3, Vector2::Zero, 0.082f);
        mAnimator->Create(L"Player_Jump_Fall", texture, Vector2(0.0f, 61.f), Vector2(12.0f, 19.0f), 3, Vector2::Zero, 0.082f);
        mAnimator->Create(L"Player_Land", texture, Vector2(0.0f, 81.f), Vector2(12.0f, 19.0f), 3, Vector2::Zero, 0.075f);

        mAnimator->PlayAnimation(L"Player_Idle", true);     

        mPlayerArm->Initialize();
        //mEquipment->Initialize();
        GameObject::Initialize();
    }

    void Player::Update()
    {
        if (Input::GetKeyDown(eKeyCode::B))
        {
            Explosion_128* bomb = new Explosion_128();
            bomb->GetComponent<Transform>()->SetPosition(Input::GetMouseWorldPos().x, Input::GetMouseWorldPos().y, 0.02f);
            CreateObject(bomb, eLayerType::Effect);
        }
        if (Input::GetKeyDown(eKeyCode::TAB))
        {
            InventoryManager::SetOpenOrCloseInventory();
        }

        Transform* tr = GetComponent<Transform>();
        Vector3 pos = tr->GetPosition();

        //if (Input::GetKeyDown(eKeyCode::W))
        //{
        //    mRigid->SetVelocityY(100.f);
        //    GetComponent<RigidBody>()->SetGround(false);
        //}
        if (Input::GetKey(eKeyCode::A))
        {
            mRigid->SetVelocityX(-50.f);
        }
        else if (Input::GetKeyUp(eKeyCode::A))
        {
            mRigid->SetVelocityX(0.0f);
        }
        if (Input::GetKey(eKeyCode::D))
        {
            mRigid->SetVelocityX(50.f);
        }
        else if (Input::GetKeyUp(eKeyCode::D))
        {
            mRigid->SetVelocityX(0.0f);
        }
        
        tr->SetPosition(pos);


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
        if (mCamera != nullptr)
        {
            mCamera->GetComponent<Transform>()->SetPosition(GetComponent<Transform>()->GetPosition());
        }

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



        flipCB = {};
        cb->SetBufferData(&flipCB);
        cb->BindConstantBuffer(eShaderStage::PS);
    }

    void Player::OnCollisionEnter(GameObject* other)
    {
    }
    void Player::OnCollisionStay(GameObject* other)
    {
    }
    void Player::OnCollisionExit(GameObject* other)
    {
    }
}