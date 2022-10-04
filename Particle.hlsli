cbuffer cbuff0 : register(b0)
{
	//	float4 color; // 色(RGBA)
	matrix mat; // ３Ｄ変換行列
	matrix matBillboard;
};

struct VSOutput
{
	float4 pos : SV_POSITION; // システム用頂点座標
	float scale : TEXCOORD;   //スケール
};

struct GSOutput
{
	float4 svpos : SV_POSITION; // システム用頂点座標
	float2 uv  :TEXCOORD;       // uv値
};