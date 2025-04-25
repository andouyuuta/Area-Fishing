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
	// アニメーションの再生速度
	constexpr float ANIM_SPEED = 0.2f;
	// 移動量
	constexpr float MOVE_MAX_SPEED = 10.0f;
	//初期モデル補正角度
	constexpr VECTOR INIT_MODEL_ROT_OFFSET = { 0.0f,DX_PI_F,0.0f };
	//衝突判定
	constexpr float COLLISION_RADIUS = 10.0f;
	//回転スピード
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
	playingEffectHandle = PlayEffekseer3DEffect(effectResourceHandle);			//川エフェクト１のハンドル

	SetPosPlayingEffekseer3DEffect(playingEffectHandle, 00.0f,/* DX_PI_F / 2*/0.0f, -20.0f);
	SetRotationPlayingEffekseer3DEffect(playingEffectHandle, 0.0f, .0f, 0.0f);
	SetScalePlayingEffekseer3DEffect(playingEffectHandle, 10.0f, 10.0f, 12.0f);

	// モデルの読み込み
	playermodel_ = MV1LoadModel("Data/Model/Stage/boat.mv1");

	boatSoundHundle = LoadSoundMem("Data/Sound/ship_engine.mp3");
	isSoundPlaying = false;

	// モデルの初期位置設定
	playerpos_ = { 50.0f, -151.0f, 50.0f };

	MV1SetPosition(playermodel_, playerpos_);
	MV1SetScale(playermodel_, scale_);

	// 再生するアニメーションの設定
	animAttachNo_ = MV1AttachAnim(playermodel_, 0, -1, -1);
	// アニメーション総時間の取得
	animTotalTime_ = MV1GetAttachAnimTotalTime(playermodel_, animAttachNo_);
	currentAnimTime_ = 0.0f;
	// モデルに指定時間のアニメーションを設定する
	MV1SetAttachAnimTime(playermodel_, animAttachNo_, currentAnimTime_);

	//移動ベクトルが作成する角度
	moveVecRad_ = { 0.0f,0.0f,0.0f };

	//モデルの角度設定
	rot_ = { 0.0f,0.0f,0.0f };

	//モデルの角度
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
	// 新しいアニメーション再生時間をセット
	currentAnimTime_ += ANIM_SPEED;
	if (currentAnimTime_ > animTotalTime_)
	{
		currentAnimTime_ = 0.0f;

		// モデルに指定時間のアニメーションを設定する
		MV1SetAttachAnimTime(playermodel_, animAttachNo_, currentAnimTime_);
	}
}

