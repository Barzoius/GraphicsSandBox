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

    bool hasNMap;
    bool hasSpec;
    bool hasGloss;
    float specularPowerConst;
    float3 specColor;
    float specWeight;
};

Texture2D tex;
Texture2D spec;
Texture2D nmap;
SamplerState splr;


float4 main(float3 viewPos : Position, float3 n : Normal, float3 tan : Tangent, float3 bitan : Bitangent, float2 tc : Texcoord) : SV_Target
{
    
    if (hasNMap)
    {
        const float3x3 tanToView = float3x3(
            normalize(tan),
            normalize(bitan),
            normalize(n)
        );
        
        const float3 nSample = nmap.Sample(splr, tc).xyz;
        
        n.x = nSample.x * 2.0f - 1.0f;
        n.y = -nSample.y * 2.0f + 1.0f; // 0.0 is upper right corner !!!!
        n.z = nSample.z * 2.0f - 1.0f;

        n = mul(n, tanToView);
    }
        
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
    
    //const float4 specularSample = spec.Sample(splr, tc);
    //const float3 specularReflectionColor = specularSample.rgb;
    //const float specularPower = pow(2.0f, specularSample.a * 13.0f);
    
    float3 specularReflectionColor;
    float specularPower = specularPowerConst;
    if (hasSpec)
    {
        const float4 specularSample = spec.Sample(splr, tc);
        specularReflectionColor = specularSample.rgb * specWeight;
        if (hasGloss)
        {
            specularPower = pow(2.0f, specularSample.a * 13.0f);
        }
        
    }
    else
    {
        specularReflectionColor = specColor;
    }
    
    const float3 specular = attenuation * (diffuseColor * diffuseIntensity) * pow(max(0.0f, dot(normalize(-r), normalize(viewPos))), specularPower);

    return float4(saturate((diffuse + ambient) * tex.Sample(splr, tc).rgb + specular * specularReflectionColor), 1.0f);
}

