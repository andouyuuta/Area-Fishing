#include "../Application.h"
#include "../Utility/Utility.h"
#include "../Manager/InputManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/Camera.h"
#include "Player.h"
#include "Stage.h"
#include"EffekseerForDXLib.h"
#include <cmath>

namespace
{
	// �A�j���[�V�����̍Đ����x
	constexpr float ANIM_SPEED = 0.2f;
	// �ړ���
	constexpr float MOVE_MAX_SPEED = 10.0f;
	//�������f���␳�p�x
	constexpr VECTOR INIT_MODEL_ROT_OFFSET = { 0.0f,DX_PI_F,0.0f };
	//�Փ˔���
	constexpr float COLLISION_RADIUS = 10.0f;
	//��]�X�s�[�h
	const float ROTATION_SPEED = 1.0f;
}

Player* Player::instance_ = nullptr;

void Player::CreateInstance()
{
	if (instance_ == nullptr)
	{
		instance_ = new Player();
	}
	instance_->Init();
}

Player& Player::GetInstance(void)
{
	return *instance_;
}

Player::Player(void)
	: playerpos_{ 0.0f,0.0f,0.0f },FallSoundHundle_(-1),RiverSoundHundle_(-1),SoundPos{0.0f,0.0f,0.0f},animAttachNo_(0),animTotalTime_(0),boatSoundHundle(0),
	boatstopHundle(0),camerarot_{0.0f,0.0f,0.0f},currentAnimTime_(0),currentMode_(PlayerMode::THIRD_PERSON),currentkamaeAnimTime_(0),currentthrowAnimTime_(0),effectResourceHandle(-1),
	isSoundPlaying(false),kamaerodAttachNo_(0),kamaerodTotalTime_(0),localRot_{0.0f,0.0f,0.0f},moveRot_{0.0f,0.0f,0.0f},moveVecRad_{0.0f,0.0f,0.0f},moveVec_{0.0f,0.0f,0.0f},playermodel_(-1),
	playerrot_{ 0.0f,0.0f,0.0f },playingEffectHandle(-1),rot_{0.0f,0.0f,0.0f},throwanimAttachNo_(0),throwanimTotalTime_(0.0f)
{
}

Player::~Player(void)
{
}

void Player::Init()
{
	SetUseDirect3DVersion(DX_DIRECT3D_11);

	if (Effekseer_Init(8000) == -1)
	{
		DxLib_End();
		return;
	}
	SetChangeScreenModeGraphicsSystemResetFlag(FALSE);
	Effekseer_SetGraphicsDeviceLostCallbackFunctions();

	effectResourceHandle = LoadEffekseerEffect("Data/Effect/run.efkefc", 10.0f);
	playingEffectHandle = PlayEffekseer3DEffect(effectResourceHandle);			//��G�t�F�N�g�P�̃n���h��

	SetPosPlayingEffekseer3DEffect(playingEffectHandle, 00.0f,/* DX_PI_F / 2*/0.0f, -20.0f);
	SetRotationPlayingEffekseer3DEffect(playingEffectHandle, 0.0f, .0f, 0.0f);
	SetScalePlayingEffekseer3DEffect(playingEffectHandle, 10.0f, 10.0f, 12.0f);

	// ���f���̓ǂݍ���
	playermodel_ = MV1LoadModel("Data/Model/Stage/boat.mv1");

	boatSoundHundle = LoadSoundMem("Data/Sound/ship_engine.mp3");
	isSoundPlaying = false;

	// ���f���̏����ʒu�ݒ�
	playerpos_ = { 50.0f, -151.0f, 50.0f };

	MV1SetPosition(playermodel_, playerpos_);
	MV1SetScale(playermodel_, scale_);

	// �Đ�����A�j���[�V�����̐ݒ�
	animAttachNo_ = MV1AttachAnim(playermodel_, 0, -1, -1);
	// �A�j���[�V���������Ԃ̎擾
	animTotalTime_ = MV1GetAttachAnimTotalTime(playermodel_, animAttachNo_);
	currentAnimTime_ = 0.0f;
	// ���f���Ɏw�莞�Ԃ̃A�j���[�V������ݒ肷��
	MV1SetAttachAnimTime(playermodel_, animAttachNo_, currentAnimTime_);

	//�ړ��x�N�g�����쐬����p�x
	moveVecRad_ = { 0.0f,0.0f,0.0f };

	//���f���̊p�x�ݒ�
	rot_ = { 0.0f,0.0f,0.0f };

	//���f���̊p�x
	localRot_ = INIT_MODEL_ROT_OFFSET;

	currentMode_ = PlayerMode::THIRD_PERSON;

	SetCreate3DSoundFlag(TRUE);
	FallSoundHundle_ = LoadSoundMem("Data/Sound/waterfal.mp3");
	SetCreate3DSoundFlag(FALSE);

	SoundPos = { 0.0f,0.0f,23000.0f };

	Set3DPositionSoundMem(SoundPos, FallSoundHundle_);

	Set3DRadiusSoundMem(RADIUS, FallSoundHundle_);
}

