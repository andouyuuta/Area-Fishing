#pragma once
#include <DxLib.h>

class Player;

class Rod
{
public:
	enum class STATE
	{
		SPAWN,		//�o������
		KAMAE,		//�\��
		THROW,		//������
		AWASE		//���킹
	};
	// �C���X�^���X�̐���
	static void CreateInstance(void);
	// �C���X�^���X�̎擾
	static Rod& GetInstance(void);
	Rod(void);
	~Rod(void);
	void Init(void);		//����������
	void Update(void);		//�X�V����
	void SpawnUpdate(void);
	void KamaeUpdate(void);
	void ThrowUpdate(void);
	void AwaseUpdate(void);
	void Draw(void);		//�`�揈��
	void Release(void);		//�������
	[[nodiscard]] VECTOR GetAngle(void) const { return rot_; }	//�����̎擾
	[[nodiscard]] bool GetFlg() const { return rodFlg; }
	[[nodiscard]] float GetRotY() const { return rot_.y; }
	void Reset(void);
private:
	// �ÓI�C���X�^���X
	static Rod* instance_;

	STATE state_;
	int rodmodel_;		//�Ƃ̃n���h��ID
	bool rodFlg;		//�`��t���O
	//�A�j���[�V�����t���O
	bool rodanimFlg;	
	bool kamaeanimFlg;
	bool throwanimFlg;
	// �A�j���[�V�����̃A�^�b�`�ԍ�
	int animAttachNo_;
	int animrodAttachNo_;
	int kamaerodAttachNo_;
	int throwanimAttachNo_;
	// �A�j���[�V�����̑��Đ�����
	float animTotalTime_;
	float animrodTotalTime_;
	float kamaerodTotalTime_;
	float throwanimTotalTime_;
	// �Đ����̃A�j���[�V��������
	float currentAnimTime_;
	float currentrodAnimTime_;
	float currentkamaeAnimTime_;
	float currentthrowAnimTime_;
	VECTOR scale_ = { 0.25f,0.25f,-0.25f };	//�傫��
	VECTOR pos_;							//���W
	VECTOR rot_;							//��]
	void UpdateAnimation(void);				//�A�j���[�V�����X�V
	void RodProcess(void);					//�Ƃ̑S�̂̓���
	void RodMove(void);						//�ړ�����
	void ChangeState(const STATE state);	//�A�j���[�V�����؂�ւ�
	void HandleInput();						
	void UpdateRodmodel()const;
	void ToggleRod();
};

