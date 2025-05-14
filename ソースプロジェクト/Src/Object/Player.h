#pragma once
#include <DxLib.h>

class Camera;

class Player
{

public:
	enum PlayerMode {
		THIRD_PERSON,
		FIRST_PERSON,
	};
	// �C���X�^���X�̐���
	static void CreateInstance(void);
	// �C���X�^���X�̎擾
	static Player& GetInstance(void);

	Player(void);
	~Player(void);
	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);
	[[nodiscard]] VECTOR GetPos(void) const { return playerpos_; }
	[[nodiscard]] VECTOR GetAngle(void) const { return rot_; }
	[[nodiscard]] int GetModel(void) const { return playermodel_; }
	[[nodiscard]] PlayerMode GetCurrentMode()const { return currentMode_; }
	void SetCurrentMode(PlayerMode mode) { currentMode_ = mode; }
	void SetCamera(Camera* camera) { camera_ = camera; }
private:
	// �ÓI�C���X�^���X
	static Player* instance_;

	PlayerMode currentMode_;
	int effectResourceHandle;
	int playingEffectHandle;
	// ���f��ID
	int playermodel_;
	// �A�j���[�V�����̃A�^�b�`�ԍ�
	int animAttachNo_;
	int kamaerodAttachNo_;
	int throwanimAttachNo_;
	// �A�j���[�V�����̑��Đ�����
	float animTotalTime_;
	float kamaerodTotalTime_;
	float throwanimTotalTime_;
	// �Đ����̃A�j���[�V��������
	float currentAnimTime_;
	float currentkamaeAnimTime_;
	float currentthrowAnimTime_;
	float movespeed = 0.0f;
	//���֘A
	int boatSoundHundle;
	int boatstopHundle;
	bool isSoundPlaying;
	int FallSoundHundle_;
	int RiverSoundHundle_;
	float previousTime = 0.0f;
	bool keyFlg = false;
	float RADIUS = 20000.0f;
	// �\�����W
	VECTOR playerpos_;	
	VECTOR camerarot_;
	VECTOR playerrot_;
	//�ړ��x�N�g��
	VECTOR moveVec_;
	VECTOR moveRot_;
	//�ړ��x�N�g������������p�x
	VECTOR moveVecRad_;
	//�p�x
	VECTOR rot_;
	//�ڕW�̉�]�p�x
	VECTOR targetRot_ = { 0.0f,0.0f,0.0f };
	//�����p�����p�x
	VECTOR localRot_;
	VECTOR scale_ = { 0.35f,0.35f,0.35f };
	VECTOR SoundPos;
	// �A�j���[�V�����X�V
	void UpdateAnimation(void);
	//�ړ��X�V
	void UpdateMove(void);
	//�x�N�g���̈ړ�����
	bool IsMove(const VECTOR _moveVec);
	//��]�ݒ�
	void SetRotation(void);
	//��]����
	void UpdateRotation(void);
	float GetDeltaTime();
	//�L�[��������Ă��邩
	bool IsAnyKeyPressed();

	Camera* camera_ = nullptr;
};