#include "zzMeshRenderer.h"
#include "zzGameObject.h"
#include "zzTransform.h"
#include "zzRenderer.h"

namespace zz
{
    MeshRenderer::MeshRenderer()
        : Component(eComponentType::MeshRenderer)
        , mMesh(nullptr)
        , mShader(nullptr)
    {
    }

    MeshRenderer::~MeshRenderer()
    {
    }

    void MeshRenderer::Initialize()
    {
    }

    void MeshRenderer::Update()
    {
    }

    void MeshRenderer::LateUpdate()
    {
    }

    void MeshRenderer::Render()
    {
        Transform* tr = GetOwner()->GetComponent<Transform>();
        tr->BindConstantBuffer();

        //mMesh->BindBuffer();
        //mShader->Binds();

        renderer::mesh->BindBuffer();
        renderer::shader->BindShaders();
        graphics::GetDevice()->DrawIndexed(renderer::mesh->GetIndexCount(), 0, 0);
    }
}