struct BlackHole
{
    float4 info;
};

struct Planet
{
    float4 info;
    float3 color;
};

cbuffer Camera
{
    matrix camViewProjInv;
    float3 camPos;
    float3 res;
};

cbuffer BlackHoles
{
    uint bhCount;
    BlackHole blackHoles[100];
};

cbuffer Planets
{
    uint plCount;
    Planet planets[100];
};

float3 getRay(float2 uv)
{
    // screen space -> NDC space
    float2 ndc = float2(
        (uv.x / res.x) * 2.0f - 1.0f,
        1.0f - (uv.y / res.y) * 2.0f
    );

    /* construct clip-space points at
     near (z=0) and far (z=1) planes */
    float4 clipFar = float4(ndc, 1.0f, 1.0f);
    float4 clipNear = float4(ndc, 0.0f, 1.0f);

    // unproject clip space points to world space
    float4 worldNear = mul(clipNear, camViewProjInv);
    float4 worldFar = mul(clipFar, camViewProjInv);

    // perspective divide
    worldNear /= worldNear.w;
    worldFar /= worldFar.w;

    // return normalized ray in world space
    return normalize(worldFar.xyz - worldNear.xyz);
}

float4 main(float4 pos : SV_POSITION) : SV_TARGET
{
    float3 ray = getRay(pos.xy);
    float3 photon = camPos;
    float3 dRay;
    
    for (int i = 0; i < 1000; i++)
    {
        // check if ray collides with a planet
        for (int planetIndex = 0; planetIndex < plCount; planetIndex++)
        {
            Planet sp = planets[planetIndex];
            if (distance(photon, sp.info.xyz) <= sp.info.w)
            {
                float d = pow(dot(normalize(sp.info.xyz - photon), ray), 2);
                return float4(saturate(sp.color * (d + 0.4)), 1);
            }
        }
        
        // check if ray collides with a black hole
        for (int k = 0; k < bhCount; k++)
            if (distance(photon, blackHoles[k].info.xyz) <= blackHoles[k].info.w)
                return float4(0, 0, 0, 1);

   
        // curve the ray towards the black hole
        dRay = float3(0, 0, 0);
        for (int k = 0; k < bhCount; k++)
            dRay += 0.03f * normalize(blackHoles[k].info.xyz - photon) / distance(blackHoles[k].info.xyz, photon);
        
        ray = normalize(ray + dRay);

        photon += ray * 0.2f;
    }
    
    float4 upColor = float4(0.4f, 0.2f, 0.7f, 1);
    float4 downColor = float4(0.1f, 0.04f, 0.3f, 1);
    return lerp(downColor, upColor, saturate(photon.y * 0.005f + 0.3f));
}