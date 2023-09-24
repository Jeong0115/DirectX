#include "zzWandMana.h"
#include "zzMeshRenderer.h"
#include "zzResourceManager.h"
#include "zzTransform.h"
#include "zzBarBackGround.h"
#include "zzHUD_Icon.h"
#include "zzUIManager.h"

#include "zzRenderer.h"


//#include 

namespace zz
{
    WandMana::WandMana()
        : UI(eUIType::HUD)
        , mBar(nullptr)
    {
        createIcon();
    }

    WandMana::~WandMana()
    {
        delete mBar;
    }

    void WandMana::Initialize()
    {
        MeshRenderer* mesh = AddComponent<MeshRenderer>();
        mesh->SetMaterial(ResourceManager::Find<Material>(L"m_colors_mana_bar"));
        mesh->SetMesh(ResourceManager::Find<Mesh>(L"RectMesh"));

        Transform* tr = GetComponent<Transform>();
        tr->SetScale(Vector3(40.f, 2.f, 1.0f));
        tr->SetPosition(Vector3(585.f, 325.5f, 1.0f));

        GameObject::Initialize();

        createBar();
    }

    void WandMana::Update()
    {
        GameObject::Update();
    }

    void WandMana::LateUpdate()
    {
        GameObject::LateUpdate();
        mBar->LateUpdate();
    }

    void WandMana::Render()
    {
        renderer::SliderCB sliderCB;
        sliderCB.rate = 0.5f;

        ConstantBuffer* cb = renderer::constantBuffer[(UINT)eCBType::Slider];
        cb->SetBufferData(&sliderCB);
        cb->BindConstantBuffer(eShaderStage::PS);

        mBar->Render();
        GameObject::Render();
    }

    void WandMana::OnCollisionEnter(GameObject* other)
    {
    }
    void WandMana::OnCollisionStay(GameObject* other)
    {
    }
    void WandMana::OnCollisionExit(GameObject* other)
    {
    }

    void WandMana::createIcon()
    {
        HUD_Icon* icon = new HUD_Icon();

        MeshRenderer* icon_mesh = icon->AddComponent<MeshRenderer>();
        icon_mesh->SetMaterial(ResourceManager::Find<Material>(L"m_mana"));
        icon_mesh->SetMesh(ResourceManager::Find<Mesh>(L"RectMesh"));

        Transform* icon_tr = icon->GetComponent<Transform>();
        icon_tr->SetScale(Vector3(8.f, 8.f, 1.0f));
        icon_tr->SetPosition(Vector3(618.f, 324.f, 1.0f));

        UIManager::AddUIObject(icon, eUIType::HUD);
    }

    void WandMana::createBar()
    {
        mBar = new BarBackGround();

        Transform* mBar_tr = mBar->GetComponent<Transform>();
        mBar_tr->SetScale(Vector3(44.f, 4.f, 1.0f));
        mBar_tr->SetPosition(Vector3(585.f, 325.f, 1.0f));

        mBar->Initialize();
    }
}