void Player::Update(void)
{

	Effekseer_Sync3DSetting();

	PlaySoundMem(FallSoundHundle_, DX_PLAYTYPE_LOOP);
	Set3DRadiusSoundMem(RADIUS, FallSoundHundle_);

	currentAnimTime_ += ANIM_SPEED;
	MV1SetAttachAnimTime(playermodel_, animAttachNo_, currentAnimTime_);

	if (currentAnimTime_ > animTotalTime_)
	{
		currentAnimTime_ = 0.0f;
	}

	auto& ins = InputManager::GetInstance();
	if (ins.IsTrgMouseRight())
	{
		if (currentMode_ == PlayerMode::THIRD_PERSON)
		{
			currentMode_ = PlayerMode::FIRST_PERSON;
		}
		else {
			currentMode_ = PlayerMode::THIRD_PERSON;
		}
	}

	switch (currentMode_)
	{
	case THIRD_PERSON:
		UpdateMove();
		break;
	case FIRST_PERSON:
		break;
	default:
		UpdateMove();
		break;
	}
}

void Player::Draw(void)
{
	MV1DrawModel(playermodel_);
}

void Player::Release(void)
{
	MV1DeleteModel(playermodel_);
	DeleteSoundMem(boatSoundHundle);
	DeleteEffekseerEffect(effectResourceHandle);

	DeleteSoundMem(FallSoundHundle_);
}

void Player::UpdateAnimation(void)
{
	// �V�����A�j���[�V�����Đ����Ԃ��Z�b�g
	currentAnimTime_ += ANIM_SPEED;
	if (currentAnimTime_ > animTotalTime_)
	{
		currentAnimTime_ = 0.0f;

		// ���f���Ɏw�莞�Ԃ̃A�j���[�V������ݒ肷��
		MV1SetAttachAnimTime(playermodel_, animAttachNo_, currentAnimTime_);
	}
}

