#pragma once
#include "SceneBase.h"
#include "../Application.h"

class TitleScene : public SceneBase
{
public:
	// �R���X�g���N�^
	TitleScene(void);
	// �f�X�g���N�^
	~TitleScene(void);
	void Init(void) override;		// ������
	void Update(void) override;		// �X�V
	void Draw(void) override;		// �`��
	void Release(void) override;	// ���
private:
	int titleimg;
	VECTOR titlePos_;
	int selection = 0;	//���݂̑I���ʒu
	bool selectflg = true;
	int manualimage[5];
	bool manualactive = false;
	int manualselect = 0;
	int mainSoundHundle_;
	int TitleHundle_;
	int fontHandle;
	int textboximg;
	int selecttextimg;

	void OutLine(void);				//�A�E�g���C��
	void UpdateSelect(void);		//�I��
	void DrawSelect(void);			//�I��`��
	void UpdateManual(void);		//������
	void DrawManual(void);			//�����ʕ`��
	void Drawtext(int x, int y, const char* text, int color);
};
