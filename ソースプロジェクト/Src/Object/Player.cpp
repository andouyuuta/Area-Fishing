#include "../Application.h"
#include "../Utility/Utility.h"
#include "../Manager/InputManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/Camera.h"
#include "Player.h"
#include "Stage.h"
#include "EffekseerForDXLib.h"
#include "Player.h"
#include <cmath>

namespace
{
	// �A�j���[�V�����E�ړ��֘A
	constexpr float ANIM_SPEED = 0.2f;
	constexpr float MOVE_MAX_SPEED = 10.0f;
	constexpr float PLAYER_ACCELERATION = 0.02f;
	constexpr float ROTATION_SPEED = 1.0f;
	constexpr float COLLISION_RADIUS = 10.0f;
	constexpr float GROUND_Y = -151.0f;

	// �v���C���[�����ݒ�
	constexpr VECTOR PLAYER_INITIAL_POS = { 50.0f, -151.0f, 50.0f };
	constexpr VECTOR INIT_MODEL_ROT_OFFSET = { 0.0f, DX_PI_F, 0.0f };
	constexpr VECTOR PLAYER_MODEL_SCALE = { 0.35f, 0.35f, 0.35f };

	// �T�E���h�֘A
	constexpr float SOUND_RADIUS = 20000.0f;
	constexpr VECTOR WATERFALL_SOUND_POS = { 0.0f, 0.0f, 23000.0f };

	// �G�t�F�N�g�֘A
	constexpr VECTOR RIVER_EFFECT_POS = { 0.0f, 0.0f, -20.0f };
	constexpr VECTOR RIVER_EFFECT_ROT = { 0.0f, 0.0f, 0.0f };
	constexpr VECTOR RIVER_EFFECT_SCALE = { 10.0f, 10.0f, 12.0f };

	// �p�X�֘A
	constexpr const char* RIVER_EFFECT_PATH = "Data/Effect/run.efkefc";
	constexpr const char* PLAYER_MODEL_PATH = "Data/Model/Stage/boat.mv1";
	constexpr const char* BOAT_SOUND_PATH = "Data/Sound/ship_engine.mp3";
}

Player::Player(void)
	:pos_(PLAYER_INITIAL_POS),
	scale_(PLAYER_MODEL_SCALE),
	animAttachNo_(0),
	animTotalTime_(0),
	boatSoundHandle_(-1),
	currentAnimTime_(0.0f),
	currentMode_(PlayerMode::THIRD_PERSON),
	effectResourceHandle_(-1),
	isSoundPlaying_(false),
	localRot_(INIT_MODEL_ROT_OFFSET),
	moveVec_{ 0.0f, 0.0f, 0.0f },
	moveSpeed_(0.0f),
	rot_{ 0.0f, 0.0f, 0.0f },
	targetRot_{ 0.0f, 0.0f, 0.0f },
	playerModel_(-1),
	playingEffectHandle_(-1)
{
}

Player::~Player(void)
{
}

void Player::Init(Camera* camera)
{
	camera_ = camera;

	SetUseDirect3DVersion(DX_DIRECT3D_11);

	if (Effekseer_Init(8000) == -1)
	{
		DxLib_End();
		return;
	}

	SetChangeScreenModeGraphicsSystemResetFlag(FALSE);
	Effekseer_SetGraphicsDeviceLostCallbackFunctions();

	effectResourceHandle_ = LoadEffekseerEffect(RIVER_EFFECT_PATH, 10.0f);
	playingEffectHandle_ = PlayEffekseer3DEffect(effectResourceHandle_);

	SetPosPlayingEffekseer3DEffect(playingEffectHandle_, RIVER_EFFECT_POS.x, RIVER_EFFECT_POS.y, RIVER_EFFECT_POS.z);
	SetRotationPlayingEffekseer3DEffect(playingEffectHandle_, RIVER_EFFECT_ROT.x, RIVER_EFFECT_ROT.y, RIVER_EFFECT_ROT.z);
	SetScalePlayingEffekseer3DEffect(playingEffectHandle_, RIVER_EFFECT_SCALE.x, RIVER_EFFECT_SCALE.y, RIVER_EFFECT_SCALE.z);

	playerModel_ = MV1LoadModel(PLAYER_MODEL_PATH);
	boatSoundHandle_ = LoadSoundMem(BOAT_SOUND_PATH);
	isSoundPlaying_ = false;

	MV1SetPosition(playerModel_, pos_);
	MV1SetScale(playerModel_, scale_);

	animAttachNo_ = MV1AttachAnim(playerModel_, 0, -1, -1);
	animTotalTime_ = MV1GetAttachAnimTotalTime(playerModel_, animAttachNo_);
	currentAnimTime_ = 0.0f;
	MV1SetAttachAnimTime(playerModel_, animAttachNo_, currentAnimTime_);

	SetCreate3DSoundFlag(TRUE);
	SetCreate3DSoundFlag(FALSE);
}

void Player::Update(void)
{
	Effekseer_Sync3DSetting();

	currentAnimTime_ += ANIM_SPEED;
	MV1SetAttachAnimTime(playerModel_, animAttachNo_, currentAnimTime_);

	// �A�j���[�V�������[�v�Đ�
	if (currentAnimTime_ > animTotalTime_)
	{
		currentAnimTime_ = 0.0f;
	}

	auto& ins = InputManager::GetInstance();
	// �E�N���b�N�ň�l�́E�O�l�̕ϊ�
	if (ins.IsTrgMouseRight())
	{
		if (currentMode_ == PlayerMode::THIRD_PERSON)
		{
			currentMode_ = PlayerMode::FIRST_PERSON;
		}
		else 
		{
			currentMode_ = PlayerMode::THIRD_PERSON;
		}
	}

	// �O�l�̂����v���C���[�𓮂���
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
	MV1DrawModel(playerModel_);
}

