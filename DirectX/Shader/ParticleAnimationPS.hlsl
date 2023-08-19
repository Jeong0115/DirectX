#include "Common.hlsli"

struct GSOut
{
    float4 Pos : SV_Position;
    float2 UV : TEXCOORD;
    uint Instance : SV_InstanceID;
};

float4 main(GSOut In) : SV_TARGET
{   
    float4 color = (float) 0.0f;
    
    float2 UV = In.UV;
    
    UV.x = (particlesAnimation[In.Instance].ImageRate * particlesAnimation[In.Instance].index + particlesAnimation[In.Instance].ImageRate  * In.UV.x);
   
    color = albedoTexture.Sample(pointSampler, UV);
    
    //float4 color = float4(1.0f, 0.0f, 1.0f, 1.0f);
    
    return color;       
}
