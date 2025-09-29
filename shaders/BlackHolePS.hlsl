struct Sphere
{
    // info.w = radius
    // info.xyz = pos
    float4 info;
};

cbuffer Camera
{
    matrix camView;
    float3 camPos;
    float3 res;
};

float3 getRay(float2 uv)
{
    // get coords in the NDC space
    float2 rayNDC = (uv / res.xy) * 2.0f - 1.0f;
    
    // invert .y and scale .x by aspect ratio => get clip space coords
    float4 rayClip = float4(rayNDC.x * res.z, -rayNDC.y, 1.0f, 0.0f);
    
    // get the world position with camera view matrix
    float3 rayWorld = mul(rayClip, camView).xyz;
    return normalize(rayWorld);
}

float4 main(float4 pos : SV_Position) : SV_TARGET
{    
    Sphere sp = { float4(0, 0, 8.4f, 1) };
    
    float3 rayDir = getRay(pos.xy);
    float3 step = rayDir / 10.0f;
    float3 rayPos = camPos;
    
    for (int i = 0; i < 500; i++)
    {
        if (distance(rayPos, sp.info.xyz) <= sp.info.w)
            return float4(0.4, 0.2, 0.6, 1);

        rayPos += step;
    }
    
    return float4(0, 0, 0, 1);
}