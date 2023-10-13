#include "Bloom.hlsli"
#include "Common.hlsli"

[numthreads(32, 32, 1)]

void main(uint3 DTid : SV_DispatchThreadID)
{
    float4 brightColor = float4(0, 0, 0, 0);

    if (DTid.x >= 400 || DTid.y >= 225)
        return;

    float2 texCoord = float2(DTid.x / 400.0f, DTid.y / 225.0f);
    
    float4 pixelValue = inputTexture.SampleLevel(pointSampler, texCoord, 0);
    //float brightness = max(max(pixelValue.r, pixelValue.g), pixelValue.b);
    //float brightness = pixelValue.r + pixelValue.g + pixelValue.b;
    
    float brightness = pixelValue.a;
    if (brightness > 0.0f)
    {
        brightColor = float4(pixelValue.rgba);
    }
    
    pingTexture[DTid.xy] = brightColor;
}

