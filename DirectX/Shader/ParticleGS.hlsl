#include "Common.hlsli"

struct VSOut
{
    float4 CenterPos : SV_Position;
    uint Instance : SV_InstanceID;
};

struct GSOut
{
    float4 Pos : SV_Position;
    float2 UV : TEXCOORD;
};

[maxvertexcount(6)]
void main(point VSOut In[1], inout TriangleStream<GSOut> output)
{
    if (particles[In[0].Instance].active == 0)
        return;
    
    GSOut Out[4] = { (GSOut) 0.0f, (GSOut) 0.0f, (GSOut) 0.0f, (GSOut) 0.0f };
       
    float4 offset = mul(float4(0.5f, 0.5f, 0.0f, 0.0f), WorldViewProj);
    
    Out[0].Pos = In[0].CenterPos + offset;
    Out[1].Pos = In[0].CenterPos + float4(-offset.x, offset.y, 0.0f, 0.0f);
    Out[2].Pos = In[0].CenterPos + float4(offset.x, -offset.y, 0.0f, 0.0f);
    Out[3].Pos = In[0].CenterPos - offset;
    
    Out[0].UV = float2(0.0f, 0.0f);
    Out[1].UV = float2(1.0f, 0.0f);
    Out[2].UV = float2(1.0f, 1.0f);
    Out[3].UV = float2(0.0f, 1.0f);
    
    output.Append(Out[0]);
    output.Append(Out[1]);
    output.Append(Out[2]);
    output.RestartStrip();

    output.Append(Out[0]);
    output.Append(Out[2]);
    output.Append(Out[3]);
    output.RestartStrip();
}