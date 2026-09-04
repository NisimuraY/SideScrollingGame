#pragma once
#include "Engine/GameObject.h"
#include "Engine/Model.h"

class Text;

class GameOverScene :  public GameObject
{ 
public:
	//コンストラクタ
	//引数：parent  親オブジェクト（SceneManager）
	GameOverScene(GameObject* parent);

	//初期化
	void Initialize() override;

	//更新
	void Update() override;

	//描画
	void Draw() override;

	//開放
	void Release() override;
private:
	Text* pText_;
};

