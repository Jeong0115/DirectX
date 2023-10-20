#include "zzBossHPSlider.h"
#include "zzMeshRenderer.h"
#include "zzTexture.h"
#include "zzTransform.h"
#include "zzResourceManager.h"
#include "zzEventManager.h"
#include "zzRenderer.h"


namespace zz
{
    BossHPSlider::BossHPSlider()
        : UI(eUIType::HUD)
        , mHPRate(1.0f)
    {
    }
    BossHPSlider::~BossHPSlider()
    {
    }
    void BossHPSlider::Initialize()
    {
        Transform* tr = GetComponent<Transform>();
        tr->SetPosition(320.f, 60.f, 0.01f);
        tr->SetScale(45.f, 8.0f, 1.0f);

        MeshRenderer* mesh = AddComponent<MeshRenderer>();
        std::shared_ptr<Material> material = std::make_shared<Material>();
        material->SetTexture(ResourceManager::Load<Texture>(L"health_slider_front", L"..\\Resources\\Texture\\Centipede\\health_slider_front.png"));
        material->SetShader(ResourceManager::Find<Shader>(L"SliderShader"));
        mesh->SetMesh(ResourceManager::Find<Mesh>(L"RectMesh"));
        mesh->SetMaterial(material);

        EventManager::RegisterListener(eEvent::Boss_Health, [this](const EvenetData& data) {OnEvent(data); });
        GameObject::Initialize();
    }
    void BossHPSlider::Update()
    {
        if (IsActive())
        {
            GameObject::Update();
        }
    }
    void BossHPSlider::LateUpdate()
    {
        if (IsActive())
        {
            GameObject::LateUpdate();
        }
    }
    void BossHPSlider::Render()
    {
        if (IsActive())
        {
            renderer::SliderCB sliderCB;
            sliderCB.rate = mHPRate;

            ConstantBuffer* cb = renderer::constantBuffer[(UINT)eCBType::Slider];
            cb->SetBufferData(&sliderCB);
            cb->BindConstantBuffer(eShaderStage::PS);

            GameObject::Render();

            sliderCB = {};
            cb->SetBufferData(&sliderCB);
            cb->BindConstantBuffer(eShaderStage::PS);
        }
    }
    void BossHPSlider::OnCollisionEnter(GameObject* other)
    {
    }
    void BossHPSlider::OnCollisionStay(GameObject* other)
    {
    }
    void BossHPSlider::OnCollisionExit(GameObject* other)
    {
    }
    void BossHPSlider::OnEvent(const EvenetData& data)
    {
        mHPRate = data.health / 500.f;
    }
}