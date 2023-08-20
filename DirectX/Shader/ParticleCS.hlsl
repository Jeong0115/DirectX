#include "Common.hlsli"
#include "Particle.hlsli"

RWStructuredBuffer<Particle> ParticleBuffer : register(u0);
RWStructuredBuffer<Projectile> ProjectileBuffer : register(u1);

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
            particle.position += particle.velocity * particle.speed * deltaTime;
            ParticleBuffer[DTid.x] = particle;
        }
    }
    else if (ProjectileBuffer[0].RemainingActiveCount > 0)
    {
        int count;
        InterlockedAdd(ProjectileBuffer[0].RemainingActiveCount, -1, count);
        
        if (count > 0)
        {
            Projectile projectile = ProjectileBuffer[0];
            
            particle.position = projectile.curPosition - (projectile.distance * ((float) count / projectile.SetActiveCount));
            
            int index = projectile.index + projectile.SetActiveCount - count;
            
            float2 vel = float2(5.0f, sin(index / 10.0f) * 3.f);
            float angle = projectile.angle;
            
            particle.velocity.x = vel.x * cos(angle) - vel.y * sin(angle);
            particle.velocity.y = vel.x * sin(angle) + vel.y * cos(angle);
            
            particle.active = 1;
            particle.lifeTime = 0.7f;
            particle.speed = 5.0f;
            particle.scale = float4(1.0f, 1.0f, 1.0f, 0.0f);
            
            ParticleBuffer[DTid.x] = particle;
        }
    }
}