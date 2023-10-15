#include "Common.hlsli"
#include "Particle.hlsli"

[numthreads(128, 1, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    if (elementCount <= DTid.x)
        return;
    
    Particle particle = ParticleBuffer[DTid.x];
    
    if (particle.active == 1)
    {
        particle.lifeTime -= deltaTime;
        
        if (particle.lifeTime <= 0)
        {
            particle.active = 0;
            ParticleBuffer[DTid.x] = particle;
        }
        else
        {
            particle.position.xy += particle.velocity.xy * deltaTime;
            ParticleBuffer[DTid.x] = particle;
        }
    }
    else if (ParticleSharedBuffer[0].activeCount > 0)
    {
        int count;
        InterlockedAdd(ParticleSharedBuffer[0].activeCount, -1, count);
        
        if (count > 0)
        {
            float2 uv = float2((float) DTid.x / elementCount, 0.5f);
            uv.x += deltaTime * 154.216f * count;
            uv.y += sin((uv.x + deltaTime * 126.276f) * 3.141592f + 2.f * 10.f) * 0.5f;
            
            float4 random = float4
            (
                  GaussianBlur(uv + float2(0.0f, 0.f)).x
                , GaussianBlur(uv + float2(0.1f, 0.f)).x
                , GaussianBlur(uv + float2(0.2f, 0.f)).x
                , GaussianBlur(uv + float2(0.3f, 0.f)).x

            );
            ParticleShared projectile = ParticleSharedBuffer[0];
            
            particle.position = projectile.curPosition - (projectile.distance * ((float) count / projectile.totalActiveCount));
            
            int index = projectile.index + projectile.totalActiveCount - count;            

            particle.lifeTime = (projectile.randLifeTime.y - projectile.randLifeTime.x) * random.x + projectile.randLifeTime.x;           
            particle.color = projectile.color;
            particle.active = 1;
            particle.scale = projectile.scale;
            particle.lightScale = projectile.lightScale;
            
            ParticleBuffer[DTid.x] = particle;
        }
    }
}