void Player::Release(void)
{
	MV1DeleteModel(playerModel_);
	DeleteSoundMem(boatSoundHandle_);
	DeleteEffekseerEffect(effectResourceHandle_);
}

void Player::UpdateAnimation(void)
{
	// �V�����A�j���[�V�����Đ����Ԃ��Z�b�g
	currentAnimTime_ += ANIM_SPEED;
	if (currentAnimTime_ > animTotalTime_)
	{
		currentAnimTime_ = 0.0f;

		// ���f���Ɏw�莞�Ԃ̃A�j���[�V������ݒ肷��
		MV1SetAttachAnimTime(playerModel_, animAttachNo_, currentAnimTime_);
	}
}

void Player::UpdateMove(void)
{
	// ���͐���擾
	InputManager& ins = InputManager::GetInstance();

	// WASD�Ńv���C���[�ړ�
	moveVec_ = { 0.0f,0.0f,0.0f };
	// ���E�E�E��O�E���̃x�N�g�����쐬����
	VECTOR RIGHT_MOVE_VEC = { 1.0f,0.0f,0.0f };
	VECTOR LEFT_MOVE_VEC = { -1.0f,0.0f,0.0f };
	VECTOR FRONT_MOVE_VEC = { 0.0f,0.0f,1.0f };
	VECTOR BACK_MOVE_VEC = { 0.0f,0.0f,-1.0f };

	// ���͕����Ɉړ��x�N�g����ǉ�����
	if (ins.IsNew(KEY_INPUT_W)) { moveVec_ = VAdd(moveVec_, FRONT_MOVE_VEC); }
	if (ins.IsNew(KEY_INPUT_A)) { moveVec_ = VAdd(moveVec_, LEFT_MOVE_VEC); }
	if (ins.IsNew(KEY_INPUT_S)) { moveVec_ = VAdd(moveVec_, BACK_MOVE_VEC); }
	if (ins.IsNew(KEY_INPUT_D)) { moveVec_ = VAdd(moveVec_, RIGHT_MOVE_VEC); }

	// �L�[�����݉�����Ă��邩�`�F�b�N
	if (IsAnyKeyPressed()) 
	{
		moveSpeed_ += PLAYER_ACCELERATION;
		if (moveSpeed_ >= MOVE_MAX_SPEED) 
		{
			moveSpeed_ = MOVE_MAX_SPEED;
		}
		// �x�N�g���̈ړ����s���Ă�������W�X�V
		if (IsMove(moveVec_))
		{
			// �J�����p�x���ݒ肷��
			MATRIX cameraMatY = MGetRotY(camera_->GetAngles().y);
			moveVec_ = VTransform(moveVec_, cameraMatY);

			// ���W�X�V
			moveVec_ = VNorm(moveVec_);
			moveVec_ = VScale(moveVec_, moveSpeed_);

			pos_ = VAdd(pos_, moveVec_);

			// �������p�x�ɕϊ�����(XZ���ʁ@Y��)
			targetRot_.y = atan2f(moveVec_.x, moveVec_.z);

			// ���W�ݒ�
			MV1SetPosition(playerModel_, pos_);
		}

		// �����Đ����łȂ��ꍇ�A�Đ����J�n
		if (isSoundPlaying_ == false) 
		{
			PlaySoundMem(boatSoundHandle_, DX_PLAYTYPE_BACK, true);
			isSoundPlaying_ = true;
		}
	}
	else {
		moveSpeed_ = 0.0f;
		// �����Đ����ł���Β�~
		if (isSoundPlaying_ == true) 
		{
			StopSoundMem(boatSoundHandle_);
			isSoundPlaying_ = false;
		}
	}

	pos_.y = GROUND_Y;

	UpdateRotation();
}

void Player::UpdateRotation(void)
{
	float deltaTime = Application::GetInstance().GetDeltaTime();

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
	// XYZ���ׂĂ̍��W�̈ړ��ʂ̐�Βl���Ƃ�
	float absX = abs(_moveVec.x);
	float absY = abs(_moveVec.y);
	float absZ = abs(_moveVec.z);

	// ������Ȃ��������l��������ɏ�������΂O�Ɣ��肷��
	bool isNoMoveX = absX < FLT_EPSILON;
	bool isNoMoveY = absY < FLT_EPSILON;
	bool isNoMoveZ = absZ < FLT_EPSILON;

	// �ǂ̍��W���ړ����Ă��Ȃ���΍��W�X�V������������
	if (isNoMoveX && isNoMoveY && isNoMoveZ) 
	{
		return false;
	}
	else
	{
		return true;
	}
}

void Player::SetRotation(void) const
{
	// ��]�s����g�p�����p�x�ݒ�

	// �P�ʍs���ݒ肷��
	MATRIX mat = MGetIdent();

	// ���f�����̂�Y����]�s����쐬����
	MATRIX mGetRotY = MGetRotY(rot_.y);

	// ���f���̕␳�pY����]�s����쐬����
	MATRIX mGetLocalRotY = MGetRotY(localRot_.y);

	// �s����쐬
	mat = MMult(mat, mGetRotY);
	mat = MMult(mat, mGetLocalRotY);

	// �s����g�p���ă��f���̊p�x��ݒ�
	MV1SetRotationMatrix(playerModel_, mat);
}


// �ړ��L�[��������true
bool Player::IsAnyKeyPressed()
{
	InputManager& ins = InputManager::GetInstance();
	return ins.IsNew(KEY_INPUT_W) || ins.IsNew(KEY_INPUT_A) || ins.IsNew(KEY_INPUT_S) || ins.IsNew(KEY_INPUT_D);
}

