#include "../Manager/InputManager.h"
#include "Player.h"
#include "Rod.h"
#include "Dobber.h"

namespace
{
	// �A�j���[�V�����̍Đ����x
	constexpr float ANIM_SPEED = 6.0f;
	constexpr float KAMAEANIM_SPEED = 5.0f;
	constexpr float THROWANIM_SPEED = 4.0f;
	constexpr float ANIM_ROD_SPEED = 10.0f;
	//��]�X�s�[�h
	constexpr float ROT_SPEED_DEG = 1.0f;
}

Rod* Rod::instance_ = nullptr;

void Rod::CreateInstance()
{
	if (instance_ == nullptr)
	{
		instance_ = new Rod();
	}
	instance_->Init();
}

Rod& Rod::GetInstance(void)
{
	return *instance_;
}

Rod::Rod(void)
	:state_(STATE::SPAWN)
{
	animAttachNo_ = 0, animTotalTime_ = 0.0f, animrodAttachNo_ = 0, animrodTotalTime_ = 0.0f, currentAnimTime_ = 0.0f, currentkamaeAnimTime_ = 0.0f,
		currentrodAnimTime_ = 0.0f, currentthrowAnimTime_ = 0.0f, kamaeanimFlg = false, kamaerodAttachNo_ = 0, kamaerodTotalTime_ = 0.0f,
		pos_ = { 0.0f,0.0f,0.0f }, rodFlg = false, rodanimFlg = false, rodmodel_ = -1, rot_ = { 0.0f,0.0f,0.0f }, throwanimAttachNo_ = 0,
		throwanimFlg = false, throwanimTotalTime_ = 0.0f;
}

Rod::~Rod(void)
{
}

void Rod::Init(void)
{
	rodmodel_ = MV1LoadModel("Data/Model/Stage/rod3.mv1");

	pos_ = { 0.0f,50.0f,0.0f };
	rot_ = { 0.0f,0.0f,0.0f };

	MV1SetPosition(rodmodel_, pos_);
	MV1SetRotationXYZ(rodmodel_, rot_);
	MV1SetScale(rodmodel_, scale_);

	rodanimFlg = false;
	rodFlg = false;

	// �A�j���[�V�����̑����Ԃ��擾���邽�߂�Attach
	kamaerodAttachNo_ = MV1AttachAnim(rodmodel_, 2, -1, -1);
	animrodAttachNo_ = MV1AttachAnim(rodmodel_, 3, -1, -1);
	throwanimAttachNo_ = MV1AttachAnim(rodmodel_, 4, -1, -1);

	// �A�j���[�V���������Ԃ̎擾
	animrodTotalTime_ = MV1GetAttachAnimTotalTime(rodmodel_, animrodAttachNo_);
	kamaerodTotalTime_ = MV1GetAttachAnimTotalTime(rodmodel_, kamaerodAttachNo_);
	throwanimTotalTime_ = MV1GetAttachAnimTotalTime(rodmodel_, throwanimAttachNo_);

	// �A�j���[�V�����̑����Ԃ��擾�����̂�Detach
	MV1DetachAnim(rodmodel_, kamaerodAttachNo_);
	MV1DetachAnim(rodmodel_, animrodAttachNo_);
	MV1DetachAnim(rodmodel_, throwanimAttachNo_);

	currentrodAnimTime_ = 0.0f;
	currentkamaeAnimTime_ = 0.0f;
	currentthrowAnimTime_ = 0.0f;

	// ���f���Ɏw�莞�Ԃ̃A�j���[�V������ݒ肷��
	MV1SetAttachAnimTime(rodmodel_, animrodAttachNo_, currentAnimTime_);

	ChangeState(STATE::SPAWN);
}

