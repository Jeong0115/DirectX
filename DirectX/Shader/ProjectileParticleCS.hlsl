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
            return;
        }
        else
        {
            particle.position.xy += particle.velocity.xy * deltaTime;
            ParticleBuffer[DTid.x] = particle;
        }
    }
    else if (ProjectileSharedBuffer[0].activeCount > 0)
    {
        int count;
        InterlockedAdd(ProjectileSharedBuffer[0].activeCount, -1, count);
        
        if (count > 0)
        {
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
            ProjectileShared projectile = ProjectileSharedBuffer[0];
            
            particle.position = projectile.curPosition - (projectile.distance * ((float) count / projectile.totalActiveCount));
            
            int index = projectile.index + projectile.totalActiveCount - count;
             
            float angle = projectile.angle;
            
            particle.velocity.x = (random.x + 1.f) * cos(angle) * (random.w > 0.5f ? 1 : -1);
            particle.velocity.y = random.y * sin(angle);
            
            particle.color = projectile.color;
            particle.active = 1;
            particle.lifeTime = 0.1f;
            particle.scale = float4(1.0f, 1.0f, 1.0f, 0.0f);
            
            ParticleBuffer[DTid.x] = particle;
        }
    }
}