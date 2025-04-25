#include"../Manager/InputManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/Camera.h"
#include "../Application.h"
#include "Dobber.h"
#include"Player.h"
#include"Rod.h"
#include "Fish.h"
#include "FishManager.h"
#include "Gauge.h"
#include "Stage.h"
#include<cmath>
#include<math.h>

namespace
{
    // ウキの移動開始座標
    constexpr VECTOR DOBBER_START_POS = { 0.0f, 110.0f, -200.0f };
}

Dobber* Dobber::instance_ = nullptr;

void Dobber::CreateInstance()
{
    if (instance_ == nullptr)
    {
        instance_ = new Dobber();
    }
    instance_->Init();
}

Dobber& Dobber::GetInstance(void)
{
    return *instance_;
}


Dobber::Dobber()
{
    basicMoveVec = { 0.0f,0.0f,0.0f }, gravity_ = { 0.0f,0.0f,0.0f }, isShot = false, rotMoveVec = { 0.0f,0.0f,0.0f }, rot_ = { 0.0f,0.0f,0.0f }, transformMoveVec = { 0.0f,0.0f,0.0f },
        ukimodelId_ = -1, ukipos_ = { 0.0f,0.0f,0.0f }, velocity_ = { 0.0f,0.0f,0.0f }, landingflg = false;
}

Dobber::~Dobber(void)
{
}

void Dobber::Init(void)
{

	// 弾のモデル
	ukimodelId_ = MV1LoadModel("Data/Model/Stage/uki.mv1");

	////////////////////////////////////////
	// ☆奥向きのベクトルが基本の移動方向として設定する
	basicMoveVec = { 0.0f, 0.0f, 100.0f };

	////////////////////////////////////////
	// ☆回転後のベクトル初期化
	rotMoveVec = { 0.0f, 0.0f, 100.0f };

	////////////////////////////////////////

	// 移動ベクトル初期化
	transformMoveVec = { 0.0f, 0.0f, 0.0f };

	// 座標初期化
    ukipos_ = { 0.0f,0.0f,0.0f };

	// 回転値初期化
	rot_ = { 0.0f, 0.0f, 0.0f };

	// 弾の使用フラグを折る
	isShot = false;

    fishingflg = false;

    gravity_ = { 0.0f,-0.02f,0.0f };
    velocity_ = { 0.0f,2.0f,0.0f };

    DOBBER_MOVE_SPEED = 8.0f;

    MV1SetupCollInfo(ukimodelId_);
}

void Dobber::Update(void)
{
    Rod& rod = Rod::GetInstance();
    Player& player = Player::GetInstance();
    FishManager& fishMng = FishManager::GetInstance();

    //竿が出ていなかったらやめる
    if (!rod.GetFlg()) {
        return;
    }

    if (rod.GetFlg())
    {
        // ukiが発射されているときの処理
        if (isShot)
        {
            // X軸回転の行列計算
            MATRIX matSphereRotX = MGetRotX(rot_.x);
            rotMoveVec = VTransform(basicMoveVec, matSphereRotX);

            // Y軸回転の行列計算
            MATRIX matSphereRotY = MGetRotY(rod.GetRotY());
            rotMoveVec = VTransform(rotMoveVec, matSphereRotY);

            // 移動ベクトルをスピードの大きさに変更する
            transformMoveVec = VNorm(rotMoveVec);
            transformMoveVec = VScale(transformMoveVec, DOBBER_MOVE_SPEED);

            //重力
            velocity_ = VAdd(velocity_, gravity_);

            // 座標を更新
            ukipos_ = VAdd(ukipos_, VAdd(transformMoveVec, velocity_));

            // 地面との衝突判定
            if (ukipos_.y <= -165.0f) // 地面のy座標が0と仮定
            {
                ukipos_.y = -165.0f; // 地面に接触したら位置を固定
                DOBBER_MOVE_SPEED = 0;
                landingflg = true;
            }

            Stage& stage = Stage::GetInstance();

            //ステージとウキが当たったらウキを出すところからやり直し
            float ukiposy = Dobber::GetInstance().GetUkiPos().y;
            auto stagetoukiCollision = MV1CollCheck_Sphere(stage.GetModelId(), -1, ukipos_, 1);

            if (ukiposy > -165.0f && stagetoukiCollision.HitNum >= 1)
            {
                Reset();
            }
            MV1SetPosition(ukimodelId_, ukipos_);  // モデルの位置を更新
            MV1CollResultPolyDimTerminate(stagetoukiCollision);
        }
        else  // ukiが発射されていないとき
        {
            // 左クリックで発射
            if (InputManager::GetInstance().IsTrgMouseLeft())
            {
                ukipos_ = player.GetPos();
                velocity_ = { 0.0f, 2.0f, 0.0f };
                isShot = true;
                fishMng.SetClosestFishInterval(0.0f);
                fishMng.SetCountFlg(false);

                MV1SetPosition(ukimodelId_, ukipos_);  // モデルの初期位置を設定
            }
        }
    }
}

void Dobber::Draw()
{
    Rod& rod = Rod::GetInstance();

    if (!rod.GetFlg()) {
        return;
    }

    // 弾が発射されているときだけ描画する
    if (isShot == true)
    {
        MV1DrawModel(ukimodelId_);
    }
}

void Dobber::Release(void)
{
}

bool Dobber::GetLandingFlg(void) const
{
    return landingflg;
}

void Dobber::SetUkiPos(const VECTOR newPos)
{
    ukipos_ = newPos;
    MV1SetPosition(ukimodelId_, ukipos_);
}

void Dobber::Reset(void)
{
    isShot = false;
    fishingflg = false;
    DOBBER_MOVE_SPEED = 8.0f;
    velocity_ = { 0.0f,2.0f,0.0f };
    gravity_ = { 0.0f,-0.02f,0.0f };
    ukipos_ = Player::GetInstance().GetPos();
    MV1SetPosition(ukimodelId_, ukipos_);
}
