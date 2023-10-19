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
            
            float offsetY = sin(index / 2.f) * 2.0f;
            
            float2 randomPosition;
            randomPosition.x = (sharedBuffer.randPositionMax.x - sharedBuffer.randPositionMin.x) * random.x + sharedBuffer.randPositionMin.x;
            randomPosition.y = (sharedBuffer.randPositionMax.y - sharedBuffer.randPositionMin.y) * random.y + sharedBuffer.randPositionMin.y;
    
            
            float4 offsetPosition = float4(0, 0, 0, 0);
            offsetPosition.x = randomPosition.x * cos(angle) - randomPosition.y * sin(angle);
            offsetPosition.y = randomPosition.x * sin(angle) + randomPosition.y * cos(angle);
            
            particle.velocity.x = -offsetY * sin(angle) * sharedBuffer.randVelocityMax.x;
            particle.velocity.y = offsetY * cos(angle) * sharedBuffer.randVelocityMax.x;
            
            
            particle.lifeTime = (sharedBuffer.randLifeTime.y - sharedBuffer.randLifeTime.x) * random.w + sharedBuffer.randLifeTime.x;      
            particle.position = sharedBuffer.curPosition - (sharedBuffer.distance * ((float) count / sharedBuffer.totalActiveCount)) + offsetPosition;
            particle.color = lerp(sharedBuffer.color, sharedBuffer.color_max, random.y);
            particle.scale = sharedBuffer.scale;
            particle.lightScale = sharedBuffer.lightScale;
            particle.active = 1;
            
            ParticleBuffer[DTid.x] = particle;
        }
    }
}