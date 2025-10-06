struct BlackHole
{
    float3 pos;
    float rad;
};

struct Planet
{
    float3 pos;
    float rad;
};

cbuffer Camera
{
    matrix camViewProjInv;
    float3 camPos;
    float3 res;
};

cbuffer BlackHoles
{
    BlackHole bh[1];
};

cbuffer Planets
{
    Planet pl[1];
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
    
    BlackHole bh = { float3(-10, 0, 20), 1.0f };
    //BlackHole wh = { float3(-10, 0, 40), 1.0f };
    
    Planet sp = { float3(0, 0, 65), 3 };

    float3 photon = camPos;

    for (int i = 0; i < 1000; i++)
    {
        if (distance(photon, sp.pos) <= sp.rad)
        {
            float d = pow(dot(normalize(sp.pos - photon), ray), 2);
            float3 color = float3(0.5, 0.2, 0.6);
            color = saturate(color * (d + 0.4));
            return float4(color, 1);
        }
        
        if (distance(photon, bh.pos) <= bh.rad)
            return float4(0, 0, 0, 1);
        
        //if (distance(photon, wh.pos) <= wh.rad)
        //    return float4(1, 1, 1, 1);

        ray = normalize(ray + normalize(bh.pos - photon) * (0.03f / distance(bh.pos, photon)));
        //ray = normalize(ray - normalize(wh.pos - photon) * (0.02f / distance(wh.pos, photon)));
        
        photon += ray * 0.2f;
    }
    
    float4 upColor = float4(0.4f, 0.2f, 0.7f, 1);
    float4 downColor = float4(0.1f, 0.04f, 0.3f, 1);    
    return lerp(downColor, upColor, saturate(photon.y * 0.005f + 0.3f));
}