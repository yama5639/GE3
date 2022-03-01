#pragma once

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include<string>
#include"Model.h"

/// 3Dオブジェクト
class Object3d
{
private: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	//モデル
	Model* model = nullptr;
public: // サブクラス
	// 定数バッファ用データ構造体
	struct ConstBufferDataB0
	{
		//XMFLOAT4 color;	// 色 (RGBA)
		XMMATRIX mat;	// ３Ｄ変換行列
	};
private:
	static const int division = 50;
	static const float radius;
	static const float prizmHeight;
	static const int planeCount = division * 2 + division * 2;
	static const int vertexCount = planeCount * 3;
public: // 静的メンバ関数
	static bool StaticInitialize(ID3D12Device* device, int window_width, int window_height);

	/// 描画前処理
	static void PreDraw(ID3D12GraphicsCommandList* cmdList);

	/// 描画後処理
	static void PostDraw();

	/// 3Dオブジェクト生
	static Object3d* Create();

	/// 視点座標の取得
	static const XMFLOAT3& GetEye() { return eye; }

	/// 視点座標の設定
	static void SetEye(XMFLOAT3 eye);

	/// 注視点座標の取得
	static const XMFLOAT3& GetTarget() { return target; }

	/// 注視点座標の設定
	static void SetTarget(XMFLOAT3 target);

	/// ベクトルによる移動
	static void CameraMoveVector(XMFLOAT3 move);

	//
	void SetModel(Model* model) { this->model = model; }
	//static void LoadMaterial(const std::string& directoryPath, const std::string& filename);

private: // 静的メンバ変数
	// デバイス
	static ID3D12Device* device;
	// コマンドリスト
	static ID3D12GraphicsCommandList* cmdList;
	// ルートシグネチャ
	static ComPtr<ID3D12RootSignature> rootsignature;
	// パイプラインステートオブジェクト
	static ComPtr<ID3D12PipelineState> pipelinestate;
	// ビュー行列
	static XMMATRIX matView;
	// 射影行列
	static XMMATRIX matProjection;
	// 視点座標
	static XMFLOAT3 eye;
	// 注視点座標
	static XMFLOAT3 target;
	// 上方向ベクトル
	static XMFLOAT3 up;


private:// 静的メンバ関数
	/// カメラ初期化
	static void InitializeCamera(int window_width, int window_height);

	/// グラフィックパイプライン生成
	static bool InitializeGraphicsPipeline();

	/// テクスチャ読み込み
	static bool LoadTexture(const std::string& directoryPath, const std::string& filename);

	static void CreateModel();

	/// ビュー行列を更新
	static void UpdateViewMatrix();

public: // メンバ関数
	//初期化
	bool Initialize();
	/// 毎フレーム処理
	void Update();
	/// 描画
	void Draw();

	/// 座標の取得
	const XMFLOAT3& GetPosition() { return position; }

	/// 座標の設定
	void SetPosition(XMFLOAT3 position) { this->position = position; }

private: // メンバ変数
	ComPtr<ID3D12Resource> constBuffB0; // 定数バッファ
	// 色
	XMFLOAT4 color = { 1,1,1,1 };
	// ローカルスケール
	XMFLOAT3 scale = { 1.0,1.0,1.0 };
	// X,Y,Z軸回りのローカル回転角
	XMFLOAT3 rotation = { 0,0,0 };
	// ローカル座標
	XMFLOAT3 position = { 0,0,1 };
	// ローカルワールド変換行列
	XMMATRIX matWorld;
	// 親オブジェクト
	Object3d* parent = nullptr;
};