#include "zzInfoBox.h"
#include "zzResourceManager.h"
#include "zzMeshRenderer.h"
#include "zzMaterial.h"
#include "zzTransform.h"
#include "zzSpellSlot.h"
#include "zzUIManager.h"
#include "zzWand.h"
#include "zzWrite.h"
#include "zzRenderer.h"
namespace zz
{
    InfoBox::InfoBox(eUIType type)
        : UI(type)
        , mWandtexture(nullptr)
        , mWand(nullptr)
        , mText(nullptr)
    {
    }

    InfoBox::~InfoBox()
    {
        if (mWandtexture != nullptr)
        {
            delete mWandtexture;
            mWandtexture = nullptr;
        }
        if (mText != nullptr)
        {
            delete mText;
            mText = nullptr;
        }
    }

    void InfoBox::Initialize()
    {
        MeshRenderer* mesh = AddComponent<MeshRenderer>();
        mesh->SetMaterial(ResourceManager::Find<Material>(L"m_info_box"));
        mesh->SetMesh(ResourceManager::Find<Mesh>(L"RectMesh"));

        GetComponent<Transform>()->SetScale(Vector3(150.f, 60.f, 1.0f));

        GameObject::Initialize();
    }

    void InfoBox::Update()
    {
        GameObject::Update();
        mWandtexture->Update();
    }

    void InfoBox::LateUpdate()
    {
        GameObject::LateUpdate();
        mWandtexture->LateUpdate();
    }

    void InfoBox::Render()
    {
        GameObject::Render();
        mWandtexture->Render();

        if(mText != nullptr)
        {
            Matrix mWorld = Matrix::Identity;
            Matrix scale = Matrix::CreateScale(Vector3(150.f * 0.8f, 60.f * 0.8f, 1.0f));
            Matrix rotation;
            rotation = Matrix::CreateRotationX(0.f);
            rotation *= Matrix::CreateRotationY(0.f);
            rotation *= Matrix::CreateRotationZ(0.f);

            Matrix position = Matrix::CreateTranslation(GetComponent<Transform>()->GetPosition());
            mWorld = scale * rotation * position;

            renderer::TransformCB trCB = {};
            trCB.mWorld = mWorld;
            trCB.mView = Camera::GetGpuViewMatrix();
            trCB.mProjection = Camera::GetGpuProjectionMatrix();
            trCB.WorldViewProj = trCB.mWorld * trCB.mView * trCB.mProjection;

            ConstantBuffer* cb = renderer::constantBuffer[(UINT)eCBType::Transform];
            cb->SetBufferData(&trCB);
            cb->BindConstantBuffer(eShaderStage::VS);

            mText->BindShader(eShaderStage::PS, 0);
            graphics::GetDevice()->DrawIndexed(6, 0, 0);
        }

    }

    void InfoBox::LinkWand(Wand* wand, InfoBoxTexture* texture)
    {
        Transform* tr = GetComponent<Transform>();

        mWand = wand;
        mWandtexture = texture;

        mWandtexture->GetComponent<Transform>()->SetParent(tr);
        mWandtexture->GetComponent<Transform>()->SetPosition(-55.f, 10.f, 0.f);

        for (int i = 0; i < mWand->GetCapacity(); i++)
        {
            SpellSlot* spellBox = new SpellSlot(eUIType::SpellSlot);
            spellBox->SetOwnerWand(wand);
            spellBox->SetSlotIndex(i);

            Transform* boxTr = spellBox->GetComponent<Transform>();
            boxTr->SetParent(tr);
            boxTr->SetPosition(-60.f + i * 20.f, -15.f, 0.0f);
            spellBox->Initialize();

            UIManager::AddUIObject(spellBox, eUIType::SpellSlot);
        }

        mText = WriteManager::Wrtie(mWand->GetInfoText(), Vector3(150.f, 60.f, 1.f));
    } 

    void InfoBox::OnCollisionEnter(GameObject* other)
    {
    }

    void InfoBox::OnCollisionStay(GameObject* other)
    {
    }

    void InfoBox::OnCollisionExit(GameObject* other)
    {
    }
}