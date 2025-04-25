#pragma once
#include "SceneBase.h"
#include "../Application.h"

class TitleScene : public SceneBase
{
public:
	// コンストラクタ
	TitleScene(void);
	// デストラクタ
	~TitleScene(void);
	void Init(void) override;		// 初期化
	void Update(void) override;		// 更新
	void Draw(void) override;		// 描画
	void Release(void) override;	// 解放
private:
	int titleimg;
	VECTOR titlePos_;
	int selection = 0;	//現在の選択位置
	bool selectflg = true;
	int manualimage[5];
	bool manualactive = false;
	int manualselect = 0;
	int mainSoundHundle_;
	int TitleHundle_;
	int fontHandle;
	int textboximg;
	int selecttextimg;

	void OutLine(void);				//アウトライン
	void UpdateSelect(void);		//選択
	void DrawSelect(void);			//選択描画
	void UpdateManual(void);		//操作画面
	void DrawManual(void);			//操作画面描画
	void Drawtext(int x, int y, const char* text, int color);
};
