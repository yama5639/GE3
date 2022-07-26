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

#include "fbxsdk.h"
#include "FbxLoader.h"
#include "Fbx_Object3d.h"
#include "DebugCamera.h"

#include "PostEffect.h"

using namespace DirectX;
using namespace Microsoft::WRL;

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    //FbxManager* fbxManager = FbxManager::Create();
#pragma region WindowsAPI初期化
    WinApp* winApp = nullptr;

    winApp = new WinApp();
    winApp->Initialize();

    MSG msg{};  // メッセージ
#pragma endregion WindowsAPI初期化

    HRESULT result;

    DirectXCommon* dxCommon = nullptr;

    dxCommon = new DirectXCommon();
    dxCommon->Initialize(winApp);


    /*Input* input = nullptr;
    input = Input :: GetInstance();
    input->Initialize(winApp);*/
    Input::GetInstance()->Initialize(winApp);


    Object3d::StaticInitialize(dxCommon->GetDev(),winApp->window_width,winApp->window_height);

    // スプライト静的初期化
    Sprite::StaticInitialize(dxCommon->GetDev(), WinApp::window_width, WinApp::window_height);

    Fbx_Model* model1 = nullptr;
    Fbx_Object3d* object1 = nullptr;

    FbxLoader::GetInstance()->Initialize(dxCommon->GetDev());
    Fbx_Object3d::SetDevice(dxCommon->GetDev());
    Fbx_Object3d::CreateGraphicsPipeline();
    model1 = FbxLoader::GetInstance()->LoadModaleFromFile("shark");

    DebugCamera* camera = nullptr;
    camera = new DebugCamera(WinApp::window_width, WinApp::window_height);
    Fbx_Object3d::SetCamera(camera);
    camera->SetTarget({ 0,2.5f,0 });
    camera->SetDistance(8.0f);
    camera->SetEye({ 0,0,0 });
    camera->GetEye();
    camera->GetTarget();
    XMFLOAT3 eye = { 0,0,-20 };

    object1 = new Fbx_Object3d;
    object1->Initialize();
    object1->SetModel(model1);
    XMFLOAT3 position = { 0,-7,0 };

