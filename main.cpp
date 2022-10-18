#include <vector>
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
#include "Safedelete.h"

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
    Input::GetInstance()->Initialize(winApp);
    Object3d::StaticInitialize(dxCommon->GetDev(),winApp->window_width,winApp->window_height);
    // スプライト静的初期化
    Sprite::StaticInitialize(dxCommon->GetDev(), WinApp::window_width, WinApp::window_height);

    Fbx_Model* model1 = nullptr;
    Fbx_Object3d* object1 = nullptr;
    //プレイヤー
    FbxLoader::GetInstance()->Initialize(dxCommon->GetDev());
    Fbx_Object3d::SetDevice(dxCommon->GetDev());
    Fbx_Object3d::CreateGraphicsPipeline();
    model1 = FbxLoader::GetInstance()->LoadModaleFromFile("shark");

    //カメラ
    DebugCamera* camera = nullptr;
    camera = new DebugCamera(WinApp::window_width, WinApp::window_height);
    Fbx_Object3d::SetCamera(camera);
    camera->GetEye();
    camera->GetTarget();
    XMFLOAT3 eye = { 0, 0, 0 };
    XMFLOAT3 target = { 0, 0, 0 };

    object1 = new Fbx_Object3d;
    object1->Initialize();
    object1->SetModel(model1);
    XMFLOAT3 position = { 0,-5,-185 };

