#pragma once
#include <DxLib.h>

class Player;
class Dobber;
class FishManager;
class Rod;

class Gauge
{
public:
	Gauge(void);				// コンストラクタ
	~Gauge(void);				// デストラクタ
	void Init(Player* player, Dobber* dobber, Rod* rod, FishManager* fishmng);			// 初期化処理
	void Update(void);			// 更新処理
	void Draw(void);			// 描画処理
	void Release(void);			// 解放処理
	void UpdateGauge(void);     // ゲージの更新
	void SetGaugeFlg(const bool flg) { isDraw_ = flg; }
	void Reset(void);
private:
	// ゲージ関連
	int gaugeHandle_;			// ゲージの画像
	float gaugeInterval_;		// 最大ゲージにいる時間
	bool isDraw_;				// ゲージを出すか
	float gaugeValue_;			// ゲージの初期値
	void DrawGauge(float x, float y, float width, float height, float value);     // ゲージの表示

	Player* player_;
	Dobber* dobber_;
	FishManager* fishmng_;
	Rod* rod_;
};


