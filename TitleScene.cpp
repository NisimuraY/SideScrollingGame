#include "TitleScene.h"
#include "TestScene.h"
#include "Engine/Input.h"
#include "Engine//Text.h"
#include "Engine/SceneManager.h"

TitleScene::TitleScene(GameObject* parent)
	: GameObject(parent, "TitleScene")
{
}

void TitleScene::Initialize()
{
	pText_ = new Text;
	pText_->Initialize();
}

void TitleScene::Update()
{
	TestScene* testScene = dynamic_cast<TestScene*>(GetParent()->GetParent());
	TitleScore = testScene->GetHiScore();

	if (Input::IsKeyDown(DIK_SPACE))
	{
		SceneManager* scene = dynamic_cast<SceneManager*>(FindObject("SceneManager"));
		scene->ChangeScene(SCENE_ID_TEST);
	}

}

void TitleScene::Draw()
{
	std::string TitleText;

	TitleText = "HI-SCORE:" + std::to_string(TitleScore);
	pText_->Draw(20, 20, TitleText.c_str());
	
	TitleText = "TITLE";
	pText_->Draw(600, 300, TitleText.c_str());
	
	TitleText = "PRESS_SPACE KEY";
	pText_->Draw(525, 500, TitleText.c_str());
}

void TitleScene::Release()
{
	pText_->Release();
}
