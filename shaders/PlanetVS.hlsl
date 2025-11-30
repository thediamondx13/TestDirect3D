cbuffer Transform
{
    matrix transform;
};

float4 main(float3 position : POSITION0) : SV_Position
{   
    return mul(float4(position, 1.0f), transform);
}