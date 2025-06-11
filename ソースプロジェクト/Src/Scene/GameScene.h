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
	void Init(void);			// ����������
	void Update(void);			// �X�V����
	void Draw(void);			// �`�揈��
	void Release(void);			// �������
	void Reset(void);			// �Q�[�����̃��Z�b�g����
private:
	int backgroundImg_;			// �ނ������̔w�i
	int fishOnImg_;				// �ނꂽ���̉摜
	int rodSpawnImg_;			// �Ƃ��o��������Ƃ��̉摜
	int ukiSpawnImg_;			// �E�L�𓊂��鎞�̉摜
	int fishModel;				// �����f��
	int fishColor;				// �F
	const char* fishName_;		// ���O
	float fishScale_;			// �傫��
	bool isDraw_;				// �`�悷�邩
	bool isClear_;				// �N���A������

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
	void SetFish(int number);	// �ނꂽ���̃|�W�V������X�P�[����ύX���郁�\�b�h
	void Text(void);			// �e�L�X�g���
};
