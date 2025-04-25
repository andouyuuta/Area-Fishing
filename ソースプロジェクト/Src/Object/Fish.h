#pragma once
#include <DxLib.h>
#include <random>

class Player;
class Dobber;

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

	Fish();
	~Fish();
	void Init(void);
	void Update(void);
	void Draw(void) const;
	void Release(void);

	void FishSwimLogic(void);						//�����������Ă��Ȃ��Ƃ��Ɏ��R�ɓ�����
	void Move(VECTOR pos, const int fishidx);		//���̑S�̂̓���
	void FishCome(int &fishidx);				//�E�L�������������ԋ߂���������Ă���
	void FishingInterval(Dobber& dobber);		//�������Ԉȓ��ɍ��N���b�N����������ނ���J�n����
	void ChangeFish(FishType newtype);
	void Reset(void);		//���Z�b�g
	[[nodiscard]] int GetFishModel(void) const { return fishmodel; }
	[[nodiscard]] VECTOR GetFishPos(void) const { return FishPos; } 
	[[nodiscard]] bool GetIntervalFlg(void) const { return intervalcountflg; }
	[[nodiscard]] bool Getleftclickpressflg(void) const { return leftclickpressflg; }
	[[nodiscard]] FishType GetFishType(void)const { return type_; }
	[[nodiscard]] VECTOR GetFishScale(void) { return { fishscale,fishscale,fishscale }; }
	[[nodiscard]] int  GetAnimAttachNo(void) const { return animAttachNo_; }
	[[nodiscard]] float GetCurrentAnimTime(void) const { return currentAnimTime_; }
	[[nodiscard]] int GetFishNumber(void) const { return fishnumber; }
	void SetAnimAttachNo(int model) { animAttachNo_ = MV1AttachAnim(model, 0, -1, -1); }
	void SetAnimTotalTime(int model) { animTotalTime_ = MV1GetAttachAnimTotalTime(model, animAttachNo_); }
	void SetCurrentAnimTime(float CurrentTime) { currentAnimTime_ = CurrentTime; }
	void SetFishModel(const int model) { fishmodel = model; }
	void SetInterval(const float interval) { fishInterval = interval; }
	void SetCountFlg(const bool flg) { intervalcountflg = flg; }
	void SetType(const FishType fishtype) { type_ = fishtype; }
	void Setleftclickpressflg(const bool flg) { leftclickpressflg = flg; }
private:
	FishType type_;						//���
	VECTOR FishPos;						//���W
	VECTOR PrevFishPos;					//��t���[���O�̍��W
	int fishmodel;						//���f��
	float fishInterval;					//�H�����Ă��鎞��
	bool intervalcountflg;		//�H�����Ă��邩
	float interval;				//���E�ɓ���������
	bool leftclickpressflg;		//���N���b�N�����Ă��邩
	float fishscale;					//�傫��
	float angle;					//��]
	int direction;					//�ǂ���ɓ�������
	int fishnumber;						//���̔ԍ�
	float currentAngleY;				//���݂̌����Ă���ꏊ

	//�A�j���[�V����
	int  animAttachNo_;
	float animTotalTime_;
	float currentAnimTime_;

	float swimTimer;  // �����j���ł��鎞�Ԃ��J�E���g����^�C�}�[
	VECTOR currentDirection; //�����Ƃ̕���
	float changeDirectionInterval;   //�ʂ̊Ԋu

	//��ԑJ��
	void MoveLR();								//�������E�̂ǂ��炩�Ɉړ������邩�����߂�
	void FishIntervalCount(void);					//����FishOn�����Ƃ��̑҂�����
	void SwimRogicRotate(void) const;			//�������R�ɓ�������

	inline float Clamp(float value, float min, float max) {
		return (value < min) ? min : (value > max) ? max : value;
	}
};

