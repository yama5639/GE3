cbuffer cbuff0 : register(b0)
{
	//	float4 color; // �F(RGBA)
	matrix mat; // �R�c�ϊ��s��
	matrix matBillboard;
};

struct VSOutput
{
	float4 pos : SV_POSITION; // �V�X�e���p���_���W
	float scale : TEXCOORD;   //�X�P�[��
};

struct GSOutput
{
	float4 svpos : SV_POSITION; // �V�X�e���p���_���W
	float2 uv  :TEXCOORD;       // uv�l
};