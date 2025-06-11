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

	Player(void);						// �R���X�g���N�^
	~Player(void);						// �f�X�g���N�^
	void Init(Camera* camera);			// ����������
	void Update(void);					// �X�V����
	void Draw(void);					// �`�揈��
	void Release(void);					// �������
	[[nodiscard]] VECTOR GetPos(void) const { return pos_; }
	[[nodiscard]] VECTOR GetAngle(void) const { return rot_; }
	[[nodiscard]] int GetModel(void) const { return playerModel_; }
	[[nodiscard]] PlayerMode GetCurrentMode()const { return currentMode_; }
	void SetCurrentMode(PlayerMode mode) { currentMode_ = mode; }
private:
	PlayerMode currentMode_;			// ���݂̃��[�h
	int effectResourceHandle_;			// ��̃G�t�F�N�g
	int playingEffectHandle_;			// �v���C���[�̃G�t�F�N�g
	int playerModel_;					// ���f��ID

	int animAttachNo_;					// �A�j���[�V�����̃A�^�b�`�ԍ�
	float animTotalTime_;				// �A�j���[�V�����̑��Đ�����
	float currentAnimTime_;				// �Đ����̃A�j���[�V��������
	float moveSpeed_;					// �����X�s�[�h
	// ���֘A
	int boatSoundHandle_;				// �{�[�g�̉�
	bool isSoundPlaying_;				// �����o�Ă��邩

	VECTOR pos_;						// �\�����W
	VECTOR moveVec_;					// �ړ��x�N�g��
	VECTOR rot_;						// �p�x
	VECTOR targetRot_ ;					// �ڕW�̉�]�p�x
	VECTOR localRot_;					// �����p�x
	VECTOR scale_;						// �傫��

	void UpdateAnimation(void);			// �A�j���[�V�����X�V
	void UpdateMove(void);				// �ړ��X�V
	bool IsMove(const VECTOR _moveVec);	// �x�N�g���̈ړ�����
	void SetRotation(void) const;		// ��]�ݒ�
	void UpdateRotation(void);			// ��]����
	bool IsAnyKeyPressed();				// �L�[��������Ă��邩

	Camera* camera_;
};