#pragma region 描画初期化処理
    XMFLOAT3 Player_Pos = { 0,+100,0 };
    XMFLOAT3 Player_Scl = { 3,3,3 };
    Model* modelsquare_1 = Model::LoadFromOBJ("block1");
    Model* modelsquare_2 = Model::LoadFromOBJ("block1");
    Object3d* objsquare_1 = Object3d::Create();
    Object3d* objsquare_2 = Object3d::Create();
    Object3d* objsquare_3 = Object3d::Create();
    objsquare_1->SetModel(modelsquare_1);
    objsquare_2->SetModel(modelsquare_1);
    objsquare_3->SetModel(modelsquare_1);

    objsquare_1->SetScale({ Player_Scl });
    objsquare_2->SetPosition({ -100,0,-10 });
    objsquare_3->SetPosition({ +100,0,-10 });
    objsquare_1->Update();
    objsquare_2->Update();
    objsquare_3->Update();

    // テクスチャ読み込み
    Sprite::LoadTexture(1, L"Resources/reticle.png");
    // 背景スプライト生成
    Sprite* sprite = nullptr;
    sprite = Sprite::Create(1, { 550.0f,300.0f });
    //sprite->SetTextureRect();
    XMFLOAT2 sppos = sprite->GetPosition();
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

    PostEffect* postEffect = nullptr;
    //Sprite::LoadTexture(100, L"Resources/white1x1.png");
    postEffect = new PostEffect();
    postEffect->Initialize();

    while (true)  // ゲームループ
    {

#pragma region ウィンドウメッセージ処理
        if (winApp->ProcessMessage()) {
            break;
        }
#pragma endregion ウィンドウメッセージ処理

#pragma region DirectX毎フレーム処理
        // DirectX毎フレーム処理　ここから
        Input::GetInstance()->Update();
        //input->Update();
        float clearColor[] = { 0.1f,0.25f, 0.5f,0.0f }; // 青っぽい色
        const int cycle = 540; // 繰り返しの周期
        counter++;
        counter %= cycle; // 周期を超えたら0に戻る
        float scale = (float)counter / cycle; // [0,1]の数値
        LONG u_r = 32768;
        LONG a = 30000;
       

        scale *= 360.0f;

        objsquare_1->SetPosition({ Player_Pos });
        objsquare_2->SetModel(modelsquare_1);
        object1->SetPosition({ position });
        sprite->SetPosition(sppos);
        
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

        //コントローラー
        //Lスティック
        if (Input::GetInstance()->GetConMove().lX < u_r - a) {
            position.x -= 0.1f;
            sppos.x -= 5.0f;
        }
        else if (Input::GetInstance()->GetConMove().lX > u_r + a) {
            position.x += 0.1f;
            sppos.x += 5.0f;
        }
        if (Input::GetInstance()->GetConMove().lY < u_r - a) {
            position.y += 0.1f;
            sppos.y -= 5.0f;
        }
        else if (Input::GetInstance()->GetConMove().lY > u_r + a) {
            position.y -= 0.1f;
            sppos.y += 5.0f;
        }

        if (Input::GetInstance()->GetConMove().lX < u_r - 1000 && Input::GetInstance()->GetConMove().lY < u_r - 1000) {
            position.x -= 0.1f;
            position.y += 0.1f;
            sppos.x -= 5.0f;
            sppos.y -= 5.0f;
        }
        if (Input::GetInstance()->GetConMove().lX > u_r + 1000 && Input::GetInstance()->GetConMove().lY < u_r - 1000) {
            position.x += 0.1f;
            position.y += 0.1f;
            sppos.x += 5.0f;
            sppos.y -= 5.0f;
        }
        if (Input::GetInstance()->GetConMove().lX < u_r - 1000 && Input::GetInstance()->GetConMove().lY > u_r + 1000) {
            position.y += 0.1f;
            position.y -= 0.1f;
            sppos.y -= 5.0f;
            sppos.y += 5.0f;
        }
        if (Input::GetInstance()->GetConMove().lX > u_r + 1000 && Input::GetInstance()->GetConMove().lY > u_r + 1000) {
            position.x += 0.1f;
            position.y -= 0.1f;
            sppos.x += 5.0f;
            sppos.y += 5.0f;
        }


        //Rスティック
        /*if (Input::GetInstance()->GetConMove().lRx < u_r - a) {
            position.x = sppos.x;
        }
        else if (Input::GetInstance()->GetConMove().lRx > u_r + a) {
            position.x = sppos.x;
        }
        if (Input::GetInstance()->GetConMove().lRy < u_r - a) {
            position.y = sppos.y;
        }
        else if (Input::GetInstance()->GetConMove().lRy > u_r + a) {
            sppos.y += 6.0f;
        }

        if (Input::GetInstance()->GetConMove().lRx < u_r - 1000 && Input::GetInstance()->GetConMove().lRy < u_r - 1000) {
            sppos.x -= 6.0f;
            sppos.y -= 6.0f;
        }
        if (Input::GetInstance()->GetConMove().lRx > u_r + 1000 && Input::GetInstance()->GetConMove().lRy < u_r - 1000) {
            sppos.x += 6.0f;
            sppos.y -= 6.0f;
        }
        if (Input::GetInstance()->GetConMove().lRx < u_r - 1000 && Input::GetInstance()->GetConMove().lRy > u_r + 1000) {
            sppos.y -= 6.0f;
            sppos.y += 6.0f;
        }
        if (Input::GetInstance()->GetConMove().lRx > u_r + 1000 && Input::GetInstance()->GetConMove().lRy > u_r + 1000) {
            sppos.x += 6.0f;
            sppos.y += 6.0f;
        }*/

        //Bボタン
        if (Input::GetInstance()->TriggerButtonB()) {
            object1->PlayAnimation();
        }

        //Rボタン
        if (Input::GetInstance()->TriggerButtonRB()) {
            
        }

        object1->Update();
#pragma region グラフィックスコマンド

        //postEffect->PreDrawScene(dxCommon->GetCmdList());
        //gameScene->Draw();
        //Object3d::PreDraw(dxCommon->GetCmdList());
        ////objsquare_1->Draw();
        //object1->Draw(dxCommon->GetCmdList());
        //Object3d::PostDraw();
        //postEffect->PostDrawScene(dxCommon->GetCmdList());
        
        dxCommon->PreDraw();

        // 3Dオブジェクト描画前処理
       
        // 3Dオブクジェクトの描画
     
        //objsquare_2->Draw();
        //objsquare_3->Draw();
       
        // 3Dオブジェクト描画後処理
        Object3d::PreDraw(dxCommon->GetCmdList());
        objsquare_1->Draw();
        object1->Draw(dxCommon->GetCmdList());
        Object3d::PostDraw();
        //postEffect->Draw(dxCommon->GetCmdList());

#pragma region 前景スプライト描画
        //前景スプライト描画前処理
        Sprite::PreDraw(dxCommon->GetCmdList());
        sprite->Draw();
        //// スプライト描画後処理
        Sprite::PostDraw();
#pragma endregion
        // ４．描画コマンドここまで
        dxCommon->PostDraw();


    }
    // XAudio2解放
    // xAudio2.Reset();
    // 音声データ解放
    // SoundUnload(&soundData1);
    FbxLoader::GetInstance()->Finalize();
#pragma region WindowsAPI後始末
    winApp->Finalize();
#pragma endregion WindowsAPI後始末
   
    delete winApp;

    delete modelsquare_1;
    delete modelsquare_2;

    delete objsquare_1;
    delete objsquare_2;
    delete objsquare_3;

    delete object1;
    delete postEffect;
    return 0;
}