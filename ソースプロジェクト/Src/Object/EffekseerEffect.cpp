#include "EffekseerEffect.h"
#include"EffekseerForDXLib.h"

EffekseerEffect::EffekseerEffect(void)
{
}

EffekseerEffect::~EffekseerEffect(void)
{
}

void EffekseerEffect::Init(void)
{
	spawnTimer = 0;

	//エフェクシアに必要な初期化
	//DirectX11を使用するようにする
	//Effekseerを使用するには必ず設定する
	SetUseDirect3DVersion(DX_DIRECT3D_11);

	//Effekseerを初期化する
	//引数にはがっ面に表示する最大パーティクル数を設定する
	if (Effekseer_Init(8000) == -1)
	{
		DxLib_End();
		return;
	}

	//フルスクリーンウィンドウの切り替えでリソースが消えるのを防ぐ
	//Effekseerwo 使用する場合は必ず設定する。
	SetChangeScreenModeGraphicsSystemResetFlag(FALSE);

	//DXライブラリのデバイスロスとした時のコールバックを設定する
	//ウィンドウとフルスクリーンの切り替えが発生する場合は必ず実行する
	Effekseer_SetGraphicsDeviceLostCallbackFunctions();

	//エフェクトの読み込み
	effectResourceHandle = LoadEffekseerEffect("Data/Effect/river.efkefc", 10.0f);
	effectResourceHandle2 = LoadEffekseerEffect("Data/Effect/river.efkefc", 10.0f);
	effectResourceHandle3 = LoadEffekseerEffect("Data/Effect/river.efkefc", 10.0f);
	effectResourceHandle4 = LoadEffekseerEffect("Data/Effect/Falls.efkefc", 10.0f);
	effectResourceHandle5 = LoadEffekseerEffect("Data/Effect/Falls.efkefc", 10.0f);
	effectResourceHandle6 = LoadEffekseerEffect("Data/Effect/mist.efkefc", 10.0f);
	effectResourceHandle7 = LoadEffekseerEffect("Data/Effect/river.efkefc", 10.0f);
	effectResourceHandle8 = LoadEffekseerEffect("Data/Effect/river.efkefc", 10.0f);
	effectResourceHandle9 = LoadEffekseerEffect("Data/Effect/river.efkefc", 10.0f);
	effectResourceHandle10 = LoadEffekseerEffect("Data/Effect/mist.efkefc", 10.0f);
	effectResourceHandle11 = LoadEffekseerEffect("Data/Effect/Falls.efkefc", 10.0f);
	effectResourceHandle12 = LoadEffekseerEffect("Data/Effect/Falls.efkefc", 10.0f);

	//エフェクトを再生する
	//PlayEffekseer3DEffect(effectResourceHandle);
	playingEffectHandle = PlayEffekseer3DEffect(effectResourceHandle);			//川エフェクト１のハンドル
	playingEffectHandle2 = PlayEffekseer3DEffect(effectResourceHandle2);		//川エフェクト２のハンドル
	playingEffectHandle3 = PlayEffekseer3DEffect(effectResourceHandle3);		//川エフェクト３のハンドル
	playingEffectHandle4 = PlayEffekseer3DEffect(effectResourceHandle4);		//滝エフェクト４のハンドル
	playingEffectHandle5 = PlayEffekseer3DEffect(effectResourceHandle5);		//滝エフェクト５のハンドル
	playingEffectHandle6 = PlayEffekseer3DEffect(effectResourceHandle6);		//滝つぼエフェクト６のハンドル
	//playingEffectHandle7 = PlayEffekseer3DEffect(effectResourceHandle7);		//滝つぼエフェクト６のハンドル
	//playingEffectHandle8 = PlayEffekseer3DEffect(effectResourceHandle8);		//滝つぼエフェクト６のハンドル
	//playingEffectHandle9 = PlayEffekseer3DEffect(effectResourceHandle9);		//滝つぼエフェクト６のハンドル
	//playingEffectHandle10 = PlayEffekseer3DEffect(effectResourceHandle10);		//滝つぼエフェクト６のハンドル
	//playingEffectHandle11 = PlayEffekseer3DEffect(effectResourceHandle11);		//滝つぼエフェクト６のハンドル
	//playingEffectHandle12 = PlayEffekseer3DEffect(effectResourceHandle12);		//滝つぼエフェクト６のハンドル

	//川１の大きさ・位置・向きの設定
	SetPosPlayingEffekseer3DEffect(playingEffectHandle, 800.0f,/* DX_PI_F / 2*/0.0f, 20000.0f);
	SetRotationPlayingEffekseer3DEffect(playingEffectHandle, 0.0f, DX_PI_F / 2.0f, 0.0f);
	SetScalePlayingEffekseer3DEffect(playingEffectHandle, 10.0f, 10.0f, 12.0f);
	//川２の大きさ・位置・向きの設定
	SetPosPlayingEffekseer3DEffect(playingEffectHandle2, -800.0f,/* DX_PI_F / 2*/0.0f, 20000.0f);
	SetRotationPlayingEffekseer3DEffect(playingEffectHandle2, 0.0f, DX_PI_F / 2.0f, 0.0f);
	SetScalePlayingEffekseer3DEffect(playingEffectHandle2, 10.0f, 10.0f, 12.0f);
	//川３の大きさ・位置・向きの設定
	SetPosPlayingEffekseer3DEffect(playingEffectHandle3, -800.0f,/* DX_PI_F / 2*/0.0f, 20000.0f);
	SetRotationPlayingEffekseer3DEffect(playingEffectHandle3, 0.0f, DX_PI_F / 2.0f, 0.0f);
	SetScalePlayingEffekseer3DEffect(playingEffectHandle3, 10.0f, 10.0f, 12.0f);
	//滝４の大きさ・位置・向きの設定
	SetPosPlayingEffekseer3DEffect(playingEffectHandle4, -200.0f,/* DX_PI_F / 2*/10000.0f, 21000.0f);
	SetRotationPlayingEffekseer3DEffect(playingEffectHandle4, 0.0f, DX_PI_F / 2.0f, 0.0f);
	SetScalePlayingEffekseer3DEffect(playingEffectHandle4, 15.0f, 30.0f, 12.0f);
	//滝５の大きさ・位置・向きの設定
	SetPosPlayingEffekseer3DEffect(playingEffectHandle5, -200.0f,/* DX_PI_F / 2*/10000.0f, 21000.0f);
	SetRotationPlayingEffekseer3DEffect(playingEffectHandle5, 0.0f, DX_PI_F / 2.0f, 0.0f);
	SetScalePlayingEffekseer3DEffect(playingEffectHandle5, 15.0f, 30.0f, 12.0f);
	//滝５の大きさ・位置・向きの設定
	SetPosPlayingEffekseer3DEffect(playingEffectHandle6, 0.0f,/* DX_PI_F / 2*/100.0f, 21000.0f);
	SetRotationPlayingEffekseer3DEffect(playingEffectHandle6, 0.0f, DX_PI_F / 2.0f, 0.0f);
	SetScalePlayingEffekseer3DEffect(playingEffectHandle6, 15.0f, 10.0f, 15.0f);



	//川１の大きさ・位置・向きの設定
	SetPosPlayingEffekseer3DEffect(playingEffectHandle7, 800.0f,/* DX_PI_F / 2*/0.0f, 22000.0f);
	SetRotationPlayingEffekseer3DEffect(playingEffectHandle7, 0.0f, DX_PI_F / 2.0f, 0.0f);
	SetScalePlayingEffekseer3DEffect(playingEffectHandle7, 10.0f, 10.0f, 12.0f);
	//川２の大きさ・位置・向きの設定
	SetPosPlayingEffekseer3DEffect(playingEffectHandle8, -800.0f,/* DX_PI_F / 2*/0.0f, 22000.0f);
	SetRotationPlayingEffekseer3DEffect(playingEffectHandle8, 0.0f, DX_PI_F / 2.0f, 0.0f);
	SetScalePlayingEffekseer3DEffect(playingEffectHandle8, 10.0f, 10.0f, 12.0f);
	//川３の大きさ・位置・向きの設定
	SetPosPlayingEffekseer3DEffect(playingEffectHandle9, -800.0f,/* DX_PI_F / 2*/0.0f, 22000.0f);
	SetRotationPlayingEffekseer3DEffect(playingEffectHandle9, 0.0f, DX_PI_F / 2.0f, 0.0f);
	SetScalePlayingEffekseer3DEffect(playingEffectHandle9, 10.0f, 10.0f, 12.0f);

	//滝４の大きさ・位置・向きの設定
	SetPosPlayingEffekseer3DEffect(playingEffectHandle11, 200.0f,/* DX_PI_F / 2*/0.0f, 21000.0f);
	SetRotationPlayingEffekseer3DEffect(playingEffectHandle11, 0.0f, DX_PI_F / 2.0f, 0.0f);
	SetScalePlayingEffekseer3DEffect(playingEffectHandle11, 5.0f, 10.0f, 12.0f);
	//滝５の大きさ・位置・向きの設定
	SetPosPlayingEffekseer3DEffect(playingEffectHandle12, -200.0f,/* DX_PI_F / 2*/100.0f, 21000.0f);
	SetRotationPlayingEffekseer3DEffect(playingEffectHandle12, 0.0f, DX_PI_F / 2.0f, 0.0f);
	SetScalePlayingEffekseer3DEffect(playingEffectHandle12, 5.0f, 10.0f, 12.0f);
	//滝５の大きさ・位置・向きの設定
	SetPosPlayingEffekseer3DEffect(playingEffectHandle10, 0.0f,/* DX_PI_F / 2*/100.0f, 21000.0f);
	SetRotationPlayingEffekseer3DEffect(playingEffectHandle10, 0.0f, DX_PI_F / 2.0f, 0.0f);
	SetScalePlayingEffekseer3DEffect(playingEffectHandle10, 15.0f, 10.0f, 15.0f);
}