void Player::UpdateMove(void)
{
	//入力制御取得
	InputManager& ins = InputManager::GetInstance();

	//WASDでプレイヤー移動
	moveVec_ = { 0.0f,0.0f,0.0f };
	//左・右・手前・奥のベクトルを作成する
	VECTOR RIGHT_MOVE_VEC = { 1.0f,0.0f,0.0f };
	VECTOR LEFT_MOVE_VEC = { -1.0f,0.0f,0.0f };
	VECTOR FRONT_MOVE_VEC = { 0.0f,0.0f,1.0f };
	VECTOR BACK_MOVE_VEC = { 0.0f,0.0f,-1.0f };

	//入力方向に移動ベクトルを追加する
	if (ins.IsNew(KEY_INPUT_W)) { moveVec_ = VAdd(moveVec_, FRONT_MOVE_VEC);}
	if (ins.IsNew(KEY_INPUT_A)) { moveVec_ = VAdd(moveVec_, LEFT_MOVE_VEC); }
	if (ins.IsNew(KEY_INPUT_S)) { moveVec_ = VAdd(moveVec_, BACK_MOVE_VEC); }
	if (ins.IsNew(KEY_INPUT_D)) { moveVec_ = VAdd(moveVec_, RIGHT_MOVE_VEC); }

	// キーが現在押されているかチェック
	keyFlg = IsAnyKeyPressed();

	if(keyFlg) {
		movespeed += 0.02f;
		if (movespeed >= MOVE_MAX_SPEED) {
			movespeed = MOVE_MAX_SPEED;
		}

		//ベクトルの移動が行われていたら座標更新
	if (IsMove(moveVec_))
	{		
		//カメラ角度分設定する
		VECTOR cameraAngles = SceneManager::GetInstance().GetCamera()->GetAngles();
		MATRIX cameraMatY = MGetRotY(cameraAngles.y);
		moveVec_ = VTransform(moveVec_, cameraMatY);

		//座標更新
		moveVec_ = VNorm(moveVec_);
		moveVec_ = VScale(moveVec_, movespeed);

		playerpos_ = VAdd(playerpos_, moveVec_);

		//方向を角度に変換する(XZ平面　Y軸)
		targetRot_.y= atan2f(moveVec_.x, moveVec_.z);

		//座標設定
		MV1SetPosition(playermodel_, playerpos_);
	}

		// 音が再生中でない場合、再生を開始
		if (isSoundPlaying==false) {
			PlaySoundMem(boatSoundHundle, DX_PLAYTYPE_BACK, true);
			isSoundPlaying = true;
		}
	}
	else {
		movespeed = 0.0f;
		// 音が再生中であれば停止
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
	//XYZすべての座標の移動量の絶対値をとる
	float absX = abs(_moveVec.x);
	float absY = abs(_moveVec.y);
	float absZ = abs(_moveVec.z);

	//かぎりなく小さい値よりもさらに小さければ０と判定する
	bool isNoMoveX = absX < FLT_EPSILON;
	bool isNoMoveY = absY < FLT_EPSILON;
	bool isNoMoveZ = absZ < FLT_EPSILON;

	//どの座標も移動していなければ座標更新をせず抜ける
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
	//回転行列を使用した角度設定

	//単位行列を設定する
	MATRIX mat = MGetIdent();

	//モデル自体のY軸回転行列を作成する
	MATRIX mGetRotY = MGetRotY(rot_.y);

	//モデルの補正用Y軸回転行列を作成する
	MATRIX mGetLocalRotY = MGetRotY(localRot_.y);

	//行列を作成
	mat = MMult(mat, mGetRotY);
	mat = MMult(mat, mGetLocalRotY);

	//行列を使用してモデルの角度を設定
	MV1SetRotationMatrix(playermodel_, mat);
}

void Player::DrawDebug(void)
{
	DrawFormatString(0, 260, 0xffffff,
		"移動ベクトルの角度(弧度法)：(X,Y,Z) = (%1.2lf %1.2lf, %1.2lf)",
		moveVecRad_.x, moveVecRad_.y, moveVecRad_.z);

	DrawFormatString(0, 300, 0xffffff,
		"移動ベクトルの角度(度数法)：(X,Y,Z) = (%1.01f %1.01f, %1.01f)",
		moveVecRad_.x * 180 / DX_PI_F,
		moveVecRad_.y * 180 / DX_PI_F,
		moveVecRad_.z * 180 / DX_PI_F);

	//デバッグ文字
	DrawFormatString(0, 20, 0xffff00,
		"モデルの回転値(弧度法)：\n(X,Y,Z) = (%1.2lf,%1.2lf,%1.2lf)", rot_.x, rot_.y, rot_.z);

	DrawFormatString(0, 60, 0xffff00,
		"モデルの回転値(度数法)：\n(X,Y,Z) = (%1.0lf,%1.0lf,%1.0lf)",
		rot_.x * 180 / DX_PI_F,
		rot_.y * 180 / DX_PI_F,
		rot_.z * 180 / DX_PI_F);

	//モデルの角度の補正をなくした角度
	const float INIT_MODEL_ROT = DX_PI_F / 2;
	float noOffsetRotY = rot_.y + INIT_MODEL_ROT;
	DrawFormatString(0, 120, 0x00ff00,
		"モデルの回転値(弧度法)(モデルの方向補正なし)：\n(X,Y,Z)=(%1.2lf,%1.2lf,%1.2lf)",
		rot_.x,
		noOffsetRotY,
		rot_.z);

	DrawFormatString(0, 160, 0x00ff00,
		"モデルの回転値(度数法)(モデルの方向補正なし)：\n(X,Y,Z)=(%1.0lf,%1.0lf,%1.0lf)",
		rot_.x * 180 / DX_PI_F,
		noOffsetRotY * 180 * DX_PI_F,
		rot_.z * 180 / DX_PI_F);

	//モデルの移動方向
	if (IsMove(moveVec_)) {
		//移動ベクトルを正規化
		VECTOR debugMoveVec = VNorm(moveVec_);

		//線の長さを設定
		constexpr float DEBUG_MOVE_LINE_LENGTH = 100.0f;
		debugMoveVec = VScale(debugMoveVec, DEBUG_MOVE_LINE_LENGTH);

		//線の終端座標を設定
		VECTOR debugMoveLineEndPos = VAdd(playerpos_, debugMoveVec);
	}
}

float Player::GetDeltaTime()
{
	// 現在のFPSを取得
	float fps = GetFPS();

	// FPSが0の場合は安全に1を返す
	if (fps == 0.0f) return 1.0f;

	// デルタタイムをFPSから逆算
	return 1.0f / fps;
}

bool Player::IsAnyKeyPressed()
{
	InputManager& ins = InputManager::GetInstance();
	return ins.IsNew(KEY_INPUT_W) || ins.IsNew(KEY_INPUT_A) || ins.IsNew(KEY_INPUT_S) || ins.IsNew(KEY_INPUT_D);
}

