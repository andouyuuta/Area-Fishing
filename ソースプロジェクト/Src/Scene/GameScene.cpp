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

GameScene::GameScene(void) : SceneBase()
{
	SunHundle = -1, backgroundimg = -1, drawflg = false, clearflg = false, effect_ = nullptr, fishcolor = 0, fishmodel = -1, fishname = "", fishonimg = -1,
		fishscale = 0.0f, rodspawnimg = -1, ukispawnimg = -1, water_ = nullptr;
}

GameScene::~GameScene(void)
{
}

void GameScene::Init(void)
{
	backgroundimg = LoadGraph("Data/Image/result.png");
	fishonimg = LoadGraph("Data/Image/FishOn.png");
	rodspawnimg = LoadGraph("Data/Image/RodSpawn.png");
	ukispawnimg = LoadGraph("Data/Image/UkiSpawn.png");
	// 初期化処理
	Stage::CreateInstance();
	Stage::GetInstance().Init();
	Player::CreateInstance();
	Player::GetInstance().Init();
	Rod::CreateInstance();
	Rod::GetInstance().Init();
	FishManager::CreateInstance();
	FishManager::GetInstance().Init();
	Dobber::CreateInstance();
	Dobber::GetInstance().Init();
	effect_ = new EffekseerEffect();
	effect_->Init();
	water_ = new Water();
	water_->Init();
	Gauge::CreateInstance();
	Gauge::GetInstance().Init();
	SceneManager& sceneManager = SceneManager::GetInstance();
	Camera* camera = sceneManager.GetCamera();
}

void GameScene::Update(void)
{
	if (Dobber::GetInstance().GetfishingFlg())
	{
		SetFish(FishManager::GetInstance().GetClosestFishNumber());
		if (clearflg) {
			if (InputManager::GetInstance().IsTrgDown(KEY_INPUT_SPACE)) {
				SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE);
			}
		}
		else {
			if (InputManager::GetInstance().IsTrgDown(KEY_INPUT_SPACE)) {
				Initialize();
			}
		}
	}
	else {
		//更新処理
		Stage::GetInstance().Update();
		Player::GetInstance().Update();
		FishManager::GetInstance().Update();
		Rod::GetInstance().Update();
		Dobber::GetInstance().Update();
		effect_->Update();
		water_->Update();
	}
}

void GameScene::Draw(void)
{
	//釣れてたらモデルだけ表示
	if (Dobber::GetInstance().GetfishingFlg())
	{
		//魚が連れたら
		DrawGraph(0, 0, backgroundimg, true);
		MV1DrawModel(fishmodel);
		// 魚の名前
		SetFontSize(48);
		DrawFormatString(0, 0, GetColor(0, 0, 0), fishname);
		SetFontSize(16);
		if (clearflg) {
			DrawString(10, Application::SCREEN_SIZE_Y - 30, "おめでとう！激レア魚「ユーステノプテロン」を釣ることに成功した！", GetColor(255, 255, 255)); // 上
			DrawString(10, Application::SCREEN_SIZE_Y - 15, "SPACEでタイトル画面に戻る", GetColor(255, 255, 255)); // 上
		}
		else {
			DrawString(10, Application::SCREEN_SIZE_Y - 50, "SPACEでゲーム画面に戻る", GetColor(255, 255, 255)); // 上
		}
	}
	else {
		//連れてない場合
		//描画処理
		Stage::GetInstance().Draw();
		Rod::GetInstance().Draw();
		effect_->Draw();
		FishManager::GetInstance().Draw();
		Dobber::GetInstance().Draw();
		water_->Draw();
		Player::GetInstance().Draw();
		Gauge::GetInstance().Draw();
		DrawString(0, 0, "目標：激レア魚「ユーステノプテロン」を釣ろう！", GetColor(0, 0, 0));
	}
	Text();
}

void GameScene::Release(void)
{
	//解放処理
	Stage::GetInstance().Release();
	Player::GetInstance().Release();
	Rod::GetInstance().Release();
	FishManager::GetInstance().Release();
	Dobber::GetInstance().Release();
	effect_->Release();
	delete effect_;
	water_->Release();
	delete water_;
	DeleteGraph(backgroundimg);
}

//ゲーム中のリセット処理
void GameScene::Initialize(void)
{
	//魚関連
	FishManager::GetInstance().Reset();
	FishManager::GetInstance().Probability();
	//ウキ関連
	Dobber::GetInstance().Reset();
	//竿関連
	Rod::GetInstance().Reset();
	//ゲージ関連
	Gauge::GetInstance().Reset();
	//プレイヤーを三人称に戻す
	Player::GetInstance().SetCurrentMode(Player::THIRD_PERSON);
}

void GameScene::SetFish(int number)
{
	switch (number) {
	case 1:
		fishscale = 0.75f;
		fishname = "アユ";
		break;
	case 2:
		fishscale = 0.75f;
		fishname = "ヤマメ";
		break;
	case 3:
		fishscale = 0.75f;
		fishname = "ハヤ";
		break;
	case 4:
		fishscale = 0.5f;
		fishname = "フナ";
		break;
	case 5:
		fishscale = 0.5f;
		fishname = "オイカワ";
		break;
	case 6:
		fishscale = 5.0f;
		fishname = "メダカ";
		break;
	case 7:
		fishscale = 0.75f;
		fishname = "カワムツ";
		break;
	case 8:
		fishscale = 0.6f;
		fishname = "イワナ";
		break;
	case 9:
		fishscale = 0.2f;
		fishname = "ポリプテルス";
		break;
	case 10:
		fishscale = 0.3f;
		fishname = "ニジマス";
		break;
	case 11:
		fishscale = 0.5f;
		fishname = "ウグイ";
		break;
	case 12:
		fishscale = 0.5f;
		fishname = "ワカサギ";
		break;
	case 13:
		fishscale = 0.3f;
		fishname = "ユーステノプテロン";
		clearflg = true;
		break;
	default:
		break;
	}

	// モデルに関すること
	fishmodel = FishManager::GetInstance().GetClosestFishModel();
	VECTOR playerpos = Player::GetInstance().GetPos();
	MV1SetPosition(fishmodel, playerpos);
	MV1SetScale(fishmodel, { fishscale,fishscale,fishscale });
}

void GameScene::Text(void)
{
	if (Rod::GetInstance().GetFlg()) {
		if (Dobber::GetInstance().GetIsShot()) {
			if (FishManager::GetInstance().GetFishHitFlg()) {
				DrawExtendGraph(Application::SCREEN_SIZE_X / 2 - 400, Application::SCREEN_SIZE_Y / 2 - 150, 600, 300, fishonimg, true);
			}
		}
		else {
			DrawExtendGraph(0, 50, 250, 100, ukispawnimg, true);
			DrawString(0, 130, "矢印キーでウキを調整", 0xff0000);
		}
	}
	else {
		DrawExtendGraph(0, 50, 250, 100, rodspawnimg, true);
	}
}