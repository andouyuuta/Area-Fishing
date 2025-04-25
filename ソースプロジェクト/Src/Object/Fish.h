#pragma once
#include <DxLib.h>
#include <random>

class Player;
class Dobber;

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

	Fish();
	~Fish();
	void Init(void);
	void Update(void);
	void Draw(void) const;
	void Release(void);

	void FishSwimLogic(void);						//魚を何もしていないときに自由に動かす
	void Move(VECTOR pos, const int fishidx);		//魚の全体の動き
	void FishCome(int &fishidx);				//ウキが着水したら一番近い魚が寄ってくる
	void FishingInterval(Dobber& dobber);		//制限時間以内に左クリックを押したら釣りを開始する
	void ChangeFish(FishType newtype);
	void Reset(void);		//リセット
	[[nodiscard]] int GetFishModel(void) const { return fishmodel; }
	[[nodiscard]] VECTOR GetFishPos(void) const { return FishPos; } 
	[[nodiscard]] bool GetIntervalFlg(void) const { return intervalcountflg; }
	[[nodiscard]] bool Getleftclickpressflg(void) const { return leftclickpressflg; }
	[[nodiscard]] FishType GetFishType(void)const { return type_; }
	[[nodiscard]] VECTOR GetFishScale(void) { return { fishscale,fishscale,fishscale }; }
	[[nodiscard]] int  GetAnimAttachNo(void) const { return animAttachNo_; }
	[[nodiscard]] float GetCurrentAnimTime(void) const { return currentAnimTime_; }
	[[nodiscard]] int GetFishNumber(void) const { return fishnumber; }
	void SetAnimAttachNo(int model) { animAttachNo_ = MV1AttachAnim(model, 0, -1, -1); }
	void SetAnimTotalTime(int model) { animTotalTime_ = MV1GetAttachAnimTotalTime(model, animAttachNo_); }
	void SetCurrentAnimTime(float CurrentTime) { currentAnimTime_ = CurrentTime; }
	void SetFishModel(const int model) { fishmodel = model; }
	void SetInterval(const float interval) { fishInterval = interval; }
	void SetCountFlg(const bool flg) { intervalcountflg = flg; }
	void SetType(const FishType fishtype) { type_ = fishtype; }
	void Setleftclickpressflg(const bool flg) { leftclickpressflg = flg; }
private:
	FishType type_;						//状態
	VECTOR FishPos;						//座標
	VECTOR PrevFishPos;					//一フレーム前の座標
	int fishmodel;						//モデル
	float fishInterval;					//食いついている時間
	bool intervalcountflg;		//食いついているか
	float interval;				//左右に動かす時間
	bool leftclickpressflg;		//左クリック押しているか
	float fishscale;					//大きさ
	float angle;					//回転
	int direction;					//どちらに動かすか
	int fishnumber;						//魚の番号
	float currentAngleY;				//現在の向いている場所

	//アニメーション
	int  animAttachNo_;
	float animTotalTime_;
	float currentAnimTime_;

	float swimTimer;  // 魚が泳いでいる時間をカウントするタイマー
	VECTOR currentDirection; //魚ごとの方向
	float changeDirectionInterval;   //個別の間隔

	//状態遷移
	void MoveLR();								//魚を左右のどちらかに移動させるかを決める
	void FishIntervalCount(void);					//魚がFishOnしたときの待ち時間
	void SwimRogicRotate(void) const;			//魚を自由に動かせる

	inline float Clamp(float value, float min, float max) {
		return (value < min) ? min : (value > max) ? max : value;
	}
};

