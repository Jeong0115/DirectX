#pragma once

#include "zzComponent.h"
#include "zzMesh.h"
#include "zzShader.h"
#include "zzMaterial.h"

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

        void SetMesh(std::shared_ptr<Mesh> mesh) { mMesh = mesh; }
        void SetMaterial(std::shared_ptr<Material> material) { mMaterial = material; }

        std::shared_ptr<Material> GetMaterial() { return mMaterial; }
        std::shared_ptr<Mesh> GetMesh() { return mMesh; }

    private:
        std::shared_ptr<Mesh> mMesh;
        std::shared_ptr<Material> mMaterial;
    };
}

