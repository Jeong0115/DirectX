#include "zzEndingView.h"
#include "zzPixelWorld.h"
#include "zzGraphicsDevice.h"
#include "zzTransform.h"
#include "zzMeshRenderer.h"
#include "zzRenderer.h"
#include "zzResourceManager.h"

using namespace zz::graphics;
namespace zz
{
    EndingView::EndingView()
    {
    }
    EndingView::~EndingView()
    {
    }
    void EndingView::Initialize()
    {
        renderer = AddComponent<MeshRenderer>();
        renderer->SetMaterial(ResourceManager::Find<Material>(L"l_explosion_hole"));
        renderer->SetMesh(ResourceManager::Find<Mesh>(L"RectMesh"));

    }
    void EndingView::Update()
    {
    }
    void EndingView::LateUpdate()
    {
        Transform* tr = GetComponent<Transform>();
        Vector3 pos = tr->GetPosition();
        pos.x -= PixelWorld::mWorldWidth / 2;
        pos.y += PixelWorld::mWorldHeight / 2;
        pos.y *= 1;
        Matrix position = Matrix::CreateTranslation(pos);
        Matrix scale = Matrix::CreateScale(tr->GetScale());

        Matrix world = scale * position;

        Matrix view = Matrix::Identity;

        float width = PixelWorld::mWorldWidth;
        float height = PixelWorld::mWorldHeight;

        Matrix projection = Matrix::CreateOrthographicLH(width, height, 0.0f, 1000.f);

        renderer::TransformCB trCB = {};
        trCB.mView = Camera::GetGpuViewMatrix();
        trCB.mProjection = Camera::GetGpuProjectionMatrix();
        trCB.WorldViewProj = world * view * projection;

        ConstantBuffer* cb = renderer::constantBuffer[(UINT)eCBType::Transform];
        cb->SetBufferData(&trCB);
        cb->BindConstantBuffer(eShaderStage::VS);
        cb->BindConstantBuffer(eShaderStage::GS);
        cb->BindConstantBuffer(eShaderStage::PS);

        GetDevice()->SetVisibilityRenderTarget();
        renderer->Render();
        GetDevice()->SetLightMapRenderTarget(false);
    }

    void EndingView::Render()
    {
    }
}