#pragma once

#include <string>
#include <vector>
#include <DirectXMath.h>
#include <DirectXTex.h>
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <fbxsdk.h>

struct Node {
	std::string name;
	DirectX::XMVECTOR scaling = { 1,1,1,0 };
	DirectX::XMVECTOR rotation = { 0,0,0,0 };
	DirectX::XMVECTOR translation = { 0,0,0,1 };
	DirectX::XMMATRIX transform;
	DirectX::XMMATRIX globalTransform;
	Node* parent = nullptr;
};

class Fbx_Model {
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using TexMetadata = DirectX::TexMetadata;
	using ScratchImage = DirectX::ScratchImage;

	using string = std::string;
	template <class T> using vector = std::vector<T>;

	ComPtr<ID3D12Resource> vertBuff;
	ComPtr<ID3D12Resource> indexBuff;
	ComPtr<ID3D12Resource> texBuff;
	D3D12_VERTEX_BUFFER_VIEW vbView = {};
	D3D12_INDEX_BUFFER_VIEW ibView = {};
	ComPtr<ID3D12DescriptorHeap> descHeapSRV;

public:
	struct Bone
	{
		//���O
		std::string name;
		//�������p���̋t�s��
		DirectX::XMMATRIX invInitialPose;
		//�N���X�^�[�iFBX���̃{�[�����j
		FbxCluster* fbxCluster;
		//�R���X�g���N�^
		Bone(const std::string& name) {
			this->name = name;
		}
	};
	~Fbx_Model();
	friend class FbxLoader;
	void CreateBuffers(ID3D12Device* device);
	void Draw(ID3D12GraphicsCommandList* cmdList);
	const XMMATRIX& GetModelTransform() { return meshNode->globalTransform; }
	FbxScene* GetFbxScene() { return fbxScene; }

private:
	std::string name;
	std::vector<Node> nodes;
	DirectX::XMFLOAT3 ambient = { 1, 1, 1 };
	DirectX::XMFLOAT3 diffuse = { 1, 1, 1 };
	DirectX::TexMetadata metadata = {};
	DirectX::ScratchImage scratchImg = {};
	std::vector<Bone> bones;

public:
	static const int MAX_BONE_INDICES = 4;

public:
	struct VertexPosNormalUvSkin {
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT3 normal;
		DirectX::XMFLOAT3 uv;
		UINT boneIndex[MAX_BONE_INDICES];
		float boneWeight[MAX_BONE_INDICES];
	};

	Node* meshNode = nullptr;
	
	std::vector<VertexPosNormalUvSkin> vertices;
	std::vector<unsigned short> indices;
	std::vector<Bone>& GetBones() { return bones; }
	FbxScene* fbxScene = nullptr;
};