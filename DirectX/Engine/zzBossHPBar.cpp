#include "zzBossHPBar.h"
#include "zzMeshRenderer.h"
#include "zzTexture.h"
#include "zzTransform.h"
#include "zzResourceManager.h"
#include "zzBossHP.h"
#include "zzBossHPIcon.h"
#include "zzBossHPSlider.h"
#include "zzUIManager.h"
#include "zzRenderer.h"
namespace zz
{
    BossHPBar::BossHPBar()
        : UI(eUIType::HUD)
        , mHPSlider(nullptr)
        , mHPIcon(nullptr)
        , mHP(nullptr)
    {
    }
    BossHPBar::~BossHPBar()
    {
    }
    void BossHPBar::Initialize()
    {
        Transform* tr = GetComponent<Transform>();
        tr->SetPosition(320.f, 60.f, 0.01f);
        tr->SetScale(50.f, 11.0f, 1.0f);

        MeshRenderer* mesh = AddComponent<MeshRenderer>();
        std::shared_ptr<Material> material = std::make_shared<Material>();
        material->SetTexture(ResourceManager::Load<Texture>(L"health_slider_back", L"..\\Resources\\Texture\\Centipede\\health_slider_back.png"));
        material->SetShader(ResourceManager::Find<Shader>(L"SpriteColorShader"));
        mesh->SetMesh(ResourceManager::Find<Mesh>(L"RectMesh"));
        mesh->SetMaterial(material);
        GameObject::Initialize();

        mHPSlider = new BossHPSlider();
        mHPSlider->Initialize();
        UIManager::AddUIObject(mHPSlider, eUIType::HUD);

        mHPIcon = new BossHPIcon();
        mHPIcon->Initialize();
        UIManager::AddUIObject(mHPIcon, eUIType::HUD);

        mHP = new BossHP();
        mHP->Initialize();
        UIManager::AddUIObject(mHP, eUIType::HUD);
    }
    void BossHPBar::Update()
    {
        if (IsActive())
        {
            GameObject::Update();
        }
    }
    void BossHPBar::LateUpdate()
    {
        if (IsActive())
        {
            GameObject::LateUpdate();
        }
    }
    void BossHPBar::Render()
    {
        if (IsActive())
        {
            renderer::ColorCB color;
            color.color = Vector4(0.8f, 0.8f, 0.8f, 0.6f);

            ConstantBuffer* cb = renderer::constantBuffer[(UINT)eCBType::Color];
            cb->SetBufferData(&color);
            cb->BindConstantBuffer(eShaderStage::PS);

            GameObject::Render();

            color = {};
            cb->SetBufferData(&color);
            cb->BindConstantBuffer(eShaderStage::PS);
        }
    }
    void BossHPBar::SetDead()
    {
        SetState(eState::Dead);

        mHP->SetState(eState::Dead);
        mHPSlider->SetState(eState::Dead);
        mHPIcon->SetState(eState::Dead);
    }
}