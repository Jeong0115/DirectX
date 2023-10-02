
struct Particle
{
    float4 position;
    float4 scale;
    float4 color;
    
    float2 velocity;
    
    float lifeTime;
    
    uint active;
};

struct ProjectileShared
{
    float4 curPosition;
    float4 distance;
    float4 color;
    
    float2 randLifeTime;
    
    float index;
    float angle;
    
    uint activeCount;
    uint totalActiveCount;
};

struct ParticleAnimation
{
    float4 position;
    float4 velocity;
    float4 scale;
    
    float ImageRate;
    float lifeTime;
    float time;
    
    uint index;
    uint active;
};

struct AnimationShared
{
    float4 curPosition;
    float4 gravity;
    
    float maxAnimationCnt;
    float durtaion;

    uint activeCount;
};

struct ParticleShared
{
    float4 curPosition;
    float4 scale;
    float4 color;
    
    float2 randPositionMin;
    float2 randPositionMax;
    float2 randVelocityMin;
    float2 randVelocityMax;
    float2 randLifeTime;
    
    uint activeCount;
};

struct SineParticleShared
{
    float4 curPosition;
    float4 scale;
    float4 color;
    
    float2 randPositionMin;
    float2 randPositionMax;
    float2 randVelocityMin;
    float2 randVelocityMax;
    float2 randLifeTime;
    
    float angle;
    float index;
    
    uint activeCount;
};

RWStructuredBuffer<Particle> ParticleBuffer : register(u0);
RWStructuredBuffer<ProjectileShared> ProjectileSharedBuffer : register(u1);
RWStructuredBuffer<ParticleAnimation> ParticleAnimationBuffer : register(u2);
RWStructuredBuffer<AnimationShared> AnimationSharedBuffer : register(u3);
RWStructuredBuffer<ParticleShared> ParticleSharedBuffer : register(u4);
RWStructuredBuffer<SineParticleShared> SineParticleSharedBuffer : register(u5);

StructuredBuffer<Particle> particles : register(t14);
StructuredBuffer<ParticleAnimation> particlesAnimation : register(t15);

Texture2D noiseTexture : register(t1);

cbuffer Noise : register(b6) { float4 noiseTextureSize; }

static float GaussianFilter[5][5] =
{
    0.003f, 0.0133f, 0.0219f, 0.0133f, 0.003f,
    0.0133f, 0.0596f, 0.0983f, 0.0596f, 0.0133f,
    0.0219f, 0.0983f, 0.1621f, 0.0983f, 0.0219f,
    0.0133f, 0.0596f, 0.0983f, 0.0596f, 0.0133f,
    0.003f, 0.0133f, 0.0219f, 0.0133f, 0.003f,

};

float4 GaussianBlur(float2 UV)
{
    float4 Out = (float4) 0.0f;
    
    if (1.f < UV.x)
        UV.x = frac(UV.x);
    else if (UV.x < 0.0f)
        UV.x = 1.0f + frac(UV.x);
        
    if (1.f < UV.y)
        UV.y = frac(UV.y);
    else if (UV.y < 0.0f)
        UV.y = 1.0f + frac(UV.y);
    
    int2 iUV = UV * noiseTextureSize.xy;
    iUV -= int2(2, 2);
    
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            int2 idx = int2(iUV.y + i, iUV.x + j);
            Out += noiseTexture[idx] * GaussianFilter[i][j];
        }
    }
    
    return Out;
}