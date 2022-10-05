#include "SceneManager.h"

SceneManager::~SceneManager()
{
	//最後のシーンの終了と解放
	scene_->Finalize();
	delete scene_;
}

void SceneManager::Update(DirectXCommon* dxComon)
{
	//シーンの切り替えがあるなら
	if (nextscene_) {
		//旧シーンの終了
		if (scene_) {
			scene_->Finalize();
			delete scene_;
			scene_ = nullptr;
		}
		//シーン切り替え
		scene_ = nextscene_;
		nextscene_ = nullptr;

		scene_->Initialize();
	}

	scene_->Update();
}

void SceneManager::Draw(DirectXCommon* dxCommon)
{
	scene_->Draw(dxCommon);
}