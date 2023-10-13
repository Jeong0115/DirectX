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
    else if (ParticleSharedBuffer[0].activeCount > 0)
    {
        int count;
        InterlockedAdd(ParticleSharedBuffer[0].activeCount, -1, count);
        
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
            ParticleShared sharedBuffer = ParticleSharedBuffer[0];
            
            int index = sharedBuffer.index - count;
            float angle = sharedBuffer.angle;
            
            float2 randomPosition;
            randomPosition.x = (sharedBuffer.randPositionMax.x - sharedBuffer.randPositionMin.x) * random.x + sharedBuffer.randPositionMin.x;
            randomPosition.y = (sharedBuffer.randPositionMax.y - sharedBuffer.randPositionMin.y) * random.y + sharedBuffer.randPositionMin.y;

            float2 randomVelocity;
            randomVelocity.x = (sharedBuffer.randVelocityMax.x - sharedBuffer.randVelocityMin.x) * random.z + sharedBuffer.randVelocityMin.x;
            randomVelocity.y = (sharedBuffer.randVelocityMax.y - sharedBuffer.randVelocityMin.y) * random.w + sharedBuffer.randVelocityMin.y;
            
            //randomVelocity.y = sin(index / 2.f) * randomVelocity.y;
            
            float2 offsetPosition;
            float offsetY = sin(index / 2.f) * 2.0f;
            offsetPosition.x = -count * cos(angle) - offsetY * sin(angle);
            offsetPosition.y = -count * sin(angle) + offsetY * cos(angle);
            
            particle.velocity.x = randomVelocity.x * cos(angle) - randomVelocity.y * sin(angle);
            particle.velocity.y = randomVelocity.x * sin(angle) + randomVelocity.y * cos(angle);
            
            random.x = GaussianBlur(uv + float2(0.4f, 0.f)).x;
            
            particle.lifeTime = (sharedBuffer.randLifeTime.x - sharedBuffer.randLifeTime.y) * random.x + sharedBuffer.randLifeTime.y;
            
            particle.position = sharedBuffer.curPosition + float4(randomPosition.xy, 0.0f, 0.0f);
            particle.position.xy += offsetPosition;
            particle.color = sharedBuffer.color;
            particle.scale = sharedBuffer.scale;
            particle.active = 1;
            
            ParticleBuffer[DTid.x] = particle;
        }
    }
}