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
            uv.x += deltaTime * 154.2816f * count;
            uv.y += sin((uv.x + deltaTime * 126.276f) * 3.141592f + 2.f * 10.f) * 0.5f;
            
            float4 random = float4
            (
                  GaussianBlur(uv + float2(0.0f, 0.f)).x
                , GaussianBlur(uv + float2(0.1f, 0.f)).x
                , GaussianBlur(uv + float2(0.2f, 0.f)).x
                , GaussianBlur(uv + float2(0.3f, 0.f)).x

            );
            ParticleShared projectile = ParticleSharedBuffer[0];
            
            float2 randomPosition;
            randomPosition.x = (projectile.randPositionMax.x - projectile.randPositionMin.x) * random.x + projectile.randPositionMin.x;
            randomPosition.y = (projectile.randPositionMax.y - projectile.randPositionMin.y) * random.y + projectile.randPositionMin.y;

            float2 randomVelocity;
            randomVelocity.x = (projectile.randVelocityMax.x - projectile.randVelocityMin.x) * random.z + projectile.randVelocityMin.x;
            randomVelocity.y = (projectile.randVelocityMax.y - projectile.randVelocityMin.y) * random.w + projectile.randVelocityMin.y;
            
            float angle = projectile.angle;
            
            float4 offsetPosition = float4(0, 0, 0, 0);
            offsetPosition.x = randomPosition.x * cos(angle) - randomPosition.y * sin(angle);
            offsetPosition.y = randomPosition.x * sin(angle) + randomPosition.y * cos(angle);
            
            particle.velocity.x = randomVelocity.x * cos(angle) - randomVelocity.y * sin(angle);
            particle.velocity.y = randomVelocity.x * sin(angle) + randomVelocity.y * cos(angle);
            
            particle.position = projectile.curPosition - (projectile.distance * ((float) count / projectile.totalActiveCount)) + offsetPosition;
            
            random.x = GaussianBlur(uv + float2(0.4f, 0.f)).x;
            
            int index = projectile.index + projectile.totalActiveCount - count;            

            particle.lifeTime = (projectile.randLifeTime.y - projectile.randLifeTime.x) * random.x + projectile.randLifeTime.x;           
            particle.color = lerp(projectile.color, projectile.color_max, random.y);
            particle.active = 1;
            particle.scale = projectile.scale;
            particle.lightScale = projectile.lightScale;
            
            ParticleBuffer[DTid.x] = particle;
        }
    }
}