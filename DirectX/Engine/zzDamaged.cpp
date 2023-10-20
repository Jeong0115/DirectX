#include "zzDamaged.h"
#include "zzMeshRenderer.h"
#include "zzTexture.h"
#include "zzResourceManager.h"
#include "zzTransform.h"
#include "zzRenderer.h"
#include "zzEventManager.h"
#include "zzTime.h"

namespace zz
{
    Damaged::Damaged()
        : mColor(Vector4::Zero)
    {
    }
    Damaged::~Damaged()
    {
    }
    void Damaged::Initialize()
    {
        std::shared_ptr<Material> material = std::make_shared<Material>();
        material->SetShader(ResourceManager::Find<Shader>(L"DamagedShader"));
        material->SetTexture(ResourceManager::Load<Texture>(L"light_mask_linear", L"..\\Resources\\Texture\\Light\\light_mask_linear.png"));
        MeshRenderer* mesh = AddComponent<MeshRenderer>();
        mesh->SetMaterial(material);
        mesh->SetMesh(ResourceManager::Find<Mesh>(L"RectMesh"));

        GetComponent<Transform>()->SetScale(400.f * 1.2f, 900.f * 0.25f * 1.2f, 1.0f);
        EventManager::RegisterListener(eEvent::Health_Change, [this](const EvenetData& data) { OnEvent(data); });

        GameObject::Initialize();
    }
    void Damaged::Update()
    {
        if (mTime > 0.6f)
        {
            mColor = Vector4(0.0f, 0.0f, 0.0f, 0.0f);
        }
        else if (mTime > 0.3f)
        {
            mColor = Vector4(0.3f - mTime * 0.5f, 0.0f, 0.0f, 0.3f - mTime * 0.5f);
            mTime += (float)Time::DeltaTime();
        }
        else if (mTime >= 0.0f)
        {
            mColor = Vector4(mTime * 0.5f, 0.0f, 0.0f, mTime * 0.5f);
            mTime += (float)Time::DeltaTime();
        }


        GameObject::Update();
    }
    void Damaged::LateUpdate()
    {
        GameObject::LateUpdate();
    }
    void Damaged::Render()
    {
        renderer::ColorCB color = {};
        color.color = mColor;

        ConstantBuffer* cb = renderer::constantBuffer[(UINT)eCBType::Color];
        cb->SetBufferData(&color);
        cb->BindConstantBuffer(eShaderStage::PS);

        GameObject::Render();

        color = {};
        cb->SetBufferData(&color);
        cb->BindConstantBuffer(eShaderStage::PS);
    }

    void Damaged::OnEvent(const EvenetData& data)
    {
        mTime = 0.0f;
    }
}