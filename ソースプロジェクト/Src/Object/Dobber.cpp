#include "../Manager/InputManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/Camera.h"
#include "../Application.h"
#include "Dobber.h"
#include "Player.h"
#include "Rod.h"
#include "Fish.h"
#include "FishManager.h"
#include "Gauge.h"
#include "Stage.h"
#include <cmath>
#include <math.h>

namespace
{
    // モデル・パス関連
    constexpr const char* UKI_MODEL_PATH = "Data/Model/Stage/uki.mv1";  // ウキのモデルパス

    // 初期状態・位置
    constexpr VECTOR DOBBER_START_POS = { 0.0f, 110.0f, -200.0f };      // ウキの初期位置
    constexpr VECTOR DEFAULT_MOVE_VEC = { 0.0f, 0.0f, 100.0f };         // 初期の移動ベクトル
    constexpr VECTOR DEFAULT_VELOCITY = { 0.0f, 2.0f, 0.0f };           // 初速ベクトル
    constexpr VECTOR DEFAULT_GRAVITY = { 0.0f, -0.02f, 0.0f };          // 重力ベクトル

    // 移動・当たり判定
    constexpr float DEFAULT_MOVE_SPEED = 8.0f;                          // 移動速度
    constexpr float COLLISION_RADIUS = 1.0f;                            // 当たり判定の半径
    constexpr float GROUND_Y = -165.0f;                                 // 地面のY座標
}

Dobber::Dobber(void)
    :ukiModelId_(-1),
    moveSpeed_(DEFAULT_MOVE_SPEED),
    basicMoveVec_(DEFAULT_MOVE_VEC),
    gravity_(DEFAULT_GRAVITY),
    rot_{ 0.0f, 0.0f, 0.0f },
    scale_{ 1.0f, 1.0f, 1.0f },
    rotMoveVec_(DEFAULT_MOVE_VEC),
    transformMoveVec_{ 0.0f, 0.0f, 0.0f },
    ukiPos_{ 0.0f, 0.0f, 0.0f },
    velocity_(DEFAULT_VELOCITY),
    isLanding(false),
    isShot_(false),
    isFishing(false)
{
}

Dobber::~Dobber(void)
{
}

void Dobber::Init(Player* player, Rod* rod, FishManager* fishmng, Stage* stage)
{
    player_ = player;
    rod_ = rod;
    fishmng_ = fishmng;
    stage_ = stage;

	// ウキの設定
    ukiModelId_ = MV1LoadModel(UKI_MODEL_PATH);
    MV1SetPosition(ukiModelId_, ukiPos_);
    MV1SetPosition(ukiModelId_, scale_);

    // 当たり判定セットアップ
    MV1SetupCollInfo(ukiModelId_);
}

void Dobber::Update(void)
{

    // 竿が出ていなかったらやめる
    if (!rod_->GetFlg()) 
    {
        return;
    }

    if (rod_->GetFlg())
    {
        // ukiが発射されているときの処理
        if (isShot_)
        {
            // X軸回転の行列計算
            MATRIX matSphereRotX = MGetRotX(rot_.x);
            rotMoveVec_ = VTransform(basicMoveVec_, matSphereRotX);

            // Y軸回転の行列計算
            MATRIX matSphereRotY = MGetRotY(rod_->GetAngle().y);
            rotMoveVec_ = VTransform(rotMoveVec_, matSphereRotY);

            // 移動ベクトルをスピードの大きさに変更する
            transformMoveVec_ = VNorm(rotMoveVec_);
            transformMoveVec_ = VScale(transformMoveVec_, moveSpeed_);

            // 重力
            velocity_ = VAdd(velocity_, gravity_);

            // 座標を更新
            ukiPos_ = VAdd(ukiPos_, VAdd(transformMoveVec_, velocity_));

            // 地面との衝突判定
            if (ukiPos_.y <= GROUND_Y)   // 地面のy座標が0と仮定
            {
                ukiPos_.y = GROUND_Y;    // 地面に接触したら位置を固定
                moveSpeed_ = 0;
                isLanding = true;
            }

            // ステージとウキが当たったらウキを出すところからやり直し
            auto stagetoukiCollision = MV1CollCheck_Sphere(stage_->GetModelId(), -1, ukiPos_, COLLISION_RADIUS);

            if (ukiPos_.y > GROUND_Y && stagetoukiCollision.HitNum >= 1)
            {
                Reset();
            }
            MV1SetPosition(ukiModelId_, ukiPos_);  // モデルの位置を更新
            MV1CollResultPolyDimTerminate(stagetoukiCollision);
        }
        else  // ukiが発射されていないとき
        {
            // 左クリックで発射
            if (InputManager::GetInstance().IsTrgMouseLeft())
            {
                ukiPos_ = player_->GetPos();
                velocity_ = DEFAULT_VELOCITY;
                isShot_ = true;
                fishmng_->SetClosestFishInterval(0.0f);
                fishmng_->SetCountFlg(false);

                MV1SetPosition(ukiModelId_, ukiPos_);  // モデルの初期位置を設定
            }
        }
    }
}

// 描画処理
void Dobber::Draw(void)
{
    // 竿が出ていなかったらやめる
    if (!rod_->GetFlg()) 
    {
        return;
    }

    // 弾が発射されているときだけ描画する
    if (isShot_ == true)
    {
        MV1DrawModel(ukiModelId_);
    }
}

// 解放処理
void Dobber::Release(void)
{
    MV1DeleteModel(ukiModelId_);
}

// 地面についているか
bool Dobber::GetLandingFlg(void) const
{
    return isLanding;
}

// ウキの座標の設定
void Dobber::SetUkiPos(const VECTOR newPos)
{
    ukiPos_ = newPos;
    MV1SetPosition(ukiModelId_, ukiPos_);
}

// リセット
void Dobber::Reset(void)
{
    isShot_ = false;
    isFishing = false;
    moveSpeed_= DEFAULT_MOVE_SPEED;
    velocity_ = DEFAULT_VELOCITY;
    gravity_ = DEFAULT_GRAVITY;
    ukiPos_ = player_->GetPos();
    MV1SetPosition(ukiModelId_, ukiPos_);
}