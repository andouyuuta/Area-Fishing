#pragma once
#include <DxLib.h>
#include <map>

class Player;
class Dobber;

class Rod
{
public:
	enum class STATE
	{
		SPAWN,		// 出し入れ
		KAMAE,		// 構え
		THROW,		// 投げる
	};

	// アニメーション種別
	enum class AnimationType
	{
		RodSpawn,
		RodKamae,
		RodThrow,
		Count
	};

	struct AnimationData
	{
		int attachIndex_ = -1;
		float totalTime_ = 0.0f;
		float currentTime_ = 0.0f;
		bool isPlaying_ = false;
	};

	Rod(void);				// コンストラクタ
	~Rod(void);				// デストラクタ
	void Init(Player* player,Dobber* dobber);		// 初期化処理
	void Update(void);		// 更新処理
	void Draw(void);		// 描画処理
	void Release(void);		// 解放処理

	[[nodiscard]] VECTOR GetAngle(void) const { return rot_; }	// 方向の取得
	[[nodiscard]] bool GetFlg() const { return isDraw_; }
	void Reset(void);
private:

	std::map<AnimationType, AnimationData>animations_;

	STATE state_;
	int rodModel_;		// 竿のハンドルID
	bool isDraw_;		// 描画フラグ
	VECTOR scale_;		// 大きさ
	VECTOR pos_;		// 座標
	VECTOR rot_;		// 回転

	// アニメーションデータ

	void UpdateAnimation(void);				// アニメーション更新
	void RodProcess(void);					// 竿の全体の動き
	void RodMove(void);						// 移動処理
	void ChangeState(STATE state);			// アニメーション切り替え
	void HandleInput(void);						
	void ToggleRod(void);					// 竿の状態遷移
	bool IsCurrentAnimationPlaying() const;	// アニメーションさせるか

	Player* player_;
	Dobber* dobber_;
};

