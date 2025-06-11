#include <cmath>
#include <DxLib.h>
#include "GameScene.h"
#include "../Manager/Camera.h"
#include "../Manager/InputManager.h"
#include "../Manager/SceneManager.h"
#include "../Object/Dobber.h"
#include "../Object/EffekseerEffect.h"
#include "../Object/FishManager.h"
#include "../Object/Player.h"
#include "../Object/Rod.h"
#include "../Object/Stage.h"
#include "../Object/water.h"
#include "../Object/Gauge.h"
#include "../Utility/Utility.h"
#include "../Common/Random.h"
#include "../Application.h"

namespace
{
	static constexpr const char* BACKGROUND_PATH = "Data/Image/result.png";
	static constexpr const char* FISHON_PATH = "Data/Image/FishOn.png";
	static constexpr const char* ROD_SPAWN_TEXT_PATH = "Data/Image/RodSpawn.png";
	static constexpr const char* UKI_SPAWN_TEXT_PATH = "Data/Image/UkiSpawn.png";

	// ウィンドウサイズ
	static constexpr int SCREEN_WIDTH = Application::SCREEN_SIZE_X;
	static constexpr int SCREEN_HEIGHT = Application::SCREEN_SIZE_Y;

	// 文字列表示位置など
	static constexpr int FISH_NAME_FONT_SIZE = 48;
	static constexpr int INFO_FONT_SIZE = 16;
	static constexpr int INFO_MARGIN_BOTTOM = 30;

	// 魚情報構造体
	struct FishInfo 
	{
		float scale;
		const char* name;
		bool isRare = false;
	};

	// 魚情報リスト
	static constexpr FishInfo fishInfos[] = 
	{
		{0.0f, ""}, // 0番目（未使用）
		{0.75f, "アユ"},
		{0.75f, "ヤマメ"},
		{0.75f, "ハヤ"},
		{0.5f,  "フナ"},
		{0.5f,  "オイカワ"},
		{5.0f,  "メダカ"},
		{0.75f, "カワムツ"},
		{0.6f,  "イワナ"},
		{0.2f,  "ポリプテルス"},
		{0.3f,  "ニジマス"},
		{0.5f,  "ウグイ"},
		{0.5f,  "ワカサギ"},
		{0.3f,  "ユーステノプテロン", true}
	};

	// 表示サイズ定数
	static constexpr int FISHON_X = SCREEN_WIDTH / 2 - 400;
	static constexpr int FISHON_Y = SCREEN_HEIGHT / 2 - 150;
	static constexpr int FISHON_WIDTH = 600;
	static constexpr int FISHON_HEIGHT = 300;
	static constexpr int GRAPH_Y = 50;
	static constexpr int GRAPH_WEIGHT_ = 250;
	static constexpr int GRAPH_HEIGHT = 100;
	static constexpr int ROD_Y = 50;
	static constexpr int UKI_TEXT_Y = 130;
	static constexpr unsigned int TEXT_COLOR = 0xff0000;
	static constexpr int TEXT_X = 10;
	static constexpr int TEXT_Y = SCREEN_HEIGHT - 30;
	static constexpr int CLEAR_TEXT_Y = SCREEN_HEIGHT - 15;
	static constexpr int MAX_COLOR = 255;
}

GameScene::GameScene(void) 
	: SceneBase(), 
	sunHandle_(-1),
	backgroundImg_(-1),
	isDraw_(false),
	isClear_(false),
	effect_(nullptr),
	fishColor(0),
	fishModel(-1),
	fishName_(""),
	fishOnImg_(-1),
	fishScale_(0.0f),
	rodSpawnImg_(-1),
	ukiSpawnImg_(-1),
	water_(nullptr)
{
}

GameScene::~GameScene(void)
{
}

void GameScene::Init(void)
{
	backgroundImg_ = LoadGraph(BACKGROUND_PATH);
	fishOnImg_ = LoadGraph(FISHON_PATH);
	rodSpawnImg_ = LoadGraph(ROD_SPAWN_TEXT_PATH);
	ukiSpawnImg_ = LoadGraph(UKI_SPAWN_TEXT_PATH);
	// 初期化処理
	camera_ = new Camera();
	stage_ = new Stage();
	player_ = new Player();
	rod_ = new Rod();
	dobber_ = new Dobber();
	fishmng_ = new FishManager();
	effect_ = new EffekseerEffect();
	water_ = new Water();
	gauge_ = new Gauge();

	stage_->Init();
	player_->Init(camera_);
	rod_->Init(player_, dobber_);
	dobber_->Init(player_, rod_, fishmng_, stage_);
	fishmng_->Init(player_, dobber_, gauge_);
	effect_->Init();
	water_->Init();
	gauge_->Init(player_, dobber_, rod_, fishmng_);
	camera_->Init(rod_, player_);

	SceneManager& sceneManager = SceneManager::GetInstance();
}

