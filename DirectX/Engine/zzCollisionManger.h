#pragma once

#include "zzEngine.h"

namespace zz
{
    class Collider;

    class CollisionManger
    {
    public:
        union ColliderID
        {
            struct
            {
                UINT left;
                UINT right;
            };
            UINT64 id;
        };

        static void Initialize();
        static void Update();
        static void UpdateUI();

        static void LayerCollision(UINT left, UINT right, bool isUI);
        static void ColliderCollision(Collider* left, Collider* right, bool isUI);
        static bool Intersect(Collider* left, Collider* right);

        static void ResetCollisionUI();

        static void SetCollision(eLayerType left, eLayerType right, bool enable);
        static void SetCollisionUI(eUIType left, eUIType right, bool enable);
        static void Clear();
        static void ClearUI();

    private:
        static std::bitset<(UINT)eUIType::End> mUIMatrix[(UINT)eUIType::End];
        static std::map<UINT64, bool> mUICollisionMap;

        static std::bitset<(UINT)eLayerType::End> mMatrix[(UINT)eLayerType::End];
        static std::map<UINT64, bool> mCollisionMap;
    };
}
