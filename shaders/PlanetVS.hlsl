cbuffer Transform
{
    matrix transform;
};

float4 main( float4 pos : POS ) : SV_POSITION
{
    return mul(pos, transform);
}