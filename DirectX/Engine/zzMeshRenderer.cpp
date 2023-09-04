#include "zzMeshRenderer.h"
#include "zzGameObject.h"
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
    }

    void MeshRenderer::Update()
    {
    }

    void MeshRenderer::LateUpdate()
    {
    }

    void MeshRenderer::Render()
    {
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