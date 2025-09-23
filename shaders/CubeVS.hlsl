cbuffer Transform
{
    matrix transform;
};

struct VSOut
{
    float3 color : COLOR;
    float4 pos : SV_POSITION;
};

VSOut main(float3 vertex : POS, uint3 color : COLOR)
{          
    VSOut res;
    
    res.pos = mul(float4(vertex, 1), transform);
    res.color = float3(color) / 255.0f;
        
    return res;
}