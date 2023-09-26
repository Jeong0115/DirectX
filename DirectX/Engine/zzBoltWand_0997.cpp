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
#include "zzTextBox.h"

namespace zz
{
    BoltWand_0997::BoltWand_0997()
    {
        mCapacity = 4;
        mManaMax = 143.f;
        mManaChargeSpeed = 40.f;
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

        mInformation =
            L"\n"
            L"       Shuffle       No\n"
            L"       Spells/Cast   1";

        Wand::Initialize();

        std::wstring textBox =
            L" WAND\n"
            L"\n"
            L" Shuffle        No\n"
            L" Spells/Cast    1\n"
            L" Cast delay     0 s\n"
            L" Rechrg. Time   2 s\n"
            L" Mana max       143\n"
            L" Mana chg. Spd  40\n"
            L" Capacity       4\n"
            L" Spread         0\n";


        mTextBox = new TextBox(textBox, Vector3(140.f * 1.2f, 120.f * 1.2f, 1.0f));
        mTextBox->Initialize();
    }

    void BoltWand_0997::Update()
    {
        Wand::Update();
    }

    void BoltWand_0997::LateUpdate()
    {
        Wand::LateUpdate();
    }

    void BoltWand_0997::Render()
    {
        Wand::Render();
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