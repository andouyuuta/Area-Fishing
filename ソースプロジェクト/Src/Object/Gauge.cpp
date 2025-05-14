#include "../Manager/InputManager.h"
#include "Gauge.h"
#include "Player.h"
#include "Dobber.h"
#include "FishManager.h"
#include "Rod.h"

namespace
{
    // 移動速度
    constexpr float GAUGE_UP_SPEED = 0.8f;
    constexpr float GAUGE_DOWN_SPEED = 0.4f;
    constexpr float MAX_GAUGE_INTERVAL = 1.0f * 60.0f;
}

Gauge* Gauge::instance_ = nullptr;

void Gauge::CreateInstance(void)
{
    if (instance_ == nullptr)
    {
        instance_ = new Gauge();
    }
    instance_->Init();
}
Gauge& Gauge::GetInstance(void)
{
    return *instance_;
}
Gauge::Gauge()
{
    GaugePos = { 100.0f,100.0f,0.0f }, gagugebackhandle = -1;
}

Gauge::~Gauge()
{
}

void Gauge::Init(void)
{
    gaugeValue = 50;
    gagugebackhandle = LoadGraph("Data/Image/Gauge_bg.png");
}

void Gauge::Update(void)
{
}

void Gauge::Draw(void)
{
    //魚が食いついたときに表示
    if (gaugeflg) {
        
        DrawGauge(100, 100, 40, 500, gaugeValue);
        DrawString(150, 100, "左クリックでゲージを調節！", 0xffffff);
    }
}

void Gauge::Release(void)
{
}

//ゲージの更新
void Gauge::UpdateGauge(void)
{
    Player& player = Player::GetInstance();

    bool isGaugeAtLimit = (gaugeValue == 0 || gaugeValue == 100);

    //左クリックでゲージの増減
    if (InputManager::GetInstance().IsClickMouseLeft()) {
        gaugeValue += GAUGE_UP_SPEED;
        if (gaugeValue > 100) gaugeValue = 100;
    }
    else {
        gaugeValue -= GAUGE_DOWN_SPEED;
        if (gaugeValue < 0) gaugeValue = 0;
    }

    if (isGaugeAtLimit) {
        gaugeInterval++;
        if (gaugeInterval >= MAX_GAUGE_INTERVAL) {
            //初期化処理
            Reset();
            Dobber::GetInstance().Reset();
            FishManager::GetInstance().Reset();
            Rod::GetInstance().Reset();
            Player::GetInstance().SetCurrentMode(Player::PlayerMode::THIRD_PERSON);
            return;
        }
    }
    else {
        //ゲージが制限値から外れた場合、タイマーをリセット
        gaugeInterval = 0;
    }
}

void Gauge::DrawGauge(int x, int y, int width, int height, float value)
{
    if (gaugeflg) {
        DrawExtendGraph(x, y, x + width, y + height, gagugebackhandle, true);

        //ゲージの値部分の高さを計算(ゲージが減ると短くなる)
        int barHeight = height * value / 100.0f;
        int barY = y + (height - barHeight);  //上からバーが減るように調整

        //ゲージの色設定
        int red = (255 * value) / maxValue;;
        int green = 255 - red;

        //ゲージの値部分を描画(伸縮表示)
        SetDrawBright(red, green, 0);
        DrawExtendGraph(x, barY, x + width, y + height, gagugebackhandle, true);
        SetDrawBright(255, 255, 255);
    }
}

void Gauge::Reset(void)
{
    gaugeflg = false;
    gaugeValue = 50;
    gaugeInterval = 0;
}
