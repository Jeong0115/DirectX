#include "Common.hlsli"

struct VSIn
{
    float3 Pos : POSITION;
    float4 Color : COLOR;
};

struct VSOut
{
    float4 Pos : SV_Position;
    float4 Color : COLOR;
};

VSOut main(VSIn In)
{
    VSOut Out = (VSOut) 0.0f;
    
    Out.Pos = mul(mul(float4(In.Pos, 1.0f), mView), mProjection);
    
    Out.Color = In.Color;
    
    return Out;
}