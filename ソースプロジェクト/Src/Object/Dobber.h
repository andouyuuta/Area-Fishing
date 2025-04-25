#pragma once
#include <DxLib.h>
#include<random>

class Player;
class Rod;
class Fish;

class Dobber
{
public:
    //ウキの距離
    static constexpr float UKIDISTANCE = 3000.0f;
    // インスタンスの生成
    static void CreateInstance(void);
    // インスタンスの取得
    static Dobber& GetInstance(void);
    // コンストラクタ(元となるモデルのハンドルID)
    Dobber();
    // デストラクタ
    ~Dobber(void);
    // 初期化
    void Init(void);
    // 更新ステップ
    void Update(void);
    // 描画
    void Draw(void);
    // 解放処理
    void Release(void);
    void Reset(void);           //リセット
    [[nodiscard]] VECTOR GetAngle(void) const { return rot_; }	//方向の取得
    [[nodiscard]] int GetUkiModel(void)const { return ukimodelId_; }
    [[nodiscard]] bool GetfishingFlg(void) const { return fishingflg; }
    [[nodiscard]] VECTOR GetUkiPos(void) const { return ukipos_; }
    [[nodiscard]] bool GetIsShot(void) const { return isShot; }
    [[nodiscard]] bool GetLandingFlg(void)const;

    void SetUkiPos(const VECTOR newPos);
    void SetFishingFlag(const bool flg) { fishingflg = flg; }
    void SetIsShotFlag(const bool flg) { isShot = flg; }
    void SetMoveSpeed(const float speed) { DOBBER_MOVE_SPEED = speed; }
    void SetVelocity(const VECTOR Velocity) { velocity_ = Velocity; }
    void SetLandingFlg(const bool flg) { landingflg = flg; }

private:
    // 静的インスタンス
    static Dobber* instance_;
    // モデルID
    int ukimodelId_;            //ウキ
    bool isShot;                // ウキの使用フラグ
    bool fishingflg = false;    //釣れているか
    bool landingflg;
    // 弾の発射スピード
    float DOBBER_MOVE_SPEED = 8.0f;
    // 座標
    VECTOR ukipos_;
    // ウキの角度
    VECTOR rot_;
    // 何も値を変えてない移動ベクトル
    VECTOR basicMoveVec;
    // 回転した移動ベクトル
    VECTOR rotMoveVec;
    // 行列回転した移動ベクトル
    VECTOR transformMoveVec;
    //重力
    VECTOR gravity_;
    //速度
    VECTOR velocity_;
};