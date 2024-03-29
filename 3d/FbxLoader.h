﻿#pragma once

#include "fbxsdk.h"

#include <d3d12.h>
#include <d3dx12.h>
#include <string>

#include "Fbx_Model.h"

class FbxLoader
{
private:
	using string = std::string;

public:
	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	/// <returns>インスタンス</returns>
	static FbxLoader* GetInstance();
	void Initialize(ID3D12Device* device);
	void Finalize();
	Fbx_Model* LoadModaleFromFile(const string& modelName);
	void ParseNodeRecursive(Fbx_Model* model, FbxNode* fbxNode,Node* parent = nullptr);
	void ParseMesh(Fbx_Model* model, FbxNode* fbxNode);
	void ParseMeshVertices(Fbx_Model* model, FbxMesh* fbxMesh);
	void ParseMeshFaces(Fbx_Model* model, FbxMesh* fbxMesh);
	void ParseMaterial(Fbx_Model* model, FbxNode* fbxNode);
	void LoadTexture(Fbx_Model* model, const std::string& fullpath);
	void ParseSkin(Fbx_Model* model, FbxMesh* fbxMesh);

	std::string ExtractFileName(const std::string& path);
	static void ConvertMatrixFromFbx(DirectX::XMMATRIX* dst, const FbxAMatrix& src);
private:
	// privateなコンストラクタ（シングルトンパターン）
	FbxLoader() = default;
	// privateなデストラクタ（シングルトンパターン）
	~FbxLoader() = default;
	// コピーコンストラクタを禁止（シングルトンパターン）
	FbxLoader(const FbxLoader& obj) = delete;
	// コピー代入演算子を禁止（シングルトンパターン）
	void operator=(const FbxLoader& obj) = delete;
	ID3D12Device* device = nullptr;
	FbxManager* fbxManager = nullptr;
	FbxImporter* fbxImporter = nullptr;
public:
	static const string baseDirectory;
	static const string defaultTextureFileName;
	
};