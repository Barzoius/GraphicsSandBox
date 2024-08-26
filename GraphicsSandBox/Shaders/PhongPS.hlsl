cbuffer LightCbuf
{
    float3 lightPos;
};

static const float3 materialColor = { 0.7f, 0.7f, 0.9f };
static const float3 ambient = { 0.15f, 0.15f, 0.15f };
static const float3 diffusseColor = { 1.0f, 1.0f, 1.0f };
static const float diffuseIntensity = 1.0f;
static const float attenuationConst = 1.0f;
static const float attenuationLin = 1.0f;
static const float attenuationQuad = 1.0f;


float4 main(float3 worldPos : Position, float3 n : Normal) : SV_TARGET
{
    const float3 vToL = lightPos - worldPos;
    const float distToL = length(vToL);
    const float3 dirToL = vToL / distToL;
    
    const float attenuation = attenuationConst +
                              attenuationLin * distToL +
                              attenuationQuad * (distToL * distToL);

    const float diffuse = diffusseColor *
                          diffuseIntensity *
                          attenuation *
                          max(0.0f, dot(distToL, n));

    return float4(saturate(diffuse + ambient), 1.0f);
}