#pragma region 描画初期化処理
    XMFLOAT3 Player_Scl = { 4,4,4 };
    XMFLOAT3 Player_Scl4 = { 3,3,3 };
    XMFLOAT3 Player_Scl5 = { 0.5,0.5,0.5 };
    XMFLOAT3 building_Scl = { 100,80,50 };
    //敵、弾
    Model* modelsquare_1 = Model::LoadFromOBJ("enemy");
    Model* modelsquare_2 = Model::LoadFromOBJ("block1");
    
    //背景建物
    Model* build = Model::LoadFromOBJ("building");

    //プレイヤー
    Model* plane = Model::LoadFromOBJ("sqare");

    Object3d* objsquare_1 = Object3d::Create();
    Object3d* objsquare_2 = Object3d::Create();
    Object3d* objsquare_3 = Object3d::Create();
    Object3d* objsquare_4 = Object3d::Create();
    Object3d* enemybullet_1 = Object3d::Create();
    Object3d* enemybullet_2 = Object3d::Create();
    Object3d* enemybullet_3 = Object3d::Create();
    Object3d* building = Object3d::Create();
    Object3d* airplane = Object3d::Create();
    XMFLOAT3 Player_Pos1 = { 0,0,-150 };
    XMFLOAT3 Player_Pos2 = { -15,0,-150 };
    XMFLOAT3 Player_Pos3 = { +15,0,-150 };
    XMFLOAT3 Player_Pos4 = {  50,50,-180};
    XMFLOAT3 Enemybullet_Pos1 = {   0,0,-150 };
    XMFLOAT3 Enemybullet_Pos2 = { -15,0,-150 };
    XMFLOAT3 Enemybullet_Pos3 = { +15,0,-150 };
    XMFLOAT3 building5 = { 0,-30,-100 };
    objsquare_1->SetModel(modelsquare_1);
    objsquare_2->SetModel(modelsquare_1);
    objsquare_3->SetModel(modelsquare_1);
    objsquare_4->SetModel(modelsquare_2);
    building->SetModel(build);
    enemybullet_1->SetModel(modelsquare_2);
    enemybullet_2->SetModel(modelsquare_2);
    enemybullet_3->SetModel(modelsquare_2);
    airplane->SetModel(plane);

    objsquare_1->SetScale({ Player_Scl });
    objsquare_2->SetScale({ Player_Scl });
    objsquare_3->SetScale({ Player_Scl });
    objsquare_4->SetScale({ Player_Scl4 });
    building->SetScale({ building_Scl });
    enemybullet_1->SetScale({ Player_Scl4 });
    enemybullet_2->SetScale({ Player_Scl4 });
    enemybullet_3->SetScale({ Player_Scl4 });
    airplane->SetScale({ Player_Scl5 });

    // テクスチャ読み込み
    Sprite::LoadTexture(1, L"Resources/reticle1.png");
    Sprite::LoadTexture(2, L"Resources/title1.png");
    Sprite::LoadTexture(3, L"Resources/enemy.png");
    Sprite::LoadTexture(4, L"Resources/player.png");
    Sprite::LoadTexture(5, L"Resources/mini.png");
    Sprite::LoadTexture(6, L"Resources/bullet.png");
    Sprite::LoadTexture(7, L"Resources/HP1.png");
    Sprite::LoadTexture(8, L"Resources/HP.png");
    Sprite::LoadTexture(9, L"Resources/Gameover.png");
    Sprite::LoadTexture(10, L"Resources/gameclear.png");
    Sprite::LoadTexture(11, L"Resources/HPh.png");
    // 背景スプライト生成
    Sprite* sprite = nullptr;
    Sprite* sprite1 = nullptr;
    Sprite* sprite2 = nullptr;
    Sprite* sprite3 = nullptr;
    Sprite* sprite4 = nullptr;
    Sprite* sprite5 = nullptr;
    Sprite* sprite6 = nullptr;
    Sprite* sprite7 = nullptr;
    Sprite* sprite8 = nullptr;
    Sprite* sprite9 = nullptr;
    Sprite* sprite10 = nullptr;
    Sprite* sprite11 = nullptr;
    Sprite* sprite12 = nullptr;

    sprite = Sprite::Create(1, { 550.0f,300.0f });
    sprite1 = Sprite::Create(2, { 0.0f,0.0f });
    sprite2 = Sprite::Create(3, { 80.0f,50.0f });
    sprite3 = Sprite::Create(4, { 80.0f,110.0f });
    sprite4 = Sprite::Create(5, { 10.0f,10.0f });
    sprite5 = Sprite::Create(6, { 80.0f,110.0f });
    sprite6 = Sprite::Create(3, { 20.0f,50.0f });
    sprite7 = Sprite::Create(3, { 140.0f,50.0f });
    sprite8 = Sprite::Create(7, { 20.0f,600.0f });
    sprite9 = Sprite::Create(8, { 20.0f,600.0f });
    sprite10 = Sprite::Create(9, { 0.0f,0.0f });
    sprite11 = Sprite::Create(10, { 0.0f,0.0f });
    sprite12 = Sprite::Create(11, { 20.0f,600.0f });
    //sprite->SetTextureRect();
    XMFLOAT2 sppos = sprite->GetPosition();
    XMFLOAT2 minienemy_pos = sprite2->GetPosition();
    XMFLOAT2 minienemy_pos2 = sprite6->GetPosition();
    XMFLOAT2 minienemy_pos3 = sprite7->GetPosition();
    XMFLOAT2 bullet_pos = sprite5->GetPosition();

