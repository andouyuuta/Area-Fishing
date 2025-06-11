#pragma once
#include <DxLib.h>
#include <random>

class Player;
class Rod;
class FishManager;
class Stage;

class Dobber
{
public:
    static constexpr float UKIDISTANCE = 3000.0f;

    // コンストラクタ
    Dobber(void);
    // デストラクタ
    ~Dobber(void);
    void Init(Player* player, Rod* rod, FishManager* fishmng, Stage* stage);		// 初期化処理
    void Update(void);		// 更新処理
    void Draw(void);		// 描画処理
    void Release(void);		// 解放処理
    void Reset(void);       // リセット
    [[nodiscard]] int GetUkiModel(void)const { return ukiModelId_; }        // モデルの取得
    [[nodiscard]] bool GetFishingFlg(void) const { return isFishing; }      // 釣れているか
    [[nodiscard]] VECTOR GetUkiPos(void) const { return ukiPos_; }          // 座標の取得
    [[nodiscard]] bool GetIsShot(void) const { return isShot_; }            // ショットしたか
    [[nodiscard]] bool GetLandingFlg(void)const;                            // 地面についたか
    [[nodiscard]] float GetUkiDistance(void)const { return UKIDISTANCE; }

    void SetUkiPos(const VECTOR newPos);
    void SetFishingFlg(const bool flg) { isFishing = flg; }
    void SetLandingFlg(const bool flg) { isLanding = flg; }

private:

    int ukiModelId_;            // ウキモデル
    bool isShot_;               // ウキを使用しているか
    bool isFishing;             // 釣れているか
    bool isLanding;             // 着水しているか
    float moveSpeed_;           // 弾の発射スピード

    VECTOR ukiPos_;             // 座標
    VECTOR rot_;                // ウキの角度
    VECTOR scale_;              // ウキの大きさ
    VECTOR basicMoveVec_;       // 何も値を変えてない移動ベクトル
    VECTOR rotMoveVec_;         // 回転した移動ベクトル
    VECTOR transformMoveVec_;   // 行列回転した移動ベクトル
    VECTOR gravity_;            // 重力
    VECTOR velocity_;           // 速度

    Player* player_;
    Stage* stage_;
    Rod* rod_;
    FishManager* fishmng_;
};