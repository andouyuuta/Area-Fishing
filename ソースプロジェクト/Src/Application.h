#pragma once
#include <string>

class Application
{

public:

	// �X�N���[���T�C�Y
	static constexpr int SCREEN_SIZE_X = 1024;
	static constexpr int SCREEN_SIZE_Y = 640;

	const float FRAME_RATE = (1000.0f / 60.0f);

	int currentTime = 0;
	int lastFrameTime = 0;

	int frameCnt = 0;
	int updateFrameRateTime = 0;

	float frameRate = 0.f;

	// �f�[�^�p�X�֘A
	//-------------------------------------------
	static const std::string PATH_MODEL;
	//-------------------------------------------

	// �����I�ɃC���X�e���X�𐶐�����
	static void CreateInstance(void);

	// �ÓI�C���X�^���X�̎擾
	static Application& GetInstance(void);

	// ������
	void Init(void);

	// �Q�[�����[�v�̊J�n
	void Run(void);

	// ���\�[�X�̉��
	void Release(void);

	// �����������^���s�̔���
	bool IsInitFail(void) const;

	// ��������^���s�̔���
	bool IsReleaseFail(void) const;

	// �f���^�^�C���̎擾
	float GetDeltaTime();	
private:

	// �ÓI�C���X�^���X
	static Application* instance_;

	// ���������s
	bool isInitFail_;

	// ������s
	bool isReleaseFail_;

	// �f�t�H���g�R���X�g���N�^��private�ɂ��āA
	// �O�����琶���ł��Ȃ��l�ɂ���
	Application(void);

	// �f�X�g���N�^�����l
	~Application(void);

	void CalcFrameRate();

	void DrawFrameRate();
};