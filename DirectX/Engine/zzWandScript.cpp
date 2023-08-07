#include "zzWandScript.h"
#include "zzAnimator.h"
#include "zzTexture.h"
#include "zzResourceManager.h"
#include "zzGameObject.h"

namespace zz
{
    WandScript::WandScript()
    {
    }
    WandScript::~WandScript()
    {
    }
    void WandScript::Initialize()
    {
        std::shared_ptr<Texture> texture
            = ResourceManager::Load<Texture>(L"wand_0000", L"..\\Resources\\Texture\\Wand\\wand_0000.png");

        mAnimator = GetOwner()->AddComponent<Animator>();

        mAnimator->Create(L"wand_0000", texture, Vector2(1.0f, 1.0f), Vector2(18.f, 5.0f), 1, Vector2::Zero, 1.0f);

        mAnimator->PlayAnimation(L"wand_0000", true);
    }
    void WandScript::Update()
    {
    }
}