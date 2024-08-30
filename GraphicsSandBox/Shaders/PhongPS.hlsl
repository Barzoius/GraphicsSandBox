cbuffer LightCBuf
{
    float3 lightPos;
    float3 ambient;
    float3 diffuseColor;
    float diffuseIntensity;
    float attenuationConst;
    float attenuationLin;
    float attenuationQuad;
};

cbuffer ObjectCBuf
{
    float3 materialColor;
};

float4 main(float3 worldPos : Position, float3 n : Normal) : SV_Target
{
    const float3 vToL = lightPos - worldPos;
    const float distToL = length(vToL);
    const float3 dirToL = vToL / distToL;
    
    const float attenuation = 1.0f / (attenuationConst +
                                      attenuationLin * distToL +
                                      attenuationQuad * (distToL * distToL));

    const float diffuse = diffuseColor *
                          diffuseIntensity *
                          attenuation *
                          max(0.0f, dot(dirToL, n));

    return float4(saturate((diffuse + ambient) * materialColor), 1.0f);
}

