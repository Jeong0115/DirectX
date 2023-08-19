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
#define CBSLOT_COLOR		    1
#define CBSLOT_ANIMATION2D		3
#define CBSLOT_FLIP		        4
#define CBSLOT_PARTICLE			5
#define CBSLOT_NOISE			6

namespace zz
{
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
        Flip,
        Particle,
        Noise,
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

    enum class eViewType
    {
        None,
        SRV,
        UAV,
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
        math::Vector3 position;
        math::Vector3 rotation;
        math::Vector3 scale;

        float radius;
        float duration;
        float time;
    };

    struct Particle
    {
        math::Vector4 position;
        math::Vector4 velocity;

        float lifeTime;
        float time;
        float speed;

        UINT active;
    };

    struct ParticleShared
    {
        math::Vector4 curPosition;
        math::Vector4 distance;

        float index;
        float angle;

        UINT SetActiveCount;
        UINT RemainingActiveCount;
    };

    struct ParticleAnimation
    {
        math::Vector4 position;
        math::Vector4 velocity;

        float imageRate;
        float lifeTime;
        float time;
        float speed;

        UINT active;
        UINT animationIndex;
    };

    struct ParticleAnimationShared
    {
        math::Vector4 curPosition;
        math::Vector4 gravity;

        float maxAnimationCnt;
        float durtaion;

        UINT SetActiveCount;
        UINT RemainingActiveCount;
    };
}