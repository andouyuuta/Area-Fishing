#pragma once
#include <DxLib.h>

class Gauge
{
public:
	// �C���X�^���X�̐���
	static void CreateInstance(void);
	// �C���X�^���X�̎擾
	static Gauge& GetInstance(void);
	Gauge();
	~Gauge();
	// ������
	void Init(void);
	// �X�V
	void Update(void);
	// �`��
	void Draw(void);
	// �������
	void Release(void);
	void UpdateGauge(void);     //�Q�[�W�̍X�V
	void SetGaugeFlg(const bool flg) { gaugeflg = flg; }
	void Reset(void);
private:
	static Gauge* instance_;
	//�Q�[�W�֘A
	int gagugebackhandle;		//�Q�[�W�̉摜
	float maxValue = 100.0f;	//�ő�Q�[�W
	float minValue = 0.0f;		//�ŏ��Q�[�W
	float gaugeInterval = 0;	//�ő�Q�[�W�ɂ��鎞��
	bool gaugeflg = false;
	float gaugeValue = 50;		//�Q�[�W�̏����l
	VECTOR GaugePos;			//�Q�[�W�̍��W
	void DrawGauge(const int x, const int y, const int width, const int height, const float value);     //�Q�[�W�̕\��
};


