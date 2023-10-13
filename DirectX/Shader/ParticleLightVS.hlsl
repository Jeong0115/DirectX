#include "Common.hlsli"
#include "Particle.hlsli"

struct VSIn
{
    float3 CenterPos : POSITION;
    uint Instance : SV_InstanceID;
};

struct VSOut
{
    float4 CenterPos : SV_Position;
    float4 offset : POSITION;
    float4 Color : COLOR;
    uint Instance : SV_InstanceID;
};
row_major float4x4 CreateWorldMatrix(float4 position, float4 scale, float rotation);

VSOut main(VSIn In)
{
    VSOut Out = (VSOut) 0.0f;
    Particle particle = particles[In.Instance];
    
    row_major float4x4 worldViewProj
        = mul(mul(CreateWorldMatrix(particle.position, particle.lightScale, 0.0f), mView), mProjection);

    Out.CenterPos = mul(float4(In.CenterPos, 1.0f), worldViewProj);
    Out.offset = mul(float4(0.5f, 0.5f, 0.0f, 0.0f), worldViewProj);
    Out.Color = particle.color;
    Out.Instance = In.Instance;
    
    return Out;
}

row_major float4x4 CreateWorldMatrix(float4 position, float4 scale, float rotation)
{
    row_major float4x4 scaleMatrix =
    {
        scale.x, 0.0f, 0.0f, 0.0f,
        0.0f, scale.y, 0.0f, 0.0f,
        0.0f, 0.0f, scale.z, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };
    
    float cosTheta = cos(rotation);
    float sinTheta = sin(rotation);
    
    row_major float4x4 rotationMatrix =
    {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };
    
    row_major float4x4 translationMatrix =
    {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        position.x, position.y, position.z, 1.0f
    };

    return mul(mul(scaleMatrix, rotationMatrix), translationMatrix);

}
