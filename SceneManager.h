#pragma once
#include"BaseScene.h"
#include"DirectXCommon.h"

class SceneManager
{
public:
	~SceneManager();

	/// <summary>
	/// 更新
	/// </summary>
	void Update(DirectXCommon* dxComon);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(DirectXCommon* dxCommon);

	void SetNextScene(BaseScene* nextScene) { nextscene_ = nextScene; }
	void SetExplanation(bool Explanation) { this->Explanation_ = Explanation; }
	bool GetExplanation() { return Explanation_; }
private:
	//今のシーン
	BaseScene* scene_ = nullptr;
	//次のシーン
	BaseScene* nextscene_ = nullptr;
	bool Explanation_ = true;
};

