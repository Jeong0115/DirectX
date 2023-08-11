#include "Common.hlsli"

RWStructuredBuffer<Particle> ParticleBuffer : register(u0);
RWStructuredBuffer<ParticleShared> ParticleSharedBuffer : register(u1);

[numthreads(128, 1, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    if (elementCount <= DTid.x)
        return;
    
    if (ParticleBuffer[DTid.x].active == 1)
    {
        ParticleBuffer[DTid.x].position += ParticleBuffer[DTid.x].velocity * ParticleBuffer[DTid.x].speed * deltaTime;
    }
    else if (ParticleSharedBuffer[0].ActiveSharedCount > 0)
    {
        int result;
        InterlockedAdd(ParticleSharedBuffer[0].ActiveSharedCount, -1, result);
        
        if (result > 0)
        {
            ParticleBuffer[DTid.x].active = 1;
        }
    }
}