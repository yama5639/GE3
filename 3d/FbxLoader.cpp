#include "FbxLoader.h"

#include <cassert>

FbxLoader* FbxLoader::GetInstance()
{
    static FbxLoader instance;
    return &instance;
}

void FbxLoader::Initialize(ID3D12Device* device)
{
    //再初期化チェック
    assert(fbxManager == nullptr);
    //引数からメンバ変数に代入
    this->device = device;
    //FBXマネージャの生成
    fbxManager = FbxManager::Create();
    //FBXマネージャの入出力設定
    FbxIOSettings* ios = FbxIOSettings::Create(fbxManager, IOSROOT);
    fbxManager->SetIOSettings(ios);
    //FBXインポータの生成
    fbxImporter = FbxImporter::Create(fbxManager, "");
}

void FbxLoader::Finalize()
{
    //各種FBXインスタンスの破棄
    fbxImporter->Destroy();
    fbxManager->Destroy();
}