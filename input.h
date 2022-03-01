#pragma once

#include <windows.h>
#include <wrl.h>
#define DIRECTINPUT_VERSION 0x0800  //DirectInputのバージョン指定
#include <dinput.h>
#include "WinApp.h"

//入力
class Input
{
public:
	//namespace省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public://メンバ関数
	//初期化
	void Initialize(WinApp* winApp);

	//更新
	void Update();

	/// <summary>
	///	キー押したかをチェック
	/// </summary>
	/// <paragm name = "keyNumber">キー番号（DIK_0 等）</paragm>
	/// <returns>押されているか</return>
	bool PushKey(BYTE keyNumber);

	/// <summary>
	///	キーのトリガーをチェック
	/// </summary>
		/// <paragm name = "keyNumber">キー番号（DIK_0 等）</paragm>
	/// <returns>トリガーか</return>
	bool TriggerKey(BYTE keyNumber);

private://メンバ関数
	//キーボードのデバイス
	ComPtr<IDirectInputDevice8> devkeyboard;
	//DirectInputのインスタンス生成
	ComPtr<IDirectInput8> dinput = nullptr;
	//全キーの状態
	BYTE key[256] = {};
	//前回の全キーの状態
	BYTE keyPre[256] = {};

	WinApp* winApp = nullptr;
};
