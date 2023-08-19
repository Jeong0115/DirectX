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
        Matrix WorldViewProj;
    };

    CBUFFER(AnimatorCB, CBSLOT_ANIMATION2D)
    {
        Vector2 spriteLeftTop;
        Vector2 spriteSize;
        Vector2 spriteOffset;
        Vector2 atlasSize;
        UINT animationType;
    };

    CBUFFER(ColorCB, CBSLOT_COLOR)
    {
        Vector4 color;
    };

    CBUFFER(FlipCB, CBSLOT_FLIP)
    {
        Vector4 flip;
    };

    CBUFFER(ParticleCB, CBSLOT_PARTICLE)
    {
        UINT elementCount;
        float deltaTime;
    };

    CBUFFER(NoiseCB, CBSLOT_NOISE)
    {
        Vector4 textureSize;
    };

    extern Vertex vertexes[];
    extern Vertex debugVertexes[];
    extern ConstantBuffer* constantBuffer[(UINT)eCBType::End];

    extern Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerState[];
    extern Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizerStates[];
    extern Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthStencilStates[];
    extern Microsoft::WRL::ComPtr<ID3D11BlendState> blendStates[];

    extern std::vector<zz::Camera*> cameras;
    extern std::vector<DebugMesh> debugMeshs;
    extern zz::Camera* mainCamera;
    extern zz::Camera* uiCamera;

    void Initialize();
    void Render();
    void Release();

    void PushDebugMeshAttribute(DebugMesh mesh);
}