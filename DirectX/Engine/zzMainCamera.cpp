#include "zzMainCamera.h"
#include "zzTransform.h"
#include "zzGameObject.h"
#include "zzRenderer.h"
#include "zzScene.h"
#include "zzSceneManager.h"
#include "zzMeshRenderer.h"

namespace zz
{
    bool CompareZSort(GameObject* a, GameObject* b)
    {
        if (a->GetComponent<Transform>()->GetPosition().z
            <= b->GetComponent<Transform>()->GetPosition().z)
            return false;

        return true;
    }


    MainCamera::MainCamera()
        : mLayerMask{}
        , mOpaqueGameObjects{}
        , mCutOutGameObjects{}
        , mTransparentGameObjects{}
    {
        EnableLayerMasks();
    }

    MainCamera::~MainCamera()
    {
    }

    void MainCamera::Initialize()
    {
    }

    void MainCamera::Update()
    {
    }

    void MainCamera::LateUpdate()
    {
        CreateViewMatrix();
        CreateProjectionMatrix(mType);
        RegisterCameraInRenderer();
    }

    void MainCamera::Render()
    {
        View = mView;
        Projection = mProjection;

        SortGameObjects();
        RenderOpaque();

        DisableDepthStencilState();
        RenderCutOut();
        RenderTransparent();
        EnableDepthStencilState();
    }


    void MainCamera::TurnLayerMask(eLayerType type, bool enable)
    {
        mLayerMask.set((UINT)type, enable);
    }

    void MainCamera::SortGameObjects()
    {
        mOpaqueGameObjects.clear();
        mCutOutGameObjects.clear();  
        mTransparentGameObjects.clear(); 

        Scene* scene = SceneManager::GetActiveScene();
        for (size_t i = 0; i < (UINT)eLayerType::End; i++)
        {
            if (mLayerMask[i] == true)
            {
                Layer& layer = scene->GetLayer((eLayerType)i);
                std::vector<GameObject*>& gameObjs = layer.GetGameObjects();
                auto iter = gameObjs.begin();

                for (; iter != gameObjs.end();)
                {
                    if ((*iter)->IsDead())
                    {
                        iter = gameObjs.erase(iter);
                        continue;
                    }

                    MeshRenderer* mr = (*iter)->GetComponent<MeshRenderer>();
                    if (mr == nullptr)
                    {
                        iter++;
                        continue;
                    }

                    std::shared_ptr<Material> mt = mr->GetMaterial();
                    eRenderingMode mode = mt->GetRenderingMode();
                    switch (mode)
                    {
                    case eRenderingMode::Opaque:
                        mOpaqueGameObjects.push_back((*iter));
                        break;
                    case eRenderingMode::CutOut:
                        mCutOutGameObjects.push_back((*iter));
                        break;
                    case eRenderingMode::Transparent:
                        mTransparentGameObjects.push_back((*iter));
                        break;
                    default:
                        break;
                    }
                    iter++;
                }
            }
        }

        std::sort(mCutOutGameObjects.begin(), mCutOutGameObjects.end(), CompareZSort);
        std::sort(mTransparentGameObjects.begin(), mTransparentGameObjects.end(), CompareZSort);
    }
    void MainCamera::RenderOpaque()
    {
        for (GameObject* gameObj : mOpaqueGameObjects)
        {
            if (gameObj == nullptr)
                continue;

            gameObj->Render();
        }
    }
    void MainCamera::RenderCutOut()
    {
        for (GameObject* gameObj : mCutOutGameObjects)
        {
            if (gameObj == nullptr)
                continue;

            gameObj->Render();
        }
    }
    void MainCamera::RenderTransparent()
    {
        for (GameObject* gameObj : mTransparentGameObjects)
        {
            if (gameObj == nullptr)
                continue;

            gameObj->Render();
        }
    }
}