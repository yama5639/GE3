#pragma once
#include<string>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>

class Model
{
private:
	//Microsoft::WRL::ComPtr<ID3D12Resource>constBuffB1;

	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
public:
	// ���_�f�[�^�\����
	struct VertexPosNormalUv
	{
		XMFLOAT3 pos; // xyz���W
		XMFLOAT3 normal; // �@���x�N�g��
		XMFLOAT2 uv;  // uv���W
	};
	//�}�e���A��
	struct Material
	{
		std::string name;//�}�e���A����
		XMFLOAT3 ambient;//�A���r�G���g�e���x
		XMFLOAT3 diffuse;//�f�B�q���[�Y�e���x
		XMFLOAT3 specular;//�X�y�L�����e���x
		float alpha;//
		std::string textureFilename;//�e�N�X�`���t�@�C����
		//�R���X�g���N�^
		Material() {
			ambient = { 0.3f,0.3f,0.3f };
			diffuse = { 0.0f,0.0f,0.0f };
			specular = { 0.0f,0.0f,0.0f };
			alpha = 1.0f;
		}
	};
	//�萔�o�b�t�@�p�f�[�^�\����B1
	struct ConstBufferDataB1
	{
		XMFLOAT3 ambient;
		float pad1;
		XMFLOAT3 diffuse;
		float pad2;
		XMFLOAT3 specular;
		float alpha;
	};
public:
	static Model* LoadFromOBJ(const std::string& modelname);
	static void SetDevice(ID3D12Device* dev) { Model::device = dev; }


private:
	std::vector<VertexPosNormalUv>vertices;
	//static VertexPosNormalUv vertices[vertexCount];
	// ���_�C���f�b�N�X�z��
	std::vector<unsigned short>indices;
	// �f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> descHeap;
	// �V�F�[�_���\�[�X�r���[�̃n���h��(CPU)
	CD3DX12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV;
	// �V�F�[�_���\�[�X�r���[�̃n���h��(CPU)
	CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV;
	//�}�e���A��
	Material material;
	// �f�X�N���v�^�T�C�Y
	UINT descriptorHandleIncrementSize;
	// ���_�o�b�t�@
	ComPtr<ID3D12Resource> vertBuff;
	// �C���f�b�N�X�o�b�t�@
	ComPtr<ID3D12Resource> indexBuff;
	// �e�N�X�`���o�b�t�@
	ComPtr<ID3D12Resource> texbuff;
	// ���_�o�b�t�@�r���[
	D3D12_VERTEX_BUFFER_VIEW vbView;
	// �C���f�b�N�X�o�b�t�@�r���[
	D3D12_INDEX_BUFFER_VIEW ibView;
	ComPtr<ID3D12Resource> constBuffB1;

private:
	static ID3D12Device* device;
private:

	/// �f�X�N���v�^�q�[�v�̏�����
	bool InitializeDescriptorHeap();
	void CreateBuffer();
	void LoadFromObjInternal(const std::string& modelname);
	void LoadMaterial(const std::string& directoryPath, const std::string& filename);
	bool LoadTexture(const std::string& directoryPath, const std::string& filename);
public:
	void Draw(ID3D12GraphicsCommandList* cmdList, UINT rootparamindexMaterial);
};