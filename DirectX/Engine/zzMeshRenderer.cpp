#include "zzMeshRenderer.h"
#include "zzGameObject.h"
#include "zzTransform.h"
#include "zzRenderer.h"
#include "zzAnimator.h"

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
        Transform* tr = GetOwner()->GetComponent<Transform>();
        Vector2 size = mMaterial->GetSize();
        tr->SetScale(size.x, size.y, 1.0f);
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

        mMesh->BindBuffer();
        mMaterial->Binds();

        Animator* animator = GetOwner()->GetComponent<Animator>();
        if (animator)
        {
            animator->Binds();
        }

        mMesh->Render();

        mMaterial->Clear();
    }
}