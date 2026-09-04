#include "GameOverScene.h"
#include "TestScene.h"
#include "Engine/Input.h"
#include "Engine//Text.h"
#include "Engine/SceneManager.h"

GameOverScene::GameOverScene(GameObject* parent)
	: GameObject(parent, "GameOverScene")
{
}

void GameOverScene::Initialize()
{
	pText_ = new Text;
	pText_->Initialize();
}

void GameOverScene::Update()
{
	SceneManager* scene = dynamic_cast<SceneManager*>(FindObject("SceneManager"));

	if (Input::IsKeyDown(DIK_SPACE))
	{
		TestScene* testScene = dynamic_cast<TestScene*>(GetParent()->GetParent());
		testScene->ResetScore();
		scene->ChangeScene(SCENE_ID_TITLE);
	}
}

void GameOverScene::Draw()
{
	std::string GameOverText;
	GameOverText = "GAME OVER";
	pText_->Draw(570, 300, GameOverText.c_str());
	GameOverText = "PRESS_SPACE KEY";
	pText_->Draw(525, 500, GameOverText.c_str());
}

void GameOverScene::Release()
{
	pText_->Release();
}
