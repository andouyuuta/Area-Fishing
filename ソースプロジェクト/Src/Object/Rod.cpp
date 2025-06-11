#include "../Manager/InputManager.h"
#include "Player.h"
#include "Rod.h"
#include "Dobber.h"

namespace
{
	// �A�j���[�V�������x
	constexpr float SPAWN_ANIM_SPEED = 6.0f;
	constexpr float KAMAE_ANIM_SPEED = 5.0f;
	constexpr float THROW_ANIM_SPEED = 4.0f;
	constexpr float ANIM_ROD_SPEED = 10.0f;

	// ��]�E���x
	constexpr float HALF_ROTATION_DEG = 180.0f;				// ����]�i�x���j
	constexpr float FULL_ROTATION_RAD = 2.0f * DX_PI_F;		// ���]�i���W�A���j
	constexpr float ROT_SPEED_DEG = 1.0f;					// ��]���x

	// �Ƃ̑��΍��W
	constexpr VECTOR ROD_RELATIVE_POS = { 30.0f, 0.0f, 100.0f };

	// ���f���֘A
	constexpr VECTOR DEFAULT_SCALE = { 0.25f, 0.25f, -0.25f };
	constexpr const char* ROD_MODEL_PATH = "Data/Model/Stage/rod3.mv1";

	// �A�j���[�V����ID�Ƒ��x�̑Ή��e�[�u��
	const std::map<Rod::AnimationType, int> ANIM_INDEX_MAP = 
	{
		{ Rod::AnimationType::RodSpawn, 3 },
		{ Rod::AnimationType::RodKamae, 2 },
		{ Rod::AnimationType::RodThrow, 4 },
	};

	const std::map<Rod::AnimationType, float> ANIM_SPEED_MAP = 
	{
		{ Rod::AnimationType::RodSpawn, SPAWN_ANIM_SPEED },
		{ Rod::AnimationType::RodKamae, KAMAE_ANIM_SPEED },
		{ Rod::AnimationType::RodThrow, THROW_ANIM_SPEED },
	};
}

Rod::Rod(void)
	:state_(STATE::SPAWN),
	rodModel_(-1),
	isDraw_(false),
	scale_{ DEFAULT_SCALE },
	pos_{ 0.0f, 0.0f, 0.0f },
	rot_{ 0.0f, 0.0f, 0.0f }
{
}

Rod::~Rod(void)
{
}

void Rod::Init(Player* player,Dobber* dobber)
{
	player_ = player;
	dobber_ = dobber;

	rodModel_ = MV1LoadModel(ROD_MODEL_PATH);
	MV1SetScale(rodModel_, scale_);
	MV1SetPosition(rodModel_, pos_);
	MV1SetRotationXYZ(rodModel_, rot_);

	// �A�j���[�V�����̏�����
	for (auto& [type, animIndex] : ANIM_INDEX_MAP)
	{
		AnimationData data{};
		data.attachIndex_ = MV1AttachAnim(rodModel_, animIndex, -1, -1);
		data.totalTime_ = MV1GetAttachAnimTotalTime(rodModel_, data.attachIndex_);
		data.currentTime_ = 0.0f;
		data.isPlaying_ = false;
		animations_[type] = data;
	}

	ChangeState(STATE::SPAWN);
}

