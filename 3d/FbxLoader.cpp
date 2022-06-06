#include "FbxLoader.h"

#include <cassert>

using namespace DirectX;

const std::string FbxLoader::baseDirectory = "Resources/";
const std::string FbxLoader::defaultTextureFileName = "white1x1.png";

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

Fbx_Model* FbxLoader::LoadModaleFromFile(const string& modelName) {
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

    model->CreateBuffers(device);

    return model;
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

    node.transform = XMMatrixIdentity();
    node.transform *= matScaling;
    node.transform *= matRotation;
    node.transform *= matTranslation;

    node.globalTransform = node.transform;
    if (parent) {
        node.parent = parent;
        node.globalTransform *= parent->globalTransform;
    }

    FbxNodeAttribute* fbxNodeAttribute = fbxNode->GetNodeAttribute();
    if (fbxNodeAttribute) {
        if (fbxNodeAttribute->GetAttributeType() == FbxNodeAttribute::eMesh) {
            model->meshNode = &node;
            ParseMesh(model, fbxNode);
        }
    }

    for (int i = 0; i < fbxNode->GetChildCount(); i++) {
        ParseNodeRecursive(model, fbxNode->GetChild(i), &node);
    }
}

void FbxLoader::ParseMesh(Fbx_Model* model, FbxNode* fbxNode) {
    FbxMesh* fbxMesh = fbxNode->GetMesh();
    ParseMeshVertices(model, fbxMesh);
    ParseMeshFaces(model, fbxMesh);
    ParseMaterial(model, fbxNode);
}

void FbxLoader::ParseMeshVertices(Fbx_Model* model, FbxMesh* fbxMesh) {
    auto& vertices = model->vertices;
    const int controlPointsCount = fbxMesh->GetControlPointsCount();

    Fbx_Model::VertexPosNormalUv vert{};
    model->vertices.resize(controlPointsCount, vert);

    FbxVector4* pCoord = fbxMesh->GetControlPoints();
    for (int i = 0; i < controlPointsCount; i++) {
        Fbx_Model::VertexPosNormalUv& vertex = vertices[i];
        vertex.pos.x = (float)pCoord[i][0];
        vertex.pos.y = (float)pCoord[i][1];
        vertex.pos.z = (float)pCoord[i][2];
    }
}

void FbxLoader::ParseMeshFaces(Fbx_Model* model, FbxMesh* fbxMesh) {
    auto& vertices = model->vertices;
    auto& indices = model->indices;

    assert(indices.size() == 0);
    const int polygonCount = fbxMesh->GetPolygonCount();
    const int textureUVCount = fbxMesh->GetTextureUVCount();
    FbxStringList uvNames;
    fbxMesh->GetUVSetNames(uvNames);

    for (int i = 0; i < polygonCount; i++) {
        const int polygonSize = fbxMesh->GetPolygonSize(i);
        assert(polygonSize <= 4);
        for (int j = 0; j < polygonSize; j++) {
            int index = fbxMesh->GetPolygonVertex(i, j);
            assert(index >= 0);

            Fbx_Model::VertexPosNormalUv& vertex = vertices[index];
            FbxVector4 normal;
            if (fbxMesh->GetPolygonVertexNormal(i, j, normal)) {
                vertex.normal.x = (float)normal[0];
                vertex.normal.y = (float)normal[1];
                vertex.normal.z = (float)normal[2];
            }

            if (textureUVCount > 0) {
                FbxVector2 uvs;
                bool lUnmappedUV;
                if (fbxMesh->GetPolygonVertexUV(i, j, uvNames[0], uvs, lUnmappedUV)) {
                    vertex.uv.x = (float)uvs[0];
                    vertex.uv.y = (float)uvs[1];
                }
            }

            if (j < 3) {
                indices.push_back(index);
            }

            else {
                int index2 = indices[indices.size() - 1];
                int index3 = index;
                int index0 = indices[indices.size() - 3];
                indices.push_back(index2);
                indices.push_back(index3);
                indices.push_back(index0);
            }
        }
    }
}

std::string FbxLoader::ExtractFileName(const std::string& path)
{
    size_t pos1;
    //区切り文字　'\\'　が出てくる一番最後の部分を検索
    pos1 = path.rfind('\\');
    if (pos1 != string::npos) {
        return path.substr(pos1 + 1, path.size() - pos1 - 1);

    }
    //区切り文字　'/'　が出てくる一番最後の部分を検索
    pos1 = path.rfind('/');
    if (pos1 != string::npos) {
        return path.substr(pos1 + 1, path.size() - pos1 - 1);

    }

    return path;
}

void FbxLoader::ParseMaterial(Fbx_Model* model, FbxNode* fbxNode) {
    const int materialCount = fbxNode->GetMaterialCount();
    if (materialCount > 0) {
        FbxSurfaceMaterial* material = fbxNode->GetMaterial(0);
        bool textureLoaded = false;
        if (material) {
            if (material->GetClassId().Is(FbxSurfaceLambert::ClassId))
            {
                FbxSurfaceLambert* lambert = static_cast<FbxSurfaceLambert*>(material);

                //環境光係数
                FbxPropertyT<FbxDouble3> ambient = lambert->Ambient;
                model->ambient.x = (float)ambient.Get()[0];
                model->ambient.y = (float)ambient.Get()[1];
                model->ambient.z = (float)ambient.Get()[2];

                //拡散反射光係数
                FbxPropertyT<FbxDouble3> diffuse = lambert->Diffuse;
                model->diffuse.x = (float)diffuse.Get()[0];
                model->diffuse.y = (float)diffuse.Get()[1];
                model->diffuse.z = (float)diffuse.Get()[2];

                const FbxProperty diffuseProperty = material->FindProperty(FbxSurfaceMaterial::sDiffuse);
                if (diffuseProperty.IsValid())
                {
                    const FbxFileTexture* texture = diffuseProperty.GetSrcObject<FbxFileTexture>();
                    if (texture) {
                        const char* filepath = texture->GetFileName();
                        string path_str(filepath);
                        string name = ExtractFileName(path_str);
                        LoadTexture(model, baseDirectory + model->name + "/" + name);
                        textureLoaded = true;
                    }
                }
            }
        }
        if (!textureLoaded) {
            LoadTexture(model, baseDirectory + defaultTextureFileName);
        }
    }
}

void FbxLoader::LoadTexture(Fbx_Model* model, const std::string& fullpath)
{
    HRESULT result = S_FALSE;

    TexMetadata& metadata = model->metadata;
    ScratchImage& scratchImg = model->scratchImg;

    wchar_t wfilepath[128];
    MultiByteToWideChar(CP_ACP, 0, fullpath.c_str(), -1, wfilepath, _countof(wfilepath));
    result = LoadFromWICFile(wfilepath, WIC_FLAGS_NONE, &metadata, scratchImg);
    if (FAILED(result)) {
        assert(0);
    }
}