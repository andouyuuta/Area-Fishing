#pragma once
#include <DxLib.h>

class Stage
{
public:
	static constexpr float RADIUS = 2000.0f;

	Stage(void);			// コンストラクタ
	~Stage(void);			// デストラクタ
	void Init(void);		// 初期化処理
	void Update(void);		// 更新処理
	void Draw(void);		// 描画処理
	void Release(void);		// 解放処理
	[[nodiscard]]int GetModelId(void)const { return stageModel_; }
	[[nodiscard]]VECTOR GetPos()const { return stagePos_; }
private:
	int stageModel_;		// ステージモデル
	int backGroundModel_;	// 背景モデル
	int windmillModel_;		// 風車モデル
	int fallSoundHandle_;	// 滝の音
	int envSoundHandle_;	// 鳥の音

	VECTOR backGroundPos_;	// 背景座標
	VECTOR backGroundScale_;// 背景大きさ
	VECTOR soundPos_;		// 音の座標
	VECTOR stageScale_ ;	// ステージの大きさ
	VECTOR stagePos_;		// ステージの座標
	VECTOR windmillPos_;	// 風車座標
	VECTOR windmillRot_;	// 風車回転
	void WindmillMove(void);// 風車の動き
};