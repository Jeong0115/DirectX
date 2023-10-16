#include "Common.hlsli"

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

float4 main(VSOut In) : SV_TARGET
{
    float4 color = (float) 0.0f;
    color = lightTexture.Sample(anisotropicSampler, In.UV);

    //if (color.a >= 0.5f)
    //{
    //    color.rgb = float3(1, 1, 1);
    //}
    float a = color.r + color.g + color.b;
    
    if (a >= 1.7f && color.a >= 1.f)
    {
        color.rgb = float3(1, 1, 1);
    }
    return color;
}