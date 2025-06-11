#pragma once
#include <DxLib.h>
#include <random>

class Player;
class Dobber;
class FishManager;

class Fish
{
public:
	enum FishType {
		None = 0,
		Ayu,
		Yamame,
		Haya,
		Huna,
		Oikawa,
		Medaka,
		Kawamutu,
		Iwana,
		Poriputerusu,
		Nizimasu,
		Ugui,
		Wakasagi,
		Uthtenoputeron,

		MAXKIND,
	};

	Fish(void);					// �R���X�g���N�^
	~Fish(void);				// �f�X�g���N�^
	void Init(Player* player, Dobber* dobber, FishManager* fishmng);		// ����������
	void Update(void);		// �X�V����
	void Draw(void);		// �`�揈��
	void Release(void);		// �������

	void FishAnimation(void);
	void FishFreedomMove(void);					// �����������Ă��Ȃ��Ƃ��Ɏ��R�ɓ�����
	void Move(VECTOR pos, const int fishidx);	// ���̑S�̂̓���
	void FishCome(int& fishidx);				// �E�L�������������ԋ߂���������Ă���
	void FishingInterval(void);					// �������Ԉȓ��ɍ��N���b�N����������ނ���J�n����
	void ChangeFish(FishType newtype);
	void Reset(void);							//���Z�b�g

	[[nodiscard]] int GetFishModel(void) const { return fishModel_; }
	[[nodiscard]] VECTOR GetFishPos(void) const { return fishPos_; }
	[[nodiscard]] bool GetLeftClickPressFlg(void) const { return isLeftClickPress; }
	[[nodiscard]] int GetFishNumber(void) const { return fishNumber_; }
	void SetAnimAttachNo(int model) { animAttachNo_ = MV1AttachAnim(model, 0, -1, -1); }
	void SetAnimTotalTime(int model) { animTotalTime_ = MV1GetAttachAnimTotalTime(model, animAttachNo_); }
	void SetCurrentAnimTime(float CurrentTime) { currentAnimTime_ = CurrentTime; }
	void SetFishModel(const int model) { fishModel_ = model; }
	void SetInterval(const float interval) { fishInterval_ = interval; }
	void SetCountFlg(const bool flg) { isIntervalCount = flg; }
	void SetType(const FishType fishtype) { type_ = fishtype; }
	void SetLeftClickPressFlg(const bool flg) { isLeftClickPress = flg; }
	void SetFishManager(FishManager* manager){fishmng_ = manager;}
private:
	// ���
	FishType type_;						// ���
	int fishModel_;						// ���f��
	VECTOR fishPos_;					// ���W
	VECTOR prevFishPos_;				// ��t���[���O�̍��W
	VECTOR fishScale_;					// �傫��

	float fishInterval_;				// �H�����Ă��鎞��
	bool isIntervalCount;				// �H�����Ă��邩
	float timeToMove;					// ���E�ɓ���������
	bool isLeftClickPress;				// ���N���b�N�����Ă��邩
	float angle_;						// ��]
	int direction_;						// �ǂ���ɓ�������
	int fishNumber_;					// ���̔ԍ�
	float currentAngleY_;				// ���݂̌����Ă���ꏊ
	float swimTimer_;					// �����j���ł��鎞�Ԃ��J�E���g����^�C�}�[
	VECTOR currentDirection_;			// �����Ƃ̕���
	float changeDirectionInterval_;		// �ʂ̊Ԋu

	// �A�j���[�V����
	int  animAttachNo_;					// �A�j���[�V�����i���o�[
	float animTotalTime_;				// �A�j���[�V�������v����
	float currentAnimTime_;				// ���݂̃A�j���[�V��������

	// ��ԑJ��
	void MoveLR(void);					// �������E�̂ǂ��炩�Ɉړ������邩�����߂�
	void FishIntervalCount(void);		// ����FishOn�����Ƃ��̑҂�����
	void FishFreedomRotate(void) const;	// �������R�ɓ�������
	void UpdateRotationToDirection(const VECTOR& direction);
	Player* player_;
	Dobber* dobber_;
	FishManager* fishmng_;
};

