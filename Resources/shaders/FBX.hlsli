cbuffer cbuff0 : register(b0)
{
    matrix viewproj; //�r���[�v���W�F�N�V�����s��
    matrix world; //���[���h�s��
    float3 cameraPos; //�J�������W�i���[���h���W�j
};

static const int MAX_BONES = 32;
cbuffer skinning:register(b3) {
    matrix matSkinning[MAX_BONES];
}

//�o�[�e�b�N�X�o�b�t�@�[�̓���
struct VSInput
{
    float4 pos : POSITION; //�ʒu
    float3 normal : NORMAL; //���_�@��
    float2 uv : TEXCOORD; //�e�N�X�`���[���W
    uint4 boneIndices : BONEINDICES;    //�{�[���̔ԍ�
    float4 boneWeights : BONEWEIGHTS;   //�{�[���̃X�L���E�F�C�g
};

//���_�V�F�[�_�[����s�N�Z���V�F�[�_�[�ւ̂����Ɏg�p����\����
struct VSOutput
{
    float4 svpos : SV_POSITION; //�V�X�e���p���_���W
    float3 normal : NORMAL; //�@��
    float2 uv : TEXCOORD; //uv�l
};