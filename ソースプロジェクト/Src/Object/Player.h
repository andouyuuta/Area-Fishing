#pragma once
#include <DxLib.h>

class Camera;

class Player
{

public:
	enum PlayerMode {
		THIRD_PERSON,
		FIRST_PERSON,
	};
	// インスタンスの生成
	static void CreateInstance(void);
	// インスタンスの取得
	static Player& GetInstance(void);

	Player(void);
	~Player(void);
	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);
	[[nodiscard]] VECTOR GetPos(void) const { return playerpos_; }
	[[nodiscard]] VECTOR GetAngle(void) const { return rot_; }
	[[nodiscard]] int GetModel(void) const { return playermodel_; }
	[[nodiscard]] PlayerMode GetCurrentMode()const { return currentMode_; }
	void SetCurrentMode(PlayerMode mode) { currentMode_ = mode; }
	void SetCamera(Camera* camera) { camera_ = camera; }
private:
	// 静的インスタンス
	static Player* instance_;

	PlayerMode currentMode_;
	int effectResourceHandle;
	int playingEffectHandle;
	// モデルID
	int playermodel_;
	// アニメーションのアタッチ番号
	int animAttachNo_;
	int kamaerodAttachNo_;
	int throwanimAttachNo_;
	// アニメーションの総再生時間
	float animTotalTime_;
	float kamaerodTotalTime_;
	float throwanimTotalTime_;
	// 再生中のアニメーション時間
	float currentAnimTime_;
	float currentkamaeAnimTime_;
	float currentthrowAnimTime_;
	float movespeed = 0.0f;
	//音関連
	int boatSoundHundle;
	int boatstopHundle;
	bool isSoundPlaying;
	int FallSoundHundle_;
	int RiverSoundHundle_;
	float previousTime = 0.0f;
	bool keyFlg = false;
	float RADIUS = 20000.0f;
	// 表示座標
	VECTOR playerpos_;	
	VECTOR camerarot_;
	VECTOR playerrot_;
	//移動ベクトル
	VECTOR moveVec_;
	VECTOR moveRot_;
	//移動ベクトルが生成する角度
	VECTOR moveVecRad_;
	//角度
	VECTOR rot_;
	//目標の回転角度
	VECTOR targetRot_ = { 0.0f,0.0f,0.0f };
	//調整用初期角度
	VECTOR localRot_;
	VECTOR scale_ = { 0.35f,0.35f,0.35f };
	VECTOR SoundPos;
	// アニメーション更新
	void UpdateAnimation(void);
	//移動更新
	void UpdateMove(void);
	//ベクトルの移動判定
	bool IsMove(const VECTOR _moveVec);
	//回転設定
	void SetRotation(void);
	//回転処理
	void UpdateRotation(void);
	float GetDeltaTime();
	//キーが押されているか
	bool IsAnyKeyPressed();

	Camera* camera_ = nullptr;
};