#pragma once
#include "Camera.h"
#include "Input.h"

/// <summary>
/// �f�o�b�O�p�J����
/// </summary>
class DebugCamera : public Camera
{
	using XMMATRIX = DirectX::XMMATRIX;
public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="window_width">��ʕ�</param>
	/// <param name="window_height">��ʍ���</param>
	/// <param name="input">����</param>
	DebugCamera(int window_width, int window_height);

	// �X�V
	void Update() override;

	void SetDistance(float distance) {
		this->distance = distance; viewDirty = true;
	}

private:
	// ���̓N���X�̃|�C���^
	Input* input;
	// �J���������_�܂ł̋���
	float distance = 20;
	// �X�P�[�����O
	float scaleX = 1.0f;
	float scaleY = 1.0f;
	// ��]�s��
	XMMATRIX matRot = DirectX::XMMatrixIdentity();
};