void EffekseerEffect::Update(void)
{
	//DXライブラリのカメラとEffekseerのカメラの同期をする
	Effekseer_Sync3DSetting();

	//Effekseerにより再生中のエフェクトを更新する
	UpdateEffekseer3D();

	//川１のエフェクトが再生し終わったらもう一度再生
	if (IsEffekseer3DEffectPlaying(playingEffectHandle) != 0)
	{
		playingEffectHandle = PlayEffekseer3DEffect(effectResourceHandle);

		//大きさ・位置・向きの再設定
		SetScalePlayingEffekseer3DEffect(playingEffectHandle, 20.0f, 15.0f, 20.0f);
		SetRotationPlayingEffekseer3DEffect(playingEffectHandle, 0.0f, DX_PI_F / 2.0f, 0.0f);
		SetPosPlayingEffekseer3DEffect(playingEffectHandle, 1000.0f,-162.0f, 14000.0f);
	}

	//川２のエフェクトが再生し終わったらもう一度再生
	if (IsEffekseer3DEffectPlaying(playingEffectHandle2) != 0)
	{
		playingEffectHandle2 = PlayEffekseer3DEffect(effectResourceHandle2);

		//大きさ・位置・向きの再設定
		SetScalePlayingEffekseer3DEffect(playingEffectHandle2, 20.0f, 15.0f, 20.0f);
		SetRotationPlayingEffekseer3DEffect(playingEffectHandle2, 0.0f, DX_PI_F / 2.0f, 0.0f);
		SetPosPlayingEffekseer3DEffect(playingEffectHandle2, -1000.0f, -162.0f, 14000.0f);
	}

	//川３のエフェクトが再生し終わったらもう一度再生
	if (IsEffekseer3DEffectPlaying(playingEffectHandle3) != 0)
	{
		playingEffectHandle3 = PlayEffekseer3DEffect(effectResourceHandle3);

		//大きさ・位置・向きの再設定
		SetScalePlayingEffekseer3DEffect(playingEffectHandle3, 20.0f, 15.0f, 20.0f);
		SetRotationPlayingEffekseer3DEffect(playingEffectHandle3, 0.0f, DX_PI_F / 2.0f, 0.0f);
		SetPosPlayingEffekseer3DEffect(playingEffectHandle3, 0.0f, -162.0f, 14000.0f);
	}
	//滝４のエフェクトが再生し終わったらもう一度再生
	if (IsEffekseer3DEffectPlaying(playingEffectHandle4) != 0)
	{
		playingEffectHandle4 = PlayEffekseer3DEffect(effectResourceHandle4);

		//大きさ・位置・向きの再設定
		SetScalePlayingEffekseer3DEffect(playingEffectHandle4, 35.0f, 35.0f, 35.0f);
		SetRotationPlayingEffekseer3DEffect(playingEffectHandle4, 0.0f, DX_PI_F / 2.0f, 0.0f);
		SetPosPlayingEffekseer3DEffect(playingEffectHandle4, 200.0f, 355.0f, 15000.0f);
	}
	//滝５のエフェクトが再生し終わったらもう一度再生
	if (IsEffekseer3DEffectPlaying(playingEffectHandle5) != 0)
	{
		playingEffectHandle5 = PlayEffekseer3DEffect(effectResourceHandle5);

		//大きさ・位置・向きの再設定
		SetScalePlayingEffekseer3DEffect(playingEffectHandle5, 35.0f, 35.0f, 30.0f);
		SetRotationPlayingEffekseer3DEffect(playingEffectHandle5, 0.0f, DX_PI_F / 2.0f, 0.0f);
		SetPosPlayingEffekseer3DEffect(playingEffectHandle5, -200.0f, 355.0f, 15000.0f);
	}
	//滝５のエフェクトが再生し終わったらもう一度再生
	if (IsEffekseer3DEffectPlaying(playingEffectHandle6) != 0)
	{
		playingEffectHandle6 = PlayEffekseer3DEffect(effectResourceHandle6);

		//大きさ・位置・向きの再設定
		SetScalePlayingEffekseer3DEffect(playingEffectHandle6, 15.0f, 25.0f, 30.0f);
		SetRotationPlayingEffekseer3DEffect(playingEffectHandle6, 0.0f, DX_PI_F / 2.0f, 0.0f);
		SetPosPlayingEffekseer3DEffect(playingEffectHandle6, 0.0f, 5.0f, 15000.0f);
	}

	if (IsEffekseer3DEffectPlaying(playingEffectHandle7) != 0)
	{
		spawnTimer++;
		if (spawnTimer < spawnInterval) {
			return;
		}
		playingEffectHandle7 = PlayEffekseer3DEffect(effectResourceHandle7);

		//大きさ・位置・向きの再設定
		SetScalePlayingEffekseer3DEffect(playingEffectHandle7, 25.0f, 25.0f, 20.0f);
		SetRotationPlayingEffekseer3DEffect(playingEffectHandle7, 0.0f, DX_PI_F / 2.0f, 0.0f);
		SetPosPlayingEffekseer3DEffect(playingEffectHandle7, 1000.0f, -165.0f,16000.0f);
	}

	//川２のエフェクトが再生し終わったらもう一度再生
	if (IsEffekseer3DEffectPlaying(playingEffectHandle8) != 0)
	{
		spawnTimer++;
		if (spawnTimer < spawnInterval) {
			return;
		}
		playingEffectHandle8 = PlayEffekseer3DEffect(effectResourceHandle8);

		//大きさ・位置・向きの再設定
		SetScalePlayingEffekseer3DEffect(playingEffectHandle8, 25.0f, 25.0f, 20.0f);
		SetRotationPlayingEffekseer3DEffect(playingEffectHandle8, 0.0f, DX_PI_F / 2.0f, 0.0f);
		SetPosPlayingEffekseer3DEffect(playingEffectHandle8, -1000.0f, -165.0f, 15000.0f);
	}

	//川３のエフェクトが再生し終わったらもう一度再生
	if (IsEffekseer3DEffectPlaying(playingEffectHandle9) != 0)
	{
		spawnTimer++;
		if (spawnTimer < spawnInterval) {
			return;
		}
		playingEffectHandle9 = PlayEffekseer3DEffect(effectResourceHandle9);

		//大きさ・位置・向きの再設定
		SetScalePlayingEffekseer3DEffect(playingEffectHandle9, 25.0f, 25.0f, 20.0f);
		SetRotationPlayingEffekseer3DEffect(playingEffectHandle9, 0.0f, DX_PI_F / 2.0f, 0.0f);
		SetPosPlayingEffekseer3DEffect(playingEffectHandle9, 0.0f, -165.0f, 15000.0f);
	}
	if (IsEffekseer3DEffectPlaying(playingEffectHandle10) != 0)
	{
		spawnTimer++;
		if (spawnTimer < spawnInterval2) {
			return;
		}
		playingEffectHandle10 = PlayEffekseer3DEffect(effectResourceHandle10);

		//大きさ・位置・向きの再設定
		SetScalePlayingEffekseer3DEffect(playingEffectHandle10, 35.0f, 30.0f, 30.0f);
		SetRotationPlayingEffekseer3DEffect(playingEffectHandle10, 0.0f, DX_PI_F / 2.0f, 0.0f);
		SetPosPlayingEffekseer3DEffect(playingEffectHandle10, 0.0f, 5.0f, 15000.0f);
	}

	//滝４のエフェクトが再生し終わったらもう一度再生
	if (IsEffekseer3DEffectPlaying(playingEffectHandle11) != 0)
	{
		spawnTimer++;
		if (spawnTimer < spawnInterval) {
			return;
		}
		playingEffectHandle11 = PlayEffekseer3DEffect(effectResourceHandle11);

		//大きさ・位置・向きの再設定
		SetScalePlayingEffekseer3DEffect(playingEffectHandle11, 35.0f, 35.0f, 35.0f);
		SetRotationPlayingEffekseer3DEffect(playingEffectHandle11, 0.0f, DX_PI_F / 2.0f, 0.0f);
		SetPosPlayingEffekseer3DEffect(playingEffectHandle11, 200.0f, -165.0f, 15000.0f);
	}
	//滝５のエフェクトが再生し終わったらもう一度再生
	if (IsEffekseer3DEffectPlaying(playingEffectHandle12) != 0)
	{
		spawnTimer++;
		if (spawnTimer < spawnInterval) {
			return;
		}
		playingEffectHandle12 = PlayEffekseer3DEffect(effectResourceHandle12);

		//大きさ・位置・向きの再設定
		SetScalePlayingEffekseer3DEffect(playingEffectHandle12, 35.0f, 35.0f, 35.0f);
		SetRotationPlayingEffekseer3DEffect(playingEffectHandle12, 0.0f, DX_PI_F / 2.0f, 0.0f);
		SetPosPlayingEffekseer3DEffect(playingEffectHandle12, -200.0f, -165.0f, 15500.0f);
	}
	//滝５のエフェクトが再生し終わったらもう一度再生

}

void EffekseerEffect::Draw(void)
{
	DrawEffekseer3D();
}

void EffekseerEffect::Release(void)
{
	DeleteEffekseerEffect(effectResourceHandle);
	DeleteEffekseerEffect(effectResourceHandle2);
	DeleteEffekseerEffect(effectResourceHandle3);
	DeleteEffekseerEffect(effectResourceHandle4);
	DeleteEffekseerEffect(effectResourceHandle5);
	DeleteEffekseerEffect(effectResourceHandle6);
	DeleteEffekseerEffect(effectResourceHandle7);
	DeleteEffekseerEffect(effectResourceHandle8);
	DeleteEffekseerEffect(effectResourceHandle9);
	DeleteEffekseerEffect(effectResourceHandle10);
	DeleteEffekseerEffect(effectResourceHandle11);
	DeleteEffekseerEffect(effectResourceHandle12);





	Effkseer_End();
}
