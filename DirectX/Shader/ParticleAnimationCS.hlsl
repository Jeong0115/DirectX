#include "Common.hlsli"
#include "Particle.hlsli"

RWStructuredBuffer<ParticleAnimation> ParticleBuffer : register(u2);
RWStructuredBuffer<ParticleAnimationShared> SharedBuffer : register(u3);

[numthreads(128, 1, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    if (elementCount <= DTid.x)
        return;
    
    ParticleAnimation particle = ParticleBuffer[DTid.x];
    
    if (particle.active == 1)
    {
        particle.lifeTime -= deltaTime;
        particle.time += deltaTime;
        
        if (particle.lifeTime <= 0)
        {
            particle.active = 0;
            ParticleBuffer[DTid.x] = particle;
            return;
        }
        else
        {
            if (particle.time >= SharedBuffer[0].durtaion)
            {
                particle.time -= SharedBuffer[0].durtaion;
                particle.index += 1;
                
                if (particle.index >= SharedBuffer[0].maxAnimationCnt)
                {
                    particle.index = 0;
                }
                particle.ImageRate = 1 / SharedBuffer[0].maxAnimationCnt;

            }
            particle.position += particle.velocity * particle.speed * deltaTime;
            ParticleBuffer[DTid.x] = particle;
        }
    }
    else if (SharedBuffer[0].RemainingActiveCount > 0)
    {
        int count;
        InterlockedAdd(SharedBuffer[0].RemainingActiveCount, -1, count);
        
        if (count > 0)
        {
            float4 random = (float4) 0.f;
            
            ParticleAnimationShared sharedBuffer = SharedBuffer[0];            
            particle.position = sharedBuffer.curPosition;
            
            float2 uv = float2((float) DTid.x / elementCount, 0.5f);
            uv.x += deltaTime * 154.216f;
            uv.y += sin((uv.x + deltaTime * 126.276f) * 3.141592f + 2.f * 10.f) * 0.5f;
            
            random = float4
            (
                  GaussianBlur(uv + float2(0.f, 0.f)).x
                , GaussianBlur(uv + float2(0.1f, 0.f)).x
                , GaussianBlur(uv + float2(0.2f, 0.f)).x
                , GaussianBlur(uv + float2(0.3f, 0.f)).x
            );
            
            float2 vel = float2(0.0f, random.x * 10.f);
            
            particle.position.x += random.y * 30.f * (random.w > 0.5f ? 1 : -1);
            particle.position.y += random.z * 30.f;
            
            particle.velocity.x = 0.0f;
            particle.velocity.y = 100.f;
            
            particle.active = 1;
            particle.lifeTime = 10.0f;
            particle.time = 0.0f;
            particle.speed = 1.0f;
            particle.index = 0.0f;
            particle.scale = float4(16.0f, 16.0f, 1.0f, 0.0f);
            
            ParticleBuffer[DTid.x] = particle;
        }
    }
}