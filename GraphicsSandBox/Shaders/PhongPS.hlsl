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

    float specularIntensity;
    float specularPower;
    float padding[2];
};

Texture2D tex;
SamplerState splr;

float4 main(float3 viewPos : Position, float3 n : Normal, float2 tc: Texcoord) : SV_Target
{
    n = normalize(n); // renormalization
    
    const float3 vToL = lightPos - viewPos;
    const float distToL = length(vToL);
    const float3 dirToL = vToL / distToL;
    
    const float attenuation = 1.0f / (attenuationConst +
                                      attenuationLin * distToL +
                                      attenuationQuad * (distToL * distToL));

    const float diffuse = diffuseColor *
                          diffuseIntensity *
                          attenuation *
                          max(0.0f, dot(dirToL, n));
    
    const float3 w = n * dot(vToL, n);
    const float3 r = w * 2.0f - vToL;
    
    const float3 specular = attenuation * (diffuseColor * diffuseIntensity) * specularIntensity *
                            pow(max(0.0f, dot(normalize(-r), normalize(viewPos))), specularPower);

    return float4(saturate(diffuse + ambient + specular), 1.0f) * tex.Sample(splr, tc);
}

