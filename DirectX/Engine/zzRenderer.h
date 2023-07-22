#pragma once

#include "zzEngine.h"
#include "zzGraphicsDevice.h"
#include "zzMesh.h"
#include "zzShader.h"
#include "zzConstantBuffer.h"
#include "zzCamera.h"

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

    CBUFFER(ColorCB, CBSLOT_COLOR)
    {
        Vector4 color;
    };

    extern Vertex vertexes[];
    extern Vertex debugVertexes[];
    extern graphics::ConstantBuffer* constantBuffer[(UINT)eCBType::End];

    extern Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerState[];
    extern Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizerStates[];
    extern Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthStencilStates[];
    extern Microsoft::WRL::ComPtr<ID3D11BlendState> blendStates[];

    extern std::vector<zz::Camera*> cameras;
    extern std::vector<DebugMesh> debugMeshs;

    void Initialize();
    void Render();
    void Release();

    void PushDebugMeshAttribute(DebugMesh mesh);
}