
#include "Light.hlsli"

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

float3 MapNormals(const float3 tan, const float3 bitan, const float3 n, const float2 tc, Texture2D nmap, SamplerState splr)
{
    const float3x3 TBN = float3x3(tan, bitan, n);
        
    const float3 nSample = nmap.Sample(splr, tc).xyz;
        
    float3 tanNormal;
    tanNormal = nSample * 2.0f - 1.0f;

     return normalize(mul(tanNormal, TBN));
}

float4 main(float3 viewPos : Position, float3 n : Normal, float3 tan : Tangent, float3 bitan : Bitangent, float2 tc : Texcoord) : SV_Target
{
    
    
    float4 difftex = tex.Sample(splr, tc);    
    
    #ifdef MASK
    
        clip(difftex.a < 0.1f ? -1 : 1);
        if (dot(n, viewPos) >= 0.0f)
        {
            n = -n;
        }
    
    #endif
    
    n = normalize(n);
    if (hasNMap)
    {  
        n = MapNormals(normalize(tan), normalize(bitan), n, tc, nmap, splr);
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

    return float4(saturate((diffuse + ambient) * difftex.rgb + specular * specularReflectionColor), difftex.a);
}

