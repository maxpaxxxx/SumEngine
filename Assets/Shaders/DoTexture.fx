// Description: simple shader that applies color and transforms the local space vertices into NDC space

cbuffer ConstantBuffer : register(b0)
{
    matrix wvp; // World View Projection
};

Texture2D textureMap : register(t0);    // 
SamplerState textureSampler : register(s0);     // Whenever there is a texture, there is a sampler

struct VS_INPUT
{
    float3 position : POSITION;
    float2 textCoord : TEXCOORD;
};

struct VS_OUTPUT
{
    float4 position : SV_Position;
    float2 textCoord : TEXCOORD;
};

VS_OUTPUT VS(VS_INPUT input)
{
    VS_OUTPUT output;
    output.position = mul(float4(input.position, 1.0f), wvp);
    output.textCoord = input.textCoord;
    return output;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
    return textureMap.Sample(textureSampler, input.textCoord);
}