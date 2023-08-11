#pragma once

#include "zzEngine.h"

namespace zz
{
    class UICollider;

    class UICollisionManger
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
        static void LayerCollision(eUIType left, eUIType right);
        static void ColliderCollision(UICollider* left, UICollider* right);
        static bool Intersect(UICollider* left, UICollider* right);
        static void ResetCollision();

        static void SetCollisionUI(eUIType left, eUIType right, bool enable);
        static void Clear();

    private:

        static std::bitset<(UINT)eUIType::End> mMatrix[(UINT)eUIType::End];
        static std::map<UINT64, bool> mCollisionMap;
    };
}
