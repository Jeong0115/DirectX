#include "zzLight.h"
#include "zzGameObject.h"
#include "zzTransform.h"
#include "zzRenderer.h"
#include "zzMeshRenderer.h"
#include "zzResourceManager.h"
#include "zzApplication.h"

namespace zz
{
    Light::Light()
        : Component(eComponentType::Light)
        , mTransform(nullptr)
        , mLightScale(Vector3::Zero)
    {
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


		std::shared_ptr<Mesh> mesh = ResourceManager::Find<Mesh>(L"RectMesh");
		mesh->BindBuffer();

        ResourceManager::Find<Material>(L"m_light_mask")->Binds();
		mesh->Render();
        ResourceManager::Find<Material>(L"m_light_mask")->Clear();
    }

    void Light::Render()
    {
    }
}