#include "Bloom.hlsli"
#include "Common.hlsli"

[numthreads(32, 32, 1)]

void main( uint3 DTid : SV_DispatchThreadID )
{
    if (DTid.x >= 400 || DTid.y >= 225)
        return;
    
    float4 pixelValue = float4(0, 0, 0, 0);

    for (int i = -6; i <= 6; i++)
    {
        pixelValue += weight[i + 6] * pongTexture[uint2(DTid.x + i, DTid.y)];
    }
    
    pingTexture[DTid.xy] = pixelValue;
}