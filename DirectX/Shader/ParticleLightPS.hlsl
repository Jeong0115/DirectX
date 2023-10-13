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
    float4 outColor = (float) 0.0f;
    outColor = albedoTexture.Sample(anisotropicSampler, In.UV);
    
    outColor *= In.Color;
    
    return outColor;
}