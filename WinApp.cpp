#include "WinApp.h"

LRESULT WinApp::windowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
    switch (msg) {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hwnd, msg, wparam, lparam);
}

void WinApp::Initialize() {
    w.cbSize = sizeof(WNDCLASSEX);
    w.lpfnWndProc = (WNDPROC)windowProc;
    w.lpszClassName = L"DirectXGame";
    w.hInstance = GetModuleHandle(nullptr);
    w.hCursor = LoadCursor(NULL, IDC_ARROW);
    RegisterClassEx(&w);
    // �E�B���h�E�T�C�Y
    RECT wrc = { 0, 0, window_width, window_height };
    AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false); // �����ŃT�C�Y�␳

    // �E�B���h�E�I�u�W�F�N�g�̐���
    this->hwnd = CreateWindow(w.lpszClassName, // �N���X��
        L"DirectXGame",         // �^�C�g���o�[�̕���
        WS_OVERLAPPEDWINDOW,        // �W���I�ȃE�B���h�E�X�^�C��
        CW_USEDEFAULT,              // �\��X���W�iOS�ɔC����j
        CW_USEDEFAULT,              // �\��Y���W�iOS�ɔC����j
        wrc.right - wrc.left,       // �E�B���h�E����
        wrc.bottom - wrc.top,   // �E�B���h�E�c��
        nullptr,                // �e�E�B���h�E�n���h��
        nullptr,                // ���j���[�n���h��
        w.hInstance,            // �Ăяo���A�v���P�[�V�����n���h��
        nullptr);               // �I�v�V����

    // �E�B���h�E�\��
    ShowWindow(hwnd, SW_SHOW);
}

void WinApp::Update() {

}

void WinApp::Finalize() {
    // �E�B���h�E�N���X��o�^����
    UnregisterClass(w.lpszClassName, w.hInstance);
}

bool WinApp::ProcessMessage() {
    MSG msg{};
    if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    if (msg.message == WM_QUIT) {
        return true;
    }
    return false;
}
