#pragma once
#include "DirectXCommon.h"
#include <DirectXMath.h>
#include "Sprite.h"
#include"Object3d.h"
#include"Input.h"
#include"BaseScene.h"
#include"Fbx_Object3d.h"
#include"DebugCamera.h"
#include"PostEffect.h"

#include<memory>

using namespace std;

class GameScene :public BaseScene
{
private: // �G�C���A�X
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public://�����o�ϐ�

	GameScene(SceneManager* sceneManager_);

	void Initialize() override;

	void Update() override;

	void ObjDraw(DirectXCommon* dxCommon);

	void SpriteDraw(DirectXCommon* dxCommon);

	void ImgDraw();

	void PostEffectDraw(DirectXCommon* dxCommon);

	void Draw(DirectXCommon* dxCommon)override;

	void Finalize() override;

private:
	BaseScene* scene;

};
