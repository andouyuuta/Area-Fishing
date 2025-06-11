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
	// アニメーション・移動関連
	constexpr float ANIM_SPEED = 0.2f;
	constexpr float MOVE_MAX_SPEED = 10.0f;
	constexpr float PLAYER_ACCELERATION = 0.02f;
	constexpr float ROTATION_SPEED = 1.0f;
	constexpr float COLLISION_RADIUS = 10.0f;
	constexpr float GROUND_Y = -151.0f;

	// プレイヤー初期設定
	constexpr VECTOR PLAYER_INITIAL_POS = { 50.0f, -151.0f, 50.0f };
	constexpr VECTOR INIT_MODEL_ROT_OFFSET = { 0.0f, DX_PI_F, 0.0f };
	constexpr VECTOR PLAYER_MODEL_SCALE = { 0.35f, 0.35f, 0.35f };

	// サウンド関連
	constexpr float SOUND_RADIUS = 20000.0f;
	constexpr VECTOR WATERFALL_SOUND_POS = { 0.0f, 0.0f, 23000.0f };

	// エフェクト関連
	constexpr VECTOR RIVER_EFFECT_POS = { 0.0f, 0.0f, -20.0f };
	constexpr VECTOR RIVER_EFFECT_ROT = { 0.0f, 0.0f, 0.0f };
	constexpr VECTOR RIVER_EFFECT_SCALE = { 10.0f, 10.0f, 12.0f };

	// パス関連
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

	// アニメーションループ再生
	if (currentAnimTime_ > animTotalTime_)
	{
		currentAnimTime_ = 0.0f;
	}

	auto& ins = InputManager::GetInstance();
	// 右クリックで一人称・三人称変換
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

	// 三人称だけプレイヤーを動かす
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
	// 新しいアニメーション再生時間をセット
	currentAnimTime_ += ANIM_SPEED;
	if (currentAnimTime_ > animTotalTime_)
	{
		currentAnimTime_ = 0.0f;

		// モデルに指定時間のアニメーションを設定する
		MV1SetAttachAnimTime(playerModel_, animAttachNo_, currentAnimTime_);
	}
}

void Player::UpdateMove(void)
{
	// 入力制御取得
	InputManager& ins = InputManager::GetInstance();

	// WASDでプレイヤー移動
	moveVec_ = { 0.0f,0.0f,0.0f };
	// 左・右・手前・奥のベクトルを作成する
	VECTOR RIGHT_MOVE_VEC = { 1.0f,0.0f,0.0f };
	VECTOR LEFT_MOVE_VEC = { -1.0f,0.0f,0.0f };
	VECTOR FRONT_MOVE_VEC = { 0.0f,0.0f,1.0f };
	VECTOR BACK_MOVE_VEC = { 0.0f,0.0f,-1.0f };

	// 入力方向に移動ベクトルを追加する
	if (ins.IsNew(KEY_INPUT_W)) { moveVec_ = VAdd(moveVec_, FRONT_MOVE_VEC); }
	if (ins.IsNew(KEY_INPUT_A)) { moveVec_ = VAdd(moveVec_, LEFT_MOVE_VEC); }
	if (ins.IsNew(KEY_INPUT_S)) { moveVec_ = VAdd(moveVec_, BACK_MOVE_VEC); }
	if (ins.IsNew(KEY_INPUT_D)) { moveVec_ = VAdd(moveVec_, RIGHT_MOVE_VEC); }

	// キーが現在押されているかチェック
	if (IsAnyKeyPressed()) 
	{
		moveSpeed_ += PLAYER_ACCELERATION;
		if (moveSpeed_ >= MOVE_MAX_SPEED) 
		{
			moveSpeed_ = MOVE_MAX_SPEED;
		}
		// ベクトルの移動が行われていたら座標更新
		if (IsMove(moveVec_))
		{
			// カメラ角度分設定する
			MATRIX cameraMatY = MGetRotY(camera_->GetAngles().y);
			moveVec_ = VTransform(moveVec_, cameraMatY);

			// 座標更新
			moveVec_ = VNorm(moveVec_);
			moveVec_ = VScale(moveVec_, moveSpeed_);

			pos_ = VAdd(pos_, moveVec_);

			// 方向を角度に変換する(XZ平面　Y軸)
			targetRot_.y = atan2f(moveVec_.x, moveVec_.z);

			// 座標設定
			MV1SetPosition(playerModel_, pos_);
		}

		// 音が再生中でない場合、再生を開始
		if (isSoundPlaying_ == false) 
		{
			PlaySoundMem(boatSoundHandle_, DX_PLAYTYPE_BACK, true);
			isSoundPlaying_ = true;
		}
	}
	else {
		moveSpeed_ = 0.0f;
		// 音が再生中であれば停止
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

	// 現在の角度と目標角度の差を計算
	float rotDiff = targetRot_.y - rot_.y;

	// -π ~ π の範囲に収める
	if (rotDiff > DX_PI_F) rotDiff -= 2 * DX_PI_F;
	if (rotDiff < -DX_PI_F) rotDiff += 2 * DX_PI_F;

	// 回転速度に基づいて補間する
	float maxRotation = ROTATION_SPEED * deltaTime;
	if (fabsf(rotDiff) > maxRotation)
	{
		// 最大回転速度を超えていた場合、回転量を制限
		rot_.y += (rotDiff > 0 ? maxRotation : -maxRotation);
	}
	else
	{
		// 回転量が十分に小さい場合、目標角度に合わせる
		rot_.y = targetRot_.y;
	}

	// 回転行列を作成してモデルに適用
	SetRotation();
}

bool Player::IsMove(const VECTOR _moveVec)
{
	// XYZすべての座標の移動量の絶対値をとる
	float absX = abs(_moveVec.x);
	float absY = abs(_moveVec.y);
	float absZ = abs(_moveVec.z);

	// かぎりなく小さい値よりもさらに小さければ０と判定する
	bool isNoMoveX = absX < FLT_EPSILON;
	bool isNoMoveY = absY < FLT_EPSILON;
	bool isNoMoveZ = absZ < FLT_EPSILON;

	// どの座標も移動していなければ座標更新をせず抜ける
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
	// 回転行列を使用した角度設定

	// 単位行列を設定する
	MATRIX mat = MGetIdent();

	// モデル自体のY軸回転行列を作成する
	MATRIX mGetRotY = MGetRotY(rot_.y);

	// モデルの補正用Y軸回転行列を作成する
	MATRIX mGetLocalRotY = MGetRotY(localRot_.y);

	// 行列を作成
	mat = MMult(mat, mGetRotY);
	mat = MMult(mat, mGetLocalRotY);

	// 行列を使用してモデルの角度を設定
	MV1SetRotationMatrix(playerModel_, mat);
}


// 移動キー押したらtrue
bool Player::IsAnyKeyPressed()
{
	InputManager& ins = InputManager::GetInstance();
	return ins.IsNew(KEY_INPUT_W) || ins.IsNew(KEY_INPUT_A) || ins.IsNew(KEY_INPUT_S) || ins.IsNew(KEY_INPUT_D);
}

