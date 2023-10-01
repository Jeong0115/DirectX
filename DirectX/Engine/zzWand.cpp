#include "zzWand.h"
#include "zzTime.h"
#include "zzTransform.h"
#include "zzModifierSpell.h"
#include "zzProjectileSpell.h"
#include "zzSceneManager.h"
#include "zzInput.h"
#include "zzTextObject.h"

namespace zz
{
    Wand::Wand()
        : mCapacity(0)
        , mManaMax(0.f)
        , mManaChargeSpeed(0.f)
        , mCurMana(0.f)
        , mCurSpellIndex(0)
        , mCastDelay(0.f)
        , mReChargeTime(0.f)
        , mSpread(0.f)
        , mSpells{}
        , mTip(Vector3::Zero)
        , mInfoBox(nullptr)
        , mInformation{}
        , mCurCastDelay(0.0f)
        , mCurReChargeTime(0.0f)
        , mbCastDelay(false)
        , mbReCharge(false)
    {
    }

    Wand::~Wand()
    {
    }

    void Wand::Initialize()
    {
        GameObject::Initialize();
    }

    void Wand::Update()
    {
        if (mCurMana > mManaMax)
        {
            mCurMana = mManaMax;
        }
        else if (mCurMana < mManaMax)
        {
            mCurMana += static_cast<float>(Time::DeltaTime() * mManaChargeSpeed);
        }

        if (mbCastDelay)
        {
            mCurCastDelay -= static_cast<float>(Time::DeltaTime());

            if (mCurCastDelay <= 0.0f)
            {
                mCurCastDelay = 0.0f;
                mbCastDelay = false;
            }
        }
        GameObject::Update();
    }

    void Wand::LateUpdate()
    {
        GameObject::LateUpdate();
    }

    void Wand::Render()
    {
        GameObject::Render();
    }

    void Wand::UseEquipment()
    {
        if (mbCastDelay || mbReCharge)
        {
            TextObject* text = new TextObject();
            text->WriteText(L"Cast Delay", Vector3(100.f, 20.f, 1.0f));
            text->GetComponent<Transform>()->SetPosition(GetComponent<Transform>()->GetWorldPosition());
            text->GetComponent<Transform>()->SetScale(Vector3(100.f, 20.f, 1.0f));

            CreateObject(text, eLayerType::UI);
            return;
        }

        Vector3 pos = GetComponent<Transform>()->GetWorldPosition();
        Vector3 mousePos = Input::GetMouseWorldPos();

        Vector3 direction = mousePos - pos;
        direction.Normalize();
        direction.z = 0.f;

        if (mCurSpellIndex >= mCapacity)
        {
            mCurSpellIndex = 0;
        }
        UINT lastIndex = mCurSpellIndex;

        std::vector<ModifierSpell*> mModifiers;

        while (true)
        {
            if (mSpells[mCurSpellIndex] != nullptr)
            {
                Spell* spell = mSpells[mCurSpellIndex];

                if (spell->GetSpellType() == eSpellType::Projectile)
                {
                    float manaDrain = spell->GetManaDrain();

                    if (mCurMana < manaDrain)
                    {
                        break;
                    }
                    else
                    {
                        mCurMana -= manaDrain;
                        mCurCastDelay += spell->GetCastDelay();
                        mbCastDelay = true;

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
                }

                else if (spell->GetSpellType() == eSpellType::Utility)
                {
                    float manaDrain = spell->GetManaDrain();

                    if (mCurMana < manaDrain)
                    {
                        break;
                    }
                    else
                    {
                        mModifiers.push_back(dynamic_cast<ModifierSpell*>(spell));
                        mCurMana -= manaDrain;
                        mCurCastDelay += spell->GetCastDelay();
                    }

                }
            }

            mCurSpellIndex++;
            if (mCurSpellIndex >= mCapacity)
            {
                mCurSpellIndex = 0;
            }

            if (mCurSpellIndex == lastIndex)
            {
                break;
            }
        }
    }

    float Wand::GetEquipmentRate()
    {
        return mCurMana / mManaMax;
    }
}