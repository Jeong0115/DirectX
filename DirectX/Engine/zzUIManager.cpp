#include "zzUIManager.h"
#include "zzInput.h"
#include "zzPlayer.h"
#include "zzTransform.h"
#include "zzBlastWand_0585.h"
#include "zzBoltWand_0997.h"
#include "zzUI.h"
#include "zzInventoryBG.h"
#include "zzEquipment.h"
#include "zzUICamera.h"
#include "zzRenderer.h"

#include "zzMouse.h"
#include "zzCollisionManger.h"
#include "zzItemTexture.h"
#include "zzItemTextureHighlight.h"

#include "zzInfoBox.h"
#include "zzInfoBoxTexture.h"

#include "zzLightBullet.h"
#include "zzBullet.h"
#include "zzWaterTrail_UI.h"
#include "zzSpeedUp_UI.h"
#include "zzBomb_UI.h"
#include "zzMegalaserUI.h"

#include "zzHealth.h"
#include "zzLevitationEnerge.h"
#include "zzWandMana.h"
#include "zzWandRecharge.h"

namespace zz
{
    UINT UIManager::mEquipItemIndex = 9;
    std::vector<ItemSlot*> UIManager::mItemSlots = {};
    std::vector<SpellSlot*> UIManager::mSpellSlots = {};

    std::vector<std::vector<UI*>> UIManager::mUIObjects = {};
    Player* UIManager::mPlayer = nullptr;
    bool UIManager::mbOpenInventory = false;

    UIManager::UIManager()   
    {
    }
    UIManager::~UIManager()
    {
    }

    void UIManager::Test()
    {
        BoltWand_0997* wand2 = new BoltWand_0997();
        wand2->Initialize();
        AcquireItem(wand2);

        LightBullet* spell = new LightBullet();
        spell->Initialize();
        AcquireSpell(spell);

        {
            Bullet* spell = new Bullet();
            spell->Initialize();
            AcquireSpell(spell);
        }
        {
            WaterTrail_UI* spell = new WaterTrail_UI();
            spell->Initialize();
            AcquireSpell(spell);
        }
        {
            Bomb_UI* spell = new Bomb_UI();
            spell->Initialize();
            AcquireSpell(spell);
        }
        {
            MegalaserUI* spell = new MegalaserUI();
            spell->Initialize();
            AcquireSpell(spell);
        }

        SpeedUp_UI* spd = new SpeedUp_UI();
        spd->Initialize();
        AcquireSpell(spd);
    }

    void UIManager::Initialize()
    {      
        mUIObjects.resize((UINT)eUIType::End);

        InventoryBG* inventoryBG = new InventoryBG(eUIType::BG);
        mUIObjects[(UINT)eUIType::BG].push_back(inventoryBG);

        Mouse* mouse = new Mouse(eUIType::Mouse);
        mUIObjects[(UINT)eUIType::Mouse].push_back(mouse);

        Health* health = new Health();
        mUIObjects[(UINT)eUIType::HUD].push_back(health);

        LevitationEnerge* levitaionEnerge = new LevitationEnerge();
        mUIObjects[(UINT)eUIType::HUD].push_back(levitaionEnerge);

        WandMana* wandMana = new WandMana();
        mUIObjects[(UINT)eUIType::HUD].push_back(wandMana);

        WandRecharge* wandRecharge = new WandRecharge();
        mUIObjects[(UINT)eUIType::HUD].push_back(wandRecharge);

        for (int i = 0; i < 4; i++)
        {
            ItemSlot* inventoryBox = new ItemSlot(eUIType::ItemSlot);
            inventoryBox->SetSlotIndex(i);
            mItemSlots.push_back(inventoryBox);
            mUIObjects[(UINT)eUIType::ItemSlot].push_back(inventoryBox);
            inventoryBox->GetComponent<Transform>()->SetPosition(29.5f + i * 20.f, 330.f, 1.0f);
        }

        for (int i = 0; i < 4; i++)
        {
            ItemSlot* inventoryBox = new ItemSlot(eUIType::ItemSlot);
            inventoryBox->SetSlotIndex(i + 4);
            mItemSlots.push_back(inventoryBox);
            mUIObjects[(UINT)eUIType::ItemSlot].push_back(inventoryBox);
            inventoryBox->GetComponent<Transform>()->SetPosition(111.5f + i * 20.f, 330.f, 1.0f);
        }

        for (int i = 0; i < 16; i++)
        {
            SpellSlot* slot = new SpellSlot(eUIType::SpellSlot);
            slot->SetSlotIndex(i);
            mSpellSlots.push_back(slot);
            mUIObjects[(UINT)eUIType::SpellSlot].push_back(slot);
            slot->GetComponent<Transform>()->SetPosition(200.0f + i * 20.f, 330.f, 1.0f);
        }

        for (UINT i = 0; i < (UINT)eUIType::End; i++)
        {
            for (auto object : mUIObjects[i])
            {
                object->Initialize();
            }
        }

        CollisionManger::SetCollisionUI(eUIType::Mouse, eUIType::ItemSlot, true);
        CollisionManger::SetCollisionUI(eUIType::Mouse, eUIType::SpellSlot, true);
        CollisionManger::SetCollisionUI(eUIType::Mouse, eUIType::WandSlot, true);
        CollisionManger::SetCollisionUI(eUIType::Mouse, eUIType::Item, true);
    }

