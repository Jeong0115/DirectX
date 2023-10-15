#include "Common.hlsli"
#include "Particle.hlsli"

[numthreads(128, 1, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    if (elementCount <= DTid.x)
        return;
    
    Particle particle = ParticleBuffer[DTid.x];
    
    
    if (ParticleCircleSharedBuffer[0].create)
    {
        int count;
        InterlockedAdd(ParticleCircleSharedBuffer[0].particleCnt, -1, count);
        
        if (count > 0)
        {
            float2 uv = float2((float) DTid.x / elementCount, 0.5f);
            uv.x += deltaTime * 154.216f * count;
            uv.y += sin((uv.x + deltaTime * 126.276f) * 3.141592f + 2.f * 10.f) * 0.5f;
            
            float4 random = float4
            (
                  GaussianBlur(uv + float2(0.f, 0.f)).x
                , GaussianBlur(uv + float2(0.1f, 0.f)).x
                , GaussianBlur(uv + float2(0.2f, 0.f)).x
                , GaussianBlur(uv + float2(0.3f, 0.f)).x

            );
            ParticleCircleShared sharedBuffer = ParticleCircleSharedBuffer[0];
            
            if (sharedBuffer.createRate < random.x) return;       

            float radius = sharedBuffer.radius;
            
            float4 origin = sharedBuffer.curPosition;
            
            float y = count / (radius * 2) - radius;
            float x = (count % (radius * 2)) - radius;
            
            float distance = sqrt(x * x + y * y);
            if (distance > radius) return;
            
            particle.startTime = distance / radius * sharedBuffer.createTime;
            particle.velocity.x = (sharedBuffer.randVelocityMax.x - sharedBuffer.randVelocityMin.x) * random.z + sharedBuffer.randVelocityMin.x;
            particle.velocity.y = (sharedBuffer.randVelocityMax.y - sharedBuffer.randVelocityMin.y) * random.w + sharedBuffer.randVelocityMin.y;
            
            particle.lifeTime = (sharedBuffer.randLifeTime.y - sharedBuffer.randLifeTime.x) * random.w + sharedBuffer.randLifeTime.x;
            particle.lightScale = sharedBuffer.lightScale;
            particle.position = sharedBuffer.curPosition + float4(x, y, 0.0f, 0.0f);
            particle.color = lerp(sharedBuffer.color_min, sharedBuffer.color_max, random.y);
            particle.scale = sharedBuffer.scale;
            particle.active = 2;
            
            ParticleBuffer[DTid.x] = particle;
        }
    }
    else if (particle.active >= 1)
    {
        particle.lifeTime -= deltaTime;
        particle.startTime -= deltaTime;
         
        if (particle.startTime > 0.0f)
        {
            ParticleBuffer[DTid.x] = particle;
            return;
        }
        
        particle.active = 1;
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
}