#include "zzWand.h"
#include "zzTime.h"
#include "zzTransform.h"
#include "zzModifierSpell.h"
#include "zzProjectileSpell.h"
#include "zzSceneManager.h"
#include "zzInput.h"
#include "zzTextObject.h"
#include "zzEventManager.h"
#include "zzAudioSource.h"
#include "zzMultiCastSpell.h"

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
        , mSpellSound(nullptr)
    {
    }

    Wand::~Wand()
    {
    }

    void Wand::Initialize()
    {
        mSpellSound = AddComponent<AudioSource>();
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
            if (Input::GetKeyDown(eKeyCode::LBUTTON))
            {
                if (mbReCharge)
                {
                    TextObject* text = new TextObject();
                    text->WriteText(L"RECHARGING..", Vector3(100.f, 20.f, 1.0f));
                    text->GetComponent<Transform>()->SetPosition(GetComponent<Transform>()->GetWorldPosition());
                    text->GetComponent<Transform>()->SetScale(Vector3(100.f, 20.f, 1.0f));
                    text->SetColor(Vector3(1.0f, 1.0f, 0.0f));

                    CreateObject(text, eLayerType::UI);
                }
                else if (mbCastDelay)
                {
                    TextObject* text = new TextObject();
                    text->WriteText(L"CAST DELAY..", Vector3(100.f, 20.f, 1.0f));
                    text->GetComponent<Transform>()->SetPosition(GetComponent<Transform>()->GetWorldPosition());
                    text->SetColor(Vector3(1.0f, 1.0f, 0.0f));
                    text->GetComponent<Transform>()->SetScale(Vector3(100.f, 20.f, 1.0f));

                    CreateObject(text, eLayerType::UI);
                }
            }

            return;
        }

        Vector3 pos = GetComponent<Transform>()->GetWorldPositionApplyRotation();
        Vector3 mousePos = Input::GetMouseWorldPos();

        Vector3 direction = mousePos - pos;
        direction.Normalize();
        direction.z = 0.f;

        UINT lastIndex = mCurSpellIndex;

        std::vector<ModifierSpell*> mModifiers;
        bool isModifier = false;
        int projCnt = 1;

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
                        mCurCastDelay += mCastDelay;
                        mCurReChargeTime += spell->GetRechargeTime();
                        mbCastDelay = true;

                        ProjectileSpell* transClass = dynamic_cast<ProjectileSpell*>(spell);
                        ProjectileSpell* attackSpell = transClass->Clone();
                        attackSpell->ClearTrail();

                        float angle = GetComponent<Transform>()->GetWorldRotation().z;

                        Vector3 worldOffset;

                        worldOffset.x = mTip.x * cos(angle);
                        worldOffset.y = mTip.x * sin(angle);


                        attackSpell->SetDirection(direction);
                        attackSpell->GetComponent<Transform>()->SetPosition(pos.x + worldOffset.x, pos.y + worldOffset.y, pos.z);
                        attackSpell->GetComponent<Transform>()->SetRotationZ(angle);

                        if (!mModifiers.empty())
                        {
                            for(auto modifierSpell : mModifiers)
                            {
                                modifierSpell->ModifierProjectile(attackSpell);
                            }
                            mModifiers.clear();
                        }

                        attackSpell->InitialSetting();

                        GameObject* muzzle = attackSpell->GetMuzzleEffect();
                        if (muzzle != nullptr)
                        {
                            muzzle->GetComponent<Transform>()->SetPosition(mTip.x, 0.0f, BACK_PIXEL_WORLD_Z);
                            muzzle->GetComponent<Transform>()->SetParent(GetComponent<Transform>());
                        }
                        mCurSpellIndex++;

                        if (mCurSpellIndex >= mCapacity)
                        {
                            mCurSpellIndex = 0;
                            mbReCharge = true;
                            mCurReChargeTime += mReChargeTime;
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

                            if (isEmpty)
                            {
                                mbReCharge = true;
                                mCurReChargeTime += mReChargeTime;
                            }
                        }

                        break;
                    }
                }
                else if (spell->GetSpellType() == eSpellType::Utility)
                {
                    UtilitySpell* castSpell = dynamic_cast<UtilitySpell*>(spell);

                    if (castSpell->GetUtilityType() == eUtilityType::Modifier)
                    {
                        float manaDrain = spell->GetManaDrain();

                        if (mCurMana < manaDrain)
                        {
                            break;
                        }
                        else
                        {
                            mModifiers.push_back(dynamic_cast<ModifierSpell*>(castSpell));
                            mCurMana -= manaDrain;
                            mCurCastDelay += spell->GetCastDelay();
                            isModifier = true;
                        }
                    }
                    else if (castSpell->GetUtilityType() == eUtilityType::MultiCast)
                    {
                        mCurSpellIndex++;
                        MultiCastSpell* multicast = dynamic_cast<MultiCastSpell*>(castSpell);
                        std::vector<ModifierSpell*> modifier;

                        int count = multicast->GetCastCount();
                        float spread = multicast->GetSpread();
                        spread /= 20.f;

                        for (int i = 0; i < count; i++)
                        {
                            bool isBreak = false;

                            Spell* spell = mSpells[mCurSpellIndex];

                            if (spell->GetSpellType() == eSpellType::Projectile)
                            {
                                float manaDrain = spell->GetManaDrain();

                                if (mCurMana < manaDrain)
                                {
                                    isBreak = true;
                                    break;
                                }
                                else
                                {
                                    mCurMana -= manaDrain;
                                    mCurCastDelay += spell->GetCastDelay();
                                    mCurReChargeTime += spell->GetRechargeTime();
                                    mbCastDelay = true;

                                    ProjectileSpell* transClass = dynamic_cast<ProjectileSpell*>(spell);
                                    ProjectileSpell* attackSpell = transClass->Clone();
                                    attackSpell->ClearTrail();

                                    float angle = GetComponent<Transform>()->GetWorldRotation().z;
                                    Vector3 worldOffset;

                                    worldOffset.x = mTip.x * cos(angle);
                                    worldOffset.y = mTip.x * sin(angle);

                                    float randSpread = randf(spread) - spread / 2.f;
                                    Vector3 rotateDir;
                                    rotateDir.x = direction.x * cos(randSpread) - direction.y * sin(randSpread);
                                    rotateDir.y = direction.x * sin(randSpread) + direction.y * cos(randSpread);

                                    attackSpell->SetDirection(rotateDir);
                                    attackSpell->GetComponent<Transform>()->SetPosition(pos.x + worldOffset.x, pos.y + worldOffset.y, pos.z);
                                    attackSpell->GetComponent<Transform>()->SetRotationZ(angle + randSpread);

                                    if (!mModifiers.empty())
                                    {
                                        for (auto modifierSpell : mModifiers)
                                        {
                                            modifierSpell->ModifierProjectile(attackSpell);
                                        }
                                    }
                                    if (!modifier.empty())
                                    {
                                        for (auto modifierSpell : modifier)
                                        {
                                            modifierSpell->ModifierProjectile(attackSpell);
                                        }
                                        modifier.clear();
                                    }

                                    attackSpell->InitialSetting();

                                    GameObject* muzzle = attackSpell->GetMuzzleEffect();
                                    if (muzzle != nullptr)
                                    {
                                        muzzle->GetComponent<Transform>()->SetPosition(mTip.x, 0.0f, BACK_PIXEL_WORLD_Z);
                                        muzzle->GetComponent<Transform>()->SetParent(GetComponent<Transform>());
                                    }
                                    mCurSpellIndex++;

                                    if (mCurSpellIndex >= mCapacity)
                                    {
                                        mCurSpellIndex = 0;
                                        mbReCharge = true;
                                        mCurReChargeTime += mReChargeTime;
                                        isBreak = true;
                                    }
                                    else if (!mbReCharge)
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

                                        if (isEmpty)
                                        {
                                            mbReCharge = true;
                                            mCurReChargeTime += mReChargeTime;
                                            isBreak = true;
                                        }
                                    }
                                }
                            }
                            else if (spell->GetSpellType() == eSpellType::Utility)
                            {
                                UtilitySpell* castSpell = dynamic_cast<UtilitySpell*>(spell);

                                if (castSpell->GetUtilityType() == eUtilityType::Modifier)
                                {
                                    float manaDrain = spell->GetManaDrain();

                                    if (mCurMana < manaDrain)
                                    {
                                        isBreak = true;
                                    }
                                    else
                                    {
                                        modifier.push_back(dynamic_cast<ModifierSpell*>(castSpell));
                                        mCurMana -= manaDrain;
                                        mCurCastDelay += spell->GetCastDelay();
                                        isModifier = true;
                                    }
                                }
                                else
                                {
                                    mCurSpellIndex--;
                                    isBreak = true;
                                }
                            }  
                            if (isBreak)
                            {
                                break;
                            }
                            
                        }
                        break;

                    }
                }
            }

            mCurSpellIndex++;
            if (mCurSpellIndex >= mCapacity)
            {
                mCurSpellIndex = 0;
                mbReCharge = true;
                mCurReChargeTime += mReChargeTime;

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