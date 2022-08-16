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
    result = devkeyboard->SetDataFormat(&c_dfDIKeyboard); // �W���`��
    //�r���I���䃌�x���̃Z�b�g
    result = devkeyboard->SetCooperativeLevel(
        winApp->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);

    //�R���g���[���[
    result = dinput->CreateDevice(GUID_Joystick, &devCon, NULL);

    // ���̓f�[�^�`���̃Z�b�g
    if (devCon != nullptr) {
        result = devCon->SetDataFormat(&c_dfDIJoystick2); // �W���`��

        // �r�����䃌�x���̃Z�b�g
        result = devCon->SetCooperativeLevel(winApp->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
    }
}

void Input::Update() {
    HRESULT result;

    ////�O��̃L�[���͂�ۑ�
    memcpy(keyPre, key, sizeof(key));

    //�L�[�{�[�h���̎擾�J�n
    result = devkeyboard->Acquire();
    //�S�L�[�̓��͏����擾����
    result = devkeyboard->GetDeviceState(sizeof(key), key);

    //�R���g���[���[
    {
        if (devCon != nullptr) {
            result = devCon->Acquire();	// �}�E�X����J�n

            // �O��̓��͂�ۑ�
            ConStatePre = ConState;

            // �}�E�X�̓���
            result = devCon->GetDeviceState(sizeof(ConState), &ConState);
        }
    }

}

bool Input::PushKey(BYTE keyNumber) {
    //�w��L�[�������Ă����true��Ԃ�
    if (key[keyNumber]) {
        return true;
    }
    //�����łȂ����false��Ԃ�
    return false;
}

bool Input::TriggerKey(BYTE keyNumber) {
    if (!keyPre[keyNumber] && key[keyNumber]) {
        return true;
    }
    return false;
}

//�R���g���[���[
bool Input::TriggerButtonA()
{
	// �O��0�ŁA����0�łȂ���΃g���K�[
	if (!ConStatePre.rgbButtons[0] && ConState.rgbButtons[0]) {
		return true;
	}

	// �g���K�[�łȂ�
	return false;
}

bool Input::PushButtonA()
{
	// �O��0�ŁA����0�łȂ���΃g���K�[
	if (!ConStatePre.rgbButtons[0] && ConState.rgbButtons[0]) {
		return true;
	}

	// �g���K�[�łȂ�
	return false;
}

bool Input::TriggerButtonRB()
{
	// �O��0�ŁA����0�łȂ���΃g���K�[
	if (!ConStatePre.rgbButtons[5] && ConState.rgbButtons[5]) {
		return true;
	}

	// �g���K�[�łȂ�
	return false;
}

bool Input::TriggerButtonRT()
{
	// �O��0�ŁA����0�łȂ���΃g���K�[
	if (!ConStatePre.rgbButtons[7] && ConState.rgbButtons[7]) {
		return true;
	}

	// �g���K�[�łȂ�
	return false;
}


bool Input::TriggerButtonLB()
{
	// �O��0�ŁA����0�łȂ���΃g���K�[
	if (!ConStatePre.rgbButtons[4] && ConState.rgbButtons[4]) {
		return true;
	}

	// �g���K�[�łȂ�
	return false;
}

bool Input::TriggerButtonB()
{
	// �O��0�ŁA����0�łȂ���΃g���K�[
	if (!ConStatePre.rgbButtons[1] && ConState.rgbButtons[1]) {
		return true;
	}

	// �g���K�[�łȂ�
	return false;
}

bool Input::TriggerButonX()
{
	// �O��0�ŁA����0�łȂ���΃g���K�[
	if (!ConStatePre.rgbButtons[2] && ConState.rgbButtons[2]) {
		return true;
	}

	return false;
}

bool Input::TriggerButonY()
{
	// �O��0�ŁA����0�łȂ���΃g���K�[
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