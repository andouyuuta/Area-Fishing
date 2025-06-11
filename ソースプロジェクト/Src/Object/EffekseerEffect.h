#pragma once
#include <DxLib.h>

class EffekseerEffect
{
public:

	EffekseerEffect(void);
	~EffekseerEffect(void);

	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);

	static constexpr int Second = 60;
	int spawnTimer = 0;
	const int spawnInterval = Second * 3;
	const int spawnInterval2 = Second * 0;


private:
	//エフェクトハンドル
	int effectResourceHandle;

	int playingEffectHandle;

	int effectResourceHandle2;

	int playingEffectHandle2;

	int effectResourceHandle3;

	int playingEffectHandle3;

	int effectResourceHandle4;

	int playingEffectHandle4;

	int effectResourceHandle5;

	int playingEffectHandle5;

	int effectResourceHandle6;

	int playingEffectHandle6;


	int effectResourceHandle7;

	int playingEffectHandle7;


	int effectResourceHandle8;

	int playingEffectHandle8;


	int effectResourceHandle9;

	int playingEffectHandle9;

	int effectResourceHandle10;

	int playingEffectHandle10;
	int effectResourceHandle11;

	int playingEffectHandle11;
	int effectResourceHandle12;

	int playingEffectHandle12;
};
