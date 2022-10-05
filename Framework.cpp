#include "Framework.h"
#include"FbxLoader.h"


void Framework::Run()
{
	//������
	Initialize();
	while (true)//�Q�[�����[�v
	{
		//�X�V
		Update();
		if (GetFinish()) {
			break;
		}
		//�`��
		Draw();
	}
	//�I��
	Finalize();
}

void Framework::Initialize()
{
	winApp = new WinApp();
	winApp->Initialize();
	dxCommon = new DirectXCommon();
	dxCommon->Initialize(winApp);
	input = Input::GetInstance();
	input->Initialize(winApp);
	Sprite::StaticInitialize(dxCommon->GetDev(), WinApp::window_width, WinApp::window_height);

	//�V�[���}�l�[�W���[
	sceneManager_ = new SceneManager;
	//Fbx
	FbxLoader::GetInstance()->Initialize(dxCommon->GetDev());

}

void Framework::Update()
{

	if (winApp->ProcessMessage()) {
		finish = true;
		return;
	}
	//DirectX���t���[������ ��������
	input->Update();

	//�V�[���̍X�V
	sceneManager_->Update(dxCommon);
}


void Framework::Finalize()
{
	winApp->Finalize();
	delete sceneManager_;
	//�E�B���h�E�\��
	ShowWindow(winApp->GetHwnd(), SW_SHOW);
	//�R���\�[���ւ̕����o��
	OutputDebugStringA("Hello,DirectX!!\n");
	//delete input;
	delete winApp;
	//delete dxCommon;
}


void Framework::Draw()
{
	//�`��O����
	//dxCommon->PreDraw();
	//�V�[���`��
	sceneManager_->Draw(dxCommon);
	//�`��㏈��
	//dxCommon->PostDraw();
}