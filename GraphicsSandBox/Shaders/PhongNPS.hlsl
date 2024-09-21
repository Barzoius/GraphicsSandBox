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
    bool hasNMap;
    float padding[1];
};

cbuffer TransformCBuf
{
    matrix modelView;
    matrix modelViewProj;
};

Texture2D tex;
Texture2D nmap : register(t2);

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
        
        n = nSample * 2.0f - 1.0f;
        n.y = -n.y;

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
    
    const float3 specular = attenuation * (diffuseColor * diffuseIntensity) * specularIntensity *
                            pow(max(0.0f, dot(normalize(-r), normalize(viewPos))), specularPower);

    return float4(saturate((diffuse + ambient) * tex.Sample(splr, tc).rgb + specular), 1.0f);
}