#include "water.h"
#include<DxLib.h>

Water::Water(void)
{
	pos_ = { 0.0f,0.0f,0.0f }, waterSoundHundle = 0, watermodel_ = -1;
}

Water::~Water(void)
{
}

void Water::Init(void)
{
	watermodel_ = MV1LoadModel("Data/Model/Stage/water.mv1");
	MV1SetOpacityRate(watermodel_, 0.7f);

	pos_ = { 0.0f,-160.0f,0.0f };

	MV1SetPosition(watermodel_, pos_);

	MV1SetScale(watermodel_, { 100.0f,0.5f,100.0f });
}

void Water::Update(void)
{
}

void Water::Draw(void)
{
	MV1DrawModel(watermodel_);
	MV1SetOpacityRate(watermodel_, 0.7f);
}

void Water::Release(void)
{
	MV1DeleteModel(watermodel_);
}
