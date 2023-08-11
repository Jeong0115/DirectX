#include "zzUICollisionManger.h"
#include "zzUICollider.h"
#include "zzInventoryManager.h"
#include "zzUI.h"

namespace zz
{
    std::bitset<(UINT)eUIType::End > UICollisionManger::mMatrix[(UINT)eUIType::End] = {};
    std::map<UINT64, bool> UICollisionManger::mCollisionMap = {};

    void UICollisionManger::Initialize()
    {
    }

    void UICollisionManger::Update()
    {
        for (UINT column = 0; column < (UINT)eUIType::End; column++)
        {
            for (UINT row = column; row < (UINT)eUIType::End; row++)
            {
                if (mMatrix[column][row] == true)
                {
                    LayerCollision((eUIType)column, (eUIType)row);
                }
            }
        }
    }

    void UICollisionManger::LayerCollision(eUIType left, eUIType right)
    {
        const auto& inventoryUI = InventoryManager::GetInventoryUI();

        const std::vector<UI*>& lefts = inventoryUI[(UINT)left];
        const std::vector<UI*>& rights = inventoryUI[(UINT)right];

        for (UI* leftObj : lefts)
        {
            UICollider* leftCol = leftObj->GetComponent<UICollider>();

            if (leftCol == nullptr)
            {
                continue;
            }

            for (UI* rightObj : rights)
            {
                UICollider* rightCol = rightObj->GetComponent<UICollider>();

                if (rightCol == nullptr || leftObj == rightObj)
                {
                    continue;
                }

                ColliderCollision(leftCol, rightCol);
            }
        }
    }

    void UICollisionManger::ColliderCollision(UICollider* left, UICollider* right)
    {
        ColliderID id = {};
        id.left = left->GetColliderID();
        id.right = right->GetColliderID();

        std::map<UINT64, bool>::iterator iter = mCollisionMap.find(id.id);

        if (iter == mCollisionMap.end())
        {
            mCollisionMap.insert(std::make_pair(id.id, false));
            iter = mCollisionMap.find(id.id);
        }

        if (Intersect(left, right))
        {
            if (iter->second == false)
            {
                left->OnCollisionEnter(right);
                right->OnCollisionEnter(left);

                iter->second = true;
            }
            else
            {
                left->OnCollisionStay(right);
                right->OnCollisionStay(left);
            }
        }
        else
        {
            if (iter->second == true)
            {
                left->OnCollisionExit(right);
                right->OnCollisionExit(left);

                iter->second = false;
            }
        }
    }

    bool UICollisionManger::Intersect(UICollider* left, UICollider* right)
    {
        Transform* leftTr = left->GetOwner()->GetComponent<Transform>();
        Transform* rightTr = right->GetOwner()->GetComponent<Transform>();

        Vector3 leftPos = leftTr->GetPosition();
        Vector3 rightPos = rightTr->GetPosition();

        Vector3 leftScale = left->GetScale();
        Vector3 rightScale = right->GetScale();

        if (fabs(rightPos.x - leftPos.x) < (rightScale.x / 2 + leftScale.x / 2)
            && fabs(rightPos.y - leftPos.y) < (rightScale.y / 2 + leftScale.y / 2))
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    void UICollisionManger::ResetCollision()
    {
        const auto& inventoryUI = InventoryManager::GetInventoryUI();
        std::map<UINT64, bool>::iterator iter;

        for (UINT column = 0; column < (UINT)eUIType::End; column++)
        {
            for (UINT row = column; row < (UINT)eUIType::End; row++)
            {
                if (mMatrix[column][row] == true)
                {
                    const std::vector<UI*>& lefts = inventoryUI[(UINT)column];
                    const std::vector<UI*>& rights = inventoryUI[(UINT)row];

                    for (UI* leftObj : lefts)
                    {
                        UICollider* leftCol = leftObj->GetComponent<UICollider>();

                        if (leftCol == nullptr)
                        {
                            continue;
                        }

                        for (UI* rightObj : rights)
                        {
                            UICollider* rightCol = rightObj->GetComponent<UICollider>();

                            if (rightCol == nullptr || leftObj == rightObj)
                            {
                                continue;
                            }

                            ColliderID ID;

                            ID.left = leftCol->GetColliderID();
                            ID.right = rightCol->GetColliderID();

                            iter = mCollisionMap.find(ID.id);

                            if (iter == mCollisionMap.end())
                            {
                                continue;
                            }
                            else if (iter->second)
                            {
                                leftCol->OnCollisionExit(rightCol);
                                rightCol->OnCollisionExit(leftCol);
                                iter->second = false;
                            }
                        }
                    }
                }
            }
        }
    }

    void UICollisionManger::SetCollisionUI(eUIType left, eUIType right, bool enable)
    {
        UINT row = -1;
        UINT col = -1;

        UINT iLeft = (UINT)left;
        UINT iRight = (UINT)right;

        if (iLeft > iRight)
        {
            row = iLeft;
            col = iRight;
        }
        else
        {
            row = iRight;
            col = iLeft;
        }

        mMatrix[col][row] = enable;
    }

    void UICollisionManger::Clear()
    {
        mMatrix->reset();
        mCollisionMap.clear();
    }
}