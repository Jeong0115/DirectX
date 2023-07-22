
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

Texture2D smileTexture : register(t0);

SamplerState pointSampler : register(s0);
SamplerState anisotropicSampler : register(s1);
 


float4 main(VSOut In) : SV_TARGET
{
    //float4 color = float4(0.0f, 1.0f, 0.0f, 1.0f);
    
    return In.Color;
}