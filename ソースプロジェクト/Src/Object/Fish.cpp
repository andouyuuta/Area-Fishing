#include "Fish.h"
#include "FishManager.h"
#include "Player.h"
#include "Dobber.h"
#include "Gauge.h"
#include "../Manager/InputManager.h"
#include "../Manager//SceneManager.h"
#include "../Common/Random.h"
#include "../Application.h"

namespace
{
    // アニメーション・回転
    constexpr float ANIM_SPEED = 0.2f;                        // アニメーション速度
    constexpr float ROTATE_SPEED = 0.05f;                     // 魚の回転速度
    constexpr float ROTATION_SPEED = 0.1f;                    // 向き変更の補間速度
    constexpr float ANGLE_CORRECTION_RAD = DX_PI_F / 2.0f;    // ラジアン補正
    constexpr float ROTATE_THRESHOLD = 0.01f;                 // 回転補正の閾値
    constexpr float FULL_ROTATION_DEG = 2.0f * DX_PI_F;       // 360度（ラジアン）
    constexpr float HALF_ROTATION_DEG = 180.0f;               // 180度（デグリー）

    // タイミング関連
    constexpr float MAX_INTERVAL = 3.0f * 60;                 // 最大出現間隔
    constexpr float DEFAULT_INTERVAL = 60.0f;                 // デフォルトの行動間隔
    constexpr float TIMER_INCREMENT = 1.0f;                   // タイマー加算量
    constexpr float CHANGE_DIRECTION_INTERVAL_MIN = 50.0f;    // 向き変更の最小間隔
    constexpr float CHANGE_DIRECTION_INTERVAL_MAX = 200.0f;   // 向き変更の最大間隔

    // 魚の挙動
    constexpr float FISH_MOVE_SPEED = 2.0f;                   // 基本移動速度
    constexpr float SWIM_SPEED_MIN = 1.5f;                    // 最小遊泳速度
    constexpr float SWIM_SPEED_MAX = 3.0f;                    // 最大遊泳速度
    constexpr float CLOSER = 7.0f;                            // プレイヤーに近づくときの速度
    constexpr float LEAVE = 2.0f;                             // プレイヤーから離れる速度

    // 魚のスケール・初期位置
    constexpr VECTOR DEFAULT_FISH_SCALE = { 0.25f, 0.25f, 0.25f }; // 魚のスケール
    constexpr float GROUND_Y = -165.0f;                            // 地面Y座標
    
    // 初期方向ベクトル範囲
    constexpr float DIRECTION_XZ_MIN = -100.0f;
    constexpr float DIRECTION_XZ_MAX = 100.0f;
    constexpr float DIRECTION_Y_MIN = -0.5f;
    constexpr float DIRECTION_Y_MAX = 0.5f;
   
    // 魚の出現範囲
    constexpr float SPAWN_RANGE_MIN_X = -5500.0f;
    constexpr float SPAWN_RANGE_MAX_X = 5500.0f;
    constexpr float SPAWN_RANGE_MIN_Y = -350.0f;
    constexpr float SPAWN_RANGE_MAX_Y = -350.0f;
    constexpr float SPAWN_RANGE_MIN_Z = -8000.0f;
    constexpr float SPAWN_RANGE_MAX_Z = 13000.0f;
}

Fish::Fish(void)
    :fishPos_{ 0.0f, 0.0f, 0.0f },
    prevFishPos_{ 0.0f, 0.0f, 0.0f },
    fishModel_(-1),
    fishInterval_(0.0f),
    isIntervalCount(false),
    timeToMove(DEFAULT_INTERVAL),
    isLeftClickPress(false),
    fishScale_(DEFAULT_FISH_SCALE),
    angle_(0.0f),
    direction_(1),
    fishNumber_(0),
    animAttachNo_(0),
    animTotalTime_(0.0f),
    currentAnimTime_(0.0f),
    swimTimer_(0.0f),
    currentDirection_{ 0.0f, 0.0f, 0.0f },
    changeDirectionInterval_(0.0f),
    type_(FishType::None),
    currentAngleY_(0.0f)
{
}

Fish::~Fish(void)
{
}

void Fish::Init(Player* player, Dobber* dobber, FishManager* fishmng)
{
    player_ = player;
    dobber_ = dobber;
    fishmng_ = fishmng;
}

// 更新処理
void Fish::Update(void)
{
    FishAnimation();
    FishFreedomMove();
}

// 描画処理
void Fish::Draw(void)
{
    MV1DrawModel(fishModel_);
}

// 解放処理
void Fish::Release(void)
{
    MV1DeleteModel(fishModel_);
}

