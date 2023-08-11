#include "Common.hlsli"

struct VSIn
{
    float3 CenterPos : POSITION;
    uint Instance : SV_InstanceID;
};

struct VSOut
{
    float4 CenterPos : SV_Position;
    uint Instance : SV_InstanceID;
};

VSOut main(VSIn In)
{  
    VSOut Out = (VSOut) 0.0f;
        
    Out.CenterPos = mul(float4(particles[In.Instance].position.xyz, 1.0f), WorldViewProj);
    Out.Instance = In.Instance;
    
    return Out;
}