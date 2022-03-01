﻿#include <vector>
#include <string>
#include <fstream>
#include <DirectXMath.h>
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")


#include <DirectXTex.h>
#include <wrl.h>

#include <xaudio2.h>
#pragma comment(lib,"xaudio2.lib")

#include "Input.h"
#include "WinApp.h"
#include "DirectXCommon.h"
#include "Audio.h"
#include "Object3d.h"
#include "Model.h"
#include "Sprite.h"

using namespace DirectX;
using namespace Microsoft::WRL;

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{

#pragma region WindowsAPI初期化
    WinApp* winApp = nullptr;

    winApp = new WinApp();
    winApp->Initialize();

    MSG msg{};  // メッセージ
#pragma endregion WindowsAPI初期化

#pragma region DirectX初期化処理
    HRESULT result;

    DirectXCommon* dxCommon = nullptr;

    dxCommon = new DirectXCommon();
    dxCommon->Initialize(winApp);

    Input* input = nullptr;
    input = new Input();
    input->Initialize(winApp);

    Object3d::StaticInitialize(dxCommon->GetDev(),winApp->window_width,winApp->window_height);

    // スプライト静的初期化
    Sprite::StaticInitialize(dxCommon->GetDev(), WinApp::window_width, WinApp::window_height);

#pragma endregion DirectX初期化処理

#pragma region 描画初期化処理

    Model* modelsquare_1 = Model::LoadFromOBJ("sqare");
    Model* modelsquare_2 = Model::LoadFromOBJ("block1");
    Object3d* objsquare_1 = Object3d::Create();
    Object3d* objsquare_2 = Object3d::Create();
    Object3d* objsquare_3 = Object3d::Create();
    objsquare_1->SetModel(modelsquare_1);
    objsquare_2->SetModel(modelsquare_1);
    objsquare_3->SetModel(modelsquare_1);

    objsquare_2->SetPosition({ -100,0,-10 });
    objsquare_3->SetPosition({ +100,0,-10 });
    objsquare_1->Update();
    objsquare_2->Update();
    objsquare_3->Update();

    // テクスチャ読み込み
    Sprite::LoadTexture(1, L"Resources/house.png");
    // 背景スプライト生成
    Sprite* sprite = nullptr;
    sprite = Sprite::Create(1, { 0.0f,0.0f });
    //sprite->SetTextureRect();

#pragma endregion 描画初期化処理

    int counter = 0; // アニメーションの経過時間カウンター

#pragma region//オーディオ
    ////オーディオ
    //const int AudioMax = 1;
    //Audio* audio = new Audio;
    //if (!audio->Initialize()) {
    //    assert(0);
    //    return 1;
    //}
#pragma endregion

    while (true)  // ゲームループ
    {
#pragma region ウィンドウメッセージ処理
        if (winApp->ProcessMessage()) {
            break;
        }
#pragma endregion ウィンドウメッセージ処理

#pragma region DirectX毎フレーム処理
        // DirectX毎フレーム処理　ここから

        input->Update();
        float clearColor[] = { 0.1f,0.25f, 0.5f,0.0f }; // 青っぽい色
        const int cycle = 540; // 繰り返しの周期
        counter++;
        counter %= cycle; // 周期を超えたら0に戻る
        float scale = (float)counter / cycle; // [0,1]の数値

        //const int cycle = 60; // 繰り返しの周期
        //counter++;
        //float scale = sinf(XM_2PI * (float)counter / cycle); // [-1,+1]の数値
        //scale += 1.0f; // [0,+2]の数値
        //scale /= 2.0f; // [0,+1]の数値
        scale *= 360.0f;

        objsquare_2->SetModel(modelsquare_1);
        objsquare_3->SetModel(modelsquare_1);

        if (input->TriggerKey(DIK_0)) {
            OutputDebugStringA("Hit 0\n");
        }

        //スペースキー押したら
        if (input->PushKey(DIK_SPACE)) {
            // 画面クリアカラーの数値を書き換える
            clearColor[1] = 1.0f;
            objsquare_2->SetModel(modelsquare_2);
            objsquare_3->SetModel(modelsquare_2);
        }

        // 座標操作
        if (input->PushKey(DIK_UP) || input->PushKey(DIK_DOWN) || input->PushKey(DIK_RIGHT) || input->PushKey(DIK_LEFT)) {
            //操作記入欄
        }

        /*if (input->PushKey(DIK_D) || input->PushKey(DIK_A)) {
            if (input->PushKey(DIK_D)) {
                angle += XMConvertToRadians(1.0f);
            } 
            else if (input->PushKey(DIK_A)) {
                angle -= XMConvertToRadians(1.0f);
            }

            eye.x = -100 * sinf(angle);
            eye.z = -100 * cosf(angle);
            matView = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));
        }*/

        //audio->PlayWave("Resources/Alarm01.wav");
        //// X座標,Y座標を指定して表示
        //debugText.Print(spriteCommon, "Hello,DirectX!!", 200, 100);
        //// X座標,Y座標,縮尺を指定して表示
        //debugText.Print(spriteCommon, "Nihon Kogakuin", 200, 200, 2.0f);

        //sprite.rotation = 45;
        //sprite.position = {1280/2, 720/2, 0};
        //sprite.color = {0, 0, 1, 1};

        // GPU上のバッファに対応した仮想メモリを取得
        /*Vertex* vertMap = nullptr;
        result = vertBuff->Map(0, nullptr, (void**)&vertMap);*/
        // 全頂点に対して
        //for (int i = 0; i < _countof(vertices); i++) {
        //    vertMap[i] = vertices[i];   // 座標をコピー
        //}
        //// マップを解除
        //vertBuff->Unmap(0, nullptr);

        // DirectX毎フレーム処理　ここまで
#pragma endregion DirectX毎フレーム処理
        objsquare_1->Update();
        objsquare_2->Update();
        objsquare_3->Update();


#pragma region グラフィックスコマンド

        dxCommon->PreDraw();

        // 3Dオブジェクト描画前処理
        Object3d::PreDraw(dxCommon->GetCmdList());
        // 3Dオブクジェクトの描画
        objsquare_1->Draw();
        objsquare_2->Draw();
        objsquare_3->Draw();
        // 3Dオブジェクト描画後処理
        Object3d::PostDraw();
        
#pragma region 前景スプライト描画
        // 前景スプライト描画前処理
        Sprite::PreDraw(dxCommon->GetCmdList());
        sprite->Draw();
        // スプライト描画後処理
        Sprite::PostDraw();
#pragma endregion
        // ４．描画コマンドここまで
        dxCommon->PostDraw();


    }
    // XAudio2解放
   // xAudio2.Reset();
    // 音声データ解放
   // SoundUnload(&soundData1);

#pragma region WindowsAPI後始末
    winApp->Finalize();
#pragma endregion WindowsAPI後始末
    delete input;
    delete winApp;

    delete modelsquare_1;
    delete modelsquare_2;

    delete objsquare_1;
    delete objsquare_2;
    delete objsquare_3;

    return 0;
}