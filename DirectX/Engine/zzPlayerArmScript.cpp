#include "zzPlayerArmScript.h"
#include "zzAnimator.h"
#include "zzTexture.h"
#include "zzResourceManager.h"
#include "zzGameObject.h"
#include "zzInput.h"
#include "zzTransform.h"
#include "zzMaterial.h"
#include "zzMeshRenderer.h"

namespace zz
{
    PlayerArmScript::PlayerArmScript()
    {
        
    }
    PlayerArmScript::~PlayerArmScript()
    {
    }
    void PlayerArmScript::Initialize()
    {
        std::shared_ptr<Texture> texture
            = ResourceManager::Load<Texture>(L"player_arm", L"..\\Resources\\Texture\\Player\\player_arm.png");

        std::shared_ptr<Shader> spriteShader = ResourceManager::Find<Shader>(L"SpriteShader");

        std::shared_ptr<Material> spriteMateiral2 = std::make_shared<Material>();
        spriteMateiral2->SetShader(spriteShader);
        spriteMateiral2->SetTexture(texture);
        ResourceManager::Insert(L"m_player_arm", spriteMateiral2);

        MeshRenderer* meshA = GetOwner()->AddComponent<MeshRenderer>();
        meshA->SetMaterial(ResourceManager::Find<Material>(L"m_player_arm"));
        meshA->SetMesh(ResourceManager::Find<Mesh>(L"RectMesh"));

        //mAnimator = GetOwner()->AddComponent<Animator>();
        //
        //mAnimator->Create(L"player_arm", texture, Vector2(0.0f, 0.0f), Vector2(5.0f, 5.0f), 1, Vector2::Zero, 0.2f);
        //mAnimator->PlayAnimation(L"player_arm", true);
    }

    void PlayerArmScript::Update()
    {
        Vector3 playerPos = mPlayer->GetComponent<Transform>()->GetPosition();
        Vector3 cursorPos = Input::GetMouseWorldPos();

        double dx = cursorPos.x - playerPos.x;
        double dy = cursorPos.y - playerPos.y;

        double radian = atan2(dy, dx);

        GetOwner()->GetComponent<Transform>()->SetRotation(0.f, 0.f, (float)radian);

    }
}