// 魚全体の処理
void Fish::Move(VECTOR pos, const int fishidx)
{
    // プレイヤーと魚の距離
    VECTOR direction_ = VNorm(VSub(player_->GetPos(), fishPos_));

    direction_ = VScale(direction_, -1.0f);

    UpdateRotationToDirection(direction_);

    // 左クリック押したとき
    if (InputManager::GetInstance().IsClickMouseLeft())
    {
        // プレイヤーに近づける
        fishPos_ = VAdd(fishPos_, VScale(VNorm(VSub(player_->GetPos(), fishPos_)), CLOSER)); 
        pos = fishPos_;
        pos.y = GROUND_Y;
    }
    else 
    {
        // プレイヤーから遠ざける
        fishPos_ = VAdd(fishPos_, VScale(VNorm(VSub(fishPos_, player_->GetPos())), LEAVE)); 
        pos = fishPos_;
        pos.y = GROUND_Y;
    }

    MoveLR();

    MV1SetPosition(fishModel_, fishPos_);
    dobber_->SetUkiPos(pos);
}

// 魚を左右のどちらかに移動させるかを決める
void Fish::MoveLR(void)
{
    // 一定の時間でランダムに決める
    timeToMove--;
    if (timeToMove <= 0)
    {
        direction_ = Random::GetInstance().GetRandF(0, 1) > 0.5f ? 1 : -1;;
        timeToMove = 60;
    }
    angle_ += 0.05f * direction_;
    fishPos_.x += sinf(angle_ * 2.0f);
}

// 向きベクトルに合わせて回転させる
void Fish::UpdateRotationToDirection(const VECTOR& direction)
{
    // 移動していない（ほぼ向きがない）なら回転しない
    if (VSize(direction) <= ROTATE_THRESHOLD) return;

    // 向きベクトルを正規化（単位ベクトルにする）
    VECTOR normDir = VNorm(direction);

    // 魚が向くべき角度（Y軸回転）を求める
    float targetAngleY = atan2f(normDir.x, normDir.z); // XとZを使って角度算出

    // 現在の角度との差（回転量）を求める
    float deltaAngle = targetAngleY - currentAngleY_;

    // 差が±180度を超えた場合は、逆回転に修正（最短回転）
    if (deltaAngle > DX_PI_F)       deltaAngle -= FULL_ROTATION_DEG;
    else if (deltaAngle < -DX_PI_F) deltaAngle += FULL_ROTATION_DEG;

    // 回転を少しずつ（ROTATE_SPEED分だけ）近づける
    currentAngleY_ += deltaAngle * ROTATE_SPEED;

    // 実際にモデルを回転させる（補正角も加える）
    MV1SetRotationXYZ(fishModel_, VGet(0.0f, currentAngleY_ + ANGLE_CORRECTION_RAD, 0.0f));
}

// 魚がウキにくっついている時間のカウント
void Fish::FishIntervalCount(void)
{
    fishInterval_++;
    isIntervalCount = true;
}

// 制限時間以内に左クリックを押したら釣りを開始する
void Fish::FishingInterval(void)
{
    if (fishInterval_ < MAX_INTERVAL && InputManager::GetInstance().IsTrgMouseLeft()) 
    {
        isLeftClickPress = true;
        isIntervalCount = false;
    }
    else 
    {
        // 時間制限以内に左クリック押せなかったらリセット
        if (!isLeftClickPress) 
        {
            FishIntervalCount();
            if (fishInterval_ >= MAX_INTERVAL) 
            {
                dobber_->Reset();
                Reset();
            }
        }
    }
}

// 魚がウキによって来る処理
void Fish::FishCome(int& fishidx)
{
    VECTOR ukiPos = dobber_->GetUkiPos();

    // ukiに向かう方向の正規化ベクトル
    VECTOR direction_ = VNorm(VSub(ukiPos, fishPos_));

    // 速度定数を使って位置更新（x,z）
    fishPos_.x += direction_.x * FISH_MOVE_SPEED;
    fishPos_.z += direction_.z * FISH_MOVE_SPEED;

    UpdateRotationToDirection(direction_);

    MV1SetPosition(fishModel_, fishPos_);
}

void Fish::FishAnimation(void)
{
    // アニメーション設定
    currentAnimTime_ += ANIM_SPEED;

    if (currentAnimTime_ > animTotalTime_)
    {
        currentAnimTime_ = 0.0f;
    }
    MV1SetAttachAnimTime(fishModel_, animAttachNo_, currentAnimTime_);
    MV1SetPosition(fishModel_, fishPos_);
}