void Rod::Update(void)
{
	Player& player = Player::GetInstance();
	static Player::PlayerMode lastMode = player.GetCurrentMode();

	//�v���C���[���[�h���ύX���ꂽ�Ƃ��ɏ������s��
	if (lastMode != player.GetCurrentMode()) {
		lastMode = player.GetCurrentMode();

		if (lastMode == Player::PlayerMode::FIRST_PERSON) {
			//��l�̂ɂȂ������ɊƂ�������
			rodFlg = true;
			rodanimFlg = false;

			//�v���C���[�̌������擾���ĊƂ̊p�x��ݒ�
			rot_ = player.GetAngle();
			MV1SetRotationXYZ(rodmodel_, rot_);

			//�Ƃ̏����ʒu���Z�b�g
			currentAnimTime_ = 0.0f;
			animrodAttachNo_ = MV1AttachAnim(rodmodel_, 3, -1, -1);
			UpdateRodmodel();
		}
		else if(lastMode==Player::PlayerMode::THIRD_PERSON) {
			//�O�l�̂̎��ɊƂ��\��
			rodFlg = false;
			rodanimFlg = false;

			//�A�j���[�V���������Z�b�g
			MV1DetachAnim(rodmodel_, kamaerodAttachNo_);
			MV1DetachAnim(rodmodel_, animrodAttachNo_);
			MV1DetachAnim(rodmodel_, throwanimAttachNo_);

			currentAnimTime_ = 0.0f;
			currentkamaeAnimTime_ = 0.0f;
			currentthrowAnimTime_ = 0.0f;

			ChangeState(STATE::SPAWN);

			// �E�L�̏�Ԃ����Z�b�g
			Dobber::GetInstance().Reset();
		}
	}

	//�Ƃ��\������Ă�����X�V�������s��
	if (rodFlg) {
		HandleInput();
		UpdateAnimation();
		RodProcess();
	}
}

void Rod::SpawnUpdate(void)
{
	if (rodanimFlg == true)
	{
		currentAnimTime_ += ANIM_SPEED;
		currentrodAnimTime_ += ANIM_SPEED;
		MV1SetAttachAnimTime(rodmodel_, animrodAttachNo_, currentAnimTime_);
	}

	if (currentAnimTime_ > animrodTotalTime_)
	{
		currentAnimTime_ = 0.0f;
		rodanimFlg = false;
	}
}

void Rod::KamaeUpdate(void)
{
	if (kamaeanimFlg)
	{
		currentkamaeAnimTime_ += KAMAEANIM_SPEED;
		MV1SetAttachAnimTime(rodmodel_, kamaerodAttachNo_, currentkamaeAnimTime_);

		if (currentkamaeAnimTime_ > kamaerodTotalTime_)
		{
			currentkamaeAnimTime_ = 0.0f;
			kamaeanimFlg = false;

			// �A�j���[�V�����I�����̃��f����]���X�V
			MV1SetRotationXYZ(rodmodel_, rot_);
		}
	}
}

void Rod::ThrowUpdate(void)
{
	if (throwanimFlg)
	{
		currentthrowAnimTime_ += THROWANIM_SPEED;
		MV1SetAttachAnimTime(rodmodel_, throwanimAttachNo_, currentthrowAnimTime_);

		if (currentthrowAnimTime_ > throwanimTotalTime_)
		{
			currentthrowAnimTime_ = 0.0f;
			throwanimFlg = false;

			// �A�j���[�V�����I�����̃��f����]���X�V
			MV1SetRotationXYZ(rodmodel_, rot_);
		}
	}
}

void Rod::AwaseUpdate(void)
{
}

void Rod::Draw(void)
{
	if (rodFlg) {
		MV1DrawModel(rodmodel_);
	}
}

void Rod::Release(void)
{
	MV1DeleteModel(rodmodel_);
}
void Rod::UpdateAnimation(void)
{
	switch (state_)
	{
	case Rod::STATE::SPAWN:
		SpawnUpdate();
		break;
	case Rod::STATE::KAMAE:
		KamaeUpdate();
		break;
	case Rod::STATE::THROW:
		ThrowUpdate();
		break;
	case Rod::STATE::AWASE:
		AwaseUpdate();
		break;
	default:
		break;
	}
}

void Rod::RodProcess(void)
{
	auto& input = InputManager::GetInstance();
	Player& player = Player::GetInstance();

	//�O�l�̂�������Ƃ��o�������Ȃ�
	if (player.GetCurrentMode() == Player::PlayerMode::THIRD_PERSON) {
		return;
	}

	if ((!rodanimFlg) && input.IsTrgMouseRight()) {
		ToggleRod();
	}

	//�Ƃ��o�Ă���ꍇ�̓��f�����X�V
	if (rodFlg == true) {
		UpdateAnimation();
		RodMove();
		UpdateRodmodel();
	}
}

