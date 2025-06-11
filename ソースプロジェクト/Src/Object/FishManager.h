#pragma once
#include <DxLib.h>
#include <random>
#include <map>
#include <string>
#include "Fish.h"

class Player;
class Gauge;

class FishManager
{
public:
	static constexpr int MAX_FISH = 25;

	using FishType = Fish::FishType;

	std::map<Fish::FishType, std::string> fishModelPaths_;
	std::map<Fish::FishType, int> fishModels_;

	FishManager(void);			// �R���X�g���N�^
	~FishManager(void);			// �f�X�g���N�^
	void Init(Player* player, Dobber* dobber, Gauge* gauge);		// ����������
	void Update(void);			// �X�V����
	void Draw(void);			// �`�揈��
	void Release(void);			// �������

	void Probability(void);			// �m���ŋ��𐶐�
	void TouchDownMove(VECTOR pos);	// �E�L�������������̋��̏���
	void Reset(void);				// ���Z�b�g
	void FishAllDelete(void);		// ���S����

	[[nodiscard]] int GetFishModel(Fish::FishType type) const;
	[[nodiscard]] int GetClosestFishModel(void)const { return fishList[closestFishIndex_].GetFishModel(); }
	[[nodiscard]] int GetClosestFishNumber(void) { return fishList[closestFishIndex_].GetFishNumber(); }
	[[nodiscard]] bool GetFishHitFlg(void)const { return isFishHit_; }
	void SetClosestFishInterval(float interval) { for (Fish& fish : fishList)fish.SetInterval(interval); }
	void SetCountFlg(bool flg) { for (Fish& fish : fishList)fish.SetCountFlg(flg); }

	void ClosestFishAnimation(void);
private:

	std::vector<Fish> fishList;

	bool isFishHit_;		// �������Ă��邩
	int closestFishIndex_;	// �ł��߂���

	Player* player_;
	Dobber* dobber_;
	Gauge* gauge_;
};

