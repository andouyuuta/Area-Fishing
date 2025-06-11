#pragma once
#include <DxLib.h>

class Water
{
public:
	Water(void);
	~Water(void);
	void Init(void);		// 初期化処理
	void Update(void);		// 更新処理
	void Draw(void);		// 描画処理
	void Release(void);		// 解放処理
private:
	VECTOR pos_;			// 座標
	VECTOR scale_;			// 大きさ
	int waterModel_;		// 水モデル
};