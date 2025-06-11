#include "../Application.h"
#include "../Utility/Utility.h"
#include "Stage.h"

namespace
{
	// モデル・サウンドパス
	static constexpr const char* STAGE_MODEL_PATH = "Data/Model/Stage/LAKE.mv1";
	static constexpr const char* BACKGROUND_MODEL_PATH = "Data/Model/Stage/background.mv1";
	static constexpr const char* WINDMILL_MODEL_PATH = "Data/Model/Stage/huusyanohane.mv1";
	static constexpr const char* BIRD_SOUND_PATH = "Data/Sound/bird.mp3";
	static constexpr const char* FALL_SOUND_PATH = "Data/Sound/waterfal.mp3";

	// モデルの位置・スケール
	static constexpr VECTOR STAGE_POS = { 0.0f, -600.0f, 0.0f };
	static constexpr VECTOR STAGE_SCALE = { 0.75f, 0.75f, 0.75f };
	static constexpr VECTOR BG_SCALE = { 23.0f, 23.0f, 23.0f };
	static constexpr VECTOR WINDMILL_POS = { 180.0f, 300.0f, -650.0f };

	// サウンドの位置
	static constexpr VECTOR SOUND_POS = { 0.0f, 0.0f, 21000.0f };

	// その他定数
	static constexpr float WINDMILL_ROTATION_SPEED_DEG = 0.4f;
}

Stage::Stage(void)
	: fallSoundHandle_(-1),
	soundPos_(SOUND_POS),
	backGroundModel_(-1),
	backGroundPos_{ 0.0f, 0.0f, 0.0f },
	backGroundScale_(BG_SCALE),
	envSoundHandle_(-1),
	stageModel_(-1),
	stagePos_(STAGE_POS),
	stageScale_(STAGE_SCALE),
	windmillModel_(-1),
	windmillPos_(WINDMILL_POS),
	windmillRot_{ 0.0f, 0.0f, 0.0f }
{
}

Stage::~Stage(void)
{
}

void Stage::Init(void)
{
	// ステージモデルの読み込み
	stageModel_ = MV1LoadModel(STAGE_MODEL_PATH);
	backGroundModel_ = MV1LoadModel(BACKGROUND_MODEL_PATH);
	windmillModel_ = MV1LoadModel(WINDMILL_MODEL_PATH);
	envSoundHandle_ = LoadSoundMem(BIRD_SOUND_PATH);

	SetCreate3DSoundFlag(TRUE);
	fallSoundHandle_ = LoadSoundMem(FALL_SOUND_PATH);

	Set3DPositionSoundMem(soundPos_, fallSoundHandle_);

	Set3DRadiusSoundMem(RADIUS, fallSoundHandle_);

	// モデルの位置を設定
	MV1SetPosition(stageModel_, stagePos_);
	MV1SetPosition(backGroundModel_, backGroundPos_);
	MV1SetPosition(windmillModel_, windmillPos_);
	MV1SetRotationXYZ(windmillModel_, windmillRot_);
	MV1SetScale(windmillModel_, stageScale_);
	MV1SetScale(stageModel_, stageScale_);
	MV1SetScale(backGroundModel_, backGroundScale_);

	MV1SetupCollInfo(stageModel_, -1, 8, 8, 8);
}

void Stage::Update(void)
{
	WindmillMove(); //	風車回転
	if (CheckSoundMem(fallSoundHandle_) == 0) 
	{
		PlaySoundMem(fallSoundHandle_, DX_PLAYTYPE_LOOP);
	}

	if (CheckSoundMem(envSoundHandle_) == 0) 
	{
		PlaySoundMem(envSoundHandle_, DX_PLAYTYPE_LOOP);
	}
}

void Stage::Draw(void)
{
	MV1DrawModel(stageModel_);
	MV1SetMaterialDrawBlendMode(stageModel_, 128, DX_BLENDMODE_ADD);
	MV1DrawModel(windmillModel_);
	
	MV1DrawModel(backGroundModel_);
}

void Stage::Release(void)
{
	MV1DeleteModel(stageModel_);
	MV1DeleteModel(backGroundModel_);
	MV1DeleteModel(windmillModel_);
	
	DeleteSoundMem(fallSoundHandle_);
	DeleteSoundMem(envSoundHandle_);
}

void Stage::WindmillMove(void)
{
	windmillRot_.x += Utility::Deg2RadF(WINDMILL_ROTATION_SPEED_DEG);
	MV1SetRotationXYZ(windmillModel_, windmillRot_);
}