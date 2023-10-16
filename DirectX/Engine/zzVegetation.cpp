#include "zzVegetation.h"
#include "zzMeshRenderer.h"
#include "zzResourceManager.h"
#include "zzTransform.h"
#include "zzPixelCollider_Lite.h"
#include "zzTime.h"
#include "zzRenderer.h"
namespace zz
{
    Vegetation::Vegetation()
        : mbDead(false)
        , mTime(0.0f)
        , mAlpha(1.0f)
    {
    }
    Vegetation::~Vegetation()
    {
    }
    void Vegetation::Initialize()
    {
        int rand = randi(10);
        
        MeshRenderer* mesh = AddComponent<MeshRenderer>();
        if (rand <= 8)
        {
            mesh->SetMaterial(ResourceManager::Find<Material>(L"m_mushroom_small_0" + std::to_wstring(++rand)));
            mesh->SetMesh(ResourceManager::Find<Mesh>(L"RectMesh"));
        }
        else
        {
            rand -= 8;

            mesh->SetMaterial(ResourceManager::Find<Material>(L"m_shrub_" + std::to_wstring(rand)));
            mesh->SetMesh(ResourceManager::Find<Mesh>(L"RectMesh"));
        }

        float width = mesh->GetMaterial()->GetTexture()->GetImageWidth();
        float height = mesh->GetMaterial()->GetTexture()->GetImageHeight();

        Transform* trans = GetComponent<Transform>();
        trans->SetScale(width, height, 1.0f);
        trans->ShiftPositionY(height / 2.f - 1.0f);
        trans->SetPositionZ(BACK_PIXEL_WORLD_Z);
        PixelCollider_Lite* pxCollider = AddComponent<PixelCollider_Lite>();

        pxCollider->SetOffest(0.f, -height / 2.f - 1.f, 0.f);
        pxCollider->SetCollisionEvent([this](Element& element) { OnCollision(element); });

        GameObject::Initialize();
    }
    void Vegetation::Update()
    {
        if (mbDead)
        {
            mAlpha -= (float)Time::DeltaTime();

            if (mAlpha <= 0)
            {
                DeleteObject(this, eLayerType::Vegetation);
            }
        }

        GameObject::Update();
    }
    void Vegetation::LateUpdate()
    {
        GameObject::LateUpdate();
    }
    void Vegetation::Render()
    {
        renderer::ColorCB colorCB = {};
        colorCB.color.w = mAlpha;
        ConstantBuffer* cb = renderer::constantBuffer[(UINT)eCBType::Color];
        cb->SetBufferData(&colorCB);
        cb->BindConstantBuffer(eShaderStage::PS);

        GameObject::Render();

        colorCB = {};
        cb->SetBufferData(&colorCB);
        cb->BindConstantBuffer(eShaderStage::PS);
    }

    void Vegetation::OnCollision(Element& element)
    {
        if (element.Type != eElementType::SOLID)
        {
            mbDead = true;
        }
    }
}