#pragma once
#include <DxLib.h>
#include <random>
#include "Fish.h"
#include "Player.h"

class FishManager
{
public:
	static constexpr int MAX_FISH = 25;

	// インスタンスの生成
	static void CreateInstance(void);
	// インスタンスの取得
	static FishManager& GetInstance(void);

	FishManager();
	~FishManager();
	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);

	void Probability();		//確率
	void FishMove(VECTOR pos);
	void Reset(void);
	void FishAllDelete(void);

	[[nodiscard]] int GetclosestFishIndex(void) const { return closestFishindex; }	//一番近い魚の番号(配列)
	[[nodiscard]] int GetClosestFishModel(void)const;	//一番近い魚のモデル
	[[nodiscard]] int GetClosestFishNumber(void);		//一番近い魚の番号(FishTypeの番号)
	[[nodiscard]] bool GetClosestIntervalFlg(void)const;
	[[nodiscard]] bool GetFishHitFlg(void)const { return fishhitflg; }
	void SetClosestFishInterval(float interval);
	void SetCountFlg(bool flg);

	int GetAyuModel(void)const { return ayumodel; }
	int GetYamameModel(void)const { return yamamemodel; }
	int GetHayaModel(void)const { return hayamodel; }
	int GetHunaModel(void)const { return hunamodel; }
	int GetOikawaModel(void)const { return oikawamodel; }
	int GetKawamutuModel(void)const { return kawamutumodel; }
	int GetIwanaModel(void)const { return iwanamodel; }
	int GetPoriputeModel(void)const { return poriputemodel; }
	int GetNizimasuModel(void)const { return nizimasumodel; }
	int GetWakasagiModel(void)const { return wakasagimodel; }
	int GetUguiModel(void)const { return uguimodel; }
	int GetMedakaModel(void)const { return medakamodel; }
	int GetUthModel(void)const { return uthmodel; }

private:
	// 静的インスタンス
	static FishManager* instance_;

	std::vector<Fish> fishlist;

	//魚関連の情報
	int ayumodel;			//アユ
	int yamamemodel;		//ヤマメ
	int hayamodel;			//ハヤ
	int hunamodel;			//フナ
	int oikawamodel;		//オイカワ
	int kawamutumodel;		//カワムツ
	int iwanamodel;			//イワナ
	int poriputemodel;		//ポリプテルス
	int nizimasumodel;		//ニジマス
	int wakasagimodel;		//ワカサギ
	int uguimodel;			//ウグイ
	int medakamodel;		//メダカ
	int uthmodel;			//ユーステノプテロン

	bool fishhitflg;		//当たっているか

	int randValue;			//ランダムの値(魚のモデルに必要)
	int closestFishindex;	//最も近い魚

	float Clamp(float value, float min, float max) {
		if (value < min) return min;
		if (value > max) return max;
		return value;
	}
};

