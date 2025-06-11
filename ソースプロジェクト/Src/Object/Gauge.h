#pragma once
#include <DxLib.h>

class Player;
class Dobber;
class FishManager;
class Rod;

class Gauge
{
public:
	Gauge(void);				// �R���X�g���N�^
	~Gauge(void);				// �f�X�g���N�^
	void Init(Player* player, Dobber* dobber, Rod* rod, FishManager* fishmng);			// ����������
	void Update(void);			// �X�V����
	void Draw(void);			// �`�揈��
	void Release(void);			// �������
	void UpdateGauge(void);     // �Q�[�W�̍X�V
	void SetGaugeFlg(const bool flg) { isDraw_ = flg; }
	void Reset(void);
private:
	// �Q�[�W�֘A
	int gaugeHandle_;			// �Q�[�W�̉摜
	float gaugeInterval_;		// �ő�Q�[�W�ɂ��鎞��
	bool isDraw_;				// �Q�[�W���o����
	float gaugeValue_;			// �Q�[�W�̏����l
	void DrawGauge(float x, float y, float width, float height, float value);     // �Q�[�W�̕\��

	Player* player_;
	Dobber* dobber_;
	FishManager* fishmng_;
	Rod* rod_;
};