#pragma endregion 描画初期化処理

    int counter = 0; // アニメーションの経過時間カウンター
    int tf = 0;
    int scene = 0;
    int enemyf1 = 1;
    int enemyf2 = 1;
    int enemyf3 = 1;
    int playerf = 1;
    int bulletr = 3;
    int eneb = 1;
    int enebf1 = 1;
    int enebf2 = 1;
    int enebf3 = 1;
    int ener = 3;
    int playerr = 1;
    float length1;
    float length2;
    float length3;
    float length4;
    float length5;
    float length6;
    int timer = 100;
    float speed = 0.1;
    float speed1 = 0.1;
    int enemybullettime1 = 0;
    int enemybullettime2 = 0;
    int enemybullettime3 = 0;
    int killcount = 0;
    int hpcount = 0;
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
        //コントローラー
        LONG u_r = 32768;
        LONG a = 30000;
       
        scale *= 360.0f;

        objsquare_1->SetPosition({ Player_Pos1 });
        objsquare_2->SetPosition({ Player_Pos2 });
        objsquare_3->SetPosition({ Player_Pos3 });
        objsquare_4->SetPosition({ Player_Pos4 });
        airplane->SetPosition({ position });
        sprite->SetPosition(sppos);
        sprite2->SetPosition(minienemy_pos);
        sprite5->SetPosition(bullet_pos);
        sprite6->SetPosition(minienemy_pos2);
        sprite7->SetPosition(minienemy_pos3);
        building->SetPosition({ building5 });
        enemybullet_1->SetPosition({ Enemybullet_Pos1 });
        enemybullet_2->SetPosition({ Enemybullet_Pos2 });
        enemybullet_3->SetPosition({ Enemybullet_Pos3 });

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
        objsquare_4->Update();
        building->Update();
        airplane->Update();
        enemybullet_1->Update();
        enemybullet_2->Update();
        enemybullet_3->Update();

        //カメラ
        camera->SetTarget({ position });
        camera->SetDistance(-20.0f);
        camera->SetEye({ position.x,position.y,position.z });
        
        if (scene == 0) {
            if (Input::GetInstance()->PushKey(DIK_RETURN)) {
                scene = 1;
            }
        }

        if (scene == 1) {
            //キーボード操作
            if (Input::GetInstance()->PushKey(DIK_A)) {
                position.x -= 0.1f;
                minienemy_pos.x += 0.4;
                minienemy_pos2.x += 0.4;
                minienemy_pos3.x += 0.4;
            }
            if (Input::GetInstance()->PushKey(DIK_S)) {
                position.y -= 0.1f;
            }
            if (Input::GetInstance()->PushKey(DIK_D)) {
                position.x += 0.1f;
                minienemy_pos.x -= 0.4;
                minienemy_pos2.x -= 0.4;
                minienemy_pos3.x -= 0.4;
            }
            if (Input::GetInstance()->PushKey(DIK_W)) {
                position.y += 0.1f;
            }
            if (scene == 1) {
                if (Input::GetInstance()->PushKey(DIK_SPACE)) {
                    tf = 1;
                }
            }
            Player_Pos4.x = position.x;
            Player_Pos4.y = position.y;

            Enemybullet_Pos1.x = Player_Pos1.x;
            Enemybullet_Pos1.y = Player_Pos1.y;
            Enemybullet_Pos2.x = Player_Pos2.x;
            Enemybullet_Pos2.y = Player_Pos2.y;
            Enemybullet_Pos3.x = Player_Pos3.x;
            Enemybullet_Pos3.y = Player_Pos3.y;

            //敵の行動パターン
            //敵弾
            if (enemyf1 == 1) {
                if (enebf1 == 1) {
                    Enemybullet_Pos1.z -= 1;
                    if (-250 >= Enemybullet_Pos1.z) {
                        Enemybullet_Pos1.x = Player_Pos1.x;
                        Enemybullet_Pos1.y = Player_Pos1.y;
                        Enemybullet_Pos1.z = Player_Pos1.z;
                        
                    }
                }
                if (enebf1 == 0) {
                    Enemybullet_Pos1.x = Player_Pos1.x;
                    Enemybullet_Pos1.y = Player_Pos1.y;
                    Enemybullet_Pos1.z = Player_Pos1.z;
                    timer = timer - 1;
                    if (timer == 0) {
                        enebf1 = 1;
                        timer = 100;
                    }
                }
            }
            if (enemyf2 == 1) {
                Player_Pos2.y += speed;
                if (Player_Pos2.y >= 20 || Player_Pos2.y <= -10) {
                    speed = -speed;
                }
                if (enebf2 == 1) {
                    Enemybullet_Pos2.z -= 1;
                    if (-350 >= Enemybullet_Pos2.z) {
                        Enemybullet_Pos2.x = Player_Pos2.x;
                        Enemybullet_Pos2.y = Player_Pos2.y;
                        Enemybullet_Pos2.z = Player_Pos2.z;
                        
                    }
                }
                if (enebf2 == 0) {
                    Enemybullet_Pos2.x = Player_Pos2.x;
                    Enemybullet_Pos2.y = Player_Pos2.y;
                    Enemybullet_Pos2.z = Player_Pos2.z;
                    timer = timer - 1;
                    if (timer == 0) {
                        enebf2 = 1;
                        timer = 100;
                    }
                }
            }
            if (enemyf3 == 1) {
                Player_Pos3.y -= speed1;
                if (Player_Pos3.y >= 20 || Player_Pos3.y <= -10) {
                    speed1 = -speed1;
                }
                
                if (enebf3 == 1) {
                    Enemybullet_Pos3.z -= 1;
                    if (-300 >= Enemybullet_Pos3.z) {
                        Enemybullet_Pos3.x = Player_Pos3.x;
                        Enemybullet_Pos3.y = Player_Pos3.y;
                        Enemybullet_Pos3.z = Player_Pos3.z;
                        
                    }
                }
                if (enebf3 == 0) {
                    Enemybullet_Pos3.x = Player_Pos3.x;
                    Enemybullet_Pos3.y = Player_Pos3.y;
                    Enemybullet_Pos3.z = Player_Pos3.z;
                    timer = timer - 1;
                    if (timer == 0) {
                        enebf3 = 1;
                        timer = 100;
                    }
                }
                
            }

            //コントローラー
            //Lスティック
            /*if (Input::GetInstance()->GetConMove().lX < u_r - a) {
                position.x -= 0.1f;
                minienemy_pos.x += 0.4;
            }
            else if (Input::GetInstance()->GetConMove().lX > u_r + a) {
                position.x += 0.1f;
                minienemy_pos.x -= 0.4;
            }
            if (Input::GetInstance()->GetConMove().lY < u_r - a) {
                position.y += 0.1f;

            }
            else if (Input::GetInstance()->GetConMove().lY > u_r + a) {
                position.y -= 0.1f;

            }

            if (Input::GetInstance()->GetConMove().lX < u_r - 1000 && Input::GetInstance()->GetConMove().lY < u_r - 1000) {
                position.x -= 0.1f;
                position.y += 0.1f;

            }
            if (Input::GetInstance()->GetConMove().lX > u_r + 1000 && Input::GetInstance()->GetConMove().lY < u_r - 1000) {
                position.x += 0.1f;
                position.y += 0.1f;

            }
            if (Input::GetInstance()->GetConMove().lX < u_r - 1000 && Input::GetInstance()->GetConMove().lY > u_r + 1000) {
                position.y += 0.1f;
                position.y -= 0.1f;

            }
            if (Input::GetInstance()->GetConMove().lX > u_r + 1000 && Input::GetInstance()->GetConMove().lY > u_r + 1000) {
                position.x += 0.1f;
                position.y -= 0.1f;

            }*/

            //Rスティック
            /*if (Input::GetInstance()->GetConMove().lRx < u_r - a) {
                sppos.x -= 5.0f;
            }
            else if (Input::GetInstance()->GetConMove().lRx > u_r + a) {
                sppos.x += 5.0f;
            }
            if (Input::GetInstance()->GetConMove().lRy < u_r - a) {
                sppos.y -= 5.0f;
            }
            else if (Input::GetInstance()->GetConMove().lRy > u_r + a) {
                sppos.y += 5.0f;
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
            }

            if (Input::GetInstance()->TriggerButtonB()) {
                sppos.x = 590;
                sppos.y = 260;
            }*/

            //if (tf == 0) {
            //    //Rボタン
            //    if (Input::GetInstance()->TriggerButtonRB()) {
            //        tf = 1;
            //    }
            //}

            //弾と敵の衝突判定
            if (tf == 1) {
                if (enemyf1 == 1) {
                    length1 = sqrtf((Player_Pos1.x - Player_Pos4.x) * (Player_Pos1.x - Player_Pos4.x) + (Player_Pos1.y - Player_Pos4.y) * (Player_Pos1.y - Player_Pos4.y)) + (Player_Pos1.z - Player_Pos4.z) * (Player_Pos1.z - Player_Pos4.z);
                    if (length1 <= bulletr + ener) {
                        tf = 0;
                        killcount += 1;
                        enemyf1 = 0;
                    }
                }
                if (enemyf2 == 1) {
                    length2 = sqrtf((Player_Pos2.x - Player_Pos4.x) * (Player_Pos2.x - Player_Pos4.x) + (Player_Pos2.y - Player_Pos4.y) * (Player_Pos2.y - Player_Pos4.y)) + (Player_Pos2.z - Player_Pos4.z) * (Player_Pos2.z - Player_Pos4.z);
                    if (length2 <= bulletr + ener) {
                        tf = 0;
                        killcount += 1;
                        enemyf2 = 0;
                    }
                }
                if (enemyf3 == 1) {
                    length3 = sqrtf((Player_Pos3.x - Player_Pos4.x) * (Player_Pos3.x - Player_Pos4.x) + (Player_Pos3.y - Player_Pos4.y) * (Player_Pos3.y - Player_Pos4.y)) + (Player_Pos3.z - Player_Pos4.z) * (Player_Pos3.z - Player_Pos4.z);
                    if (length3 <= bulletr + ener) {
                        tf = 0;
                        killcount += 1;
                        enemyf3 = 0;
                    }
                }
            }
            if (killcount == 3) {
                scene = 3;
            }

            if (playerf == 1) {
                length4 = sqrtf((position.x - Enemybullet_Pos1.x) * (position.x - Enemybullet_Pos1.x) + (position.y - Enemybullet_Pos1.y) * (position.y - Enemybullet_Pos1.y)) + (position.z - Enemybullet_Pos1.z) * (position.z - Enemybullet_Pos1.z);
                if (length4 <= eneb + playerr) {
                    //playerf = 0;
                    hpcount += 1;
                    enebf1 = 0;
                }
                length5 = sqrtf((position.x - Enemybullet_Pos2.x) * (position.x - Enemybullet_Pos2.x) + (position.y - Enemybullet_Pos2.y) * (position.y - Enemybullet_Pos2.y)) + (position.z - Enemybullet_Pos2.z) * (position.z - Enemybullet_Pos2.z);
                if (length5 <= eneb + playerr) {
                    //playerf = 0;
                    hpcount += 1;
                    enebf2 = 0;
                }
                length6 = sqrtf((position.x - Enemybullet_Pos3.x) * (position.x - Enemybullet_Pos3.x) + (position.y - Enemybullet_Pos3.y) * (position.y - Enemybullet_Pos3.y)) + (position.z - Enemybullet_Pos3.z) * (position.z - Enemybullet_Pos3.z);
                if (length6 <= eneb + playerr) {
                    //playerf = 0;
                    hpcount += 1;
                    enebf3 = 0;
                }
            }

            if (hpcount >= 2) {
                //playerf = 0;
                scene = 2;
            }

            if (tf == 1) {
                Player_Pos4.z++;
                if (-100 <= Player_Pos4.z) {
                    tf = 0;
                }
            }

            

            if (tf == 0) {
                Player_Pos4.x = position.x;
                Player_Pos4.y = position.y;
                Player_Pos4.z = -180;
            }
            //リスポーン
            /*if (enemyf1 == 0)
            {
                timer = timer - 1;
                if (timer == 0) {
                    enemyf1 = 1;
                    timer = 100;
                }
            }
            if (enemyf2 == 0)
            {
                timer = timer - 1;
                if (timer == 0) {
                    enemyf2 = 1;
                    timer = 100;
                }
            }
            if (enemyf3 == 0)
            {
                timer = timer - 1;
                if (timer == 0) {
                    enemyf3 = 1;
                    timer = 100;
                }
            }*/
        }
        
        //if (scene == 2) {
        //    if (Input::GetInstance()->PushKey(DIK_RETURN)) {
        //        scene = 1;
        //        objsquare_1->SetPosition({ Player_Pos1 });
        //        objsquare_2->SetPosition({ Player_Pos2 });
        //        objsquare_3->SetPosition({ Player_Pos3 });
        //        objsquare_4->SetPosition({ Player_Pos4 });
        //        airplane->SetPosition({ position });
        //        sprite->SetPosition(sppos);
        //        sprite2->SetPosition(minienemy_pos);
        //        sprite5->SetPosition(bullet_pos);
        //        sprite6->SetPosition(minienemy_pos2);
        //        sprite7->SetPosition(minienemy_pos3);
        //        building->SetPosition({ building5 });
        //        enemybullet_1->SetPosition({ Enemybullet_Pos1 });
        //        enemybullet_2->SetPosition({ Enemybullet_Pos2 });
        //        enemybullet_3->SetPosition({ Enemybullet_Pos3 });
        //        counter = 0; // アニメーションの経過時間カウンター
        //        tf = 0;
        //        enemyf1 = 1;
        //        enemyf2 = 1;
        //        enemyf3 = 1;
        //        playerf = 1;
        //        bulletr = 3;
        //        eneb = 1;
        //        enebf1 = 1;
        //        enebf2 = 1;
        //        enebf3 = 1;
        //        ener = 3;
        //        playerr = 1;
        //        length1;
        //        length2;
        //        length3;
        //        length4;
        //        length5;
        //        length6;
        //        timer = 100;
        //        speed = 0.1;
        //        speed1 = 0.1;
        //        enemybullettime = 0;
        //    }
        //}
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
        // 3Dオブジェクト描画後処理
        Object3d::PreDraw(dxCommon->GetCmdList());
        if (scene == 1) {
            if (enemyf1 == 1) {
                objsquare_1->Draw();
                if (enebf1 == 1) {
                    enemybullet_1->Draw();
                }
            }
            if (enemyf2 == 1) {
                objsquare_2->Draw();
                if (enebf2 == 1) {
                    enemybullet_2->Draw();
                }
            }
            if (enemyf3 == 1) {
                objsquare_3->Draw();
                if (enebf3 == 1) {
                    enemybullet_3->Draw();
                }
            }
            if (tf == 1) {
                objsquare_4->Draw();
            }
            building->Draw();
            
            if (playerf == 1) {
                //object1->Draw(dxCommon->GetCmdList());
                airplane->Draw();
            }
        }
        
        Object3d::PostDraw();
        //postEffect->Draw(dxCommon->GetCmdList());

