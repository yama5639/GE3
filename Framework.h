#pragma once

#include"GameScene.h"
#include"WinApp.h"
#include"DirectXCommon.h"
#include"Input.h"
#include"Audio.h"
#include"Object3d.h"
#include"SceneManager.h"
#include"DebugCamera.h"
#include"PostEffect.h"

/// <summary>
/// フレームワーク
/// </summary>
class Framework
{
public:
	/// <summary>
	/// 実行
	/// </summary>
	void Run();


	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize();

	/// <summary>
	/// 終了
	/// </summary>
	virtual void Finalize();

	/// <summary>
		/// 更新
		/// </summary>
	virtual void Update();
	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw();

	bool GetFinish() { return finish; }
private:
	static const int debugTextNumber = 0;
protected:
	bool finish = false;
	DirectXCommon* dxCommon = nullptr;
	WinApp* winApp = nullptr;
	Input* input = nullptr;
	//GameScene* gamescene = nullptr;
	Audio* audio = nullptr;
	SceneManager* sceneManager_ = nullptr;
	DebugCamera* camera = nullptr;
	PostEffect* postEffect = nullptr;
};