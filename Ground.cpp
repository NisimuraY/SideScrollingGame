#include "Ground.h"
#include "Engine/Model.h"
#include "Engine/CsvReader.h"
#include "Engine//Text.h"
#include "Food.h"

namespace
{
	using std::vector;
	int model_t = -1;
	//vector< vector<int>> mapData =
	//{
	//	{1,1,1,1,1,1,1,1,1,1},
	//	{1,2,0,1,0,0,0,0,0,1},
	//	{1,1,0,1,1,1,0,1,0,1},
	//	{1,0,0,1,0,1,0,1,0,1},
	//	{1,0,1,1,0,1,0,1,1,1},
	//	{1,0,0,0,0,1,0,0,0,1},
	//	{1,1,0,1,0,1,0,1,0,1},
	//	{1,0,0,1,0,0,0,1,0,1},
	//	{1,0,0,1,0,1,0,1,2,1},
	//	{1,1,1,1,1,1,1,1,1,1}
	//};

	int FoodCount_ = 0;
}

Ground::Ground(GameObject* parent)
	:GameObject(parent), hModel_(-1) , hBlockModel_(-1), mapWidth_(-1), mapHeight_(-1)
{
	CsvReader csvData;
	csvData.Load("map.csv");//CSVファイルを読み込む
	CsvReader csvFoodData;
	csvFoodData.Load("cookieMap.csv");

	mapWidth_ = csvData.GetWidth();
	mapHeight_ = csvData.GetHeight();

	mapData_ = vector<vector<int>>(mapHeight_, vector<int>(mapWidth_, 0));
	objMap_ = vector<vector<int>>(mapHeight_, vector<int>(mapWidth_, 0));

	for(int x = 0;x < mapWidth_;x++)
	{
		for (int y = 0;y < mapHeight_;y++)
		{
			mapData_[y][x] = csvData.GetValue(x, y);
		}
	}

	/*for(int x = 0;x < mapWidth_;x++)
	{
		for(int y = 0;y < mapHeight_;y++)
		{
			objMap_[y][x] = csvFoodData.GetValue(x, y);
			if(objMap_[y][x] > 0) 
			{
				Food* food = (Food*)Instantiate<Food>(this);
				food->SetPosition({ -9.0f + 2.0f * x, 9.0f - 2.0f * y ,0.0f });

				if (objMap_[y][x] == 1)
				{
					food->SetFoodType(FoodType::FOODTYPE_NORMAL);
				}
				if (objMap_[y][x] == 2)
				{
					food->SetFoodType(FoodType::FOODTYPE_POWER);
				}

				FoodCount_ += 1;
			}
		}
	}*/
}

void Ground::Initialize()
{

	hModel_ = Model::Load("RainbowMap.fbx");
	hBlockModel_ = Model::Load("GreenBlock.fbx");

	pText_ = new Text;
	pText_->Initialize();
}

void Ground::Update()
{
}

void Ground::Draw()
{
	//マップ
	for (int x = 0; x < 4; x++) {
		for (int y = 0; y < 3; y++) {
			
			Transform tr_;
			transform_.position_ = { 0.0f + 20.0f * x , 0.5f , 1.0f };
			transform_.rotate_ = { -90.0f , 0.0f , 0.0f };
			Model::SetTransform(hModel_, transform_);
			Model::Draw(hModel_);
			
		}
	}
	
	//ブロック
	for (int x = 0; x < 38; x++) {
		for (int y = 0; y < 20; y++) {
			if (mapData_[y][x] == 1) {
				Transform tr_;
				tr_.position_ = { -9.0f + 2.0f * x, 6.0f - 0.8f * y , 0.0f };
				tr_.rotate_ = { 0.0f , 0.0f , 0.0f };
				Model::SetTransform(hBlockModel_, tr_);
				Model::Draw(hBlockModel_);
			}
		}
	}

	std::string FCText;
	FCText = "NOKORI:" + std::to_string(FoodCount_);
	pText_->Draw(20, 50, FCText.c_str());
}

void Ground::Release()
{
	pText_->Release();
}

void Ground::DecFoodCount(int type)
{
	if(type == 0|| type == 1)
	FoodCount_ -= 1;
}

int Ground::GetFoodCount()
{
	return FoodCount_;
}
