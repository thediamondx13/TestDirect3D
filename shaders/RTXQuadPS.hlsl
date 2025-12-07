struct BlackHole
{
    float4 info;
};

struct Planet
{
    float4 info;
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
    
    BlackHole bh = { blackHoles[0] };
    
    Planet sp0 = { planets[0] };
    Planet sp1 = { planets[1] };

    float3 photon = camPos;

    for (int i = 0; i < 1000; i++)
    {
        if (distance(photon, sp0.info.xyz) <= sp0.info.w)
        {
            float d = pow(dot(normalize(sp0.info.xyz - photon), ray), 2);
            float3 color = float3(0.5, 0.2, 0.6);
            color = saturate(color * (d + 0.4));
            return float4(color, 1);
        }
        
        if (distance(photon, sp1.info.xyz) <= sp1.info.w)
        {
            float d = pow(dot(normalize(sp1.info.xyz - photon), ray), 2);
            float3 color = float3(1, 0.5, 0.5);
            color = saturate(color * (d + 0.4));
            return float4(color, 1);
        }
        
        if (distance(photon, bh.info.xyz) <= bh.info.w)
            return float4(0, 0, 0, 1);
        
        //if (distance(photon, wh.pos) <= wh.rad)
        //    return float4(1, 1, 1, 1);

        ray = normalize(ray + normalize(bh.info.xyz - photon) * (0.03f / distance(bh.info.xyz, photon)));
        //ray = normalize(ray - normalize(wh.pos - photon) * (0.02f / distance(wh.pos, photon)));
        
        photon += ray * 0.2f;
    }
    
    float4 upColor = float4(0.4f, 0.2f, 0.7f, 1);
    float4 downColor = float4(0.1f, 0.04f, 0.3f, 1);    
    return lerp(downColor, upColor, saturate(photon.y * 0.005f + 0.3f));
}