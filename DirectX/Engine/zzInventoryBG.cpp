#include "zzInventoryBG.h"
#include "zzResourceManager.h"
#include "zzTexture.h"
#include "zzMeshRenderer.h"
#include "zzMaterial.h"
#include "zzTransform.h"

namespace zz
{
    InventoryBG::InventoryBG(eUIType type)
        : UI(type)
    {
        
    }

    InventoryBG::~InventoryBG()
    {
    }
    void InventoryBG::Initialize()
    {
        std::shared_ptr<Texture> texture = ResourceManager::Load<Texture>(L"background", L"..\\Resources\\Texture\\Inventory\\background.png");
        std::shared_ptr<Shader> spriteShader = ResourceManager::Find<Shader>(L"SpriteShader");
        std::shared_ptr<Material> material = std::make_shared<Material>();
        material->SetShader(spriteShader);
        material->SetTexture(texture);
        ResourceManager::Insert(L"m_background", material);

        MeshRenderer* mesh = AddComponent<MeshRenderer>();
        mesh->SetMaterial(ResourceManager::Find<Material>(L"m_background"));
        mesh->SetMesh(ResourceManager::Find<Mesh>(L"RectMesh"));

        GetComponent<Transform>()->SetScale(Vector3(640.f, 360.f, 1.0f));
        GetComponent<Transform>()->SetPosition(Vector3(320.f, 180.f, 10.0f));

        GameObject::Initialize();
    }

    void InventoryBG::Update()
    {
        GameObject::Update();
    }

    void InventoryBG::LateUpdate()
    {
        GameObject::LateUpdate();
    }

    void InventoryBG::Render()
    {
        GameObject::Render();
    }

    void InventoryBG::OnCollisionEnter(GameObject* other)
    {
    }
    void InventoryBG::OnCollisionStay(GameObject* other)
    {
    }
    void InventoryBG::OnCollisionExit(GameObject* other)
    {
    }
}