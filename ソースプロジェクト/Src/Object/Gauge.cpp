#include "../Manager/InputManager.h"
#include "Gauge.h"
#include "Player.h"
#include "Dobber.h"
#include "FishManager.h"
#include "Rod.h"

namespace
{
    // ゲージの挙動
    constexpr float GAUGE_UP_SPEED = 0.8f;          // ゲージ上昇速度
    constexpr float GAUGE_DOWN_SPEED = 0.4f;        // ゲージ下降速度
    constexpr float MAX_GAUGE_INTERVAL = 60.0f;     // 最大インターバル（1秒＝60フレーム）

    // ゲージ値の初期設定
    constexpr float DEFAULT_GAUGE_VALUE = 50.0f;
    constexpr float DEFAULT_MAX_VALUE = 100.0f;
    constexpr float DEFAULT_MIN_VALUE = 0.0f;

    // ゲージの表示位置
    constexpr float DEFAULT_GAUGE_X = 100.0f;
    constexpr float DEFAULT_GAUGE_Y = 100.0f;

    // 魚が食いついた時のテキストの表示位置
    constexpr int GAUGE_TEXT_X = 150;
    constexpr int GAUGE_TEXT_Y = 100;

    // ゲージの色・大きさ
    constexpr int MAX_COLOR = 255;
    constexpr int GAUGE_WIDTH = 40;
    constexpr int GAUGE_HEIGHT = 500;

    // テキスト・パス関連
    constexpr const char* GAUGE_HINT_MESSAGE = "左クリックでゲージを調節！";
    constexpr const char* GAUGE_IMAGE_PATH = "Data/Image/Gauge_bg.png";
}

Gauge::Gauge(void)
    : gaugeHandle_(-1),
    gaugeInterval_(0),
    isDraw_(false),
    gaugeValue_(DEFAULT_GAUGE_VALUE)
{
}

Gauge::~Gauge(void)
{
}

// 初期化処理
void Gauge::Init(Player* player, Dobber* dobber, Rod* rod, FishManager* fishmng)
{
    player_ = player;
    dobber_ = dobber;
    rod_ = rod;
    fishmng_ = fishmng;
    gaugeHandle_ = LoadGraph(GAUGE_IMAGE_PATH);
}

// 更新処理
void Gauge::Update(void)
{
}

// 描画処理
void Gauge::Draw(void)
{
    // 魚が食いついたときに表示
    if (isDraw_) 
    {
        DrawGauge(DEFAULT_GAUGE_X, DEFAULT_GAUGE_Y, GAUGE_WIDTH, GAUGE_HEIGHT, gaugeValue_);
        DrawString(GAUGE_TEXT_X, GAUGE_TEXT_Y, GAUGE_HINT_MESSAGE, 0xffffff);
    }
}
// 解放処理
void Gauge::Release(void)
{
    DeleteGraph(gaugeHandle_);
}

// ゲージの更新
void Gauge::UpdateGauge(void)
{
    // ゲージが上限・下限に達しているか
    bool isGaugeAtLimit = (gaugeValue_ == DEFAULT_MIN_VALUE || gaugeValue_ == DEFAULT_MAX_VALUE);

    // 左クリックでゲージの増減
    if (InputManager::GetInstance().IsClickMouseLeft())
    {
        // 値を増やす
        gaugeValue_ += GAUGE_UP_SPEED;
        if (gaugeValue_ > DEFAULT_MAX_VALUE) gaugeValue_ = DEFAULT_MAX_VALUE;
    }
    else 
    {
        // 値を減らす
        gaugeValue_ -= GAUGE_DOWN_SPEED;
        if (gaugeValue_ < DEFAULT_MIN_VALUE) gaugeValue_ = DEFAULT_MIN_VALUE;
    }

    // ゲージが限界値かどうか
    if (isGaugeAtLimit) 
    {
        // 限界値でINTERVAL秒経過した場合釣り終了
        gaugeInterval_++;
        if (gaugeInterval_ >= MAX_GAUGE_INTERVAL) 
        {
            // 初期化処理
            Reset();
            dobber_->Reset();
            fishmng_->Reset();
            rod_->Reset();
            player_->SetCurrentMode(Player::PlayerMode::THIRD_PERSON);
            return;
        }
    }
    else 
    {
        // ゲージが制限値から外れた場合、タイマーをリセット
        gaugeInterval_ = 0;
    }
}

// ゲージ表示(初期値x、初期値ｙ、横の長さ、縦の長さ、値)
void Gauge::DrawGauge(float x, float y, float width, float height, float value)
{
    if (!isDraw_)return;

    DrawExtendGraph(static_cast<int>(x), static_cast<int>(y),
        static_cast<int>(x + width), static_cast<int>(y + height),
        gaugeHandle_, true);

    // ゲージの値部分の高さを計算(ゲージが減ると短くなる)
    int barHeight = static_cast<int>(height * (value / DEFAULT_MAX_VALUE));
    int barY = static_cast<int>(y + (height - barHeight));  // 上からバーが減るように調整

    // ゲージの色設定
    int red = static_cast<int>(MAX_COLOR * (value / DEFAULT_MAX_VALUE));
    int green = MAX_COLOR - red;

    // ゲージの値部分を描画(伸縮表示)
    SetDrawBright(red, green, 0);
    DrawExtendGraph(static_cast<int>(x), barY,
        static_cast<int>(x + width), static_cast<int>(y + height),
        gaugeHandle_, true);
    SetDrawBright(MAX_COLOR, MAX_COLOR, MAX_COLOR);
}

// リセット
void Gauge::Reset(void)
{
    isDraw_ = false;
    gaugeValue_ = DEFAULT_GAUGE_VALUE;
    gaugeInterval_ = 0;
}