void Player::UpdateMove(void)
{
	//���͐���擾
	InputManager& ins = InputManager::GetInstance();

	//WASD�Ńv���C���[�ړ�
	moveVec_ = { 0.0f,0.0f,0.0f };
	//���E�E�E��O�E���̃x�N�g�����쐬����
	VECTOR RIGHT_MOVE_VEC = { 1.0f,0.0f,0.0f };
	VECTOR LEFT_MOVE_VEC = { -1.0f,0.0f,0.0f };
	VECTOR FRONT_MOVE_VEC = { 0.0f,0.0f,1.0f };
	VECTOR BACK_MOVE_VEC = { 0.0f,0.0f,-1.0f };

	//���͕����Ɉړ��x�N�g����ǉ�����
	if (ins.IsNew(KEY_INPUT_W)) { moveVec_ = VAdd(moveVec_, FRONT_MOVE_VEC);}
	if (ins.IsNew(KEY_INPUT_A)) { moveVec_ = VAdd(moveVec_, LEFT_MOVE_VEC); }
	if (ins.IsNew(KEY_INPUT_S)) { moveVec_ = VAdd(moveVec_, BACK_MOVE_VEC); }
	if (ins.IsNew(KEY_INPUT_D)) { moveVec_ = VAdd(moveVec_, RIGHT_MOVE_VEC); }

	// �L�[�����݉�����Ă��邩�`�F�b�N
	keyFlg = IsAnyKeyPressed();

	if(keyFlg) {
		movespeed += 0.02f;
		if (movespeed >= MOVE_MAX_SPEED) {
			movespeed = MOVE_MAX_SPEED;
		}

		//�x�N�g���̈ړ����s���Ă�������W�X�V
	if (IsMove(moveVec_))
	{		
		//�J�����p�x���ݒ肷��
		VECTOR cameraAngles = SceneManager::GetInstance().GetCamera()->GetAngles();
		MATRIX cameraMatY = MGetRotY(cameraAngles.y);
		moveVec_ = VTransform(moveVec_, cameraMatY);

		//���W�X�V
		moveVec_ = VNorm(moveVec_);
		moveVec_ = VScale(moveVec_, movespeed);

		playerpos_ = VAdd(playerpos_, moveVec_);

		//�������p�x�ɕϊ�����(XZ���ʁ@Y��)
		targetRot_.y= atan2f(moveVec_.x, moveVec_.z);

		//���W�ݒ�
		MV1SetPosition(playermodel_, playerpos_);
	}

		// �����Đ����łȂ��ꍇ�A�Đ����J�n
		if (isSoundPlaying==false) {
			PlaySoundMem(boatSoundHundle, DX_PLAYTYPE_BACK, true);
			isSoundPlaying = true;
		}
	}
	else {
		movespeed = 0.0f;
		// �����Đ����ł���Β�~
		if (isSoundPlaying == true) {
			StopSoundMem(boatSoundHundle);
			isSoundPlaying = false;
		}
	}

	if (playerpos_.y!= -151.0f) {
		playerpos_.y = -151.0f;
	}

	UpdateRotation();

}

void Player::UpdateRotation(void)
{
	float deltaTime = GetDeltaTime();

	// ���݂̊p�x�ƖڕW�p�x�̍����v�Z
	float rotDiff = targetRot_.y - rot_.y;

	// -�� ~ �� �͈̔͂Ɏ��߂�
	if (rotDiff > DX_PI_F) rotDiff -= 2 * DX_PI_F;
	if (rotDiff < -DX_PI_F) rotDiff += 2 * DX_PI_F;

	// ��]���x�Ɋ�Â��ĕ�Ԃ���
	float maxRotation = ROTATION_SPEED * deltaTime;
	if (fabsf(rotDiff) > maxRotation)
	{
		// �ő��]���x�𒴂��Ă����ꍇ�A��]�ʂ𐧌�
		rot_.y += (rotDiff > 0 ? maxRotation : -maxRotation);
	}
	else
	{
		// ��]�ʂ��\���ɏ������ꍇ�A�ڕW�p�x�ɍ��킹��
		rot_.y = targetRot_.y;
	}

	// ��]�s����쐬���ă��f���ɓK�p
	SetRotation();
}

bool Player::IsMove(const VECTOR _moveVec)
{
	//XYZ���ׂĂ̍��W�̈ړ��ʂ̐�Βl���Ƃ�
	float absX = abs(_moveVec.x);
	float absY = abs(_moveVec.y);
	float absZ = abs(_moveVec.z);

	//������Ȃ��������l��������ɏ�������΂O�Ɣ��肷��
	bool isNoMoveX = absX < FLT_EPSILON;
	bool isNoMoveY = absY < FLT_EPSILON;
	bool isNoMoveZ = absZ < FLT_EPSILON;

	//�ǂ̍��W���ړ����Ă��Ȃ���΍��W�X�V������������
	if (isNoMoveX && isNoMoveY && isNoMoveZ) 
	{
		return false;
	}
	else
	{
		return true;
	}
}

