#include "TestScene.h"
#include "Player.h"
#include "Ground.h"
#include "Food.h"
#include "Engine/Input.h"
#include "Engine//Camera.h"
#include "Engine//Text.h"
#include "Engine/SceneManager.h"

namespace
{
	int myScore = 0;
	int HiScore = 0;
	int FoodValue_ = 0;
	float CameraPT_x = 0.0f;
	float CameraPT_y = 0.0f;
	float MinCamPT_x = 1.0f;
	float MinCamPT_y = -3.3f;
	float MaxCamPT_x = 55.0f;
	float MaxCamPT_y = 19.0f;

	XMFLOAT3 pPos = { 0.0f,0.0f,0.0f };
}

//コンストラクタ
TestScene::TestScene(GameObject * parent)
	: GameObject(parent, "TestScene")
{
}

//初期化
void TestScene::Initialize()
{	
	CameraPT_x = MinCamPT_x;
	CameraPT_y = MinCamPT_y;
	//pWp = Instantiate<Weapon>(this);
	pPlayer = Instantiate <Player>(this);
	pGround = Instantiate <Ground>(this);
	pPlayer->SetGround(pGround);

	Camera::SetPosition({ CameraPT_x,CameraPT_y,-15 });
	Camera::SetTarget({ CameraPT_x,CameraPT_y, 0 });

	pText_ = new Text;
	pText_->Initialize();


}

//更新
void TestScene::Update()
{
	pPos = pPlayer->GetPosition();

	if (Input::IsKey(DIK_LEFT) && CameraPT_x > MinCamPT_x
		&& CameraPT_x > pPos.x + 3.0f)
	{
		CameraPT_x -= 0.17f;
	};
	if (Input::IsKey(DIK_RIGHT)&& CameraPT_x < MaxCamPT_x
		&& CameraPT_x < pPos.x - 3.0f)
	{
		CameraPT_x += 0.17f;
	}

	Camera::SetPosition({ CameraPT_x,CameraPT_y,-15 });
	Camera::SetTarget({ CameraPT_x,CameraPT_y, 0 });

	if (HiScore < myScore)
	{
		HiScore = myScore;
	}

	if (pGround == nullptr)
		return;

	FoodValue_ = pGround->GetFoodCount();

	//if (FoodValue_ <= 0)
	//{
	//	SceneManager* scene = dynamic_cast<SceneManager*>(FindObject("SceneManager"));
	//	if (scene == nullptr)
	//		return;

	//	scene->ChangeScene(SCENE_ID_CLEAR);
	//}
}

//餌を数えて数を表示
// スコアを表示

//描画
void TestScene::Draw()
{
	std::string scrText;
	scrText = "SCORE:" + std::to_string(myScore);
	pText_->Draw(20, 20, scrText.c_str());
}

//開放
void TestScene::Release()
{
	pText_->Release();
}

void TestScene::AddScore(int score)
{
	myScore += score;
}

void TestScene::ResetScore()
{
	myScore = 0;
}

int TestScene::GetHiScore()
{
	return  HiScore;
}