    void UIManager::Update()
    {
        if (mbOpenInventory)
        {
            dynamic_cast<UICamera*>(renderer::uiCamera)->TurnLayerMask(eUIType::BG, true);
            dynamic_cast<UICamera*>(renderer::uiCamera)->TurnLayerMask(eUIType::InfoBox, true);
            dynamic_cast<UICamera*>(renderer::uiCamera)->TurnLayerMask(eUIType::WandSlot, true);
            dynamic_cast<UICamera*>(renderer::uiCamera)->TurnLayerMask(eUIType::SpellSlot, true);
            CollisionManger::UpdateUI();
        }
        else
        {
            // 나중에 한번만 실행되게 수정
            dynamic_cast<UICamera*>(renderer::uiCamera)->TurnLayerMask(eUIType::BG, false);
            dynamic_cast<UICamera*>(renderer::uiCamera)->TurnLayerMask(eUIType::InfoBox, false);
            dynamic_cast<UICamera*>(renderer::uiCamera)->TurnLayerMask(eUIType::WandSlot, false);
            dynamic_cast<UICamera*>(renderer::uiCamera)->TurnLayerMask(eUIType::SpellSlot, false);
            CollisionManger::ResetCollisionUI();
        }

        if (Input::IsInputNumberKey())
        {
            UINT index = Input::IsInputNumberKey() - 1;

            if (index != mEquipItemIndex && mItemSlots[index]->GetItem() != nullptr)
            {
                mEquipItemIndex = index;
                mPlayer->SetEquipment(mItemSlots[index]->GetItem());
                mUIObjects[(UINT)eUIType::Highligt].front()->GetComponent<Transform>()->SetParent(mItemSlots[mEquipItemIndex]->GetItemTexture()->GetComponent<Transform>());
            }
        }

        for (UINT i = 0; i < (UINT)eUIType::End; i++)
        {
            for (auto object : mUIObjects[i])
            {
                if (!mbOpenInventory && i == (UINT)eUIType::BG)
                {
                    continue;
                }
                object->Update();
            }
        }

        if (mbOpenInventory)
        {
            int index = 0;
            for (int i = 0; i < 4; i++)
            {
                if (mItemSlots[i]->GetItem() != nullptr)
                {
                    dynamic_cast<Wand*>(mItemSlots[i]->GetItem())->GetInfoBox()->GetComponent<Transform>()->SetPosition(95.f, 280.f - 65.f * index, 0.2f);
                    //dynamic_cast<Wand*>(mItemSlots[i]->GetItem())->GetInfoBox()->UpdateUI();
                    index++;
                }
            }
        }
    }

    void UIManager::LateUpdate()
    {
        for (UINT i = 0; i < (UINT)eUIType::End; i++)
        {
            for (auto object : mUIObjects[i])
            {
                if (!mbOpenInventory && i == (UINT)eUIType::BG)
                {
                    continue;
                }
                object->LateUpdate();
            }
        }
    }

    void UIManager::Release()
    {
        for (UINT i = 0; i < (UINT)eUIType::End; i++)
        {
            for (auto& uiObject : mUIObjects[i])
            {
                delete uiObject;
            }
        }
    }

    void UIManager::AddUIObject(UI* object, eUIType type)
    {
        mUIObjects[(UINT)type].push_back(object);
    }