#pragma region 前景スプライト描画
        //前景スプライト描画前処理
        Sprite::PreDraw(dxCommon->GetCmdList());
        
        if (scene == 0) {
            sprite1->Draw();
        }
        if (scene == 1) {
            //sprite->Draw();
            sprite4->Draw();
            sprite8->Draw();
            if (hpcount == 0) {
                sprite9->Draw();
            }
            if (hpcount == 1) {
                sprite12->Draw();
            }
            
            if (enemyf1 == 1) {
                if (15 < minienemy_pos.x && minienemy_pos.x <= 145) {
                    sprite2->Draw();
                }
            }
            if (enemyf2 == 1) {
                if (15 < minienemy_pos2.x && minienemy_pos2.x <= 145) {
                    sprite6->Draw();
                }
            }
            if (enemyf3 == 1) {
                if (15 < minienemy_pos3.x && minienemy_pos3.x < 145) {
                    sprite7->Draw();
                }
            }
            if (playerf == 1) {
                sprite3->Draw();
            }
            if (tf == 1) {
                if (15 <= bullet_pos.y) {
                    sprite5->Draw();
                    bullet_pos.y -= 2.0;
                }
            }
            if (tf == 0) {
                bullet_pos.y = 110;
            }
        }
        if (scene == 2) {
            sprite10->Draw();
        }
        if (scene == 3) {
            sprite11->Draw();
        }
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
    delete objsquare_4;
    delete object1;
    delete postEffect;
    return 0;
}