#include <string>
#include <DxLib.h>
#include "../Utility/Utility.h"
#include "../Manager/SceneManager.h"
#include "../Manager/InputManager.h"
#include "../Manager/Camera.h"
#include "TitleScene.h"

// コンストラクタ
TitleScene::TitleScene(void)
	: TitleHundle_ (-1), mainSoundHundle_(-1), titlePos_{0.0f,0.0f,0.0f}, titleimg(-1),fontHandle(-1),textboximg(-1)
{
}

// デストラクタ
TitleScene::~TitleScene(void) {}

// 初期化
void TitleScene::Init(void)
{
	titleimg = LoadGraph("Data/Image/Title.png");
	manualimage[0] = LoadGraph("Data/Image/manual1-1.png");
	manualimage[1] = LoadGraph("Data/Image/manual2-1.png");
	manualimage[2] = LoadGraph("Data/Image/manual4-1.png");
	manualimage[3] = LoadGraph("Data/Image/manual3-1.png");
	manualimage[4] = LoadGraph("Data/Image/manual5-1.png");

	mainSoundHundle_ = LoadSoundMem("Data/Sound/download_c3cd42c09c.mp3");

	textboximg = LoadGraph("Data/Image/textbox.png");
	selecttextimg = LoadGraph("Data/Image/selecttext.png");

	AddFontResourceEx("Data/Font/Nikumaru.otf", FR_PRIVATE, NULL);

	TitleHundle_ = LoadGraph("Data/Image/titlelogo.png");
}

// 更新
void TitleScene::Update(void)
{
	if (CheckSoundMem(mainSoundHundle_) == 0) {
		PlaySoundMem(mainSoundHundle_, DX_PLAYTYPE_LOOP);
	}
	ChangeFont("07にくまるフォント");

	// シーン遷移
	InputManager& ins = InputManager::GetInstance();

	UpdateSelect();		//モード選択画面の処理

	UpdateManual();		//操作説明の処理

	// マウスの左クリックによる選択処理
	if (ins.IsTrgDown(KEY_INPUT_RETURN)) {
		switch (selection) {
		case 0:
			// ゲーム開始処理
			SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAME);
			break;
		case 1:
			//ENTERキーでモード選択画面に戻す
			if (ins.IsTrgDown(KEY_INPUT_RETURN)) {
				manualactive = !manualactive;
				selectflg = true;
			}
			break;
		case 2:
			DxLib_End();
			break;
		}
	}
}

// 描画
void TitleScene::Draw(void)
{
	DrawGraph(0, 0, titleimg, true);
	DrawGraph(160, 10, TitleHundle_, true);

	//モード選択画面の描画
	if (selectflg) {
		DrawSelect();
	}

	// 操作説明の描画
	if (manualactive) {
		DrawManual();
	}
}

// 解放
void TitleScene::Release(void)
{
	// タイトルロゴ削除
	DeleteGraph(TitleHundle_);
	DeleteGraph(manualimage[0]);
	DeleteGraph(manualimage[1]);
	DeleteGraph(manualimage[2]);
	DeleteGraph(manualimage[3]);
	DeleteGraph(manualimage[4]);

	DeleteSoundMem(mainSoundHundle_);
	// 作成したフォントデータを削除する
	//DeleteFontToHandle(fontHandle);
	RemoveFontResourceEx("Data/Font/Nikumaru.otf", FR_PRIVATE, NULL);
}

//アウトライン
void TitleScene::OutLine(void)
{
	//Drawtext(100, 299, "　↑↓キーで移動", GetColor(0, 0, 0)); // 上
	//Drawtext(100, 301, "　↑↓キーで移動", GetColor(0, 0, 0)); // 下
	//Drawtext(99, 300, "　↑↓キーで移動", GetColor(0, 0, 0)); // 左
	//Drawtext(101, 300, "　↑↓キーで移動", GetColor(0, 0, 0)); // 右	

	//Drawtext(100, 324, "EnterキーでSelect", GetColor(0, 0, 0)); // 上
	//Drawtext(100, 326, "EnterキーでSelect", GetColor(0, 0, 0)); // 下
	//Drawtext(99, 325, "EnterキーでSelect", GetColor(0, 0, 0)); // 左
	//Drawtext(101, 325, "EnterキーでSelect", GetColor(0, 0, 0)); // 右	

	//Drawtext(100, 399, "1. ゲームスタート", GetColor(0, 0, 0)); // 上
	//Drawtext(100, 401, "1. ゲームスタート", GetColor(0, 0, 0)); // 下
	//Drawtext(99, 400, "1. ゲームスタート", GetColor(0, 0, 0)); // 左
	//Drawtext(101, 400, "1. ゲームスタート", GetColor(0, 0, 0)); // 右	

	//Drawtext(100, 474, "2. 操作設定", GetColor(0, 0, 0)); // 上
	//Drawtext(100, 476, "2. 操作設定", GetColor(0, 0, 0)); // 下
	//Drawtext(99, 475, "2. 操作設定", GetColor(0, 0, 0)); // 左
	//Drawtext(101, 475, "2. 操作設定", GetColor(0, 0, 0)); // 右	

	//Drawtext(100, 549, "3. 終了", GetColor(0, 0, 0)); // 上
	//Drawtext(100, 551, "3. 終了", GetColor(0, 0, 0)); // 下
	//Drawtext(99, 550, "3. 終了", GetColor(0, 0, 0)); // 左
	//Drawtext(101, 550, "3. 終了", GetColor(0, 0, 0)); // 右	
}


