
struct Particle
{
    float4 position;
    float4 velocity;
    float4 scale;
    float4 color;
    
    float lifeTime;
    float time;
    float speed;
    uint active;
};

struct Projectile
{
    float4 curPosition;
    float4 distance;
    float4 color;
    
    float index;
    float angle;
    
    uint SetActiveCount;
    uint RemainingActiveCount;
};

struct ParticleAnimation
{
    float4 position;
    float4 velocity;
    float4 scale;
    
    float ImageRate;
    float lifeTime;
    float time;
    float speed;
    
    uint index;
    uint active;
};

struct ParticleAnimationShared
{
    float4 curPosition;
    float4 gravity;
    
    float maxAnimationCnt;
    float durtaion;

    uint SetActiveCount;
    uint RemainingActiveCount;
};


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