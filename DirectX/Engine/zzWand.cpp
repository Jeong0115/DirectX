#include "zzWand.h"
#include "zzTime.h"
#include "zzTransform.h"
#include "zzModifierSpell.h"
#include "zzProjectileSpell.h"
#include "zzSceneManager.h"
#include "zzInput.h"
#include "zzTextObject.h"
#include "zzEventManager.h"

#include "zzUIManager.h"
#include "zzPlayer.h"

#include "zzMuzzleEffect.h"
#include "zzAnimator.h"
#include "zzResourceManager.h"

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
        if (mbReCharge)
        {
            mCurReChargeTime -= static_cast<float>(Time::DeltaTime());

            if (mCurReChargeTime <= 0.0f)
            {
                mCurReChargeTime = 0.0f;
                mbReCharge = false;
                mCurSpellIndex = 0;
            }

            EvenetData data;
            data.eventType = eEvent::ReCharge_Change;
            data.recharge = mCurReChargeTime / mReChargeTime;

            EventManager::RegisterEvent(data);
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
            if (mbReCharge)
            {
                TextObject* text = new TextObject();
                text->WriteText(L"RECHARGING..", Vector3(100.f, 20.f, 1.0f));
                text->GetComponent<Transform>()->SetPosition(GetComponent<Transform>()->GetWorldPosition());
                text->GetComponent<Transform>()->SetScale(Vector3(100.f, 20.f, 1.0f));

                CreateObject(text, eLayerType::UI);
            }
            else if (mbCastDelay)
            {
                TextObject* text = new TextObject();
                text->WriteText(L"CAST DELAY..", Vector3(100.f, 20.f, 1.0f));
                text->GetComponent<Transform>()->SetPosition(GetComponent<Transform>()->GetWorldPosition());
                text->GetComponent<Transform>()->SetScale(Vector3(100.f, 20.f, 1.0f));

                CreateObject(text, eLayerType::UI);
            }

            return;
        }

        Vector3 pos = GetComponent<Transform>()->GetWorldPosition();
        Vector3 mousePos = Input::GetMouseWorldPos();

        Vector3 direction = mousePos - pos;
        direction.Normalize();
        direction.z = 0.f;

        UINT lastIndex = mCurSpellIndex;

        std::vector<ModifierSpell*> mModifiers;
        bool isModifier = false;

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

                        Vector3 playerPos = UIManager::mPlayer->GetComponent<Transform>()->GetPosition();
                        Vector3 cursorPos = Input::GetMouseWorldPos();

                        double dx = cursorPos.x - playerPos.x;
                        double dy = cursorPos.y - playerPos.y;

                        double radian = atan2(dy, dx);

                        Vector3 rotaition = GetComponent<Transform>()->GetWorldRotation();
                        float angle = radian;

                        Vector3 worldOffset;

                        worldOffset.x = mTip.x * cos(angle);
                        worldOffset.y = mTip.x * sin(angle);

                        attackSpell->SetDirection(direction);
                        attackSpell->GetComponent<Transform>()->SetPosition(pos.x + worldOffset.x, pos.y + worldOffset.y, pos.z);
                        attackSpell->GetComponent<Transform>()->SetRotationZ(angle);

                        MuzzleEffect* muzzle = new MuzzleEffect();
                        std::shared_ptr<Texture> muzzleTexture = ResourceManager::Find<Texture>(L"Muzzle_SparkBolt");
                        Animator* manimator = new Animator();
                        manimator->Create(L"Muzzle_SparkBolt_Play", muzzleTexture, Vector2(0.0f, 0.0f), Vector2(16.0f, 16.0f), 1, Vector2::Zero, 0.1f);
                        manimator->PlayAnimation(L"Muzzle_SparkBolt_Play", false);

                        muzzle->SetAnimator(manimator, L"Muzzle_SparkBolt_Play");
                        muzzle->GetComponent<Transform>()->SetPosition(mTip.x, 0.0f, BACK_PIXEL_WORLD_Z);
                        //muzzle->GetComponent<Transform>()->SetRotationZ(GetComponent<Transform>()->GetRotation().z);
                        muzzle->GetComponent<Transform>()->SetScale(16.0f, 16.0f, 1.0f);
                        CreateObject(muzzle, eLayerType::Effect);

                        muzzle->GetComponent<Transform>()->SetParent(GetComponent<Transform>());

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


                        if (mCurSpellIndex >= mCapacity)
                        {
                            mCurSpellIndex = 0;
                            mbReCharge = true;
                            mCurReChargeTime = mReChargeTime;
                        }
                        else if(!mbReCharge)
                        {
                            bool isEmpty = true;

                            for (int i = mCurSpellIndex; i < mCapacity; i++)
                            {
                                if (mSpells[i] != nullptr)
                                {
                                    isEmpty = false;
                                    break;
                                }
                            }

                            // all of을 사용해서도 가능 일단은 for-loop로 사용
                            //bool isEmpty = std::all_of(mSpells.begin() + mCurSpellIndex, mSpells.begin() + mCapacity,
                            //    [](auto spell) { return spell == nullptr; });

                            if (isEmpty)
                            {
                                mbReCharge = true;
                                mCurReChargeTime = mReChargeTime;
                            }
                        }

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
                        isModifier = true;
                    }

                }
            }

            mCurSpellIndex++;
            if (mCurSpellIndex >= mCapacity)
            {
                mCurSpellIndex = 0;
                mbReCharge = true;
                mCurReChargeTime = mReChargeTime;

                if (!isModifier)
                {
                    break;
                }
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