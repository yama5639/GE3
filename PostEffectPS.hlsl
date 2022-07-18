#include"PostEffect.hlsli"
Texture2D<float4>tex:register(t0);

SamplerState smp:register(s0);
float4 main(Output input) : SV_TARGET
{
	float4 texcolor = tex.Sample(smp,input.uv);
    //default
    return float4(texcolor.rgb, 1);
    //bokasi
}