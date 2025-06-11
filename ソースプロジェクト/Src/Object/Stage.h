#pragma once
#include <DxLib.h>

class Stage
{
public:
	static constexpr float RADIUS = 2000.0f;

	Stage(void);			// �R���X�g���N�^
	~Stage(void);			// �f�X�g���N�^
	void Init(void);		// ����������
	void Update(void);		// �X�V����
	void Draw(void);		// �`�揈��
	void Release(void);		// �������
	[[nodiscard]]int GetModelId(void)const { return stageModel_; }
	[[nodiscard]]VECTOR GetPos()const { return stagePos_; }
private:
	int stageModel_;		// �X�e�[�W���f��
	int backGroundModel_;	// �w�i���f��
	int windmillModel_;		// ���ԃ��f��
	int fallSoundHandle_;	// ��̉�
	int envSoundHandle_;	// ���̉�

	VECTOR backGroundPos_;	// �w�i���W
	VECTOR backGroundScale_;// �w�i�傫��
	VECTOR soundPos_;		// ���̍��W
	VECTOR stageScale_ ;	// �X�e�[�W�̑傫��
	VECTOR stagePos_;		// �X�e�[�W�̍��W
	VECTOR windmillPos_;	// ���ԍ��W
	VECTOR windmillRot_;	// ���ԉ�]
	void WindmillMove(void);// ���Ԃ̓���
};