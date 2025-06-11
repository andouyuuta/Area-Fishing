#include "water.h"
#include <DxLib.h>

namespace
{
	// モデルパス
	static constexpr const char* WATER_MODEL_PATH = "Data/Model/Stage/water.mv1";

	// 水の位置・スケール
	static constexpr VECTOR WATER_POS = { 0.0f, -160.0f, 0.0f };
	static constexpr VECTOR WATER_SCALE = { 100.0f, 0.5f, 100.0f };

	// 水の透明度
	static constexpr float OPACITY_RATE = 0.7f;
}

Water::Water(void)
	: pos_(WATER_POS),
	scale_(WATER_SCALE),
	waterModel_(-1)
{
}

Water::~Water(void)
{
}

void Water::Init(void)
{
	waterModel_ = MV1LoadModel(WATER_MODEL_PATH);
	MV1SetOpacityRate(waterModel_, OPACITY_RATE);

	MV1SetPosition(waterModel_, pos_);

	MV1SetScale(waterModel_, scale_);
}

void Water::Update(void)
{
}

void Water::Draw(void)
{
	MV1DrawModel(waterModel_);
	MV1SetOpacityRate(waterModel_, OPACITY_RATE);
}

void Water::Release(void)
{
	MV1DeleteModel(waterModel_);
}