// 魚が自由に動く
void Fish::FishFreedomMove(void)
{
    if (!(dobber_->GetFishingFlg()))
    {
        // 初期化(最初だけ)
        if (swimTimer_ == 0.0f && changeDirectionInterval_ == 0.0f)
        {
            currentDirection_ = VNorm(VGet(
                Random::GetInstance().GetRandF(DIRECTION_XZ_MIN, DIRECTION_XZ_MAX),
                Random::GetInstance().GetRandF(DIRECTION_Y_MIN, DIRECTION_Y_MAX),
                Random::GetInstance().GetRandF(DIRECTION_XZ_MIN, DIRECTION_XZ_MAX))
            );
            changeDirectionInterval_ = Random::GetInstance().GetRandF(CHANGE_DIRECTION_INTERVAL_MIN, CHANGE_DIRECTION_INTERVAL_MAX);
        }

        // タイマー増加
        swimTimer_ += TIMER_INCREMENT;

        // 方向変更判定
        if (swimTimer_ >= changeDirectionInterval_)
        {
            currentDirection_ = VNorm(VGet(
                Random::GetInstance().GetRandF(DIRECTION_XZ_MIN, DIRECTION_XZ_MAX),
                Random::GetInstance().GetRandF(DIRECTION_Y_MIN, DIRECTION_Y_MAX),
                Random::GetInstance().GetRandF(DIRECTION_XZ_MIN, DIRECTION_XZ_MAX))
            );

            changeDirectionInterval_ = Random::GetInstance().GetRandF(CHANGE_DIRECTION_INTERVAL_MIN, CHANGE_DIRECTION_INTERVAL_MAX);
            swimTimer_ = 0.0f;
        }
        float swimSpeed = Random::GetInstance().GetRandF(SWIM_SPEED_MIN, SWIM_SPEED_MAX);

        // 魚の移動
        prevFishPos_ = fishPos_;
        fishPos_ = VAdd(fishPos_, VScale(currentDirection_, swimSpeed));

        FishFreedomRotate();

        MV1SetPosition(fishModel_, fishPos_);
    }
}

// 自由に動いているときの魚の回転
void Fish::FishFreedomRotate(void) const
{
    // 現在と一フレーム前の座標を比べる
    VECTOR direction_ = VSub(fishPos_, prevFishPos_);
    
    const_cast<Fish*>(this)->UpdateRotationToDirection(direction_);
}

// 魚の切り替え
void Fish::ChangeFish(FishType newtype)
{
    if (fishModel_ != -1)
    {
        MV1DeleteModel(fishModel_);
    }

    type_ = newtype;

    static const std::map<FishType, int> fishTypeToNumber = 
    {
        { FishType::Ayu, 1 },
        { FishType::Yamame, 2 },
        { FishType::Haya, 3 },
        { FishType::Huna, 4 },
        { FishType::Oikawa, 5 },
        { FishType::Medaka, 6 },
        { FishType::Kawamutu, 7 },
        { FishType::Iwana, 8 },
        { FishType::Poriputerusu, 9 },
        { FishType::Nizimasu, 10 },
        { FishType::Ugui, 11 },
        { FishType::Wakasagi, 12 },
        { FishType::Uthtenoputeron, 13 }
    };

    int baseModel = fishmng_->GetFishModel(type_);

    if (baseModel != -1)
    {
        fishModel_ = MV1DuplicateModel(baseModel);

        fishPos_ = 
        {
            Random::GetInstance().GetRandY(SPAWN_RANGE_MIN_X, SPAWN_RANGE_MAX_X),
            Random::GetInstance().GetRandY(SPAWN_RANGE_MIN_Y, SPAWN_RANGE_MAX_Y),
            Random::GetInstance().GetRandY(SPAWN_RANGE_MIN_Z, SPAWN_RANGE_MAX_Z)
        };

        MV1SetPosition(fishModel_, fishPos_);
        MV1SetScale(fishModel_, fishScale_);

        animAttachNo_ = MV1AttachAnim(fishModel_, 0, -1, -1);
        animTotalTime_ = MV1GetAttachAnimTime(fishModel_, animAttachNo_);
        currentAnimTime_ = 0.0f;
        MV1SetAttachAnimTime(fishModel_, animAttachNo_, currentAnimTime_);
    }

    // fishNumber_ の設定（見つからなければ 0）
    auto it = fishTypeToNumber.find(type_);
    fishNumber_ = (it != fishTypeToNumber.end()) ? it->second : 0;
}

// リセット
void Fish::Reset(void)
{
    isIntervalCount = false;
    fishInterval_ = 0.0f;
    MV1DeleteModel(fishModel_);
    isLeftClickPress = false;
}
