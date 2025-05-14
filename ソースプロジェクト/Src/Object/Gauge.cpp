#include "../Manager/InputManager.h"
#include "Gauge.h"
#include "Player.h"
#include "Dobber.h"
#include "FishManager.h"
#include "Rod.h"

namespace
{
    // �ړ����x
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
    //�����H�������Ƃ��ɕ\��
    if (gaugeflg) {
        
        DrawGauge(100, 100, 40, 500, gaugeValue);
        DrawString(150, 100, "���N���b�N�ŃQ�[�W�𒲐߁I", 0xffffff);
    }
}

void Gauge::Release(void)
{
}

//�Q�[�W�̍X�V
void Gauge::UpdateGauge(void)
{
    Player& player = Player::GetInstance();

    bool isGaugeAtLimit = (gaugeValue == 0 || gaugeValue == 100);

    //���N���b�N�ŃQ�[�W�̑���
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
            //����������
            Reset();
            Dobber::GetInstance().Reset();
            FishManager::GetInstance().Reset();
            Rod::GetInstance().Reset();
            Player::GetInstance().SetCurrentMode(Player::PlayerMode::THIRD_PERSON);
            return;
        }
    }
    else {
        //�Q�[�W�������l����O�ꂽ�ꍇ�A�^�C�}�[�����Z�b�g
        gaugeInterval = 0;
    }
}

void Gauge::DrawGauge(int x, int y, int width, int height, float value)
{
    if (gaugeflg) {
        DrawExtendGraph(x, y, x + width, y + height, gagugebackhandle, true);

        //�Q�[�W�̒l�����̍������v�Z(�Q�[�W������ƒZ���Ȃ�)
        int barHeight = height * value / 100.0f;
        int barY = y + (height - barHeight);  //�ォ��o�[������悤�ɒ���

        //�Q�[�W�̐F�ݒ�
        int red = (255 * value) / maxValue;;
        int green = 255 - red;

        //�Q�[�W�̒l������`��(�L�k�\��)
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
