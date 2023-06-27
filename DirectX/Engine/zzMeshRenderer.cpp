#include "zzMeshRenderer.h"
#include "zzGameObject.h"
#include "zzTransform.h"
#include "zzRenderer.h"

namespace zz
{
    MeshRenderer::MeshRenderer()
        : Component(eComponentType::MeshRenderer)
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

        mMesh->BindBuffer();
        mMaterial->Binds();
        mMesh->Render();

        mMaterial->Clear();
    }
}