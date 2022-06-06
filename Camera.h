#pragma once

#include <DirectXMath.h>

/// <summary>
/// �J������{�@�\
/// </summary>
class Camera
{
protected: // �G�C���A�X
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

public:

	Camera(int window_width, int window_height);

	virtual ~Camera() = default;

	virtual void Update();

	void UpdateViewMatrix();

	void UpdateProjectionMatrix();

	inline const XMMATRIX& GetViewMatrix() {
		return matView;
	}

	inline const XMMATRIX& GetProjectionMatrix() {
		return matProjection;
	}

	inline const XMMATRIX& GetViewProjectionMatrix() {
		return matViewProjection;
	}

	inline const XMMATRIX& GetBillboardMatrix() {
		return matBillboard;
	}

	inline const XMFLOAT3& GetEye() {
		return eye;
	}

	inline void SetEye(XMFLOAT3 eye) {
		this->eye = eye; viewDirty = true;
	}

	inline const XMFLOAT3& GetTarget() {
		return target;
	}

	inline void SetTarget(XMFLOAT3 target) {
		this->target = target; viewDirty = true;
	}

	inline const XMFLOAT3& GetUp() {
		return up;
	}

	inline void SetUp(XMFLOAT3 up) {
		this->up = up; viewDirty = true;
	}

	void MoveEyeVector(const XMFLOAT3& move);
	void MoveEyeVector(const XMVECTOR& move);

	void MoveVector(const XMFLOAT3& move);
	void MoveVector(const XMVECTOR& move);

protected: // �����o�ϐ�
	// �r���[�s��
	XMMATRIX matView = DirectX::XMMatrixIdentity();
	// �r���{�[�h�s��
	XMMATRIX matBillboard = DirectX::XMMatrixIdentity();
	// Y�����r���{�[�h�s��
	XMMATRIX matBillboardY = DirectX::XMMatrixIdentity();
	// �ˉe�s��
	XMMATRIX matProjection = DirectX::XMMatrixIdentity();
	// �r���[�ˉe�s��
	XMMATRIX matViewProjection = DirectX::XMMatrixIdentity();
	// �r���[�s��_�[�e�B�t���O
	bool viewDirty = false;
	// �ˉe�s��_�[�e�B�t���O
	bool projectionDirty = false;
	// ���_���W
	XMFLOAT3 eye = { 0, 0, -20 };
	// �����_���W
	XMFLOAT3 target = { 0, 0, 0 };
	// ������x�N�g��
	XMFLOAT3 up = { 0, 1, 0 };
	// �A�X�y�N�g��
	float aspectRatio = 1.0f;

};