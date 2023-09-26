#include "zzLevitationEnerge.h"
#include "zzMeshRenderer.h"
#include "zzResourceManager.h"
#include "zzTransform.h"
#include "zzBarBackGround.h"
#include "zzHUD_Icon.h"
#include "zzUIManager.h"

#include "zzRenderer.h"

#include "zzPlayer.h"
#include "zzLevitation.h"

#include "zzEventManager.h"

namespace zz
{
    LevitationEnerge::LevitationEnerge()
        : UI(eUIType::HUD)
        , mBar(nullptr)
        , mEnergeRate(0.f)
    {
        createIcon();
    }

    LevitationEnerge::~LevitationEnerge()
    {
        delete mBar;
    }

    void LevitationEnerge::Initialize()
    {
        MeshRenderer* mesh = AddComponent<MeshRenderer>();
        mesh->SetMaterial(ResourceManager::Find<Material>(L"m_colors_flying_bar"));
        mesh->SetMesh(ResourceManager::Find<Mesh>(L"RectMesh"));

        Transform* tr = GetComponent<Transform>();
        tr->SetScale(Vector3(40.f, 2.f, 1.0f));
        tr->SetPosition(Vector3(585.f, 332.f, 1.0f));

        EventManager::RegisterListener(eEvent::Enenerge_Change, [this](const EvenetData& data) {OnEvent(data); });
        GameObject::Initialize();
      
        createBar();
    }

    void LevitationEnerge::Update()
    {
        GameObject::Update();
    }

    void LevitationEnerge::LateUpdate()
    {
        GameObject::LateUpdate();
        mBar->LateUpdate();
    }

    void LevitationEnerge::Render()
    {
        renderer::SliderCB sliderCB;
        sliderCB.rate = mEnergeRate;

        ConstantBuffer* cb = renderer::constantBuffer[(UINT)eCBType::Slider];
        cb->SetBufferData(&sliderCB);
        cb->BindConstantBuffer(eShaderStage::PS);

        mBar->Render();
        GameObject::Render();
    }

    void LevitationEnerge::OnCollisionEnter(GameObject* other)
    {
    }
    void LevitationEnerge::OnCollisionStay(GameObject* other)
    {
    }
    void LevitationEnerge::OnCollisionExit(GameObject* other)
    {
    }

    void LevitationEnerge::OnEvent(const EvenetData& data)
    {
        mEnergeRate = data.energe;
    }

    void LevitationEnerge::createIcon()
    {
        HUD_Icon* icon = new HUD_Icon();

        MeshRenderer* icon_mesh = icon->AddComponent<MeshRenderer>();
        icon_mesh->SetMaterial(ResourceManager::Find<Material>(L"m_jetpack"));
        icon_mesh->SetMesh(ResourceManager::Find<Mesh>(L"RectMesh"));

        Transform* icon_tr = icon->GetComponent<Transform>();
        icon_tr->SetScale(Vector3(8.f, 8.f, 1.0f));
        icon_tr->SetPosition(Vector3(618.f, 331.f, 1.0f));

        UIManager::AddUIObject(icon, eUIType::HUD);
    }

    void LevitationEnerge::createBar()
    {
        mBar = new BarBackGround();

        Transform* mBar_tr = mBar->GetComponent<Transform>();
        mBar_tr->SetScale(Vector3(44.f, 4.f, 1.0f));
        mBar_tr->SetPosition(Vector3(585.f, 332.f, 1.0f));

        mBar->Initialize();
    }
}