void Player::SetRotation(void)
{
	//��]�s����g�p�����p�x�ݒ�

	//�P�ʍs���ݒ肷��
	MATRIX mat = MGetIdent();

	//���f�����̂�Y����]�s����쐬����
	MATRIX mGetRotY = MGetRotY(rot_.y);

	//���f���̕␳�pY����]�s����쐬����
	MATRIX mGetLocalRotY = MGetRotY(localRot_.y);

	//�s����쐬
	mat = MMult(mat, mGetRotY);
	mat = MMult(mat, mGetLocalRotY);

	//�s����g�p���ă��f���̊p�x��ݒ�
	MV1SetRotationMatrix(playermodel_, mat);
}

void Player::DrawDebug(void)
{
	DrawFormatString(0, 260, 0xffffff,
		"�ړ��x�N�g���̊p�x(�ʓx�@)�F(X,Y,Z) = (%1.2lf %1.2lf, %1.2lf)",
		moveVecRad_.x, moveVecRad_.y, moveVecRad_.z);

	DrawFormatString(0, 300, 0xffffff,
		"�ړ��x�N�g���̊p�x(�x���@)�F(X,Y,Z) = (%1.01f %1.01f, %1.01f)",
		moveVecRad_.x * 180 / DX_PI_F,
		moveVecRad_.y * 180 / DX_PI_F,
		moveVecRad_.z * 180 / DX_PI_F);

	//�f�o�b�O����
	DrawFormatString(0, 20, 0xffff00,
		"���f���̉�]�l(�ʓx�@)�F\n(X,Y,Z) = (%1.2lf,%1.2lf,%1.2lf)", rot_.x, rot_.y, rot_.z);

	DrawFormatString(0, 60, 0xffff00,
		"���f���̉�]�l(�x���@)�F\n(X,Y,Z) = (%1.0lf,%1.0lf,%1.0lf)",
		rot_.x * 180 / DX_PI_F,
		rot_.y * 180 / DX_PI_F,
		rot_.z * 180 / DX_PI_F);

	//���f���̊p�x�̕␳���Ȃ������p�x
	const float INIT_MODEL_ROT = DX_PI_F / 2;
	float noOffsetRotY = rot_.y + INIT_MODEL_ROT;
	DrawFormatString(0, 120, 0x00ff00,
		"���f���̉�]�l(�ʓx�@)(���f���̕����␳�Ȃ�)�F\n(X,Y,Z)=(%1.2lf,%1.2lf,%1.2lf)",
		rot_.x,
		noOffsetRotY,
		rot_.z);

	DrawFormatString(0, 160, 0x00ff00,
		"���f���̉�]�l(�x���@)(���f���̕����␳�Ȃ�)�F\n(X,Y,Z)=(%1.0lf,%1.0lf,%1.0lf)",
		rot_.x * 180 / DX_PI_F,
		noOffsetRotY * 180 * DX_PI_F,
		rot_.z * 180 / DX_PI_F);

	//���f���̈ړ�����
	if (IsMove(moveVec_)) {
		//�ړ��x�N�g���𐳋K��
		VECTOR debugMoveVec = VNorm(moveVec_);

		//���̒�����ݒ�
		constexpr float DEBUG_MOVE_LINE_LENGTH = 100.0f;
		debugMoveVec = VScale(debugMoveVec, DEBUG_MOVE_LINE_LENGTH);

		//���̏I�[���W��ݒ�
		VECTOR debugMoveLineEndPos = VAdd(playerpos_, debugMoveVec);
	}
}

float Player::GetDeltaTime()
{
	// ���݂�FPS���擾
	float fps = GetFPS();

	// FPS��0�̏ꍇ�͈��S��1��Ԃ�
	if (fps == 0.0f) return 1.0f;

	// �f���^�^�C����FPS����t�Z
	return 1.0f / fps;
}

bool Player::IsAnyKeyPressed()
{
	InputManager& ins = InputManager::GetInstance();
	return ins.IsNew(KEY_INPUT_W) || ins.IsNew(KEY_INPUT_A) || ins.IsNew(KEY_INPUT_S) || ins.IsNew(KEY_INPUT_D);
}

