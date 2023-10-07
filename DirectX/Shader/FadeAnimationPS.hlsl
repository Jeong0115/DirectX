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
    float4 outColor = (float4) 0.0f;
    float2 UV;
    
    if (flip.x == 1)
    {
    
        UV = float2(1.0 - In.UV.x, In.UV.y);
        UV.x = UV.x - (SpriteSize.x / AtlasSize.x);
    }
    else
    {
        UV = In.UV;
    }
    
    if (animationType == 1)
    {
        UV = (SpriteLeftTop / AtlasSize + SpriteOffset / AtlasSize) + (SpriteSize / AtlasSize * UV);
   
        outColor = atlasTexture.Sample(pointSampler, UV);
    }
    
    outColor.a = color.a;
    
    return outColor;
}