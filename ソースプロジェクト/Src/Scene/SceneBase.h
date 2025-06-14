#pragma once

class SceneBase
{
public:
	// コンストラクタ
	SceneBase(void);
	// デストラクタ
	virtual ~SceneBase(void) = 0;
	// 初期化処理
	virtual void Init(void) = 0;
	// 更新ステップ
	virtual void Update(void) = 0;
	// 描画処理
	virtual void Draw(void) = 0;
	// 解放処理
	virtual void Release(void) = 0;
private:

};
