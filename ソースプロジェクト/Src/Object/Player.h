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

	Player(void);						// コンストラクタ
	~Player(void);						// デストラクタ
	void Init(Camera* camera);			// 初期化処理
	void Update(void);					// 更新処理
	void Draw(void);					// 描画処理
	void Release(void);					// 解放処理
	[[nodiscard]] VECTOR GetPos(void) const { return pos_; }
	[[nodiscard]] VECTOR GetAngle(void) const { return rot_; }
	[[nodiscard]] int GetModel(void) const { return playerModel_; }
	[[nodiscard]] PlayerMode GetCurrentMode()const { return currentMode_; }
	void SetCurrentMode(PlayerMode mode) { currentMode_ = mode; }
private:
	PlayerMode currentMode_;			// 現在のモード
	int effectResourceHandle_;			// 川のエフェクト
	int playingEffectHandle_;			// プレイヤーのエフェクト
	int playerModel_;					// モデルID

	int animAttachNo_;					// アニメーションのアタッチ番号
	float animTotalTime_;				// アニメーションの総再生時間
	float currentAnimTime_;				// 再生中のアニメーション時間
	float moveSpeed_;					// 動くスピード
	// 音関連
	int boatSoundHandle_;				// ボートの音
	bool isSoundPlaying_;				// 音が出ているか

	VECTOR pos_;						// 表示座標
	VECTOR moveVec_;					// 移動ベクトル
	VECTOR rot_;						// 角度
	VECTOR targetRot_ ;					// 目標の回転角度
	VECTOR localRot_;					// 初期角度
	VECTOR scale_;						// 大きさ

	void UpdateAnimation(void);			// アニメーション更新
	void UpdateMove(void);				// 移動更新
	bool IsMove(const VECTOR _moveVec);	// ベクトルの移動判定
	void SetRotation(void) const;		// 回転設定
	void UpdateRotation(void);			// 回転処理
	bool IsAnyKeyPressed();				// キーが押されているか

	Camera* camera_;
};