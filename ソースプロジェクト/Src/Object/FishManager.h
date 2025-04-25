#pragma once
#include <DxLib.h>
#include <random>
#include "Fish.h"
#include "Player.h"

class FishManager
{
public:
	static constexpr int MAX_FISH = 25;

	// �C���X�^���X�̐���
	static void CreateInstance(void);
	// �C���X�^���X�̎擾
	static FishManager& GetInstance(void);

	FishManager();
	~FishManager();
	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);

	void Probability();		//�m��
	void FishMove(VECTOR pos);
	void Reset(void);
	void FishAllDelete(void);

	[[nodiscard]] int GetclosestFishIndex(void) const { return closestFishindex; }	//��ԋ߂����̔ԍ�(�z��)
	[[nodiscard]] int GetClosestFishModel(void)const;	//��ԋ߂����̃��f��
	[[nodiscard]] int GetClosestFishNumber(void);		//��ԋ߂����̔ԍ�(FishType�̔ԍ�)
	[[nodiscard]] bool GetClosestIntervalFlg(void)const;
	[[nodiscard]] bool GetFishHitFlg(void)const { return fishhitflg; }
	void SetClosestFishInterval(float interval);
	void SetCountFlg(bool flg);

	int GetAyuModel(void)const { return ayumodel; }
	int GetYamameModel(void)const { return yamamemodel; }
	int GetHayaModel(void)const { return hayamodel; }
	int GetHunaModel(void)const { return hunamodel; }
	int GetOikawaModel(void)const { return oikawamodel; }
	int GetKawamutuModel(void)const { return kawamutumodel; }
	int GetIwanaModel(void)const { return iwanamodel; }
	int GetPoriputeModel(void)const { return poriputemodel; }
	int GetNizimasuModel(void)const { return nizimasumodel; }
	int GetWakasagiModel(void)const { return wakasagimodel; }
	int GetUguiModel(void)const { return uguimodel; }
	int GetMedakaModel(void)const { return medakamodel; }
	int GetUthModel(void)const { return uthmodel; }

private:
	// �ÓI�C���X�^���X
	static FishManager* instance_;

	std::vector<Fish> fishlist;

	//���֘A�̏��
	int ayumodel;			//�A��
	int yamamemodel;		//���}��
	int hayamodel;			//�n��
	int hunamodel;			//�t�i
	int oikawamodel;		//�I�C�J��
	int kawamutumodel;		//�J�����c
	int iwanamodel;			//�C���i
	int poriputemodel;		//�|���v�e���X
	int nizimasumodel;		//�j�W�}�X
	int wakasagimodel;		//���J�T�M
	int uguimodel;			//�E�O�C
	int medakamodel;		//���_�J
	int uthmodel;			//���[�X�e�m�v�e����

	bool fishhitflg;		//�������Ă��邩

	int randValue;			//�����_���̒l(���̃��f���ɕK�v)
	int closestFishindex;	//�ł��߂���

	float Clamp(float value, float min, float max) {
		if (value < min) return min;
		if (value > max) return max;
		return value;
	}
};

