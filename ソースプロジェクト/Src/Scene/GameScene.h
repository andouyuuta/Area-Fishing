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
	void Initialize(void);		//�Q�[�����̃��Z�b�g����
private:
	int backgroundimg;			//�ނ������̔w�i
	int fishonimg;				//�ނꂽ���̉摜
	int rodspawnimg;			//�Ƃ��o��������Ƃ��̉摜
	int ukispawnimg;			//�E�L�𓊂��鎞�̉摜
	int fishmodel;				//�����f��
	int fishcolor;				//�F
	const char* fishname;		//���O
	float fishscale;			//�傫��
	bool drawflg;				//�`�悷�邩
	bool clearflg;				//�N���A������
	EffekseerEffect* effect_;	
	Water* water_;
	Camera* camera_;
	int SunHundle;				
	void SetFish(int number);	//�ނꂽ���̃|�W�V������X�P�[����ύX���郁�\�b�h
	void Text(void);			//�e�L�X�g���
};
