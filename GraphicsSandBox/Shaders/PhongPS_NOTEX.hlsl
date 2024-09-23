
cbuffer LightCBuf
{
    float3 lightPos;
    float3 ambient;
    float3 diffuseColor;
    float diffuseIntensity;
    float attConst;
    float attLin;
    float attQuad;
};

cbuffer ObjectCBuf
{
    float4 materialColor;
    float4 specularColor;
    float specularPower;
};


float4 main(float3 viewPos : Position, float3 n : Normal) : SV_Target
{

    n = normalize(n);
    
    const float3 vToL = lightPos - viewPos;
    const float distToL = length(vToL);
    const float3 dirToL = vToL / distToL;

    const float att = 1.0f / (attConst + attLin * distToL + attQuad * (distToL * distToL));

    const float3 diffuse = diffuseColor * diffuseIntensity * att * max(0.0f, dot(dirToL, n));

    const float3 w = n * dot(vToL, n);
    const float3 r = w * 2.0f - vToL;
	
    const float4 specular = att * (float4(diffuseColor, 1.0f) * diffuseIntensity) * specularColor * pow(max(0.0f, dot(normalize(-r), normalize(viewPos))), specularPower);
	
    return saturate(float4(diffuse + ambient, 1.0f) * materialColor + specular);
}
