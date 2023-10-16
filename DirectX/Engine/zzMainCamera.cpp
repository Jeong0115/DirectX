#include "zzMainCamera.h"
#include "zzTransform.h"
#include "zzGameObject.h"
#include "zzRenderer.h"
#include "zzScene.h"
#include "zzSceneManager.h"
#include "zzMeshRenderer.h"

#include "zzGraphicsDevice.h"
#include "zzResourceManager.h"
#include "zzApplication.h"
#include "zzBloomManager.h"
#include "zzExplosion_128.h"

#include "zzTransform.h"
#include "zzPixelWorld.h"

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

        DisableDepthStencilState();
        RenderPlayerView();

        SortGameObjects();
        RenderOpaque();

        DisableDepthStencilState();
        RenderCutOut();     
        RenderTransparent();
        EnableDepthStencilState();

        if (Application::LightDisabled) return;

        DisableDepthStencilState();
        ResourceManager::Find<Mesh>(L"LightMesh")->BindBuffer();
        ResourceManager::Find<Shader>(L"AddDarkShader")->BindShaders();

        ID3D11ShaderResourceView* lightSrv = graphics::GetDevice()->GetFinalLightResource();
        ID3D11ShaderResourceView* null = nullptr;

        graphics::GetDevice()->BindShaderResource(eShaderStage::PS, 2, &lightSrv);
        graphics::GetDevice()->DrawIndexed(6, 0, 0);
        graphics::GetDevice()->BindShaderResource(eShaderStage::PS, 2, &null);

        ResourceManager::Find<Mesh>(L"LightMesh")->BindBuffer();
        ResourceManager::Find<Shader>(L"BloomShader")->BindShaders();


        BloomManager::Render();      

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
        mTp.clear();

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
                    if ((*iter)->IsAllowDelete())
                    {
                        iter = gameObjs.erase(iter);
                        continue;
                    }
                    else if ((*iter)->GetActive())
                    {
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
                    else
                    {
                        iter++;
                    }
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

    void MainCamera::RenderPlayerView()
    {
        graphics::GetDevice()->SetFinalLightRenderTarget();

        ResourceManager::Find<Mesh>(L"LightMesh")->BindBuffer();
        ResourceManager::Find<Shader>(L"FinalLightShader")->BindShaders();

        ID3D11ShaderResourceView* lightSrv = graphics::GetDevice()->GetLightMapResource();
        ID3D11ShaderResourceView* null = nullptr;

        graphics::GetDevice()->BindShaderResource(eShaderStage::PS, 2, &lightSrv);
        graphics::GetDevice()->DrawIndexed(6, 0, 0);

        Vector3 pos = Vector3(PixelWorld::mWorldWidth / 2.f, -PixelWorld::mWorldHeight / 2.f, 0.0f);
        Matrix position = Matrix::CreateTranslation(pos);
        Matrix scale = Matrix::CreateScale(Vector3(PixelWorld::mWorldWidth, PixelWorld::mWorldHeight, 1.0f));
        Matrix world = scale * position;
        
        renderer::TransformCB trCB = {};
        
        trCB.mView = Camera::GetGpuViewMatrix();
        trCB.mProjection = Camera::GetGpuProjectionMatrix();
        trCB.WorldViewProj = world * trCB.mView * trCB.mProjection;
        world._11 = 84.f;
        trCB.mWorld = world;

        ConstantBuffer* cb = renderer::constantBuffer[(UINT)eCBType::Transform];
        cb->SetBufferData(&trCB);
        cb->BindConstantBuffer(eShaderStage::VS);
        cb->BindConstantBuffer(eShaderStage::PS);

        ResourceManager::Find<Mesh>(L"RectMesh")->BindBuffer();
        ResourceManager::Find<Shader>(L"ViewShader")->BindShaders();
        
        ID3D11ShaderResourceView* visibilitySrv = graphics::GetDevice()->GetVisibilityResource();
        
        graphics::GetDevice()->BindShaderResource(eShaderStage::PS, 2, &visibilitySrv);
        graphics::GetDevice()->DrawIndexed(6, 0, 0);
        graphics::GetDevice()->BindShaderResource(eShaderStage::PS, 2, &null);

        graphics::GetDevice()->ClearRenderTarget();
    }
}