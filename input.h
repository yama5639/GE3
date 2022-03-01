#pragma once

#include <windows.h>
#include <wrl.h>
#define DIRECTINPUT_VERSION 0x0800  //DirectInput�̃o�[�W�����w��
#include <dinput.h>
#include "WinApp.h"

//����
class Input
{
public:
	//namespace�ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public://�����o�֐�
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

private://�����o�֐�
	//�L�[�{�[�h�̃f�o�C�X
	ComPtr<IDirectInputDevice8> devkeyboard;
	//DirectInput�̃C���X�^���X����
	ComPtr<IDirectInput8> dinput = nullptr;
	//�S�L�[�̏��
	BYTE key[256] = {};
	//�O��̑S�L�[�̏��
	BYTE keyPre[256] = {};

	WinApp* winApp = nullptr;
};
