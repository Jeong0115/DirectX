#pragma once

#include <wrl.h>
#include <d3d11.h>
#include <d3dcompiler.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

#include "zzMath.h"

#define CB_GETBINDSLOT(name) __CBUFFERBINDSLOT__##name##__
#define CBUFFER(name, slot) static const int CB_GETBINDSLOT(name) = slot; struct alignas(16) name 

#define CBSLOT_TRANSFORM		0
#define CBSLOT_COLOR		1
#define CBSLOT_ANIMATION2D		3
//#define CBSLOT_PARTICLE			1

enum class eShaderStage
{
    VS,
    HS,
    DS,
    GS,
    PS,
    CS,
    End,
};

enum class eCBType
{
    Transform,
    Color,
    Material,
    Animator,
    End,
};

enum class eSamplerType
{
    Point,
    Anisotropic,
    End,
};

enum class eRenderingMode
{
    Opaque,
    CutOut,
    Transparent,
    End,
};

enum class eRSType
{
    SolidBack,
    SolidFront,
    SolidNone,
    WireframeNone,
    End,
};

enum class eDSType
{
    Less,
    Greater,
    NoWrite,
    None,
    End,
};

enum class eBSType
{
    Default,
    AlphaBlend,
    OneOne,
    End,
};

struct GpuBuffer
{
    Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;
    D3D11_BUFFER_DESC desc;

    GpuBuffer()
        : buffer(nullptr)
        , desc{}
    {
    }

    virtual ~GpuBuffer() = default;
};

struct DebugMesh
{
   // enums::eColliderType type;
    int temp = 0;
    zz::math::Vector3 position;
    zz::math::Vector3 rotation;
    zz::math::Vector3 scale;

    float radius;
    float duration;
    float time;
};