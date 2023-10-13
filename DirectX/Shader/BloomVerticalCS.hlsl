#include "Bloom.hlsli"
#include "Common.hlsli"

[numthreads(32, 32, 1)]

void main(uint3 DTid : SV_DispatchThreadID)
{
    if (DTid.x >= 400 || DTid.y >= 225)
        return;
    

    float4 pixelValue = float4(0, 0, 0, 0);

    for (int i = -6; i <= 6; i++)
    {
        //if (DTid.y + i < 0)
        //    continue;
        pixelValue += weight[i + 6] * pingTexture[int2(DTid.x, DTid.y + i)];
    }
   
    pongTexture[DTid.xy] = pixelValue;
}