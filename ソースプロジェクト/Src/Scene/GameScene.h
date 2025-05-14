#pragma once
#include "SceneBase.h"

class Stage;
class Camera;
class Player;
class Enemy;
class Rod;
class Fish;
class Dobber;
class EffekseerEffect;
class Fishing;
class Water;

class GameScene : public SceneBase
{

public:
	GameScene(void);
	~GameScene(void) override;
	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Release(void) override;
	void Initialize(void);		//ゲーム中のリセット処理
private:
	int backgroundimg;			//釣った時の背景
	int fishonimg;				//釣れた時の画像
	int rodspawnimg;			//竿を出現させるときの画像
	int ukispawnimg;			//ウキを投げる時の画像
	int fishmodel;				//魚モデル
	int fishcolor;				//色
	const char* fishname;		//名前
	float fishscale;			//大きさ
	bool drawflg;				//描画するか
	bool clearflg;				//クリアしたか
	EffekseerEffect* effect_;	
	Water* water_;
	Camera* camera_;
	int SunHundle;				
	void SetFish(int number);	//釣れた時のポジションやスケールを変更するメソッド
	void Text(void);			//テキスト画面
};
