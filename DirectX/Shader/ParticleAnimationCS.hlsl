#include "Common.hlsli"
#include "Particle.hlsli"

[numthreads(128, 1, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    if (elementCount <= DTid.x)
        return;
    
    ParticleAnimation particle = ParticleAnimationBuffer[DTid.x];
    
    if (particle.active == 1)
    {
        particle.lifeTime -= deltaTime;
        particle.time += deltaTime;
        
        if (particle.lifeTime <= 0)
        {
            particle.active = 0;
            ParticleAnimationBuffer[DTid.x] = particle;
            return;
        }
        else
        {
            if (particle.time >= AnimationSharedBuffer[0].durtaion)
            {
                particle.time -= AnimationSharedBuffer[0].durtaion;
                particle.index += 1;
                
                if (particle.index >= (uint) AnimationSharedBuffer[0].maxAnimationCnt) // 나중에 수정
                {
                    particle.index = 0;
                }
                particle.ImageRate = 1 / AnimationSharedBuffer[0].maxAnimationCnt;

            }
            particle.position += particle.velocity * deltaTime;
            ParticleAnimationBuffer[DTid.x] = particle;
        }
    }
    else if (AnimationSharedBuffer[0].activeCount > 0)
    {
        int count;
        InterlockedAdd(AnimationSharedBuffer[0].activeCount, -1, count);
        
        if (count > 0)
        {
            AnimationShared sharedBuffer = AnimationSharedBuffer[0];
            particle.position = sharedBuffer.curPosition;
            
            float2 uv = float2((float) DTid.x / elementCount, 0.5f);
            uv.x += deltaTime * 154.216f;
            uv.y += sin((uv.x + deltaTime * 126.276f) * 3.141592f + 2.f * 10.f) * 0.5f;
            
            float4 random = float4
            (
                  GaussianBlur(uv + float2(0.f, 0.f)).x
                , GaussianBlur(uv + float2(0.1f, 0.f)).x
                , GaussianBlur(uv + float2(0.2f, 0.f)).x
                , GaussianBlur(uv + float2(0.3f, 0.f)).x
            );
            
            particle.position.x += random.y * 30.f * (random.w > 0.5f ? 1 : -1);
            particle.position.y += random.z * 30.f;
            
            particle.velocity.x = 0.0f;
            particle.velocity.y = 100.f;
            
            particle.active = 1;
            particle.lifeTime = 10.0f;
            particle.time = 0.0f;
            particle.index = 0.0f;
            particle.scale = float4(16.0f, 16.0f, 1.0f, 0.0f);
            
            ParticleAnimationBuffer[DTid.x] = particle;
        }
    }
}