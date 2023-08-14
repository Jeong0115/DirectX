#include "zzInventoryManager.h"
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
#include "zzUICollisionManger.h"
#include "zzItemTexture.h"
#include "zzItemTextureHighlight.h"

#include "zzInfoBox.h"
#include "zzInfoBoxTexture.h"

namespace zz
{
    UINT InventoryManager::mEquipItemIndex = 9;
    std::vector<InventoryBox*> InventoryManager::mItemSlots = {};
    std::vector<std::vector<UI*>> InventoryManager::mInventoryUI = {};
    Player* InventoryManager::mPlayer = nullptr;
    bool InventoryManager::mbOpenInventory = false;

    InventoryManager::InventoryManager()   
    {
    }

    InventoryManager::~InventoryManager()
    {
    }

    void InventoryManager::Test()
    {
        BoltWand_0997* wand2 = new BoltWand_0997();
        wand2->Initialize();
        AcquireItem(wand2);
    }

    void InventoryManager::Initialize()
    {      
        mInventoryUI.resize((UINT)eUIType::End);

        InventoryBG* inventoryBG = new InventoryBG(eUIType::BG);
        mInventoryUI[(UINT)eUIType::BG].push_back(inventoryBG);

        Mouse* mouse = new Mouse(eUIType::Mouse);
        mInventoryUI[(UINT)eUIType::Mouse].push_back(mouse);

        for (int i = 0; i < 4; i++)
        {
            InventoryBox* inventoryBox = new InventoryBox(eUIType::ItemSlot);
            inventoryBox->SetSlotIndex(i);
            mItemSlots.push_back(inventoryBox);
            mInventoryUI[(UINT)eUIType::ItemSlot].push_back(inventoryBox);
            inventoryBox->GetComponent<Transform>()->SetPosition(29.5f + i * 20.f, 412.5f, 1.0f);
        }

        for (int i = 0; i < 4; i++)
        {
            InventoryBox* inventoryBox = new InventoryBox(eUIType::ItemSlot);
            inventoryBox->SetSlotIndex(i + 4);
            mItemSlots.push_back(inventoryBox);
            mInventoryUI[(UINT)eUIType::ItemSlot].push_back(inventoryBox);
            inventoryBox->GetComponent<Transform>()->SetPosition(111.5f + i * 20.f, 412.5f, 1.0f);
        }

        for (UINT i = 0; i < (UINT)eUIType::End; i++)
        {
            for (auto object : mInventoryUI[i])
            {
                object->Initialize();
            }
        }

        UICollisionManger::SetCollisionUI(eUIType::Mouse, eUIType::ItemSlot, true);
        UICollisionManger::SetCollisionUI(eUIType::Mouse, eUIType::Item, true);
    }

    void InventoryManager::Update()
    {
        if (mbOpenInventory)
        {
            dynamic_cast<UICamera*>(renderer::uiCamera)->TurnLayerMask(eUIType::BG, true);
            UICollisionManger::Update();
        }
        else
        {
            // 나중에 한번만 실행되게 수정
            dynamic_cast<UICamera*>(renderer::uiCamera)->TurnLayerMask(eUIType::BG, false);
            UICollisionManger::ResetCollision();
        }

        if (Input::IsInputNumberKey())
        {
            UINT index = Input::IsInputNumberKey() - 1;

            if (index != mEquipItemIndex && mItemSlots[index]->GetItem() != nullptr)
            {
                mEquipItemIndex = index;
                mPlayer->SetEquipment(mItemSlots[index]->GetItem());
                mInventoryUI[(UINT)eUIType::Highligt].front()->GetComponent<Transform>()->SetParent(mItemSlots[mEquipItemIndex]->GetItemTexture()->GetComponent<Transform>());
            }
        }

        for (UINT i = 0; i < (UINT)eUIType::End; i++)
        {
            for (auto object : mInventoryUI[i])
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
                    dynamic_cast<Wand*>(mItemSlots[i]->GetItem())->GetInfoBox()->GetComponent<Transform>()->SetPosition(70.f, 250.f - 46.f * index, 0.f);
                    //dynamic_cast<Wand*>(mItemSlots[i]->GetItem())->GetInfoBox()->Update();
                    index++;
                }
            }
        }
    }

    void InventoryManager::LateUpdate()
    {
        for (UINT i = 0; i < (UINT)eUIType::End; i++)
        {
            for (auto object : mInventoryUI[i])
            {
                if (!mbOpenInventory && i == (UINT)eUIType::BG)
                {
                    continue;
                }
                object->LateUpdate();
            }
        }
    }

    void InventoryManager::Release()
    {
        for (UINT i = 0; i < (UINT)eUIType::End; i++)
        {
            for (auto uiObject : mInventoryUI[i])
            {
                delete uiObject;
            }
        }
    }

    void InventoryManager::AddUIObject(UI* object, eUIType type)
    {
        mInventoryUI[(UINT)type].push_back(object);
    }

    void InventoryManager::CreateStartItems()
    {
        ItemTextureHighlight* highlight = new ItemTextureHighlight(eUIType::Highligt);
        highlight->Initialize();
        mInventoryUI[(UINT)eUIType::Highligt].push_back(highlight);

        BlastWand_0585* wand = new BlastWand_0585();
        wand->Initialize();
        AcquireItem(wand);

        mEquipItemIndex = 0;
        mPlayer->SetEquipment(mItemSlots[0]->GetItem());
        highlight->GetComponent<Transform>()->SetParent(mItemSlots[0]->GetItemTexture()->GetComponent<Transform>());
    }

    void InventoryManager::MoveItemToSlot(UINT prevSlot, UINT moveSlot)
    {
        InventoryBox* prevBox = mItemSlots[prevSlot];
        InventoryBox* moveBox = mItemSlots[moveSlot];

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

    void InventoryManager::AcquireItem(Equipment* equipment)
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
                    item->CreateItemTexture(material_name, mItemSlots[i]->GetComponent<Transform>()->GetPosition(), textureScale);

                    equipment->GetComponent<Transform>()->SetParent(mPlayer->GetPlayerArm()->GetComponent<Transform>());
                    mItemSlots[i]->SetItem(equipment, item);

                    InfoBoxTexture* infoTex = new InfoBoxTexture(eUIType::Item);
                    infoTex->CreateItemTexture(material_name, textureScale * 1.5f);

                    InfoBox* infoBox = new InfoBox(eUIType::InfoBox);
                    infoBox->LinkWand(dynamic_cast<Wand*>(equipment), infoTex);
                    infoBox->Initialize();

                    dynamic_cast<Wand*>(equipment)->SetInfoBox(infoBox);
                    mInventoryUI[(UINT)eUIType::InfoBox].push_back(infoBox);
                    break;
                }
            }
        }
    }
}