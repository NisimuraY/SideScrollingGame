#include "ClearScene.h"
#include "Engine/Input.h"
#include "Engine//Text.h"
#include "Engine/SceneManager.h"

ClearScene::ClearScene(GameObject* parent)
	: GameObject(parent, "ClearScene")
{
}

void ClearScene::Initialize()
{
	pText_ = new Text;
	pText_->Initialize();
}

void ClearScene::Update()
{
	SceneManager* scene = dynamic_cast<SceneManager*>(FindObject("SceneManager"));

	if (Input::IsKey(DIK_Y))
	{
		scene->ChangeScene(SCENE_ID_TEST);
	}
	if (Input::IsKey(DIK_N))
	{
		scene->ChangeScene(SCENE_ID_GAMEOVER);
	}
}

void ClearScene::Draw()
{
	std::string ClearText;
	ClearText = "STAGE CLEAR";
	pText_->Draw(570, 300, ClearText.c_str());
	ClearText = "Y -> NextStage       N ->GameOver";
	pText_->Draw(395, 500, ClearText.c_str());
}

void ClearScene::Release()
{
	pText_->Release();
}
