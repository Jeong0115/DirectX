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
        Vector2 uv;
    };

    CBUFFER(TransformCB, CBSLOT_TRANSFORM)
    {
        Matrix mWorld;
        Matrix mView;
        Matrix mProjection;
    };

    extern Vertex vertexes[];
    extern graphics::ConstantBuffer* constantBuffer[(UINT)eCBType::End];

    void Initialize();
    void Release();
}