    void UIManager::CreateStartItems()
    {
        ItemTextureHighlight* highlight = new ItemTextureHighlight(eUIType::Highligt);
        highlight->Initialize();
        mUIObjects[(UINT)eUIType::Highligt].push_back(highlight);

        BlastWand_0585* wand = new BlastWand_0585();
        wand->Initialize();
        AcquireItem(wand);

        mEquipItemIndex = 0;
        mPlayer->SetEquipment(mItemSlots[0]->GetItem());
        highlight->GetComponent<Transform>()->SetParent(mItemSlots[0]->GetItemTexture()->GetComponent<Transform>());
    }

    void UIManager::MoveItemToSlot(UINT prevSlot, UINT moveSlot)
    {
        ItemSlot* prevBox = mItemSlots[prevSlot];
        ItemSlot* moveBox = mItemSlots[moveSlot];

        if (moveBox->GetItem() == nullptr)
        {
            moveBox->SetItem(prevBox->GetItem(), prevBox->GetItemTexture());
            prevBox->SetItem(nullptr, nullptr);
        }
        else
        {
            Equipment* tempItem = moveBox->GetItem();
            ItemTexture* tempItemTexture = moveBox->GetItemTexture();

            moveBox->SetItem(prevBox->GetItem(), prevBox->GetItemTexture());
            prevBox->SetItem(tempItem, tempItemTexture);
        }

        if (prevSlot == mEquipItemIndex)
        {
            mEquipItemIndex = moveSlot;

        }
        else if (moveSlot == mEquipItemIndex)
        {
            mEquipItemIndex = prevSlot;
        }
    }

    void UIManager::MoveSpellToSlot(UINT prevSlot, UINT moveSlot)
    {
        SpellSlot* prevBox = mSpellSlots[prevSlot];
        SpellSlot* moveBox = mSpellSlots[moveSlot];

        if (moveBox->GetSpell() == nullptr)
        {
            moveBox->SetSpell(prevBox->GetSpell(), prevBox->GetItemTexture());
            prevBox->SetSpell(nullptr, nullptr);
        }
        else
        {
            SpellUI* tempSpell = moveBox->GetSpell();
            ItemTexture* tempItemTexture = moveBox->GetItemTexture();

            moveBox->SetSpell(prevBox->GetSpell(), prevBox->GetItemTexture());
            prevBox->SetSpell(tempSpell, tempItemTexture);
        }
    }

    void UIManager::AcquireItem(Equipment* equipment)
    {
        if(dynamic_cast<Wand*>(equipment))
        {
            for (int i = 0; i < mItemSlots.size(); i++)
            {
                if (mItemSlots[i]->GetItem() == nullptr)
                {
                    std::wstring material_modifier = L"m_";
                    std::wstring material_name = material_modifier + equipment->GetName();

                    Vector3 textureScale = equipment->GetComponent<Transform>()->GetScale() * 0.8f;

                    ItemTexture* item = new ItemTexture(eUIType::Item);
                    item->CreateItemTexture(material_name, eTextureType::Wand, mItemSlots[i]->GetComponent<Transform>()->GetPosition(), textureScale);
                    
                    equipment->GetComponent<Transform>()->SetParent(mPlayer->GetPlayerArm()->GetComponent<Transform>());
                    mItemSlots[i]->SetItem(equipment, item);

                    InfoBoxTexture* infoTex = new InfoBoxTexture(eUIType::Item);
                    infoTex->CreateItemTexture(material_name, textureScale * 2.0f);

                    InfoBox* infoBox = new InfoBox(eUIType::InfoBox);
                    infoBox->LinkWand(dynamic_cast<Wand*>(equipment), infoTex);
                    infoBox->Initialize();

                    dynamic_cast<Wand*>(equipment)->SetInfoBox(infoBox);

                    mUIObjects[(UINT)eUIType::InfoBox].push_back(infoBox);
                    break;
                }
            }
        }
        
    }

    void UIManager::AcquireSpell(SpellUI* spell)
    {
        if (dynamic_cast<SpellUI*>(spell))
        {
            for (int i = 0; i < mSpellSlots.size(); i++)
            {
                if (mSpellSlots[i]->GetSpell() == nullptr)
                {
                    std::wstring material_modifier = L"m_";
                    std::wstring material_name = material_modifier + spell->GetName();

                    Vector3 textureScale = spell->GetComponent<Transform>()->GetScale();

                    ItemTexture* item = new ItemTexture(eUIType::Item);
                    item->CreateItemTexture(material_name, eTextureType::Spell, mSpellSlots[i]->GetComponent<Transform>()->GetPosition(), textureScale);
                    item->SetOwner(mSpellSlots[i]);

                    mSpellSlots[i]->SetSpell(spell, item);
                    break;
                }
            }
        }
    }


}