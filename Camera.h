#pragma once

#include <DirectXMath.h>

/// <summary>
/// カメラ基本機能
/// </summary>
class Camera
{
protected: // エイリアス
	// DirectX::を省略
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

protected: // メンバ変数
	// ビュー行列
	XMMATRIX matView = DirectX::XMMatrixIdentity();
	// ビルボード行列
	XMMATRIX matBillboard = DirectX::XMMatrixIdentity();
	// Y軸回りビルボード行列
	XMMATRIX matBillboardY = DirectX::XMMatrixIdentity();
	// 射影行列
	XMMATRIX matProjection = DirectX::XMMatrixIdentity();
	// ビュー射影行列
	XMMATRIX matViewProjection = DirectX::XMMatrixIdentity();
	// ビュー行列ダーティフラグ
	bool viewDirty = false;
	// 射影行列ダーティフラグ
	bool projectionDirty = false;
	// 視点座標
	XMFLOAT3 eye = { 0, 0, -20 };
	// 注視点座標
	XMFLOAT3 target = { 0, 0, 0 };
	// 上方向ベクトル
	XMFLOAT3 up = { 0, 1, 0 };
	// アスペクト比
	float aspectRatio = 1.0f;

};