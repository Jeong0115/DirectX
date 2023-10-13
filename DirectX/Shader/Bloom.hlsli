
RWTexture2D<float4> pingTexture : register(u1);
RWTexture2D<float4> pongTexture : register(u5);

Texture2D inputTexture : register(t1);
const static float weight[13] =
{
    0.004f, 0.017f, 0.040f, 0.082f, 0.117f, 0.148f, 0.168f, 0.148f, 0.117f, 0.082f, 0.040f, 0.017f, 0.004f
};

cbuffer bloom : register(b6)
{   
    int Synchronization = 400 * 255;
}