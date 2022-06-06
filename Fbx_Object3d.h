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
public:
	void Initialize();
	void Update();
	void SetModel(Fbx_Model* model) { this -> model = model; }
	void Draw(ID3D12GraphicsCommandList* cmdList);
protected:
	ComPtr<ID3D12Resource> constBuffTransform;
	//�X�P�[��
	XMFLOAT3 scale = { 0.01f,0.01f,0.01f };
	//��]
	XMFLOAT3 rotation = { 0, 0, 0 };
	//���W
	XMFLOAT3 position = { 0, 0, 0 };
	//���[���h�ϊ��s��
	XMMATRIX matWorld;
	//���f��
	Fbx_Model* model = nullptr;
};