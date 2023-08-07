#include "zzPlayerArm.h"
#include "zzTexture.h"
#include "zzResourceManager.h"
#include "zzInput.h"
#include "zzTransform.h"
#include "zzMeshRenderer.h"
#include "zzAnimator.h"

namespace zz
{
    PlayerArm::PlayerArm()
        : mPlayer(nullptr)
    {
    }

    PlayerArm::~PlayerArm()
    {
    }

    void PlayerArm::Initialize()
    {
        GetComponent<Transform>()->SetScale(Vector3(5.f, 5.f, 1.0f));
        GetComponent<Transform>()->SetPosition(Vector3(0.f, 2.f, 0.000f));
        //GetComponent<Transform>()->SetRotation(Vector3(0.f, 0.f, 1.1f));

        std::shared_ptr<Texture> texture
            = ResourceManager::Load<Texture>(L"player_arm", L"..\\Resources\\Texture\\Player\\player_arm.png");

        MeshRenderer* mesh = AddComponent<MeshRenderer>();
        mesh->SetMaterial(ResourceManager::Find<Material>(L"m_SpriteAnimation"));
        mesh->SetMesh(ResourceManager::Find<Mesh>(L"RectMesh"));

        Animator* animator = AddComponent<Animator>();       
        animator->Create(L"player_arm", texture, Vector2(0.0f, 0.0f), Vector2(5.0f, 5.0f), 1, Vector2::Zero, 1.f);
        animator->PlayAnimation(L"player_arm", true);

        GameObject::Initialize();
    }

    void PlayerArm::Update()
    {
        Vector3 playerPos = mPlayer->GetComponent<Transform>()->GetPosition();
        Vector3 cursorPos = Input::GetMouseWorldPos();

        double dx = cursorPos.x - playerPos.x;
        double dy = cursorPos.y - playerPos.y;

        double radian = atan2(dy, dx);

        if (fabs(radian) > PI / 2)
        {
            radian -= PI;
        }

        GetComponent<Transform>()->SetRotation(0.f, 0.f, (float)radian);

        GameObject::Update();
    }

    void PlayerArm::LateUpdate()
    {
        GameObject::LateUpdate();
    }

    void PlayerArm::Render()
    {
        GameObject::Render();
    }

    void PlayerArm::SetPlayer(GameObject* player)
    {
        mPlayer = player;
        GetComponent<Transform>()->SetParent(mPlayer->GetComponent<Transform>());
    }
}