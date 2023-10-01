#include "zzWandRecharge.h"
#include "zzMeshRenderer.h"
#include "zzResourceManager.h"
#include "zzTransform.h"
#include "zzBarBackGround.h"
#include "zzHUD_Icon.h"
#include "zzUIManager.h"

#include "zzRenderer.h"
#include "zzEventManager.h"

namespace zz
{
    WandRecharge::WandRecharge()
        : UI(eUIType::HUD)
        , mBar(nullptr)
        , mRechargeRate(0.0f)
        , mbRender(false)
        , mIcon(nullptr)
    {
        createIcon();
    }
    WandRecharge::~WandRecharge()
    {
        delete mBar;
    }

    void WandRecharge::Initialize()
    {
        MeshRenderer* mesh = AddComponent<MeshRenderer>();
        mesh->SetMaterial(ResourceManager::Find<Material>(L"m_colors_reload_bar"));
        mesh->SetMesh(ResourceManager::Find<Mesh>(L"RectMesh"));

        Transform* tr = GetComponent<Transform>();
        tr->SetScale(Vector3(40.f, 2.f, 1.0f));
        tr->SetPosition(Vector3(585.f, 318.f, 1.0f));

        EventManager::RegisterListener(eEvent::ReCharge_Change, [this](const EvenetData& data) { OnEvent(data); });

        GameObject::Initialize();

        createBar();
    }

    void WandRecharge::Update()
    {
        if (mbRender)
        {
            GameObject::Update();
        }
    }

    void WandRecharge::LateUpdate()
    {
        if (mbRender)
        {
            GameObject::LateUpdate();
            mBar->LateUpdate();
        }
    }

    void WandRecharge::Render()
    {
        if(mbRender)
        {
            renderer::SliderCB sliderCB;
            sliderCB.rate = 1.0f - mRechargeRate;

            ConstantBuffer* cb = renderer::constantBuffer[(UINT)eCBType::Slider];
            cb->SetBufferData(&sliderCB);
            cb->BindConstantBuffer(eShaderStage::PS);

            mBar->Render();
            GameObject::Render();

            mbRender = false;
            mIcon->SetActive(false);
        }
    }

    void WandRecharge::OnCollisionEnter(GameObject* other)
    {
    }
    void WandRecharge::OnCollisionStay(GameObject* other)
    {
    }
    void WandRecharge::OnCollisionExit(GameObject* other)
    {
    }

    void WandRecharge::OnEvent(const EvenetData& data)
    {
        mRechargeRate = data.recharge;
        mbRender = true;
        mIcon->SetActive(true);
    }

    void WandRecharge::createIcon()
    {
        mIcon = new HUD_Icon();

        MeshRenderer* icon_mesh = mIcon->AddComponent<MeshRenderer>();
        icon_mesh->SetMaterial(ResourceManager::Find<Material>(L"m_reload"));
        icon_mesh->SetMesh(ResourceManager::Find<Mesh>(L"RectMesh"));

        Transform* icon_tr = mIcon->GetComponent<Transform>();
        icon_tr->SetScale(Vector3(7.f, 7.f, 1.0f));
        icon_tr->SetPosition(Vector3(618.f, 317.f, 1.0f));

        mIcon->SetActive(false);

        UIManager::AddUIObject(mIcon, eUIType::HUD);
    }

    void WandRecharge::createBar()
    {
        mBar = new BarBackGround();

        Transform* mBar_tr = mBar->GetComponent<Transform>();
        mBar_tr->SetScale(Vector3(44.f, 4.f, 1.0f));
        mBar_tr->SetPosition(Vector3(585.f, 318.f, 1.0f));

        mBar->Initialize();
    }
}