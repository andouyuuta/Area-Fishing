#include "../Application.h"
#include "../Utility/Utility.h"
#include "Stage.h"

Stage* Stage::instance_ = nullptr;

void Stage::CreateInstance()
{
	if (instance_ == nullptr)
	{
		instance_ = new Stage();
	}
	instance_->Init();
}

Stage& Stage::GetInstance(void)
{
	return *instance_;
}


Stage::Stage(void)
{
	FallSoundHundle_ = -1;
	RiverSoundHundle_ = -1;
	SoundPos = { 0.0f,0.0f,0.0f }
	; background_ = -1;
	backgroundpos_ = { 0.0f,0.0f,0.0f };
	envSoundHundle_ = -1;
	stagemodel_ = -1;
	stagepos_ = { 0.0f,0.0f,0.0f };
	watermodel_ = -1;
	waterpos_ = { 0.0f,0.0f,0.0f };
	windmillmodel_ = -1;
	windmillpos_ = { 0.0,0.0f,0.0f };
	windmillrot_ = { 0.0f,0.0f,0.0f };
}

Stage::~Stage(void)
{
}

void Stage::Init(void)
{
	// ステージモデルの読み込み
	stagemodel_ = MV1LoadModel("Data/Model/Stage/LAKE.mv1");
	background_ = MV1LoadModel("Data/Model/Stage/background.mv1");
	windmillmodel_ = MV1LoadModel("Data/Model/Stage/huusyanohane.mv1");
	envSoundHundle_ = LoadSoundMem("Data/Sound/bird.mp3");

	SetCreate3DSoundFlag(TRUE);
	FallSoundHundle_ = LoadSoundMem("Data/Sound/waterfal.mp3");

	SoundPos = { 0.0f,0.0f,21000.0f };

	Set3DPositionSoundMem(SoundPos, FallSoundHundle_);

	Set3DRadiusSoundMem(RADIUS, FallSoundHundle_);

	// モデルの位置
	stagepos_ = { 0.0f, -600.0f, 0.0f };
	windmillpos_ = { 180.0f,300.0f,-650.0f };

	backgroundpos_ = { 0.0f,0.0f,0.0f };

	windmillrot_ = { 0.0f,0.0f,0.0f };

	// モデルの位置を設定
	MV1SetPosition(stagemodel_, stagepos_);
	MV1SetPosition(background_, backgroundpos_);
	MV1SetPosition(windmillmodel_, windmillpos_);
	MV1SetPosition(watermodel_, waterpos_);
	MV1SetRotationXYZ(windmillmodel_, windmillrot_);
	MV1SetScale(windmillmodel_, scale);
	MV1SetScale(stagemodel_, scale);
	MV1SetScale(watermodel_, { 100.0f,0.5f,100.0f });
	MV1SetScale(background_, { 23.0f,23.0f,23.0f });

	MV1SetupCollInfo(stagemodel_, -1, 8, 8, 8);
}

void Stage::Update(void)
{
	WindmillMove();
	if (CheckSoundMem(FallSoundHundle_) == 0) {
		PlaySoundMem(FallSoundHundle_, DX_PLAYTYPE_LOOP);
	}

	if (CheckSoundMem(envSoundHundle_) == 0) {
		PlaySoundMem(envSoundHundle_, DX_PLAYTYPE_LOOP);
	}
}

void Stage::Draw(void)
{
	MV1DrawModel(stagemodel_);
	MV1SetMaterialDrawBlendMode(stagemodel_, 128, DX_BLENDMODE_ADD);
	MV1DrawModel(windmillmodel_);
	
	MV1DrawModel(background_);
}

void Stage::Release(void)
{
	MV1DeleteModel(stagemodel_);
	MV1DeleteModel(background_);
	MV1DeleteModel(windmillmodel_);
	
	DeleteSoundMem(FallSoundHundle_);
	DeleteSoundMem(envSoundHundle_);

}

void Stage::WindmillMove(void)
{
	windmillrot_.x += Utility::Deg2RadF(0.4f);
	MV1SetRotationXYZ(windmillmodel_, windmillrot_);
}
