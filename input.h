#pragma once

#include <windows.h>
#include <wrl.h>
#define DIRECTINPUT_VERSION 0x0800  //DirectInput�̃o�[�W�����w��
#include <dinput.h>
#include "WinApp.h"

using namespace Microsoft::WRL;

//����
class Input
{
public:
	static Input* GetInstance();
	//namespace�ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public://�����o�֐�
	struct ConMove {
		LONG    lX;
		LONG    lY;
		LONG    lZ;
		LONG    lRx;
		LONG    lRy;
		LONG    lRz;
		BYTE    rgbButtons[32];
	};

	//������
	void Initialize(WinApp* winApp);

	//�X�V
	void Update();

	/// <summary>
	///	�L�[�����������`�F�b�N
	/// </summary>
	/// <paragm name = "keyNumber">�L�[�ԍ��iDIK_0 ���j</paragm>
	/// <returns>������Ă��邩</return>
	bool PushKey(BYTE keyNumber);

	/// <summary>
	///	�L�[�̃g���K�[���`�F�b�N
	/// </summary>
		/// <paragm name = "keyNumber">�L�[�ԍ��iDIK_0 ���j</paragm>
	/// <returns>�g���K�[��</return>
	bool TriggerKey(BYTE keyNumber);

	//�R���g���[���[
	bool TriggerButtonA();
	bool TriggerButtonB();
	bool TriggerButonX();
	bool TriggerButonY();
	bool TriggerButtonRB();
	bool TriggerButtonRT();
	bool TriggerButtonLB();
	bool PushButtonA();
	ConMove GetConMove();

private://�����o�֐�
	//�L�[�{�[�h�̃f�o�C�X
	ComPtr<IDirectInputDevice8> devkeyboard;
	//DirectInput�̃C���X�^���X����
	ComPtr<IDirectInput8> dinput = nullptr;
	//�S�L�[�̏��
	BYTE key[256] = {};
	//�O��̑S�L�[�̏��
	BYTE keyPre[256] = {};

	//�R���g���[���[
	ComPtr<IDirectInputDevice8> devCon;
	DIJOYSTATE2 ConState = {};
	DIJOYSTATE2 ConStatePre = {};

	WinApp* winApp = nullptr;
};
