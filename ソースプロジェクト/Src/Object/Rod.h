#pragma once
#include <DxLib.h>
#include <map>

class Player;
class Dobber;

class Rod
{
public:
	enum class STATE
	{
		SPAWN,		// �o������
		KAMAE,		// �\��
		THROW,		// ������
	};

	// �A�j���[�V�������
	enum class AnimationType
	{
		RodSpawn,
		RodKamae,
		RodThrow,
		Count
	};

	struct AnimationData
	{
		int attachIndex_ = -1;
		float totalTime_ = 0.0f;
		float currentTime_ = 0.0f;
		bool isPlaying_ = false;
	};

	Rod(void);				// �R���X�g���N�^
	~Rod(void);				// �f�X�g���N�^
	void Init(Player* player,Dobber* dobber);		// ����������
	void Update(void);		// �X�V����
	void Draw(void);		// �`�揈��
	void Release(void);		// �������

	[[nodiscard]] VECTOR GetAngle(void) const { return rot_; }	// �����̎擾
	[[nodiscard]] bool GetFlg() const { return isDraw_; }
	void Reset(void);
private:

	std::map<AnimationType, AnimationData>animations_;

	STATE state_;
	int rodModel_;		// �Ƃ̃n���h��ID
	bool isDraw_;		// �`��t���O
	VECTOR scale_;		// �傫��
	VECTOR pos_;		// ���W
	VECTOR rot_;		// ��]

	// �A�j���[�V�����f�[�^

	void UpdateAnimation(void);				// �A�j���[�V�����X�V
	void RodProcess(void);					// �Ƃ̑S�̂̓���
	void RodMove(void);						// �ړ�����
	void ChangeState(STATE state);			// �A�j���[�V�����؂�ւ�
	void HandleInput(void);						
	void ToggleRod(void);					// �Ƃ̏�ԑJ��
	bool IsCurrentAnimationPlaying() const;	// �A�j���[�V���������邩

	Player* player_;
	Dobber* dobber_;
};