void Rod::Update(void)
{
	static Player::PlayerMode lastMode = player_->GetCurrentMode();

	// �v���C���[���[�h���ύX���ꂽ�Ƃ��ɏ������s��
	if (lastMode != player_->GetCurrentMode()) 
	{
		lastMode = player_->GetCurrentMode();

		if (lastMode == Player::PlayerMode::FIRST_PERSON) 
		{
			// ��l�̂ɂȂ������ɊƂ�������
			isDraw_ = true;

			// �v���C���[�̌������擾���ĊƂ̊p�x��ݒ�
			rot_ = player_->GetAngle();
			MV1SetRotationXYZ(rodModel_, rot_);

			// �Ƃ̏����ʒu���Z�b�g
			for (auto& [animType, animData] : animations_)
			{
				if (animData.attachIndex_ >= 0)
					MV1DetachAnim(rodModel_, animData.attachIndex_);
				animData.attachIndex_ = MV1AttachAnim(rodModel_, static_cast<int>(animType), -1, -1);
				animData.currentTime_ = 0.0f;
				animData.isPlaying_ = false;
			}

			ChangeState(STATE::SPAWN);
		}
		else if(lastMode==Player::PlayerMode::THIRD_PERSON) 
		{
			// �O�l�̂̎��ɊƂ��\��
			isDraw_ = false;

			// ���ׂẴA�j���[�V�������f�^�b�`���Ē�~
			for (auto& [_, animData] : animations_)
			{
				if (animData.attachIndex_ >= 0)
					MV1DetachAnim(rodModel_, animData.attachIndex_);
				animData.isPlaying_ = false;
				animData.currentTime_ = 0.0f;
			}

			ChangeState(STATE::SPAWN);

			//  �E�L�̏�Ԃ����Z�b�g
			dobber_->Reset();
		}
	}

	// �Ƃ��\������Ă�����X�V�������s��
	if (isDraw_) 
	{
		HandleInput();
		UpdateAnimation();
		RodProcess();
	}
}

void Rod::Draw(void)
{
	if (isDraw_) 
	{
		MV1DrawModel(rodModel_);
	}
}

void Rod::Release(void)
{
	for (auto& [type, animData] : animations_)
	{
		MV1DetachAnim(rodModel_, animData.attachIndex_);
	}
	MV1DeleteModel(rodModel_);
}

// �A�j���[�V�����X�V
void Rod::UpdateAnimation(void)
{
	for (auto& [animType, animData] : animations_)
	{
		if (!animData.isPlaying_)
			continue;

		float speed = ANIM_SPEED_MAP.at(animType);  // ���x�}�b�v����Đ����x�擾
		animData.currentTime_ += speed;

		if (animData.currentTime_ >= animData.totalTime_)
		{
			animData.currentTime_ = animData.totalTime_;
			animData.isPlaying_ = false;
		}
		MV1SetAttachAnimTime(rodModel_, animData.attachIndex_, animData.currentTime_);
	}
}

// �ƑS�̂̓���
void Rod::RodProcess(void)
{
	auto& input = InputManager::GetInstance();

	// �O�l�̂�������Ƃ��o�������Ȃ�
	if (player_->GetCurrentMode() == Player::PlayerMode::THIRD_PERSON) 
	{
		return;
	}

	if (!IsCurrentAnimationPlaying() && input.IsTrgMouseRight()) 
	{
		ToggleRod();
	}

	// �Ƃ��o�Ă���ꍇ�̓��f�����X�V
	if (isDraw_) 
	{
		UpdateAnimation();
		RodMove();
		MV1SetPosition(rodModel_, pos_);
		MV1SetRotationXYZ(rodModel_, rot_);
	}
}

void Rod::RodMove(void)
{
	// ���͐���擾
	InputManager& ins = InputManager::GetInstance();

	// ��]�X�s�[�h���f�O���[�����W�A���ɕύX
	float rotPow = ROT_SPEED_DEG * DX_PI_F / HALF_ROTATION_DEG;

	// �㉺���E�L�[�ŃJ�����̍��W��ύX����
	if (ins.IsNew(KEY_INPUT_RIGHT))  rot_.y += rotPow;
	if (ins.IsNew(KEY_INPUT_LEFT)) rot_.y -= rotPow;

	// 360�x�ȏ�ɂȂ�����߂�
	if (rot_.y >= FULL_ROTATION_RAD)
	{
		rot_.y -= FULL_ROTATION_RAD;
	}
	else if (rot_.y < 0.0f)
	{
		rot_.y += FULL_ROTATION_RAD;
	}

	// �v���C���[�̈ʒu���擾
	const VECTOR playerPos = player_->GetPos();

	// Y����]�s����쐬
	const MATRIX rotMatrix = MGetRotY(rot_.y);

	// ���b�h�̐V�����ʒu = �v���C���[�ʒu + ��]�s��K�p�������Έʒu
	const VECTOR newRodPos = VTransform(ROD_RELATIVE_POS, rotMatrix);

	pos_ = VAdd(playerPos, newRodPos);

	// ���f���̍��W�Ɖ�]��ݒ�
	MV1SetPosition(rodModel_, pos_);
	MV1SetRotationXYZ(rodModel_, rot_);
}

