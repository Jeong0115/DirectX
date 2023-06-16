#pragma once

#include "zzComponent.h"
#include "zzMesh.h"
#include "zzShader.h"

namespace zz
{
    class MeshRenderer : public Component
    {
    public:
        MeshRenderer();
        virtual ~MeshRenderer();

        virtual void Initialize() override;
        virtual void Update() override;
        virtual void LateUpdate() override;
        virtual void Render() override;

    private:
        Mesh* mMesh;
        Shader* mShader;
    };
}

