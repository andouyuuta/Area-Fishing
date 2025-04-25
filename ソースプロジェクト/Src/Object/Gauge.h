#pragma once
#include <DxLib.h>

class Gauge
{
public:
	// インスタンスの生成
	static void CreateInstance(void);
	// インスタンスの取得
	static Gauge& GetInstance(void);
	Gauge();
	~Gauge();
	// 初期化
	void Init(void);
	// 更新
	void Update(void);
	// 描画
	void Draw(void);
	// 解放処理
	void Release(void);
	void UpdateGauge(void);     //ゲージの更新
	void SetGaugeFlg(const bool flg) { gaugeflg = flg; }
	void Reset(void);
private:
	static Gauge* instance_;
	//ゲージ関連
	int gagugebackhandle;		//ゲージの画像
	float maxValue = 100.0f;	//最大ゲージ
	float minValue = 0.0f;		//最小ゲージ
	float gaugeInterval = 0;	//最大ゲージにいる時間
	bool gaugeflg = false;
	float gaugeValue = 50;		//ゲージの初期値
	VECTOR GaugePos;			//ゲージの座標
	void DrawGauge(const int x, const int y, const int width, const int height, const float value);     //ゲージの表示
};


