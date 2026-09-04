#pragma once
#include "Engine/GameObject.h"
#include <vector>

class Ground;//前方宣言

class Player :
    public GameObject
{	
public:
	//コンストラクタ
	//引数：parent  親オブジェクト（SceneManager）
	Player(GameObject* parent);

	//初期化
	void Initialize() override;

	//更新
	void Update() override;

	//描画
	void Draw() override;

	//開放
	void Release() override;

	void SetGround(Ground* ground) { ground_ = ground; }

	bool OnGround();
	
private:
	int hIdleModel_;
	int hWalkModel_;
	Ground* ground_;
	bool jump =false;
	float jumpPower = 0.0f;
};

