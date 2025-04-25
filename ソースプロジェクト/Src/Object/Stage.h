#pragma once
#include <DxLib.h>
class Stage
{
public:
	// �C���X�^���X�̐���
	static void CreateInstance(void);
	// �C���X�^���X�̎擾
	static Stage& GetInstance(void);
	Stage(void);
	~Stage(void);
	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);
	[[nodiscard]]int GetModelId(void)const { return stagemodel_; }
	[[nodiscard]]VECTOR GetPos()const { return stagepos_; }
private:
	// �ÓI�C���X�^���X
	static Stage* instance_;
	// �X�e�[�W���f���̃n���h��ID
	int stagemodel_;
	int background_;
	int windmillmodel_;
	int watermodel_;
	VECTOR SoundPos;
	int FallSoundHundle_;
	int RiverSoundHundle_;
	int envSoundHundle_;
	float RADIUS = 2000.0f;
	VECTOR scale = { 0.75f,0.75f,0.75f };
	VECTOR stagepos_;
	VECTOR backgroundpos_;
	VECTOR windmillpos_;
	VECTOR waterpos_;
	VECTOR windmillrot_;
	void WindmillMove(void);
};