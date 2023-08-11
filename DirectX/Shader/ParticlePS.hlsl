#include "Common.hlsli"

struct GSOut
{
    float4 Pos : SV_Position;
    float2 UV : TEXCOORD;
};

float4 main(GSOut In) : SV_TARGET
{
    float4 color = (float) 0.0f;
    color = albedoTexture.Sample(pointSampler, In.UV);
    
    return color;
}