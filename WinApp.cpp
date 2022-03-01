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
    // ウィンドウサイズ
    RECT wrc = { 0, 0, window_width, window_height };
    AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false); // 自動でサイズ補正

    // ウィンドウオブジェクトの生成
    this->hwnd = CreateWindow(w.lpszClassName, // クラス名
        L"DirectXGame",         // タイトルバーの文字
        WS_OVERLAPPEDWINDOW,        // 標準的なウィンドウスタイル
        CW_USEDEFAULT,              // 表示X座標（OSに任せる）
        CW_USEDEFAULT,              // 表示Y座標（OSに任せる）
        wrc.right - wrc.left,       // ウィンドウ横幅
        wrc.bottom - wrc.top,   // ウィンドウ縦幅
        nullptr,                // 親ウィンドウハンドル
        nullptr,                // メニューハンドル
        w.hInstance,            // 呼び出しアプリケーションハンドル
        nullptr);               // オプション

    // ウィンドウ表示
    ShowWindow(hwnd, SW_SHOW);
}

void WinApp::Update() {

}

void WinApp::Finalize() {
    // ウィンドウクラスを登録解除
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
