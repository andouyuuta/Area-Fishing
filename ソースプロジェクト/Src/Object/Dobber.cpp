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
    // ���f���E�p�X�֘A
    constexpr const char* UKI_MODEL_PATH = "Data/Model/Stage/uki.mv1";  // �E�L�̃��f���p�X

    // ������ԁE�ʒu
    constexpr VECTOR DOBBER_START_POS = { 0.0f, 110.0f, -200.0f };      // �E�L�̏����ʒu
    constexpr VECTOR DEFAULT_MOVE_VEC = { 0.0f, 0.0f, 100.0f };         // �����̈ړ��x�N�g��
    constexpr VECTOR DEFAULT_VELOCITY = { 0.0f, 2.0f, 0.0f };           // �����x�N�g��
    constexpr VECTOR DEFAULT_GRAVITY = { 0.0f, -0.02f, 0.0f };          // �d�̓x�N�g��

    // �ړ��E�����蔻��
    constexpr float DEFAULT_MOVE_SPEED = 8.0f;                          // �ړ����x
    constexpr float COLLISION_RADIUS = 1.0f;                            // �����蔻��̔��a
    constexpr float GROUND_Y = -165.0f;                                 // �n�ʂ�Y���W
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

	// �E�L�̐ݒ�
    ukiModelId_ = MV1LoadModel(UKI_MODEL_PATH);
    MV1SetPosition(ukiModelId_, ukiPos_);
    MV1SetPosition(ukiModelId_, scale_);

    // �����蔻��Z�b�g�A�b�v
    MV1SetupCollInfo(ukiModelId_);
}

void Dobber::Update(void)
{

    // �Ƃ��o�Ă��Ȃ��������߂�
    if (!rod_->GetFlg()) 
    {
        return;
    }

    if (rod_->GetFlg())
    {
        // uki�����˂���Ă���Ƃ��̏���
        if (isShot_)
        {
            // X����]�̍s��v�Z
            MATRIX matSphereRotX = MGetRotX(rot_.x);
            rotMoveVec_ = VTransform(basicMoveVec_, matSphereRotX);

            // Y����]�̍s��v�Z
            MATRIX matSphereRotY = MGetRotY(rod_->GetAngle().y);
            rotMoveVec_ = VTransform(rotMoveVec_, matSphereRotY);

            // �ړ��x�N�g�����X�s�[�h�̑傫���ɕύX����
            transformMoveVec_ = VNorm(rotMoveVec_);
            transformMoveVec_ = VScale(transformMoveVec_, moveSpeed_);

            // �d��
            velocity_ = VAdd(velocity_, gravity_);

            // ���W���X�V
            ukiPos_ = VAdd(ukiPos_, VAdd(transformMoveVec_, velocity_));

            // �n�ʂƂ̏Փ˔���
            if (ukiPos_.y <= GROUND_Y)   // �n�ʂ�y���W��0�Ɖ���
            {
                ukiPos_.y = GROUND_Y;    // �n�ʂɐڐG������ʒu���Œ�
                moveSpeed_ = 0;
                isLanding = true;
            }

            // �X�e�[�W�ƃE�L������������E�L���o���Ƃ��납���蒼��
            auto stagetoukiCollision = MV1CollCheck_Sphere(stage_->GetModelId(), -1, ukiPos_, COLLISION_RADIUS);

            if (ukiPos_.y > GROUND_Y && stagetoukiCollision.HitNum >= 1)
            {
                Reset();
            }
            MV1SetPosition(ukiModelId_, ukiPos_);  // ���f���̈ʒu���X�V
            MV1CollResultPolyDimTerminate(stagetoukiCollision);
        }
        else  // uki�����˂���Ă��Ȃ��Ƃ�
        {
            // ���N���b�N�Ŕ���
            if (InputManager::GetInstance().IsTrgMouseLeft())
            {
                ukiPos_ = player_->GetPos();
                velocity_ = DEFAULT_VELOCITY;
                isShot_ = true;
                fishmng_->SetClosestFishInterval(0.0f);
                fishmng_->SetCountFlg(false);

                MV1SetPosition(ukiModelId_, ukiPos_);  // ���f���̏����ʒu��ݒ�
            }
        }
    }
}

// �`�揈��
void Dobber::Draw(void)
{
    // �Ƃ��o�Ă��Ȃ��������߂�
    if (!rod_->GetFlg()) 
    {
        return;
    }

    // �e�����˂���Ă���Ƃ������`�悷��
    if (isShot_ == true)
    {
        MV1DrawModel(ukiModelId_);
    }
}

// �������
void Dobber::Release(void)
{
    MV1DeleteModel(ukiModelId_);
}

// �n�ʂɂ��Ă��邩
bool Dobber::GetLandingFlg(void) const
{
    return isLanding;
}

// �E�L�̍��W�̐ݒ�
void Dobber::SetUkiPos(const VECTOR newPos)
{
    ukiPos_ = newPos;
    MV1SetPosition(ukiModelId_, ukiPos_);
}

// ���Z�b�g
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