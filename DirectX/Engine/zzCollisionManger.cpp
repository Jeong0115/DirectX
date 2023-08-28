#include "zzCollisionManger.h"
#include "zzCollider.h"
#include "zzInventoryManager.h"
#include "zzUI.h"
#include "zzSceneManager.h"
#include "zzLayer.h"
#include "zzScene.h"

namespace zz
{
    std::bitset<(UINT)eUIType::End > CollisionManger::mUIMatrix[(UINT)eUIType::End] = {};
    std::map<UINT64, bool> CollisionManger::mUICollisionMap = {};
    std::bitset<(UINT)eLayerType::End > CollisionManger::mMatrix[(UINT)eLayerType::End] = {};
    std::map<UINT64, bool> CollisionManger::mCollisionMap = {};

    void CollisionManger::Initialize()
    {
    }

    void CollisionManger::Update()
    {
        for (UINT column = 0; column < (UINT)eLayerType::End; column++)
        {
            for (UINT row = column; row < (UINT)eLayerType::End; row++)
            {
                if (mMatrix[column][row] == true)
                {
                    LayerCollision(column, row, false);
                }
            }
        }
    }

    void CollisionManger::UpdateUI()
    {
        for (UINT column = 0; column < (UINT)eUIType::End; column++)
        {
            for (UINT row = column; row < (UINT)eUIType::End; row++)
            {
                if (mUIMatrix[column][row] == true)
                {
                    LayerCollision(column, row, true);
                }
            }
        }
    }

    void CollisionManger::LayerCollision(UINT left, UINT right, bool isUI)
    {
        if (isUI)
        {
            const auto& inventoryUI = InventoryManager::GetInventoryUI();

            const std::vector<UI*>& lefts = inventoryUI[left];
            const std::vector<UI*>& rights = inventoryUI[right];

            for (UI* leftObj : lefts)
            {
                Collider* leftCol = leftObj->GetComponent<Collider>();

                if (leftCol == nullptr)
                {
                    continue;
                }

                for (UI* rightObj : rights)
                {
                    Collider* rightCol = rightObj->GetComponent<Collider>();

                    if (rightCol == nullptr || leftObj == rightObj)
                    {
                        continue;
                    }

                    ColliderCollision(leftCol, rightCol, isUI);
                }
            }
        }
        else
        {
            Scene* activeScene = SceneManager::GetActiveScene();

            const std::vector<GameObject*>& lefts = activeScene->GetLayer((eLayerType)left).GetGameObjects();
            const std::vector<GameObject*>& rights = activeScene->GetLayer((eLayerType)right).GetGameObjects();

            for (GameObject* leftObj : lefts)
            {
                Collider* leftCol = leftObj->GetComponent<Collider>();

                if (leftCol == nullptr)
                {
                    continue;
                }

                for (GameObject* rightObj : rights)
                {
                    Collider* rightCol = rightObj->GetComponent<Collider>();

                    if (rightCol == nullptr || leftObj == rightObj)
                    {
                        continue;
                    }

                    ColliderCollision(leftCol, rightCol, isUI);
                }
            }
        }


    }

    void CollisionManger::ColliderCollision(Collider* left, Collider* right, bool isUI)
    {
        ColliderID id = {};
        id.left = left->GetColliderID();
        id.right = right->GetColliderID();

        std::map<UINT64, bool>::iterator iter = {};

        if(isUI)
        {
            iter = mUICollisionMap.find(id.id);

            if (iter == mUICollisionMap.end())
            {
                mUICollisionMap.insert(std::make_pair(id.id, false));
                iter = mUICollisionMap.find(id.id);
            }
        }
        else
        {
            iter = mCollisionMap.find(id.id);

            if (iter == mCollisionMap.end())
            {
                mCollisionMap.insert(std::make_pair(id.id, false));
                iter = mCollisionMap.find(id.id);
            }
        }

        GameObject* leftObj = left->GetOwner();
        GameObject* rightObj = right->GetOwner();

        if (Intersect(left, right))
        {
            if (iter->second)
            {
                if (leftObj->IsDead() || rightObj->IsDead())
                {
                    left->OnCollisionExit(right);
                    right->OnCollisionExit(left);
                    iter->second = false;
                }
                else
                {
                    left->OnCollisionStay(right);
                    right->OnCollisionStay(left);
                    iter->second = true;
                }
            }
            else
            {
                if (!leftObj->IsDead() && !rightObj->IsDead())
                {
                    left->OnCollisionEnter(right);
                    right->OnCollisionEnter(left);
                    iter->second = true;
                }
            }
        }
        else
        {
            if (iter->second)
            {
                left->OnCollisionExit(right);
                right->OnCollisionExit(left);
                iter->second = false;
            }
        }
    }

    bool CollisionManger::Intersect(Collider* left, Collider* right)
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

    void CollisionManger::ResetCollisionUI()
    {
        const auto& inventoryUI = InventoryManager::GetInventoryUI();
        std::map<UINT64, bool>::iterator iter;

        for (UINT column = 0; column < (UINT)eUIType::End; column++)
        {
            for (UINT row = column; row < (UINT)eUIType::End; row++)
            {
                if (mUIMatrix[column][row] == true)
                {
                    const std::vector<UI*>& lefts = inventoryUI[(UINT)column];
                    const std::vector<UI*>& rights = inventoryUI[(UINT)row];

                    for (UI* leftObj : lefts)
                    {
                        Collider* leftCol = leftObj->GetComponent<Collider>();

                        if (leftCol == nullptr)
                        {
                            continue;
                        }

                        for (UI* rightObj : rights)
                        {
                            Collider* rightCol = rightObj->GetComponent<Collider>();

                            if (rightCol == nullptr || leftObj == rightObj)
                            {
                                continue;
                            }

                            ColliderID ID;

                            ID.left = leftCol->GetColliderID();
                            ID.right = rightCol->GetColliderID();

                            iter = mUICollisionMap.find(ID.id);

                            if (iter == mUICollisionMap.end())
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

    void CollisionManger::SetCollision(eLayerType left, eLayerType right, bool enable)
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
    void CollisionManger::SetCollisionUI(eUIType left, eUIType right, bool enable)
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

        mUIMatrix[col][row] = enable;
    }

    void CollisionManger::Clear()
    {
        mMatrix->reset();
        mCollisionMap.clear();
    }
    void CollisionManger::ClearUI()
    {
        mUIMatrix->reset();
        mUICollisionMap.clear();
    }
}