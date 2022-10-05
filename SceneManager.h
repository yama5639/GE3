#pragma once
#include"BaseScene.h"
#include"DirectXCommon.h"

class SceneManager
{
public:
	~SceneManager();

	/// <summary>
	/// �X�V
	/// </summary>
	void Update(DirectXCommon* dxComon);

	/// <summary>
	/// �`��
	/// </summary>
	void Draw(DirectXCommon* dxCommon);

	void SetNextScene(BaseScene* nextScene) { nextscene_ = nextScene; }
	void SetExplanation(bool Explanation) { this->Explanation_ = Explanation; }
	bool GetExplanation() { return Explanation_; }
private:
	//���̃V�[��
	BaseScene* scene_ = nullptr;
	//���̃V�[��
	BaseScene* nextscene_ = nullptr;
	bool Explanation_ = true;
};

