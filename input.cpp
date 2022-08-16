#include "input.h"


#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")


Input* Input::GetInstance() {
    static Input instance;
    return &instance;
}

void Input::Initialize(WinApp* winApp) {
    HRESULT result;
    this->winApp = winApp;


    result = DirectInput8Create(
        winApp->GetHInstance(), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&dinput, nullptr);
    result = dinput->CreateDevice(GUID_SysKeyboard, &devkeyboard, NULL);
    result = devkeyboard->SetDataFormat(&c_dfDIKeyboard); // 標準形式
    //排他的制御レベルのセット
    result = devkeyboard->SetCooperativeLevel(
        winApp->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);

    //コントローラー
    result = dinput->CreateDevice(GUID_Joystick, &devCon, NULL);

    // 入力データ形式のセット
    if (devCon != nullptr) {
        result = devCon->SetDataFormat(&c_dfDIJoystick2); // 標準形式

        // 排他制御レベルのセット
        result = devCon->SetCooperativeLevel(winApp->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
    }
}

void Input::Update() {
    HRESULT result;

    ////前回のキー入力を保存
    memcpy(keyPre, key, sizeof(key));

    //キーボード情報の取得開始
    result = devkeyboard->Acquire();
    //全キーの入力情報を取得する
    result = devkeyboard->GetDeviceState(sizeof(key), key);

    //コントローラー
    {
        if (devCon != nullptr) {
            result = devCon->Acquire();	// マウス動作開始

            // 前回の入力を保存
            ConStatePre = ConState;

            // マウスの入力
            result = devCon->GetDeviceState(sizeof(ConState), &ConState);
        }
    }

}

bool Input::PushKey(BYTE keyNumber) {
    //指定キーを押していればtrueを返す
    if (key[keyNumber]) {
        return true;
    }
    //そうでなければfalseを返す
    return false;
}

bool Input::TriggerKey(BYTE keyNumber) {
    if (!keyPre[keyNumber] && key[keyNumber]) {
        return true;
    }
    return false;
}

//コントローラー
bool Input::TriggerButtonA()
{
	// 前回が0で、今回が0でなければトリガー
	if (!ConStatePre.rgbButtons[0] && ConState.rgbButtons[0]) {
		return true;
	}

	// トリガーでない
	return false;
}

bool Input::PushButtonA()
{
	// 前回が0で、今回が0でなければトリガー
	if (!ConStatePre.rgbButtons[0] && ConState.rgbButtons[0]) {
		return true;
	}

	// トリガーでない
	return false;
}

bool Input::TriggerButtonRB()
{
	// 前回が0で、今回が0でなければトリガー
	if (!ConStatePre.rgbButtons[5] && ConState.rgbButtons[5]) {
		return true;
	}

	// トリガーでない
	return false;
}

bool Input::TriggerButtonRT()
{
	// 前回が0で、今回が0でなければトリガー
	if (!ConStatePre.rgbButtons[7] && ConState.rgbButtons[7]) {
		return true;
	}

	// トリガーでない
	return false;
}


bool Input::TriggerButtonLB()
{
	// 前回が0で、今回が0でなければトリガー
	if (!ConStatePre.rgbButtons[4] && ConState.rgbButtons[4]) {
		return true;
	}

	// トリガーでない
	return false;
}

bool Input::TriggerButtonB()
{
	// 前回が0で、今回が0でなければトリガー
	if (!ConStatePre.rgbButtons[1] && ConState.rgbButtons[1]) {
		return true;
	}

	// トリガーでない
	return false;
}

bool Input::TriggerButonX()
{
	// 前回が0で、今回が0でなければトリガー
	if (!ConStatePre.rgbButtons[2] && ConState.rgbButtons[2]) {
		return true;
	}

	return false;
}

bool Input::TriggerButonY()
{
	// 前回が0で、今回が0でなければトリガー
	if (!ConStatePre.rgbButtons[3] && ConState.rgbButtons[3]) {
		return true;
	}

	return false;
}

Input::ConMove Input::GetConMove()
{
	ConMove tmp;
	tmp.lX = ConState.lX;
	tmp.lY = ConState.lY;
	tmp.lZ = ConState.lZ;
	tmp.lRx = ConState.lRx;
	tmp.lRy = ConState.lRy;
	tmp.lRz = ConState.lRz;
	return tmp;
}