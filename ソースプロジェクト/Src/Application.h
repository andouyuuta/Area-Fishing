#pragma once
#include <string>

class Application
{

public:

	// スクリーンサイズ
	static constexpr int SCREEN_SIZE_X = 1024;
	static constexpr int SCREEN_SIZE_Y = 640;

	const float FRAME_RATE = (1000.0f / 60.0f);

	int currentTime = 0;
	int lastFrameTime = 0;

	int frameCnt = 0;
	int updateFrameRateTime = 0;

	float frameRate = 0.f;

	// データパス関連
	//-------------------------------------------
	static const std::string PATH_MODEL;
	//-------------------------------------------

	// 明示的にインステンスを生成する
	static void CreateInstance(void);

	// 静的インスタンスの取得
	static Application& GetInstance(void);

	// 初期化
	void Init(void);

	// ゲームループの開始
	void Run(void);

	// リソースの解放
	void Release(void);

	// 初期化成功／失敗の判定
	bool IsInitFail(void) const;

	// 解放成功／失敗の判定
	bool IsReleaseFail(void) const;

	// デルタタイムの取得
	float GetDeltaTime();	
private:

	// 静的インスタンス
	static Application* instance_;

	// 初期化失敗
	bool isInitFail_;

	// 解放失敗
	bool isReleaseFail_;

	// デフォルトコンストラクタをprivateにして、
	// 外部から生成できない様にする
	Application(void);

	// デストラクタも同様
	~Application(void);

	void CalcFrameRate();

	void DrawFrameRate();
};