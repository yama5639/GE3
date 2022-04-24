#include "FbxLoader.h"

#include <cassert>

using namespace DirectX;

const std::string FbxLoader::baseDirectory = "Resources/";

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

void FbxLoader::LoadModaleFromFile(const string& modelName) {
    //モデルと同じ名前のファイルから読み込む
    const string directoryPath = baseDirectory + modelName + "/";
    //拡張子,FBXを付与
    const string fileName = modelName + ".fbx";
    //連結してフルパスを得る
    const string fullpath = directoryPath + fileName;

    //ファイル名を指定してFBXファイルを読み込む
    if (!fbxImporter->Initialize(fullpath.c_str(), -1, fbxManager->GetIOSettings())) {
        assert(0);
    }

    //シーン生成
    FbxScene* fbxScene = FbxScene::Create(fbxManager, "fbxScene");

    //ファイルからロードしたFBXに情報をシーンにインポート
    fbxImporter->Import(fbxScene);

    Fbx_Model* model = new Fbx_Model();
    model->name = modelName;
    int nodeCount = fbxScene->GetNodeCount();
    model->nodes.reserve(nodeCount);
    ParseNodeRecursive(model, fbxScene->GetRootNode());
    fbxScene->Destroy();
}

void FbxLoader::ParseNodeRecursive(Fbx_Model* model, FbxNode* fbxNode,Node* parent) {
    string name = fbxNode->GetName();

    model->nodes.emplace_back();
    Node& node = model->nodes.back();
    node.name = fbxNode->GetName();
    FbxDouble3 rotation = fbxNode->LclRotation.Get();
    FbxDouble3 scaling = fbxNode->LclScaling.Get();
    FbxDouble3 translation = fbxNode->LclTranslation.Get();

    node.rotation = { (float)rotation[0],(float)rotation[1],(float)rotation[2],0.0f };
    node.scaling = { (float)scaling[0],(float)scaling[1],(float)scaling[2],0.0f };
    node.translation = { (float)translation[0],(float)translation[1],(float)translation[2],1.0f };

    node.rotation.m128_f32[0] = XMConvertToRadians(node.rotation.m128_f32[0]);
    node.rotation.m128_f32[1] = XMConvertToRadians(node.rotation.m128_f32[1]);
    node.rotation.m128_f32[2] = XMConvertToRadians(node.rotation.m128_f32[2]);

    XMMATRIX matScaling, matRotation, matTranslation;
    matScaling = XMMatrixScalingFromVector(node.scaling);
    matRotation = XMMatrixRotationRollPitchYawFromVector(node.rotation);
    matTranslation = XMMatrixTranslationFromVector(node.translation);

   /* node.transform = XMMatrixIdentity();
    node.transform *= matScaling;
    node.transform *= matRotation;
    node.transform *= matTranslation;*/

    node.globalTransform = node.transform;
    if (parent) {
        node.parent = parent;
        node.globalTransform *= parent->globalTransform;
    }

    for (int i = 0; i < fbxNode->GetChildCount(); i++) {
        ParseNodeRecursive(model, fbxNode->GetChild(i), &node);
    }
}