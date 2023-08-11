cbuffer Transform : register(b0)
{
    row_major matrix mWorld;
    row_major matrix mView;
    row_major matrix mProjection;
    row_major matrix WorldViewProj; // 나중에 딴거 지우자
}

cbuffer Color : register(b1)
{
    float4 color;
}

cbuffer Animator : register(b3)
{
    float2 SpriteLeftTop;
    float2 SpriteSize;
    float2 SpriteOffset;
    float2 AtlasSize;
    uint animationType;
}

cbuffer Flip : register(b4)
{
    float4 flip;
}

cbuffer ParticleSystem : register(b5)
{
    uint elementCount;
    float deltaTime;
}

Texture2D albedoTexture : register(t0);
Texture2D atlasTexture : register(t12);

SamplerState pointSampler : register(s0);
SamplerState anisotropicSampler : register(s1);


struct Particle
{
    float4 position;
    float4 velocity;
    
    float lifeTime;
    float time;
    float speed;
    uint active;
};

struct Projectile
{
    float4 curPosition;
    float4 distance;
    
    float index;
    float angle;
    
    uint SetActiveCount;
    uint RemainingActiveCount;
};

StructuredBuffer<Particle> particles : register(t14);



