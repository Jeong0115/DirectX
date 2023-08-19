#include "Common.hlsli"

struct VSOut
{
    float4 CenterPos : SV_Position;
    float4 offset : POSITION;
    uint Instance : SV_InstanceID;
};

struct GSOut
{
    float4 Pos : SV_Position;
    float2 UV : TEXCOORD;
    uint Instance : SV_InstanceID;
};

[maxvertexcount(6)]
void main(point VSOut In[1], inout TriangleStream<GSOut> output)
{
    if (particlesAnimation[In[0].Instance].active == 0)
        return;
    
    GSOut Out[4] = { (GSOut) 0.0f, (GSOut) 0.0f, (GSOut) 0.0f, (GSOut) 0.0f };
        
    Out[0].Pos = In[0].CenterPos + float4(-In[0].offset.x, In[0].offset.y, 0.0f, 0.0f);
    Out[1].Pos = In[0].CenterPos + In[0].offset;
    Out[2].Pos = In[0].CenterPos + float4(In[0].offset.x, -In[0].offset.y, 0.0f, 0.0f);
    Out[3].Pos = In[0].CenterPos - In[0].offset;
    
    Out[0].UV = float2(0.0f, 0.0f);
    Out[1].UV = float2(1.0f, 0.0f);
    Out[2].UV = float2(1.0f, 1.0f);
    Out[3].UV = float2(0.0f, 1.0f);
    
    Out[0].Instance = In[0].Instance;
    Out[1].Instance = In[0].Instance;
    Out[2].Instance = In[0].Instance;
    Out[3].Instance = In[0].Instance;
    
    output.Append(Out[0]);
    output.Append(Out[1]);
    output.Append(Out[2]);
    output.RestartStrip();

    output.Append(Out[0]);
    output.Append(Out[2]);
    output.Append(Out[3]);
    output.RestartStrip();
}