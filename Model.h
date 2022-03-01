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
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
public:
	// 頂点データ構造体
	struct VertexPosNormalUv
	{
		XMFLOAT3 pos; // xyz座標
		XMFLOAT3 normal; // 法線ベクトル
		XMFLOAT2 uv;  // uv座標
	};
	//マテリアル
	struct Material
	{
		std::string name;//マテリアル名
		XMFLOAT3 ambient;//アンビエント影響度
		XMFLOAT3 diffuse;//ディヒューズ影響度
		XMFLOAT3 specular;//スペキュラ影響度
		float alpha;//
		std::string textureFilename;//テクスチャファイル名
		//コンストラクタ
		Material() {
			ambient = { 0.3f,0.3f,0.3f };
			diffuse = { 0.0f,0.0f,0.0f };
			specular = { 0.0f,0.0f,0.0f };
			alpha = 1.0f;
		}
	};
	//定数バッファ用データ構造体B1
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
	// 頂点インデックス配列
	std::vector<unsigned short>indices;
	// デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> descHeap;
	// シェーダリソースビューのハンドル(CPU)
	CD3DX12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV;
	// シェーダリソースビューのハンドル(CPU)
	CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV;
	//マテリアル
	Material material;
	// デスクリプタサイズ
	UINT descriptorHandleIncrementSize;
	// 頂点バッファ
	ComPtr<ID3D12Resource> vertBuff;
	// インデックスバッファ
	ComPtr<ID3D12Resource> indexBuff;
	// テクスチャバッファ
	ComPtr<ID3D12Resource> texbuff;
	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView;
	// インデックスバッファビュー
	D3D12_INDEX_BUFFER_VIEW ibView;
	ComPtr<ID3D12Resource> constBuffB1;

private:
	static ID3D12Device* device;
private:

	/// デスクリプタヒープの初期化
	bool InitializeDescriptorHeap();
	void CreateBuffer();
	void LoadFromObjInternal(const std::string& modelname);
	void LoadMaterial(const std::string& directoryPath, const std::string& filename);
	bool LoadTexture(const std::string& directoryPath, const std::string& filename);
public:
	void Draw(ID3D12GraphicsCommandList* cmdList, UINT rootparamindexMaterial);
};