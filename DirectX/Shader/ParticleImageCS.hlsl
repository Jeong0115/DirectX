#include "Common.hlsli"
#include "Particle.hlsli"

void CreateParticle(int2 id);

[numthreads(32, 32, 1)]
void main( uint3 DTid : SV_DispatchThreadID )
{
    ParticleImageShared sharedBuffer = ParticleImageSharedBuffer[0];
    
    if (DTid.x < sharedBuffer.imageSize.x || DTid.y < sharedBuffer.imageSize.y)
    {
        int index = ((int)(sharedBuffer.imageSize.x / 32.f) + 1) * 32 * DTid.y + DTid.x;
        Particle particle = ParticleBuffer[index];

        if (particle.active == 1)
        {           
            if (particle.active == 1)
            {
                particle.lifeTime -= deltaTime;
        
                if (particle.lifeTime <= 0)
                {
                    particle.active = 0;
                    ParticleBuffer[index] = particle;
                    return;
                }
                else
                {
                    particle.position.xy += particle.velocity.xy * deltaTime;
                    ParticleBuffer[index] = particle;
                }
            }
        }
        else
        {
            float3 color = particleTexture[DTid.xy].rgb;
            
            if (!all(color.rb == float2(0, 0)))
            {
                float green = color.g;

                if (green >= colorRange.x && green < colorRange.y)
                {
                    CreateParticle(DTid.xy);
                }

            }
        }
    }

}
   
void CreateParticle(int2 id)
{
    float2 uv = float2((float) id.x / id.y, 0.5f);
    uv.x += deltaTime * 154.216f * id.y;
    uv.y += sin((uv.x + deltaTime * 126.276f) * 3.141592f + 2.f * 10.f) * 0.5f * id.x;
            
    float4 random = float4
            (
                  GaussianBlur(uv + float2(0.0f, 0.f)).x, GaussianBlur(uv + float2(0.1f, 0.0f)).x
                , GaussianBlur(uv + float2(0.2f, 0.f)).x, GaussianBlur(uv + float2(0.3f, 0.0f)).x
            );
    
    ParticleImageShared sharedBuffer = ParticleImageSharedBuffer[0];
    Particle particle = ParticleBuffer[((int)(sharedBuffer.imageSize.x / 32.f) + 1) * 32 * id.y + id.x];
    
    float angle = sharedBuffer.angle;
    float2 offset;
    float2 relative = float2(id.x - sharedBuffer.imageSize.x / 2.f, id.y - sharedBuffer.imageSize.y / 2.f);
    
    float2 randomVelocity;
    randomVelocity.x = (sharedBuffer.randVelocityMax.x - sharedBuffer.randVelocityMin.x) * random.z + sharedBuffer.randVelocityMin.x;
    randomVelocity.y = (sharedBuffer.randVelocityMax.y - sharedBuffer.randVelocityMin.y) * random.w + sharedBuffer.randVelocityMin.y;
    
    offset.x = relative.x * cos(angle) - relative.y * sin(angle);
    offset.y = relative.x * sin(angle) + relative.y * cos(angle);
    
    particle.position = sharedBuffer.curPosition + float4(offset.xy, 0, 0);
    particle.color = lerp(sharedBuffer.color_min, sharedBuffer.color_max, random.y);
    particle.lifeTime = (sharedBuffer.randLifeTime.y - sharedBuffer.randLifeTime.x) * random.x + sharedBuffer.randLifeTime.x;
    particle.scale = sharedBuffer.scale;
    particle.lightScale = sharedBuffer.lightScale;
    particle.velocity = randomVelocity;
    particle.active = 1;
    
    ParticleBuffer[((int)(sharedBuffer.imageSize.x / 32.f) + 1) * 32 * id.y + id.x] = particle;
}






// --------------------------------------------------------------------------------------------------------------------

//#include "Common.hlsli"
//#include "Particle.hlsli"

//[numthreads(32, 32, 1)]
//void CreateParticle(int index, int2 id);

//void main(uint3 DTid : SV_DispatchThreadID)
//{
//    ParticleImageShared sharedBuffer = ParticleImageSharedBuffer[0];
    
//    if (DTid.x >= sharedBuffer.imageSize.x || DTid.y >= sharedBuffer.imageSize.y)
//    {
//        float3 color = particleTexture[DTid.xy].rgb;

//        if (all(color.rb != float2(0, 0)))
//        {
//            float green = color.g;

//            if (green >= sharedBuffer.colorRange.x && green < sharedBuffer.colorRange.y)
//            {
//                int count;
//                InterlockedAdd(sharedBuffer.activeCount, -1, count);
//                if (count > 0)
//                {
//                    CreateParticle(count - 1, DTid.xy);
//                }
//            }

//        }
//    }
//    else
//    {
//        int count;
//        InterlockedAdd(sharedBuffer.particleCount, -1, count);
//        if (count > 0)
//        {
//            int index = count - 1;
//            Particle particle = ParticleBuffer[index];
    
//            if (particle.active == 1)
//            {
//                particle.lifeTime -= deltaTime;
        
//                if (particle.lifeTime <= 0)
//                {
//                    particle.active = 0;
//                    ParticleBuffer[index] = particle;
//                    return;
//                }
//                else
//                {
//                    particle.position.xy += particle.velocity.xy * deltaTime;
//                    ParticleBuffer[index] = particle;
//                }
//            }
//        }
//    }

//}
   
//void CreateParticle(int index, int2 id)
//{
//    float2 uv = float2((float) id.x / id.y, 0.5f);
//    uv.x += deltaTime * 154.216f * index;
//    uv.y += sin((uv.x + deltaTime * 126.276f) * 3.141592f + 2.f * 10.f) * 0.5f;
            
//    float4 random = float4
//            (
//                  GaussianBlur(uv + float2(0.0f, 0.f)).x, GaussianBlur(uv + float2(0.1f, 0.0f)).x
//                , GaussianBlur(uv + float2(0.2f, 0.f)).x, GaussianBlur(uv + float2(0.3f, 0.0f)).x
//            );
    
//    ParticleImageShared sharedBuffer = ParticleImageSharedBuffer[0];
//    Particle particle = ParticleBuffer[index];
    
//    float angle = sharedBuffer.angle;
//    float2 offset;
//    float2 relative = float2(id.x - sharedBuffer.imageSize.x / 2.f, id.y - sharedBuffer.imageSize.y / 2.f);
    
//    offset.x = relative.x * cos(angle) - relative.y * sin(angle);
//    offset.y = relative.x * sin(angle) + relative.y * cos(angle);
    
//    particle.position = sharedBuffer.curPosition + float4(offset.xy, 0, 0);
//    particle.color = lerp(sharedBuffer.color_min, sharedBuffer.color_max, random.y);
//    particle.lifeTime = (sharedBuffer.randLifeTime.y - sharedBuffer.randLifeTime.x) * random.x + sharedBuffer.randLifeTime.x;
//    particle.scale = sharedBuffer.scale;
//    particle.lightScale = sharedBuffer.lightScale;
//    particle.active = 1;
    
//    ParticleBuffer[index] = particle;
//}