void GameScene::Update(void)
{
	if (dobber_->GetFishingFlg())
	{
		// 魚が釣れた時釣れた魚だけを描画させる
		SetFish(fishmng_->GetClosestFishNumber());
		fishmng_->ClosestFishAnimation();
		camera_->Update();
		if (isClear_) 
		{
			if (InputManager::GetInstance().IsTrgDown(KEY_INPUT_SPACE))
			{
				SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE);
			}
		}
		else
		{
			if (InputManager::GetInstance().IsTrgDown(KEY_INPUT_SPACE)) 
			{
				// 初期化
				Reset();
			}
		}
	}
	else 
	{
		// 更新処理
		stage_->Update();
		player_->Update();
		fishmng_->Update();
		rod_->Update();
		dobber_->Update();
		effect_->Update();
		water_->Update();
		camera_->Update();
	}
}

void GameScene::Draw(void)
{
	//　釣れてたらモデルだけ表示
	if (dobber_->GetFishingFlg())
	{
		//　魚が連れたら
		DrawGraph(0, 0, backgroundImg_, true);
		MV1DrawModel(fishModel);
		// 魚の名前
		SetFontSize(FISH_NAME_FONT_SIZE);
		DrawFormatString(0, 0, GetColor(0, 0, 0), fishName_);
		SetFontSize(INFO_FONT_SIZE);
		if (isClear_)
		{
			DrawString(TEXT_X, CLEAR_TEXT_Y,
				"おめでとう！激レア魚「ユーステノプテロン」を釣ることに成功した！", GetColor(MAX_COLOR, MAX_COLOR, MAX_COLOR));
			DrawString(TEXT_X, TEXT_Y,
				"SPACEでタイトル画面に戻る", GetColor(MAX_COLOR, MAX_COLOR, MAX_COLOR));
		}
		else
		{
			DrawString(TEXT_X, TEXT_Y,
				"SPACEでゲーム画面に戻る", GetColor(MAX_COLOR, MAX_COLOR, MAX_COLOR));
		}
	}
	else 
	{
		// 釣れていない場合
		// 描画処理
		stage_->Draw();
		rod_->Draw();
		effect_->Draw();
		fishmng_->Draw();
		dobber_->Draw();
		water_->Draw();
		player_->Draw();
		gauge_->Draw();
		DrawString(0, 0, "目標：激レア魚「ユーステノプテロン」を釣ろう！", GetColor(0, 0, 0));
	}
	Text();
}

void GameScene::Release(void)
{
	// 解放処理
	stage_->Release();
	player_->Release();
	rod_->Release();
	fishmng_->Release();
	dobber_->Release();
	effect_->Release();
	delete effect_;
	water_->Release();
	delete water_;
	DeleteGraph(backgroundImg_);
	camera_->Release();
}

//ゲーム中のリセット処理
void GameScene::Reset(void)
{
	// 魚関連
	fishmng_->Reset();
	fishmng_->Probability();
	// ウキ関連
	dobber_->Reset();
	// 竿関連
	rod_->Reset();
	// ゲージ関連
	gauge_->Reset();
	// プレイヤーを三人称に戻す
	player_->SetCurrentMode(Player::THIRD_PERSON);
}

void GameScene::SetFish(int number)
{
	if (number <= 0 || number >= static_cast<int>(std::size(fishInfos)))
	{
		// 範囲外の番号なら何もしない
		return;
	}

	const FishInfo& fish = fishInfos[number];							// 番号に対応した魚情報を取得
	fishScale_ = fish.scale;											// 魚の大きさを設定
	fishName_ = fish.name;												// 魚の名前を設定
	isClear_ = fish.isRare;												// 激レアかどうかのフラグをセット
	fishModel = fishmng_->GetClosestFishModel();						// 魚の3Dモデルを取得
	MV1SetPosition(fishModel, player_->GetPos());						// 魚モデルの位置をプレイヤー位置に合わせる
	MV1SetScale(fishModel, { fishScale_, fishScale_, fishScale_ });		// 魚モデルのスケールを設定
}

void GameScene::Text(void)
{
	if (rod_->GetFlg())
	{
		if (dobber_->GetIsShot())
		{
			if (fishmng_->GetFishHitFlg())
			{
				DrawExtendGraph(
					FISHON_X,
					FISHON_Y,
					FISHON_WIDTH, FISHON_HEIGHT,
					fishOnImg_,
					true);
			}
		}
		else
		{
			DrawExtendGraph(0, GRAPH_Y, GRAPH_WEIGHT_, GRAPH_HEIGHT, ukiSpawnImg_, true);
			DrawString(0, UKI_TEXT_Y, "矢印キーでウキを調整", 0xff0000);
		}
	}
	else
	{
		DrawExtendGraph(0, GRAPH_Y, GRAPH_WEIGHT_, GRAPH_HEIGHT, rodSpawnImg_, true);
	}
}