#include "zzBoltWand_0997.h"
#include "zzTransform.h"
#include "zzTexture.h"
#include "zzResourceManager.h"
#include "zzAnimator.h"
#include "zzMeshRenderer.h"
#include "zzMaterial.h"

#include "zzSparkBolt.h"
#include "zzInput.h"
#include "zzSceneManager.h"
#include "zzScene.h"

#include "zzModifierSpell.h"

namespace zz
{
    BoltWand_0997::BoltWand_0997()
    {
        mCapacity = 4;
        mManaMax = 143;
        mManaChargeSpeed = 40;
        mCurMana = mManaMax;
        mCastDelay = 0.f;
        mReChargeTime = 2.f;
        mSpread = 0.f;
        mSpells.resize(mCapacity);
        mTip = Vector3(15.f, 3.f, 1.0f);

        SetName(L"BoltWand_0997");
    }

    BoltWand_0997::~BoltWand_0997()
    {
    }

    void BoltWand_0997::Initialize()
    {
        GetComponent<Transform>()->SetPosition(4.f, -2.f, 0.0f);
        GetComponent<Transform>()->SetScale(18.f, 7.f, 1.0f);

        mInitialPosition = Vector3(4.f, -2.f, 0.0f);

        std::shared_ptr<Texture> texture
            = ResourceManager::Load<Texture>(L"BoltWand_0997", L"..\\Resources\\Texture\\Wand\\BoltWand_0997.png");

        MeshRenderer* mesh = AddComponent<MeshRenderer>();
        mesh->SetMaterial(ResourceManager::Find<Material>(L"m_SpriteAnimation"));
        mesh->SetMesh(ResourceManager::Find<Mesh>(L"RectMesh"));

        Animator* animator = AddComponent<Animator>();
        animator->Create(L"BoltWand_0997", texture, Vector2(0.0f, 0.0f), Vector2(18.0f, 7.0f), 1, Vector2::Zero, 1.f);
        animator->PlayAnimation(L"BoltWand_0997", true);

        GameObject::Initialize();
    }

    void BoltWand_0997::Update()
    {
        GameObject::Update();
    }

    void BoltWand_0997::LateUpdate()
    {
        GameObject::LateUpdate();
    }

    void BoltWand_0997::Render()
    {
        GameObject::Render();
    }

    void BoltWand_0997::UseEquipment()
    {
        Vector3 pos = GetComponent<Transform>()->GetWorldPosition();
        Vector3 mousePos = Input::GetMouseWorldPos();

        Vector3 direction = mousePos - pos;
        direction.Normalize();
        direction.z = 0.f;

        UINT lastIndex = mCurSpellIndex;

        std::vector<ModifierSpell*> mModifiers;

        while (true)
        {
            if (mSpells[mCurSpellIndex] != nullptr)
            {
                Spell* spell = mSpells[mCurSpellIndex];

                if (spell->GetSpellType() == eSpellType::Projectile)
                {
                    ProjectileSpell* transClass = dynamic_cast<ProjectileSpell*>(spell);
                    ProjectileSpell* attackSpell = transClass->Clone();

                    attackSpell->SetDirection(direction);
                    attackSpell->GetComponent<Transform>()->SetPosition(pos.x + mTip.x / 2, pos.y, pos.z);
                    attackSpell->GetComponent<Transform>()->SetRotation(GetComponent<Transform>()->GetWorldRotation());

                    if (!mModifiers.empty())
                    {
                        mModifiers[0]->ModifierProjectile(attackSpell);
                        mModifiers.clear();
                    }
                    else
                    {
                        int a = 0;
                    }

                    attackSpell->Initialize();

                    SceneManager::GetActiveScene()->AddGameObject(attackSpell, eLayerType::PlayerAttack);
                    mCurSpellIndex++;
                    break;
                }

                else if (spell->GetSpellType() == eSpellType::Utility)
                {
                    mModifiers.push_back(dynamic_cast<ModifierSpell*>(spell));
                }
            }

            mCurSpellIndex++;
            if (mCurSpellIndex == mCapacity)
            {
                mCurSpellIndex = 0;
            }

            if (mCurSpellIndex == lastIndex)
            {
                break;
            }
        }

    }
}