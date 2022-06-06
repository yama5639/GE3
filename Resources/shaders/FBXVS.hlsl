#include "FBX.hlsli"

VSOutput main(VSInput input)
{
    //�@���Ƀ��[���h�s��ɂ��X�P�[�����O�E��]��K�p
    float4 wnormal = normalize(mul(world, float4(input.normal, 0)));
    //�s�N�Z���V�F�[�_�[�ɓn���l
    VSOutput output;
    //�s��ɂ����W�ϊ�
    output.svpos = mul(mul(viewproj, world), input.pos);
    //���[���h�@�������̃X�e�[�W�ɓn��
    output.normal = wnormal.xyz;
    //���͒l�����̂܂܎��̃X�e�[�W�ɓn��
    output.uv = input.uv;

    return output;
}