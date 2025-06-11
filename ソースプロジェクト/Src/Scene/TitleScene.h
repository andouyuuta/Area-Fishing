#pragma once
#include <DxLib.h>
#include "SceneBase.h"
#include "../Application.h"

class TitleScene : public SceneBase
{
public:
	TitleScene();
	~TitleScene();
	void Init() override;
	void Update() override;
	void Draw() override;
	void Release() override;

private:
	int titleImg_;
	VECTOR titlePos_;
	int selection_;
	bool isSelect_;
	int manualImage_[5];
	bool isManual_;
	int manualSelect_;
	int mainSoundHandle_;
	int fontHandle;
	int textBoxImg_;
	int selectTextImg_;

	void UpdateSelect();
	void DrawSelect();
	void UpdateManual();
	void DrawManual();
	void DrawText(int x, int y, const char* text, int color);
};
