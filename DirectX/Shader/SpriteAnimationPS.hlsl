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
    float4 outClor = (float4) 0.0f;  
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
   
        outClor = atlasTexture.Sample(pointSampler, UV);
    }
    
    if(color.w != 0)
    {
        outClor.rgb = color.rgb;
    }
    return outClor;
    
    
    //float4 color = float4(0.0f, 0.0f, 0.0f, 0.0f); // √ ±‚»≠
    
    //float2 UV;
    
    //UV = (flip.x == 1) ? float2(1.0 - In.UV.x, In.UV.y) : In.UV;
    //UV.x = (flip.x == 1) ? UV.x - (SpriteSize.x / AtlasSize.x) : UV.x;
    
    //UV = (animationType == 1) ? (SpriteLeftTop / AtlasSize + SpriteOffset / AtlasSize) + (SpriteSize / AtlasSize * UV) : UV;
    //color = (animationType == 1) ? atlasTexture.Sample(pointSampler, UV) : color;
    
    //return color;
}