#pragma once

#include "zzInventoryBox.h"
#include "zzUI.h"

namespace zz
{
    class Player;
    class InventoryManager
    {
    public:
        InventoryManager();
        ~InventoryManager();

        static void Test();

        static void Initialize();
        static void Update();
        static void LateUpdate();
        static void Release();

        static void AddUIObject(UI* object, eUIType type);
        static void CreateStartItems();
        static void MoveItemToSlot(UINT prevSlot, UINT moveSlot);
        static void AcquireItem(Equipment* equipment);
        static void SetPlayer(Player* player) { mPlayer = player; }
        static void SetOpenOrCloseInventory() { mbOpenInventory = !mbOpenInventory; }
        static bool GetIsOpenInventory() { return mbOpenInventory; }

        static const std::vector<std::vector<UI*>>& GetInventoryUI() { return mInventoryUI; }

    private:
        static std::vector<InventoryBox*> mItemSlots;
        static UINT mEquipItemIndex;
        static Player* mPlayer;
        static bool mbOpenInventory;

        static std::vector<std::vector<UI*>> mInventoryUI;
    };
}