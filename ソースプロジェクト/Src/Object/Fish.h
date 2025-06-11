#pragma once
#include <DxLib.h>
#include <random>

class Player;
class Dobber;
class FishManager;

class Fish
{
public:
	enum FishType {
		None = 0,
		Ayu,
		Yamame,
		Haya,
		Huna,
		Oikawa,
		Medaka,
		Kawamutu,
		Iwana,
		Poriputerusu,
		Nizimasu,
		Ugui,
		Wakasagi,
		Uthtenoputeron,

		MAXKIND,
	};

	Fish(void);					// コンストラクタ
	~Fish(void);				// デストラクタ
	void Init(Player* player, Dobber* dobber, FishManager* fishmng);		// 初期化処理
	void Update(void);		// 更新処理
	void Draw(void);		// 描画処理
	void Release(void);		// 解放処理

	void FishAnimation(void);
	void FishFreedomMove(void);					// 魚を何もしていないときに自由に動かす
	void Move(VECTOR pos, const int fishidx);	// 魚の全体の動き
	void FishCome(int& fishidx);				// ウキが着水したら一番近い魚が寄ってくる
	void FishingInterval(void);					// 制限時間以内に左クリックを押したら釣りを開始する
	void ChangeFish(FishType newtype);
	void Reset(void);							//リセット

	[[nodiscard]] int GetFishModel(void) const { return fishModel_; }
	[[nodiscard]] VECTOR GetFishPos(void) const { return fishPos_; }
	[[nodiscard]] bool GetLeftClickPressFlg(void) const { return isLeftClickPress; }
	[[nodiscard]] int GetFishNumber(void) const { return fishNumber_; }
	void SetAnimAttachNo(int model) { animAttachNo_ = MV1AttachAnim(model, 0, -1, -1); }
	void SetAnimTotalTime(int model) { animTotalTime_ = MV1GetAttachAnimTotalTime(model, animAttachNo_); }
	void SetCurrentAnimTime(float CurrentTime) { currentAnimTime_ = CurrentTime; }
	void SetFishModel(const int model) { fishModel_ = model; }
	void SetInterval(const float interval) { fishInterval_ = interval; }
	void SetCountFlg(const bool flg) { isIntervalCount = flg; }
	void SetType(const FishType fishtype) { type_ = fishtype; }
	void SetLeftClickPressFlg(const bool flg) { isLeftClickPress = flg; }
	void SetFishManager(FishManager* manager){fishmng_ = manager;}
private:
	// 情報
	FishType type_;						// 状態
	int fishModel_;						// モデル
	VECTOR fishPos_;					// 座標
	VECTOR prevFishPos_;				// 一フレーム前の座標
	VECTOR fishScale_;					// 大きさ

	float fishInterval_;				// 食いついている時間
	bool isIntervalCount;				// 食いついているか
	float timeToMove;					// 左右に動かす時間
	bool isLeftClickPress;				// 左クリック押しているか
	float angle_;						// 回転
	int direction_;						// どちらに動かすか
	int fishNumber_;					// 魚の番号
	float currentAngleY_;				// 現在の向いている場所
	float swimTimer_;					// 魚が泳いでいる時間をカウントするタイマー
	VECTOR currentDirection_;			// 魚ごとの方向
	float changeDirectionInterval_;		// 個別の間隔

	// アニメーション
	int  animAttachNo_;					// アニメーションナンバー
	float animTotalTime_;				// アニメーション合計時間
	float currentAnimTime_;				// 現在のアニメーション時間

	// 状態遷移
	void MoveLR(void);					// 魚を左右のどちらかに移動させるかを決める
	void FishIntervalCount(void);		// 魚がFishOnしたときの待ち時間
	void FishFreedomRotate(void) const;	// 魚を自由に動かせる
	void UpdateRotationToDirection(const VECTOR& direction);
	Player* player_;
	Dobber* dobber_;
	FishManager* fishmng_;
};

