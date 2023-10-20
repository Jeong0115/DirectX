#include "zzBossHP.h"
#include "zzEventManager.h"
#include "zzWrite.h"
#include "zzMeshRenderer.h"
#include "zzTexture.h"
#include "zzResourceManager.h"
#include "zzTransform.h"
#include "zzRenderer.h"

namespace zz
{
    BossHP::BossHP()
        : UI(eUIType::HUD)
    {
    }
    BossHP::~BossHP()
    {
    }
    void BossHP::Initialize()
    {
        Transform* tr = GetComponent<Transform>();
        tr->SetPosition(320.f, 59.f, 0.0f);
        tr->SetScale(3 * 5.0f * 1.2f, 6.0f * 1.2f, 1.0f);

        Texture* tex = WriteManager::WrtieDamage(L"500", Vector3(3 * 5.0f, 6.0f, 1.0f));
        std::shared_ptr<Texture> sharedPtr(tex);

        MeshRenderer* mesh = AddComponent<MeshRenderer>();
        std::shared_ptr<Material> material = std::make_shared<Material>();
        material->SetTexture(sharedPtr);
        material->SetShader(ResourceManager::Find<Shader>(L"SpriteColorShader"));
        mesh->SetMesh(ResourceManager::Find<Mesh>(L"RectMesh"));
        mesh->SetMaterial(material);

        EventManager::RegisterListener(eEvent::Boss_Health, [this](const EvenetData& data) {OnEvent(data); });
        GameObject::Initialize();
    }
    void BossHP::Update()
    {
        if(IsActive())
        {
            GameObject::Update();
        }
    }
    void BossHP::LateUpdate()
    {
        if (IsActive())
        {
            GameObject::LateUpdate();
        }
    }
    void BossHP::Render()
    {
        if (IsActive())
        {
            renderer::ColorCB color;
            color.color = Vector4(0.0f, 0.0f, 0.0f, 1.0f);

            ConstantBuffer* cb = renderer::constantBuffer[(UINT)eCBType::Color];
            cb->SetBufferData(&color);
            cb->BindConstantBuffer(eShaderStage::PS);

            GameObject::Render();

            color = {};
            cb->SetBufferData(&color);
            cb->BindConstantBuffer(eShaderStage::PS);
        }
    }
    void BossHP::OnCollisionEnter(GameObject* other)
    {
    }
    void BossHP::OnCollisionStay(GameObject* other)
    {
    }
    void BossHP::OnCollisionExit(GameObject* other)
    {
    }

    void BossHP::OnEvent(const EvenetData& data)
    {
        int hp = (int)data.health;

        Texture* tex = WriteManager::WrtieDamage(std::to_wstring((int)hp), Vector3(3 * 5.0f, 6.0f, 1.0f));
        std::shared_ptr<Texture> sharedPtr(tex);

        MeshRenderer* mesh = GetComponent<MeshRenderer>();
        mesh->GetMaterial()->SetTexture(sharedPtr);
    }
}