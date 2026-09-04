#include "Player.h"
#include "Engine/Model.h"
#include "Engine/Debug.h"
#include "Engine/Input.h"
#include "TestScene.h"
#include "Ground.h"

namespace
{
	enum PLAYER_STATE
	{
		PLAYER_IDLE,
		PLAYER_WALK,
		PLAYER_TURN,
		PLAYER_STATE_MAX
	};

	PLAYER_STATE pstate = PLAYER_STATE::PLAYER_IDLE;

	enum PLAYER_DIRECTION
	{
		PLAYER_UP,
		PLAYER_DOWN,
		PLAYER_LEFT,
		PLAYER_RIGHT,
		PLAYER_DIRECTION_MAX
	};

	PLAYER_DIRECTION pdirection = PLAYER_DOWN;
	float P_ANGLE[4] = { 180.0f,0.0f,90.0f,270.0f };
	XMVECTOR P_MOVE[4] = { XMVectorSet(0, 1, 0, 0), XMVectorSet(0, -1, 0, 0),
							XMVectorSet(-1, 0, 0, 0), XMVectorSet(1, 0, 0, 0) };
	const float TURN_FRAME = 6.0f;//回転にかかるフレーム数

	float turnStartAngle = 0.0f;
	float turnEndAngle = 0.0f;

	PLAYER_DIRECTION turnEndDirection = PLAYER_DOWN;
	std::vector <std::vector<int>> gmap;

	const float GroundHigh = 6.0f - (0.8f * 18.0f);
}

Player::Player(GameObject* parent)
	:GameObject(parent,"Player"), hIdleModel_(-1), hWalkModel_(-1)
{

}

void Player::Initialize()
{
	hIdleModel_ = Model::Load("idle.fbx");
	Model::SetAnimFrame(hIdleModel_, 0, 117, 1.0);

	hWalkModel_ = Model::Load("Walking.fbx");
	Model::SetAnimFrame(hWalkModel_, 0, 59, 1.0);

	transform_.scale_ = { 0.7,0.7,0.7 };
	transform_.position_ = { -6.5f, GroundHigh,-0.0f };

	SphereCollider* collision = new SphereCollider(XMFLOAT3(0, 0.25, 0), 0.25f);
	AddCollider(collision);
}

void Player::Update()
{
	XMVECTOR pos = XMLoadFloat3(&transform_.position_);
	XMVECTOR move = XMVectorSet(0, 0, 0, 0);

	const float SPEED = 0.17f;
	const float GRAVITY = 0.1f;
	float angle = 0.0f;
	static float turnFrame = 0.0f;

	gmap = ground_->GetMapData();

	if (pstate != PLAYER_STATE::PLAYER_TURN) {
		pstate = PLAYER_STATE::PLAYER_IDLE;
	}//回転状態でなければ待機

	PLAYER_DIRECTION oldDir = pdirection;//pdirection <= 今の向き

	if (pstate != PLAYER_STATE::PLAYER_TURN) {
		if (Input::IsKey(DIK_LEFT))
		{
			pdirection = PLAYER_DIRECTION::PLAYER_LEFT;
			pstate = PLAYER_STATE::PLAYER_WALK;
		}
		if (Input::IsKey(DIK_RIGHT))
		{
			pdirection = PLAYER_DIRECTION::PLAYER_RIGHT;
			pstate = PLAYER_STATE::PLAYER_WALK;
		}
		//if (Input::IsKey(DIK_UP))
		//{
		//	pdirection = PLAYER_DIRECTION::PLAYER_UP;
		//	pstate = PLAYER_STATE::PLAYER_WALK;
		//}
		if (Input::IsKey(DIK_UP) && !jump && OnGround())
		{
			jump = true;
			jumpPower = 0.3f;
		}

		// ジャンプ・落下
		if (jump)
		{
			pos += XMVectorSet(0.0f, jumpPower, 0.0f, 0.0f);

			jumpPower -= 0.02f;

			// 通常の地面
			if (XMVectorGetY(pos) <= GroundHigh)
			{
				pos = XMVectorSetY(pos, GroundHigh);

				jump = false;
				jumpPower = 0.0f;
			}
		}
		//if (Input::IsKey(DIK_DOWN))
		//{
		//	pdirection = PLAYER_DIRECTION::PLAYER_DOWN;
		//	pstate = PLAYER_STATE::PLAYER_WALK;
		//}
	}

	if (oldDir != pdirection) {
		//回転する
		pstate = PLAYER_STATE::PLAYER_TURN;
		turnFrame = 0.0f;
		turnStartAngle = P_ANGLE[oldDir];
		turnEndAngle = P_ANGLE[pdirection];
		turnEndDirection = pdirection;
	}

	if (pstate == PLAYER_STATE::PLAYER_TURN)
	{
		turnFrame += 1.0f;
		float t = turnFrame / TURN_FRAME;
		if (t > 1.0f)
		{
			t = 1.0f;
		}

		float deltaTurn = turnEndAngle - turnStartAngle;

		if (deltaTurn > 180.0f)
			deltaTurn -= 360.0f;
		else if (deltaTurn < -180.0f)
			deltaTurn += 360.0f;

		angle = turnStartAngle + deltaTurn * t;

		if (angle < 0.0f)
			angle += 360.0f;
		else if (angle >= 360.0f)
			angle -= 360.0f;

		transform_.rotate_.y = angle;

		if (turnFrame >= TURN_FRAME)
		{
			pdirection = turnEndDirection;
			transform_.rotate_.y = P_ANGLE[pdirection];
			pstate = PLAYER_STATE::PLAYER_WALK;
		}
		return;
	}
	else if (pstate != PLAYER_STATE::PLAYER_IDLE)
	{
		move = P_MOVE[pdirection];
		angle = P_ANGLE[pdirection];
		transform_.rotate_.y = angle;
	}

	pos = pos + SPEED * move;

	XMStoreFloat3(&transform_.position_, pos);
	XMFLOAT3 wpos = transform_.position_;

	int mapX = (int)((wpos.x + 10.0f) / 2.0f);
	//int mapY = (int)((7.0f - wpos.y) / 0.8f);
	
	// プレイヤーの少し上をチェック
	float checkY = wpos.y + 0.4f;

	int mapY = (int)((7.0f - checkY) / 0.8f);

	if (mapY >= 0 &&
		mapY < (int)gmap.size() &&
		mapX >= 0 &&
		mapX < (int)gmap[mapY].size())
	{
		if (gmap[mapY][mapX] == 1)
		{
			pos = pos - SPEED * move;
		}
	}

	/*if (gmap[mapY][mapX] == 1)
	{
		pos = pos - SPEED * move;
		XMStoreFloat3(&transform_.position_, pos);
	}*/

	/*if (!OnGround() && !jump)
	{
		transform_.position_.y -= GRAVITY;
	}*/

	XMStoreFloat3(&transform_.position_, pos);
}

void Player::Draw()
{
	if (pstate == PLAYER_STATE::PLAYER_IDLE || pstate == PLAYER_STATE::PLAYER_TURN)
	{
		Model::SetTransform(hIdleModel_, transform_);
		Model::Draw(hIdleModel_);
	}
	if (pstate == PLAYER_STATE::PLAYER_WALK)
	{
		Model::SetTransform(hWalkModel_, transform_);
		Model::Draw(hWalkModel_);
	}
}


void Player::Release()
{
}

bool Player::OnGround()
{

	if (transform_.position_.y <= GroundHigh )
	{
		transform_.position_.y = GroundHigh;
		return true;
	}

	return false;
}