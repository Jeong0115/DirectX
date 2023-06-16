#pragma once

#include "zzEngine.h"
#include "zzGraphicsDevice.h"
#include "zzMesh.h"
#include "zzShader.h"
#include "zzConstantBuffer.h"

using namespace zz::math;
using namespace zz;

namespace zz::renderer
{
    struct Vertex
    {
        Vector3 pos;
        Vector4 color;
    };

    extern Vertex vertexes[];
    extern Mesh* mesh;
    extern Shader* shader;
    extern graphics::ConstantBuffer* constantBuffer;

    void Initialize();
    void Release();
}