void Rod::RodMove(void)
{
	//���͐���擾
	InputManager& ins = InputManager::GetInstance();

	//��]�X�s�[�h���f�O���[�����W�A���ɕύX
	float rotPow = ROT_SPEED_DEG * DX_PI_F / 180.0f;

	//�㉺���E�L�[�ŃJ�����̍��W��ύX����
	if (ins.IsNew(KEY_INPUT_RIGHT)) { rot_.y += rotPow; }
	if (ins.IsNew(KEY_INPUT_LEFT)) { rot_.y -= rotPow; }

	if (rot_.y >= 360.0f)
	{
		rot_.y -= 360.0f;
	}

	VECTOR playerpos_ = Player::GetInstance().GetPos();

	MATRIX rotMatrix = MGetRotY(rot_.y);

	VECTOR relativePos = { 30.0f,0.0f,100.0f };

	VECTOR newRodPos = VTransform(relativePos, rotMatrix);

	pos_ = VAdd(playerpos_, newRodPos);

	MV1SetPosition(rodmodel_, pos_);
	MV1SetRotationXYZ(rodmodel_, rot_);
}

void Rod::ChangeState(const STATE state)
{
	if (state_ == state)return;

	state_ = state;

	//���݂̃A�j���[�V���������ׂăf�^�b�`������
	MV1DetachAnim(rodmodel_, kamaerodAttachNo_);
	MV1DetachAnim(rodmodel_, animrodAttachNo_);
	MV1DetachAnim(rodmodel_, throwanimAttachNo_);

	//�A�j���[�V�������ԃ��Z�b�g
	currentAnimTime_ = 0.0f;
	currentkamaeAnimTime_ = 0.0f;
	currentthrowAnimTime_ = 0.0f;

	switch (state_)
	{
	case Rod::STATE::SPAWN:
		rodanimFlg = true;
		animrodAttachNo_ = MV1AttachAnim(rodmodel_, 3, -1, -1);
		break;
	case Rod::STATE::KAMAE:
		kamaeanimFlg = true;
		kamaerodAttachNo_ = MV1AttachAnim(rodmodel_, 2, -1, -1);
		break;
	case Rod::STATE::THROW:
		throwanimFlg = true;
		throwanimAttachNo_ = MV1AttachAnim(rodmodel_, 4, -1, -1);
		break;
	case Rod::STATE::AWASE:
		break;
	default:
		break;
	}
}

void Rod::HandleInput()
{
	auto& input = InputManager::GetInstance();

	if (input.IsTrgMouseRight()) {
		ToggleRod();
	}

	if (rodFlg && !rodanimFlg) {
		if (input.IsTrgDown(KEY_INPUT_SPACE)) {
			ChangeState(STATE::KAMAE);
		}
		else if (input.IsTrgMouseLeft()) {
			ChangeState(STATE::THROW);
		}
	}
}

void Rod::UpdateRodmodel()const
{
	MV1SetPosition(rodmodel_, pos_);
	MV1SetRotationXYZ(rodmodel_, rot_);
}

void Rod::ToggleRod()
{
	Player& player = Player::GetInstance();
	Player::PlayerMode currentMode = player.GetCurrentMode();

	//�O�l�̂�������Ƃ�\�������Ȃ�
	if (currentMode == Player::PlayerMode::THIRD_PERSON) {
		return;
	}

	if (rodFlg) {
		//�Ƃ����[����
		MV1DetachAnim(rodmodel_, kamaerodAttachNo_);

		rodFlg = false;
		rodanimFlg = false;
		kamaeanimFlg = false;
		throwanimFlg = false;

		//�A�j���[�V�������Ԃ����Z�b�g
		currentAnimTime_ = 0.0f;
		currentkamaeAnimTime_ = 0.0f;
		currentthrowAnimTime_ = 0.0f;

		ChangeState(STATE::SPAWN);

		//�E�L�̏�Ԃ����Z�b�g
		Dobber::GetInstance().Reset();
	}
	else {
		//�Ƃ��o��
		rodFlg = true;
		rodanimFlg = true;

		//�A�j���[�V�������Z�b�g
		MV1DetachAnim(rodmodel_, kamaerodAttachNo_);
		MV1DetachAnim(rodmodel_, animrodAttachNo_);
		MV1DetachAnim(rodmodel_, throwanimAttachNo_);

		currentAnimTime_ = 0.0f;

		//�v���C���[�̌������擾���ĊƂɐݒ肷��
		rot_ = player.GetAngle();
		MV1SetRotationXYZ(rodmodel_, rot_);

		//�Ƃ̏o���A�j���[�V������ݒ�
		animrodAttachNo_ = MV1AttachAnim(rodmodel_, 3, -1, -1);

		//��Ԃ�Spawn�ɕύX
		ChangeState(STATE::SPAWN);
	}
}

void Rod::Reset(void)
{
	rodFlg = false;
}
