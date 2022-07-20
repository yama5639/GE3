#pragma once
#include "Fbx_Model.h"
#include "Camera.h"
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <DirectXMath.h>
#include <string>

class Fbx_Object3d {
protected:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public:
	static void SetDevice(ID3D12Device* device) { Fbx_Object3d::device = device; }
	static void SetCamera(Camera* camera) { Fbx_Object3d::camera = camera; }

	static void CreateGraphicsPipeline();
	static const int MAX_BONES = 32;
private:
	static ID3D12Device* device;
	static Camera* camera;

	static ComPtr<ID3D12RootSignature> rootsignature;
	static ComPtr<ID3D12PipelineState> pipelinestate;
public:
	struct ConstBufferDataTransform
	{
		XMMATRIX viewproj;
		XMMATRIX world;
		XMFLOAT3 cameraPos;
	};

	struct ConstBufferDataSkin
	{
		XMMATRIX bones[MAX_BONES];
	};
public:
	void Initialize();
	void Update();
	void SetModel(Fbx_Model* model) { this -> model = model; }
	void SetRotation(XMFLOAT3 rota) { this->rotation = rota; }
	void Draw(ID3D12GraphicsCommandList* cmdList);
	void PlayAnimation();
protected:
	ComPtr<ID3D12Resource> constBuffTransform;
	ComPtr<ID3D12Resource> constBuffSkin;
	//スケール
	XMFLOAT3 scale = { 1.5f,1.5f,1.5f };
	//回転
	XMFLOAT3 rotation = { 0, 90, 0 };
	//座標
	XMFLOAT3 position = { 0, 0, 1 };
	//ワールド変換行列
	XMMATRIX matWorld;
	//モデル
	Fbx_Model* model = nullptr;
	//1フレーム
	FbxTime frameTime;
	//アニメーション開始時間
	FbxTime startTime;
	//アニメーション終了時間
	FbxTime endTime;
	//現在時間（アニメーション）
	FbxTime currentTime;
	//アニメーション再生中
	bool isPlay = false;
};