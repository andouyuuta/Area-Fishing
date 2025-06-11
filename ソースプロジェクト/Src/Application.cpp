#include <DxLib.h>
#include "Manager/InputManager.h"
#include "Manager/SceneManager.h"
#include "Application.h"

Application* Application::instance_ = nullptr;

const std::string Application::PATH_MODEL = "Data/Model/";

void Application::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new Application();
	}
	instance_->Init();
}

Application& Application::GetInstance(void)
{
	return *instance_;
}

void Application::Init(void)
{
	// アプリケーションの初期設定
	SetWindowText("Area Fishing");

	// ウィンドウサイズ
	SetGraphMode(SCREEN_SIZE_X, SCREEN_SIZE_Y, 32);
	ChangeWindowMode(false);		//falseでフルスクリーン

	// DxLibの初期化
	SetUseDirect3DVersion(DX_DIRECT3D_11);
	isInitFail_ = false;
	if (DxLib_Init() == -1)
	{
		isInitFail_ = true;
		return;
	}

	SetDrawScreen(DX_SCREEN_BACK);

	// キー制御初期化
	SetUseDirectInputFlag(true);
	InputManager::CreateInstance();

	// シーン管理初期化
	SceneManager::CreateInstance();

}

void Application::Run(void)
{
	auto& inputManager = InputManager::GetInstance();
	auto& sceneManager = SceneManager::GetInstance();

	lastFrameTime = GetNowCount();
	updateFrameRateTime = lastFrameTime;

	// ゲームループ
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		currentTime = GetNowCount();
		int elapsedTime = currentTime - lastFrameTime;

		if (elapsedTime >= FRAME_RATE)
		{
			lastFrameTime = currentTime;

			// 画面を初期化
			ClearDrawScreen();

			inputManager.Update();
			sceneManager.Update();
			//上のUpdateが終わった後にエラーが発生してる
			sceneManager.Draw();

			frameCnt++;
			CalcFrameRate();
			
			ScreenFlip();
		}
	}
}

void Application::Release(void)
{
	SceneManager::GetInstance().Release();
	// DxLib終了
	if (DxLib_End() == -1)
	{
		isReleaseFail_ = true;
	}
}

bool Application::IsInitFail(void) const
{
	return isInitFail_;
}

bool Application::IsReleaseFail(void) const
{
	return isReleaseFail_;
}

Application::Application(void)
{
	isInitFail_ = false;
	isReleaseFail_ = false;
}

Application::~Application(void)
{
	delete instance_;
}

void Application::CalcFrameRate()
{
	int nDifTime = currentTime - updateFrameRateTime;

	if (nDifTime > 1000)
	{
		float fFrameCnt = (float)(frameCnt * 1000);

		frameRate = fFrameCnt / nDifTime;

		frameCnt = 0;

		updateFrameRateTime = currentTime;
	}
}

void Application::DrawFrameRate()
{
	DrawFormatString(0, 0, GetColor(0, 0, 0), "FPS[%.2f", frameRate);
}

float Application::GetDeltaTime()
{
	// 現在のFPSを取得
	float fps = GetFPS();

	// FPSが0の場合は安全に1を返す
	if (fps == 0.0f) return 1.0f;

	// デルタタイムをFPSから逆算
	return 1.0f / fps;
}
