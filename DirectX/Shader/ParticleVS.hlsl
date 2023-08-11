#include "Common.hlsli"

struct VSIn
{
    float3 CenterPos : POSITION;
    uint Instance : SV_InstanceID;
};

struct VSOut
{
    float4 CenterPos : SV_Position;
    float4 offset : POSITION;
    uint Instance : SV_InstanceID;
};
row_major float4x4 CreateWorldMatrix(float3 position1, float scale, float rotation);

VSOut main(VSIn In)
{  
    VSOut Out = (VSOut) 0.0f;
     
    row_major float4x4 worldViewProj 
        = mul(mul(CreateWorldMatrix(particles[In.Instance].position.xyz, 1.0f, 0.0f), mView), mProjection);

    Out.CenterPos = mul(float4(In.CenterPos, 1.0f), worldViewProj);
    Out.offset = mul(float4(0.5f, 0.5f, 0.0f, 0.0f), worldViewProj);
    Out.Instance = In.Instance;
    
    return Out;
}

row_major float4x4 CreateWorldMatrix(float3 position1, float scale, float rotation)
{
    row_major float4x4 scaleMatrix =
    {
        scale, 0.0f, 0.0f, 0.0f,
        0.0f, scale, 0.0f, 0.0f,
        0.0f, 0.0f, scale, 0.0f,
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
        position1.x, position1.y, position1.z, 1.0f
    };

    return mul(mul(scaleMatrix, rotationMatrix), translationMatrix);

}
