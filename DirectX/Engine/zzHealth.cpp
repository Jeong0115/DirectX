#include "zzHealth.h"
#include "zzMeshRenderer.h"
#include "zzResourceManager.h"
#include "zzTransform.h"
#include "zzBarBackGround.h"
#include "zzHUD_Icon.h"
#include "zzUIManager.h"
#include "zzEventManager.h"
#include "zzRenderer.h"

namespace zz
{
    Health::Health()
        : UI(eUIType::HUD)
        , mBar(nullptr)
        , mHealthRate(1.0f)
    {
        createIcon();
    }

    Health::~Health()
    {
        delete mBar;
    }

    void Health::Initialize()
    {
        MeshRenderer* mesh = AddComponent<MeshRenderer>();
        mesh->SetMaterial(ResourceManager::Find<Material>(L"m_colors_health_bar"));
        mesh->SetMesh(ResourceManager::Find<Mesh>(L"RectMesh"));

        Transform* tr = GetComponent<Transform>();
        tr->SetScale(Vector3(40.f, 4.5f, 1.0f));
        tr->SetPosition(Vector3(585.f, 340.f, 1.0f));

        EventManager::RegisterListener(eEvent::Health_Change, [this](const EvenetData& data) {OnEvent(data); });
        GameObject::Initialize();
     
        createBar();
    }

    void Health::Update()
    {
        GameObject::Update();
    }

    void Health::LateUpdate()
    {
        mBar->LateUpdate();
        GameObject::LateUpdate();
    }

    void Health::Render()
    {
        renderer::SliderCB sliderCB;
        sliderCB.rate = mHealthRate;

        
        ConstantBuffer* cb = renderer::constantBuffer[(UINT)eCBType::Slider];
        cb->SetBufferData(&sliderCB);
        cb->BindConstantBuffer(eShaderStage::PS);

        mBar->Render();
        GameObject::Render();
    }

    void Health::OnCollisionEnter(GameObject* other)
    {
    }
    void Health::OnCollisionStay(GameObject* other)
    {
    }
    void Health::OnCollisionExit(GameObject* other)
    {
    }

    void Health::OnEvent(const EvenetData& data)
    {
        mHealthRate = data.health;
    }

    void Health::createIcon()
    {
        HUD_Icon* icon = new HUD_Icon();

        MeshRenderer* icon_mesh = icon->AddComponent<MeshRenderer>();
        icon_mesh->SetMaterial(ResourceManager::Find<Material>(L"m_health"));
        icon_mesh->SetMesh(ResourceManager::Find<Mesh>(L"RectMesh"));

        Transform* icon_tr = icon->GetComponent<Transform>();
        icon_tr->SetScale(Vector3(8.f, 8.f, 1.0f));
        icon_tr->SetPosition(Vector3(618.f, 340.f, 1.0f));

        UIManager::AddUIObject(icon, eUIType::HUD);
    }
    void Health::createBar()
    {
        mBar = new BarBackGround();

        Transform* mBar_tr = mBar->GetComponent<Transform>();
        mBar_tr->SetScale(Vector3(44.f, 5.5f, 1.0f));
        mBar_tr->SetPosition(Vector3(585.f, 339.5f, 1.0f));

        mBar->Initialize();
    }
}