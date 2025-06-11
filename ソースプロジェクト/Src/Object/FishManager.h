#pragma once
#include <DxLib.h>
#include <random>
#include <map>
#include <string>
#include "Fish.h"

class Player;
class Gauge;

class FishManager
{
public:
	static constexpr int MAX_FISH = 25;

	using FishType = Fish::FishType;

	std::map<Fish::FishType, std::string> fishModelPaths_;
	std::map<Fish::FishType, int> fishModels_;

	FishManager(void);			// コンストラクタ
	~FishManager(void);			// デストラクタ
	void Init(Player* player, Dobber* dobber, Gauge* gauge);		// 初期化処理
	void Update(void);			// 更新処理
	void Draw(void);			// 描画処理
	void Release(void);			// 解放処理

	void Probability(void);			// 確率で魚を生成
	void TouchDownMove(VECTOR pos);	// ウキが着水した時の魚の処理
	void Reset(void);				// リセット
	void FishAllDelete(void);		// 魚全消し

	[[nodiscard]] int GetFishModel(Fish::FishType type) const;
	[[nodiscard]] int GetClosestFishModel(void)const { return fishList[closestFishIndex_].GetFishModel(); }
	[[nodiscard]] int GetClosestFishNumber(void) { return fishList[closestFishIndex_].GetFishNumber(); }
	[[nodiscard]] bool GetFishHitFlg(void)const { return isFishHit_; }
	void SetClosestFishInterval(float interval) { for (Fish& fish : fishList)fish.SetInterval(interval); }
	void SetCountFlg(bool flg) { for (Fish& fish : fishList)fish.SetCountFlg(flg); }

	void ClosestFishAnimation(void);
private:

	std::vector<Fish> fishList;

	bool isFishHit_;		// 当たっているか
	int closestFishIndex_;	// 最も近い魚

	Player* player_;
	Dobber* dobber_;
	Gauge* gauge_;
};

