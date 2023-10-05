#include "zzLight.h"
#include "zzGameObject.h"
#include "zzTransform.h"
#include "zzRenderer.h"
#include "zzMeshRenderer.h"
#include "zzResourceManager.h"
#include "zzApplication.h"
#include "zzTime.h"

namespace zz
{
    Light::Light()
        : Component(eComponentType::Light)
        , mTransform(nullptr)
        , mLightScale(Vector3::Zero)
        , mLightColor(Vector4::One)
        , mbAfterimageEffect(false)
        , mAfterimageDurtaion(0.0f)
        , mbLight(true)
        , mAfterimageEffect{}
    {
        mMaterial = ResourceManager::Find<Material>(L"m_light_mask");
    }

    Light::~Light()
    {
    }

    void Light::Initialize()
    {
        mTransform = GetOwner()->GetComponent<Transform>();
    }

    void Light::Update()
    {

    }
    void Light::LateUpdate()
    {
        if (Application::LightDisabled) return;
   
        Matrix world = Matrix::Identity;
        Matrix scale = Matrix::CreateScale(mLightScale);
        Matrix position = Matrix::CreateTranslation(mTransform->GetPosition());

        Matrix rotation;
        rotation = Matrix::CreateRotationX(0.f);
        rotation *= Matrix::CreateRotationY(0.f);
        rotation *= Matrix::CreateRotationZ(0.f);

        world = scale * rotation * position;

        renderer::TransformCB trCB = {};
        trCB.mWorld = world;
        trCB.mView = Camera::GetGpuViewMatrix();
        trCB.mProjection = Camera::GetGpuProjectionMatrix();
        trCB.WorldViewProj = trCB.mWorld * trCB.mView * trCB.mProjection;

        ConstantBuffer* cb = renderer::constantBuffer[(UINT)eCBType::Transform];
        cb->SetBufferData(&trCB);
        cb->BindConstantBuffer(eShaderStage::VS);

        renderer::ColorCB colorCB = {};
        colorCB.color = mLightColor;

        ConstantBuffer* colorBuff = renderer::constantBuffer[(UINT)eCBType::Color];
        colorBuff->SetBufferData(&colorCB);
        colorBuff->BindConstantBuffer(eShaderStage::PS);

		std::shared_ptr<Mesh> mesh = ResourceManager::Find<Mesh>(L"RectMesh");
		mesh->BindBuffer();

        mMaterial->Binds();

        if (mbLight)
        {
            mesh->Render();
        }

        if (mbAfterimageEffect)
        {
            for (auto iter = mAfterimageEffect.begin(); iter != mAfterimageEffect.end(); )
            {
                (*iter).second += (float)Time::DeltaTime();

                if ((*iter).second >= mAfterimageDurtaion)
                {
                    iter = mAfterimageEffect.erase(iter);
                }
                else
                {
                    trCB.WorldViewProj = (*iter).first * trCB.mView * trCB.mProjection;

                    cb->SetBufferData(&trCB);
                    cb->BindConstantBuffer(eShaderStage::VS);
                    mesh->Render();

                    iter++;
                }
            }

            if (mbLight)
            {
                mAfterimageEffect.push_back({ world, 0.0f });
            }
        }

        mMaterial->Clear(); // 이거 좀 들여다 보자
    }

    void Light::Render()
    {
    }

    void Light::SetLightType(int type)
    {
        switch (type)
        {
        case 0: mMaterial = ResourceManager::Find<Material>(L"m_light_mask");           break;
        case 1: mMaterial = ResourceManager::Find<Material>(L"m_light_mask_smoothed");  break;
        case 2: mMaterial = ResourceManager::Find<Material>(L"m_particle_glow");        break;
        case 3: mMaterial = ResourceManager::Find<Material>(L"m_light_mask_linear");    break;
        default: break;
        }
    }
}