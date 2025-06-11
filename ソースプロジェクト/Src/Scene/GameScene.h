#pragma once
#include "SceneBase.h"

class Stage;
class Camera;
class Player;
class Enemy;
class Rod;
class Fish;
class FishManager;
class Dobber;
class EffekseerEffect;
class Water;
class Gauge;

class GameScene : public SceneBase
{

public:
	GameScene(void);
	~GameScene(void) override;
	void Init(void);			// 初期化処理
	void Update(void);			// 更新処理
	void Draw(void);			// 描画処理
	void Release(void);			// 解放処理
	void Reset(void);			// ゲーム中のリセット処理
private:
	int backgroundImg_;			// 釣った時の背景
	int fishOnImg_;				// 釣れた時の画像
	int rodSpawnImg_;			// 竿を出現させるときの画像
	int ukiSpawnImg_;			// ウキを投げる時の画像
	int fishModel;				// 魚モデル
	int fishColor;				// 色
	const char* fishName_;		// 名前
	float fishScale_;			// 大きさ
	bool isDraw_;				// 描画するか
	bool isClear_;				// クリアしたか

	Player* player_;
	Stage* stage_;
	Rod* rod_;
	Dobber* dobber_;
	FishManager* fishmng_;
	Camera* camera_;
	Gauge* gauge_;
	EffekseerEffect* effect_;	
	Water* water_;
	int sunHandle_;				
	void SetFish(int number);	// 釣れた時のポジションやスケールを変更するメソッド
	void Text(void);			// テキスト画面
};
