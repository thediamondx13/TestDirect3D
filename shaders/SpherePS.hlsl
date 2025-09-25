struct Triangle
{
    float3 a, b, c;
};

cbuffer Camera
{
    matrix proj;
    matrix view;
    float3 camLook;
    float3 camPos;
};

const float3 center = { 0.0f, 0.0f, 5.0f };
const float r_s = 1.0f;

float3 getRayDir(float2 uv, float2 resolution)
{
    // Normalize to [-1,1]
    float2 ndc = (uv / resolution) * 2.0f - 1.0f;
    
    // Assume a simple pinhole camera
    float4 rayDir = float4(normalize(float3(ndc.x, ndc.y, 1.0f)), 1.0f);
    return rayDir.xyz;
}

bool RayTriangleIntersect(float3 orig, float3 dir, Triangle tri, out float t)
{
    float3 v0v1 = tri.b - tri.a;
    float3 v0v2 = tri.c - tri.a;
    float3 pvec = cross(dir, v0v2);
    float det = dot(v0v1, pvec);

    if (abs(det) < 1e-8)
        return false;

    float invDet = 1.0 / det;
    float3 tvec = orig - tri.a;
    float u = dot(tvec, pvec) * invDet;
    if (u < 0 || u > 1)
        return false;

    float3 qvec = cross(tvec, v0v1);
    float v = dot(dir, qvec) * invDet;
    if (v < 0 || u + v > 1)
        return false;

    t = dot(v0v2, qvec) * invDet;
    return (t > 0);
}


float4 main( float4 pos : SV_Position ) : SV_TARGET
{
    float2 resolution = float2(1280, 720); // hardcode for now
    float3 rayDir = getRayDir(pos.xy, resolution);
    
    Triangle tr;
    tr.a = mul(mul(float4(-1, -1, 5, 1), view), proj);
    tr.b = mul(mul(float4(0, 1, 5, 1), view), proj);
    tr.c = mul(mul(float4(1, -1, 5, 1), view), proj);
    
    float t;
    if (RayTriangleIntersect(camPos, rayDir, tr, t))
    {
        return float4(
            saturate(abs(rayDir.x) * rayDir.z + 0.8),
            abs(rayDir.y) * rayDir.z * 0.5,
            rayDir.z * 0.5,
            1.0f
        );
    }

    return float4(
        abs(rayDir.x) * rayDir.z,
        abs(rayDir.y) * rayDir.z,
        rayDir.z,
        1.0f
    );
}