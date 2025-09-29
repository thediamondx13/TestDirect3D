cbuffer Transform
{
    matrix transform;
};

float4 main( float3 pos : POS ) : SV_POSITION
{
    return mul(float4(pos, 1.0f), transform);
}