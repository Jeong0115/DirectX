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
        math::Vector4 scale;
        math::Vector4 color;

        math::Vector2 velocity;

        float lifeTime;

        UINT active;
    };

    struct ProjectileShared
    {
        math::Vector4 curPosition;
        math::Vector4 distance;
        math::Vector4 color;

        float index;
        float angle;

        UINT activeCount;
        UINT totalActiveCount;
    };

    struct ParticleAnimation
    {
        math::Vector4 position;
        math::Vector4 velocity;
        math::Vector4 scale;

        float imageRate;
        float lifeTime;
        float time;

        UINT animationIndex;
        UINT active;
    };

    struct AnimationShared
    {
        math::Vector4 curPosition;
        math::Vector4 gravity;

        float maxAnimationCnt;
        float durtaion;

        UINT activeCount;
    };

    struct ParticleShared
    {
        math::Vector4 curPosition;
        math::Vector4 scale;
        math::Vector4 color;

        math::Vector2 randPositionMin;
        math::Vector2 randPositionMax;
        math::Vector2 randVelocityMin;
        math::Vector2 randVelocityMax;
        math::Vector2 randLifeTime;

        UINT activeCount;
    };
}