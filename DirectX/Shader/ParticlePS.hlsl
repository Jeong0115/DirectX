#include "Common.hlsli"
#include "Particle.hlsli"

struct GSOut
{
    float4 Pos : SV_Position;
    float4 Color : COLOR;
    float2 UV : TEXCOORD;
    uint Instance : SV_InstanceID;
};

float4 main(GSOut In) : SV_TARGET
{
    float4 color = (float) 0.0f;
    
    if (In.Color.w == 0)
    {
        color = albedoTexture.Sample(pointSampler, In.UV);
    }
    else
    {
        color = In.Color;
    }
    
    return color;
}