void TitleScene::UpdateSelect(void)
{
	// シーン遷移
	InputManager& ins = InputManager::GetInstance();

	if (selectflg) {
		if (ins.IsTrgDown(KEY_INPUT_DOWN)) {
			selection = (selection + 1) % 3;
		}
		else if (ins.IsTrgDown(KEY_INPUT_UP)) {
			selection = (selection + 2) % 3;
		}
	}
}

void TitleScene::DrawSelect(void)
{
	DrawExtendGraph(60, 270, 350, 360, selecttextimg, true);
	DrawExtendGraph(60, 380, 350, 590, textboximg, true);

	SetFontSize(24);

	DrawString(100, 300, "　↑↓キーで移動", GetColor(255, 0, 0));
	DrawString(100, 325, "EnterキーでSelect", GetColor(255, 0, 0));

	DrawString(100, 400, "1. ゲームスタート", selection == 0 ? GetColor(255, 0, 0) : GetColor(0, 0, 0));
	DrawString(100, 475, "2. 操作設定", selection == 1 ? GetColor(255, 0, 0) : GetColor(0, 0, 0));
	DrawString(100, 550, "3. 終了", selection == 2 ? GetColor(255, 0, 0) : GetColor(0, 0, 0));
}

void TitleScene::UpdateManual(void)
{
	// シーン遷移
	InputManager& ins = InputManager::GetInstance();

	if (manualactive) {
		if (ins.IsTrgDown(KEY_INPUT_RIGHT)) {
			manualselect = (manualselect + 1) % 5;
		}
		else if (ins.IsTrgDown(KEY_INPUT_LEFT)) {
			manualselect = (manualselect + 4) % 5;
		}
	}
}

void TitleScene::DrawManual(void)
{
	SetFontSize(16);
	switch (manualselect) {
	case 0:
		DrawGraph(0, 0, manualimage[0], true);
		DrawString(Application::SCREEN_SIZE_X - 70, Application::SCREEN_SIZE_Y - 30, "１/５", GetColor(0, 0, 0));
		break;
	case 1:
		DrawGraph(0, 0, manualimage[1], true);
		DrawString(Application::SCREEN_SIZE_X - 70, Application::SCREEN_SIZE_Y - 30, "２/５", GetColor(0, 0, 0));
		break;
	case 2:
		DrawGraph(0, 0, manualimage[2], true);
		DrawString(Application::SCREEN_SIZE_X - 70, Application::SCREEN_SIZE_Y - 30, "３/５", GetColor(0, 0, 0));
		break;
	case 3:
		DrawGraph(0, 0, manualimage[3], true);
		DrawString(Application::SCREEN_SIZE_X - 70, Application::SCREEN_SIZE_Y - 30, "４/５", GetColor(0, 0, 0));
		break;
	case 4:
		DrawGraph(0, 0, manualimage[4], true);
		DrawString(Application::SCREEN_SIZE_X - 70, Application::SCREEN_SIZE_Y - 30, "５/５", GetColor(0, 0, 0));
		break;
	}
	DrawString(0, 0, "←キー：戻る　　→キー：進む", GetColor(0, 0, 0));;
	DrawString(Application::SCREEN_SIZE_X - 250, 0, "ENTERキーでモード選択に戻る", GetColor(0, 0, 0));
}

void TitleScene::Drawtext(int x, int y, const char* text, int color)
{

	DrawStringToHandle(x, y, text, color, fontHandle);
}