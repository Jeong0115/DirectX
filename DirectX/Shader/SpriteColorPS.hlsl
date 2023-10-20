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
    //return In.Color;
    float4 outColor = (float) 0.0f;
    outColor = albedoTexture.Sample(pointSampler, In.UV);
    
    if(outColor.a != 0)
    {
        outColor.rgb = color.rgb;

    }
    
    return outColor;
}