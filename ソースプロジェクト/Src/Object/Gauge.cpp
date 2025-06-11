#include "../Manager/InputManager.h"
#include "Gauge.h"
#include "Player.h"
#include "Dobber.h"
#include "FishManager.h"
#include "Rod.h"

namespace
{
    // �Q�[�W�̋���
    constexpr float GAUGE_UP_SPEED = 0.8f;          // �Q�[�W�㏸���x
    constexpr float GAUGE_DOWN_SPEED = 0.4f;        // �Q�[�W���~���x
    constexpr float MAX_GAUGE_INTERVAL = 60.0f;     // �ő�C���^�[�o���i1�b��60�t���[���j

    // �Q�[�W�l�̏����ݒ�
    constexpr float DEFAULT_GAUGE_VALUE = 50.0f;
    constexpr float DEFAULT_MAX_VALUE = 100.0f;
    constexpr float DEFAULT_MIN_VALUE = 0.0f;

    // �Q�[�W�̕\���ʒu
    constexpr float DEFAULT_GAUGE_X = 100.0f;
    constexpr float DEFAULT_GAUGE_Y = 100.0f;

    // �����H���������̃e�L�X�g�̕\���ʒu
    constexpr int GAUGE_TEXT_X = 150;
    constexpr int GAUGE_TEXT_Y = 100;

    // �Q�[�W�̐F�E�傫��
    constexpr int MAX_COLOR = 255;
    constexpr int GAUGE_WIDTH = 40;
    constexpr int GAUGE_HEIGHT = 500;

    // �e�L�X�g�E�p�X�֘A
    constexpr const char* GAUGE_HINT_MESSAGE = "���N���b�N�ŃQ�[�W�𒲐߁I";
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

// ����������
void Gauge::Init(Player* player, Dobber* dobber, Rod* rod, FishManager* fishmng)
{
    player_ = player;
    dobber_ = dobber;
    rod_ = rod;
    fishmng_ = fishmng;
    gaugeHandle_ = LoadGraph(GAUGE_IMAGE_PATH);
}

// �X�V����
void Gauge::Update(void)
{
}

// �`�揈��
void Gauge::Draw(void)
{
    // �����H�������Ƃ��ɕ\��
    if (isDraw_) 
    {
        DrawGauge(DEFAULT_GAUGE_X, DEFAULT_GAUGE_Y, GAUGE_WIDTH, GAUGE_HEIGHT, gaugeValue_);
        DrawString(GAUGE_TEXT_X, GAUGE_TEXT_Y, GAUGE_HINT_MESSAGE, 0xffffff);
    }
}
// �������
void Gauge::Release(void)
{
    DeleteGraph(gaugeHandle_);
}

// �Q�[�W�̍X�V
void Gauge::UpdateGauge(void)
{
    // �Q�[�W������E�����ɒB���Ă��邩
    bool isGaugeAtLimit = (gaugeValue_ == DEFAULT_MIN_VALUE || gaugeValue_ == DEFAULT_MAX_VALUE);

    // ���N���b�N�ŃQ�[�W�̑���
    if (InputManager::GetInstance().IsClickMouseLeft())
    {
        // �l�𑝂₷
        gaugeValue_ += GAUGE_UP_SPEED;
        if (gaugeValue_ > DEFAULT_MAX_VALUE) gaugeValue_ = DEFAULT_MAX_VALUE;
    }
    else 
    {
        // �l�����炷
        gaugeValue_ -= GAUGE_DOWN_SPEED;
        if (gaugeValue_ < DEFAULT_MIN_VALUE) gaugeValue_ = DEFAULT_MIN_VALUE;
    }

    // �Q�[�W�����E�l���ǂ���
    if (isGaugeAtLimit) 
    {
        // ���E�l��INTERVAL�b�o�߂����ꍇ�ނ�I��
        gaugeInterval_++;
        if (gaugeInterval_ >= MAX_GAUGE_INTERVAL) 
        {
            // ����������
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
        // �Q�[�W�������l����O�ꂽ�ꍇ�A�^�C�}�[�����Z�b�g
        gaugeInterval_ = 0;
    }
}

// �Q�[�W�\��(�����lx�A�����l���A���̒����A�c�̒����A�l)
void Gauge::DrawGauge(float x, float y, float width, float height, float value)
{
    if (!isDraw_)return;

    DrawExtendGraph(static_cast<int>(x), static_cast<int>(y),
        static_cast<int>(x + width), static_cast<int>(y + height),
        gaugeHandle_, true);

    // �Q�[�W�̒l�����̍������v�Z(�Q�[�W������ƒZ���Ȃ�)
    int barHeight = static_cast<int>(height * (value / DEFAULT_MAX_VALUE));
    int barY = static_cast<int>(y + (height - barHeight));  // �ォ��o�[������悤�ɒ���

    // �Q�[�W�̐F�ݒ�
    int red = static_cast<int>(MAX_COLOR * (value / DEFAULT_MAX_VALUE));
    int green = MAX_COLOR - red;

    // �Q�[�W�̒l������`��(�L�k�\��)
    SetDrawBright(red, green, 0);
    DrawExtendGraph(static_cast<int>(x), barY,
        static_cast<int>(x + width), static_cast<int>(y + height),
        gaugeHandle_, true);
    SetDrawBright(MAX_COLOR, MAX_COLOR, MAX_COLOR);
}

// ���Z�b�g
void Gauge::Reset(void)
{
    isDraw_ = false;
    gaugeValue_ = DEFAULT_GAUGE_VALUE;
    gaugeInterval_ = 0;
}
