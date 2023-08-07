#include "zzInventoryManager.h"
#include "zzInput.h"
#include "zzPlayer.h"
#include "zzTransform.h"
#include "zzBlastWand_0585.h"
#include "zzBoltWand_0997.h"
#include "zzUI.h"
#include "zzInventoryBG.h"
#include "zzEquipment.h"

#include "zzMouse.h"
#include "zzUICollisionManger.h"
#include "zzItemTexture.h"
#include "zzItemTextureHighlight.h"

namespace zz
{
    UINT InventoryManager::mEquipItemIndex = 9;
    std::vector<InventoryBox*> InventoryManager::mItemSlots = {};
    std::vector<std::vector<UI*>> InventoryManager::mInventoryUI = {};
    Player* InventoryManager::mPlayer = nullptr;

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

        InventoryBG* inventoryBG = new InventoryBG(eUIType::NoneCollider);
        mInventoryUI[(UINT)eUIType::NoneCollider].push_back(inventoryBG);

        Mouse* mouse = new Mouse(eUIType::Mouse);
        mInventoryUI[(UINT)eUIType::Mouse].push_back(mouse);

        for (int i = 0; i < 4; i++)
        {
            InventoryBox* inventoryBox = new InventoryBox(eUIType::ItemSlot);
            inventoryBox->SetSlotIndex(i);
            mItemSlots.push_back(inventoryBox);
            mInventoryUI[(UINT)eUIType::ItemSlot].push_back(inventoryBox);
            inventoryBox->GetComponent<Transform>()->SetPosition(29.5f + i * 20.f, 330.f, 1.0f);
        }

        for (int i = 0; i < 4; i++)
        {
            InventoryBox* inventoryBox = new InventoryBox(eUIType::ItemSlot);
            inventoryBox->SetSlotIndex(i + 4);
            mItemSlots.push_back(inventoryBox);
            mInventoryUI[(UINT)eUIType::ItemSlot].push_back(inventoryBox);
            inventoryBox->GetComponent<Transform>()->SetPosition(111.5f + i * 20.f, 330.f, 1.0f);
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
        UICollisionManger::Update();

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
                object->Update();
            }
        }
    }

    void InventoryManager::LateUpdate()
    {
        for (UINT i = 0; i < (UINT)eUIType::End; i++)
        {
            for (auto object : mInventoryUI[i])
            {
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

                break;
            }
        }
    }
}