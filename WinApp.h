#pragma once

#include <Windows.h>

class WinApp
{
	//�ÓI�����o
public:
	static LRESULT windowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	//�����o�֐�
public:
	//������
	void Initialize();

	//�X�V
	void Update();

	//�I��
	void Finalize();

	//���b�Z�[�W����
	bool ProcessMessage();

	////getter
	HWND GetHwnd() { return hwnd; }
	HINSTANCE GetHInstance() { return w.hInstance; }

	//�ÓI�����o
public:

	// �E�B���h�E�T�C�Y
	static const int window_width = 1280;  // ����
	static const int window_height = 720;  // �c��

private://�����o�ϐ�
	HWND hwnd = nullptr;
	WNDCLASSEX w{}; // �E�B���h�E�N���X�̐ݒ�
};