

struct VSIn
{
    float3 Pos : POSITION;
    float4 Color : COLOR;
    float2 UV : TEXCOORD;
};

struct VSOut
{
    float4 Pos : SV_Position;
    float4 Color : COLOR;
    float2 UV : TEXCOORD;
};

cbuffer Animator : register(b3)
{
    float2 SpriteLeftTop;
    float2 SpriteSize;
    float2 SpriteOffset;
    float2 AtlasSize;
    uint animationType;
}

Texture2D albedoTexture : register(t0);
Texture2D atlasTexture : register(t12);

SamplerState pointSampler : register(s0);
SamplerState anisotropicSampler : register(s1);

float4 main(VSOut In) : SV_TARGET
{
    float4 color = (float4) 0.0f;
    
    color = albedoTexture.Sample(pointSampler, In.UV);
    
    if (animationType == 1)
    {
        float2 UV = (SpriteLeftTop / AtlasSize + SpriteOffset / AtlasSize) + (SpriteSize / AtlasSize * In.UV);
   
        
        color = atlasTexture.Sample(pointSampler, UV);
    }
    
    return color;
}