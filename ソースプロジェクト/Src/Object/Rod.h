#pragma once
#include <DxLib.h>

class Player;

class Rod
{
public:
	enum class STATE
	{
		SPAWN,		//出し入れ
		KAMAE,		//構え
		THROW,		//投げる
		AWASE		//合わせ
	};
	// インスタンスの生成
	static void CreateInstance(void);
	// インスタンスの取得
	static Rod& GetInstance(void);
	Rod(void);
	~Rod(void);
	void Init(void);		//初期化処理
	void Update(void);		//更新処理
	void SpawnUpdate(void);
	void KamaeUpdate(void);
	void ThrowUpdate(void);
	void AwaseUpdate(void);
	void Draw(void);		//描画処理
	void Release(void);		//解放処理
	[[nodiscard]] VECTOR GetAngle(void) const { return rot_; }	//方向の取得
	[[nodiscard]] bool GetFlg() const { return rodFlg; }
	[[nodiscard]] float GetRotY() const { return rot_.y; }
	void Reset(void);
private:
	// 静的インスタンス
	static Rod* instance_;

	STATE state_;
	int rodmodel_;		//竿のハンドルID
	bool rodFlg;		//描画フラグ
	//アニメーションフラグ
	bool rodanimFlg;	
	bool kamaeanimFlg;
	bool throwanimFlg;
	// アニメーションのアタッチ番号
	int animAttachNo_;
	int animrodAttachNo_;
	int kamaerodAttachNo_;
	int throwanimAttachNo_;
	// アニメーションの総再生時間
	float animTotalTime_;
	float animrodTotalTime_;
	float kamaerodTotalTime_;
	float throwanimTotalTime_;
	// 再生中のアニメーション時間
	float currentAnimTime_;
	float currentrodAnimTime_;
	float currentkamaeAnimTime_;
	float currentthrowAnimTime_;
	VECTOR scale_ = { 0.25f,0.25f,-0.25f };	//大きさ
	VECTOR pos_;							//座標
	VECTOR rot_;							//回転
	void UpdateAnimation(void);				//アニメーション更新
	void RodProcess(void);					//竿の全体の動き
	void RodMove(void);						//移動処理
	void ChangeState(const STATE state);	//アニメーション切り替え
	void HandleInput();						
	void UpdateRodmodel()const;
	void ToggleRod();
};

