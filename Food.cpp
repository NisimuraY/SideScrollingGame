#include "Food.h"
#include "TestScene.h"
#include "Ground.h"
#include "Engine/Model.h"

namespace
{
}

Food::Food(GameObject* parent)
	:GameObject(parent,"Food"),type_(FOODTYPE_NORMAL),hModel_(-1),score_(0)
{
}

Food::~Food()
{
}

void Food::Initialize()
{
	hModel_ = Model::Load("food.fbx");

	transform_.position_ = { 0.0f, 0.0f, 0.0f };
	transform_.rotate_ = { 0.0f, 0.0f, 0.0f };
}

void Food::Update()
{
	if (type_ == FOODTYPE_POWER)
	{
		transform_.rotate_.y += 1.0f;
	}
}

void Food::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
}

void Food::Release()
{
}

void Food::SetFoodType(FoodType type)
{
	type_ = type;
	if (type_ == FOODTYPE_NORMAL)
	{
		SphereCollider* collision = new SphereCollider(XMFLOAT3(0, 0.3, 0), 0.3f);
		AddCollider(collision);
		hModel_ = Model::Load("food.fbx");
		score_ = 1;
	}
	else if (type_ == FOODTYPE_POWER)
	{
		SphereCollider* collision = new SphereCollider(XMFLOAT3(0, 0.6, 0), 0.6f);
		AddCollider(collision);
		hModel_ = Model::Load("foodBIG.fbx");
		score_ = 5;
	}
}

void Food::OnCollision(GameObject* pTarget)
{
	TestScene* testScene = dynamic_cast<TestScene*>(GetParent()->GetParent());
	testScene->AddScore(score_);
	Ground* ground = dynamic_cast<Ground*>(FindObject("Ground"));
	ground->DecFoodCount(type_);
	if (pTarget->GetObjectName() == "Player")
	{
		KillMe();
	}
}
