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
    
    outColor = albedoTexture.Sample(pointSampler, In.UV);
    
    if (color.a != 0.0f)
    {
        if ((float)outColor.rgb != 0.0f)
        {
            outColor.rgba *= color.rgba;
        }
    } 
    
    return outColor;
}