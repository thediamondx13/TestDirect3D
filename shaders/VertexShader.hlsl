float4 main(float2 vert : POS0, float2 cur : POS1) : SV_Position
{
    const float mass = 0.05;
    float2 vec = cur - vert;
    vert += vec * min(1, mass / pow(length(vec), 3));
    return float4(vert, 0.0, 1.0);
}