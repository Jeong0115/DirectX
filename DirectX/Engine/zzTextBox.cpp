#include "zzTextBox.h"
#include "zzMeshRenderer.h"
#include "zzResourceManager.h"
#include "zzTransform.h"
#include "zzWrite.h"

namespace zz
{
    TextBox::TextBox(const std::wstring& text, Vector3 scale)
        : UI(eUIType::TextBox)
    {
        mText = WriteManager::Wrtie(text, scale);
    }
    TextBox::~TextBox()
    {
    }

    void TextBox::Initialize()
    {
        MeshRenderer* mesh = AddComponent<MeshRenderer>();
        mesh->SetMaterial(ResourceManager::Find<Material>(L"m_info_box"));
        mesh->SetMesh(ResourceManager::Find<Mesh>(L"RectMesh"));

        GetComponent<Transform>()->SetScale(Vector3(140.f, 120.f, 1.0f));

        GameObject::Initialize();
    }

    void TextBox::Update()
    {
        GameObject::Update();
    }

    void TextBox::LateUpdate()
    {
        GameObject::LateUpdate();
    }

    void TextBox::Render()
    {
        GameObject::Update();
        GameObject::LateUpdate();
        GameObject::Render();

        mText->BindShader(eShaderStage::PS, 0);
        graphics::GetDevice()->DrawIndexed(6, 0, 0);
    }

    void TextBox::OnCollisionEnter(GameObject* other)
    {
    }
    void TextBox::OnCollisionStay(GameObject* other)
    {
    }
    void TextBox::OnCollisionExit(GameObject* other)
    {
    }
}