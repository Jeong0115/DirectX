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
    else if (ParticleCircleSharedBuffer[0].create)
    {
        int count;
        InterlockedAdd(ParticleCircleSharedBuffer[0].particleCnt, -1, count);
        
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
            ParticleCircleShared sharedBuffer = ParticleCircleSharedBuffer[0];
            
            float2 randomPosition;
            randomPosition.x = (sharedBuffer.randPositionMax.x - sharedBuffer.randPositionMin.x) * random.x + sharedBuffer.randPositionMin.x;
            randomPosition.y = (sharedBuffer.randPositionMax.y - sharedBuffer.randPositionMin.y) * random.y + sharedBuffer.randPositionMin.y;

            float speed = (sharedBuffer.randSpeed.y - sharedBuffer.randSpeed.x) * random.z + sharedBuffer.randSpeed.x;

            float angle = radians((float)count / (float)sharedBuffer.maxParticleCnt * 360.0f);
            float radius = sharedBuffer.radius;
            
            float2 position;
            position.x = radius * cos(angle) - radius * sin(angle);
            position.y = radius * sin(angle) + radius * cos(angle);
            
            float2 offsetPosition;
            offsetPosition.x = randomPosition.x * cos(angle) - randomPosition.y * sin(angle);
            offsetPosition.y = randomPosition.x * sin(angle) + randomPosition.y * cos(angle);
            
            particle.velocity.x = speed * cos(angle) - speed * sin(angle);
            particle.velocity.y = speed * sin(angle) + speed * cos(angle);
            
            
            particle.lifeTime = (sharedBuffer.randLifeTime.y - sharedBuffer.randLifeTime.x) * random.w + sharedBuffer.randLifeTime.x;
            
            particle.position = sharedBuffer.curPosition + float4(position.xy + offsetPosition.xy, 0.0f, 0.0f);
            particle.color = sharedBuffer.color;
            particle.scale = sharedBuffer.scale;
            particle.active = 1;
            
            ParticleBuffer[DTid.x] = particle;
        }
    }
}