void Rod::ChangeState(STATE state)
{
	if (state_ == state)
		return;

	// �܂��S�A�j���[�V������~
	for (auto& [_, animData] : animations_)
	{
		animData.isPlaying_ = false;
		animData.currentTime_ = 0.0f;
		MV1DetachAnim(rodModel_, animData.attachIndex_);
		animData.attachIndex_ = -1; // �Y�ꂸ�Ƀ��Z�b�g
	}

	state_ = state;

	// ��Ԃɉ����ăA�j���[�V�����Đ��J�n
	AnimationType playAnim;
	switch (state_)
	{
	case STATE::SPAWN: playAnim = AnimationType::RodSpawn; break;
	case STATE::KAMAE: playAnim = AnimationType::RodKamae; break;
	case STATE::THROW: playAnim = AnimationType::RodThrow; break;
	default: return;
	}

	animations_[playAnim].attachIndex_ = MV1AttachAnim(rodModel_, ANIM_INDEX_MAP.at(playAnim), -1, -1);
	animations_[playAnim].isPlaying_ = true;
	animations_[playAnim].currentTime_ = 0.0f;
}

void Rod::HandleInput()
{
	auto& input = InputManager::GetInstance();

	if (input.IsTrgMouseRight()) 
	{
		ToggleRod();
	}

	if (isDraw_ && !IsCurrentAnimationPlaying())
	{
		if (input.IsTrgDown(KEY_INPUT_SPACE)) 
		{
			ChangeState(STATE::KAMAE);
		}
		else if (input.IsTrgMouseLeft()) 
		{
			ChangeState(STATE::THROW);
		}
	}
}

void Rod::ToggleRod()
{
	Player::PlayerMode currentMode = player_->GetCurrentMode();

	// �O�l�̂�������Ƃ�\�������Ȃ�
	if (currentMode == Player::PlayerMode::THIRD_PERSON) 
	{
		return;
	}

	if (isDraw_) 
	{
		for (auto& [_, animData] : animations_)
		{
			if (animData.attachIndex_ >= 0)
				MV1DetachAnim(rodModel_, animData.attachIndex_);
			animData.isPlaying_ = false;
			animData.currentTime_ = 0.0f;
		}

		ChangeState(STATE::SPAWN);

		// �E�L�̏�Ԃ����Z�b�g
		dobber_->Reset();
	}
	else 
	{
		// �Ƃ��o��
		isDraw_ = true;

		rot_ = player_->GetAngle();
		MV1SetRotationXYZ(rodModel_, rot_);

		for (auto& [animType, animData] : animations_)
		{
			animData.attachIndex_ = MV1AttachAnim(rodModel_, static_cast<int>(animType), -1, -1);
			animData.currentTime_ = 0.0f;
			animData.isPlaying_ = false;
		}

		// ��Ԃ�Spawn�ɕύX
		ChangeState(STATE::SPAWN);
	}
}

bool Rod::IsCurrentAnimationPlaying() const
{
	switch (state_)
	{
	case STATE::SPAWN:
		return animations_.at(AnimationType::RodSpawn).isPlaying_;
	case STATE::KAMAE:
		return animations_.at(AnimationType::RodKamae).isPlaying_;
	case STATE::THROW:
		return animations_.at(AnimationType::RodThrow).isPlaying_;
	default:
		return false;
	}
}

void Rod::Reset(void)
{
	isDraw_ = false;
}
