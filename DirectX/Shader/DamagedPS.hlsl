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
    float4 outColor = (float) 0.0f;
    
    outColor = albedoTexture.Sample(anisotropicSampler, In.UV);
    
    if (color.r == 0.f)
    {
        return float4(0, 0, 0, 0);
    }
    //if (outColor.r + outColor.g + outColor.b > 0.5f)
    //{
    //    return float4(0, 0, 0, 0);
    //}
    
    float4 rColor = (float) 0.0f;
    rColor.a = 1.0f - outColor.r;
    rColor.a += color.a;
    rColor.r = color.r;
    
    return rColor;
}