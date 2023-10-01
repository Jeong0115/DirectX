#include "zzTextObject.h"
#include "zzWrite.h"
#include "zzTexture.h"
#include "zzMaterial.h"
#include "zzResourceManager.h"
#include "zzMeshRenderer.h"
#include "zzTransform.h"
#include "zzTime.h"
#include "zzRenderer.h"

namespace zz
{
    TextObject::TextObject()
        : mText(nullptr)
        , mLifeTime(1.5f)
    {
    }
    TextObject::~TextObject()
    {
        if (mText != nullptr)
        {
            //delete mText;
            //mText = nullptr;
        }
    }

    void TextObject::Initialize()
    {
        GameObject::Initialize();
    }

    void TextObject::Update()
    {
        mLifeTime -= static_cast<float>(Time::DeltaTime());

        if (mLifeTime <= 0.0f)
        {
            DeleteObject(this, eLayerType::UI);
        }

        Vector3 pos = GetComponent<Transform>()->GetPosition();
        pos.y += Time::DeltaTime() * 20.f;

        GetComponent<Transform>()->SetPositionY(pos.y);

        GameObject::Update();
    }

    void TextObject::LateUpdate()
    {
        GameObject::LateUpdate();
    }

    void TextObject::Render()
    {
        renderer::ColorCB color;
        color.color = Vector4(1.0f, 1.0f, 0.0f, mLifeTime / 1.5f);

        ConstantBuffer* cb = renderer::constantBuffer[(UINT)eCBType::Color];
        cb->SetBufferData(&color);
        cb->BindConstantBuffer(eShaderStage::PS);

        GameObject::Render();

        color.color = Vector4(0.0f, 0.0f, 0.0f, 0.0f);
        cb->BindConstantBuffer(eShaderStage::PS);
    }

    void TextObject::WriteText(const std::wstring& text, Vector3 scale)
    {
        mText = WriteManager::Wrtie(text, scale);
        std::shared_ptr<Texture> texture(mText);

        std::shared_ptr<Shader> spriteShader = ResourceManager::Find<Shader>(L"TextShader");
        std::shared_ptr<Material> material = std::make_shared<Material>();
        material->SetShader(spriteShader);
        material->SetTexture(texture);
        
        MeshRenderer* mesh = AddComponent<MeshRenderer>();
        mesh->SetMaterial(material);
        mesh->SetMesh(ResourceManager::Find<Mesh>(L"RectMesh"));
    }
}