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
    // �E�L�̈ړ��J�n���W
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

	// �e�̃��f��
	ukimodelId_ = MV1LoadModel("Data/Model/Stage/uki.mv1");

	////////////////////////////////////////
	// ���������̃x�N�g������{�̈ړ������Ƃ��Đݒ肷��
	basicMoveVec = { 0.0f, 0.0f, 100.0f };

	////////////////////////////////////////
	// ����]��̃x�N�g��������
	rotMoveVec = { 0.0f, 0.0f, 100.0f };

	////////////////////////////////////////

	// �ړ��x�N�g��������
	transformMoveVec = { 0.0f, 0.0f, 0.0f };

	// ���W������
    ukipos_ = { 0.0f,0.0f,0.0f };

	// ��]�l������
	rot_ = { 0.0f, 0.0f, 0.0f };

	// �e�̎g�p�t���O��܂�
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

    //�Ƃ��o�Ă��Ȃ��������߂�
    if (!rod.GetFlg()) {
        return;
    }

    if (rod.GetFlg())
    {
        // uki�����˂���Ă���Ƃ��̏���
        if (isShot)
        {
            // X����]�̍s��v�Z
            MATRIX matSphereRotX = MGetRotX(rot_.x);
            rotMoveVec = VTransform(basicMoveVec, matSphereRotX);

            // Y����]�̍s��v�Z
            MATRIX matSphereRotY = MGetRotY(rod.GetRotY());
            rotMoveVec = VTransform(rotMoveVec, matSphereRotY);

            // �ړ��x�N�g�����X�s�[�h�̑傫���ɕύX����
            transformMoveVec = VNorm(rotMoveVec);
            transformMoveVec = VScale(transformMoveVec, DOBBER_MOVE_SPEED);

            //�d��
            velocity_ = VAdd(velocity_, gravity_);

            // ���W���X�V
            ukipos_ = VAdd(ukipos_, VAdd(transformMoveVec, velocity_));

            // �n�ʂƂ̏Փ˔���
            if (ukipos_.y <= -165.0f) // �n�ʂ�y���W��0�Ɖ���
            {
                ukipos_.y = -165.0f; // �n�ʂɐڐG������ʒu���Œ�
                DOBBER_MOVE_SPEED = 0;
                landingflg = true;
            }

            Stage& stage = Stage::GetInstance();

            //�X�e�[�W�ƃE�L������������E�L���o���Ƃ��납���蒼��
            float ukiposy = Dobber::GetInstance().GetUkiPos().y;
            auto stagetoukiCollision = MV1CollCheck_Sphere(stage.GetModelId(), -1, ukipos_, 1);

            if (ukiposy > -165.0f && stagetoukiCollision.HitNum >= 1)
            {
                Reset();
            }
            MV1SetPosition(ukimodelId_, ukipos_);  // ���f���̈ʒu���X�V
            MV1CollResultPolyDimTerminate(stagetoukiCollision);
        }
        else  // uki�����˂���Ă��Ȃ��Ƃ�
        {
            // ���N���b�N�Ŕ���
            if (InputManager::GetInstance().IsTrgMouseLeft())
            {
                ukipos_ = player.GetPos();
                velocity_ = { 0.0f, 2.0f, 0.0f };
                isShot = true;
                fishMng.SetClosestFishInterval(0.0f);
                fishMng.SetCountFlg(false);

                MV1SetPosition(ukimodelId_, ukipos_);  // ���f���̏����ʒu��ݒ�
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

    // �e�����˂���Ă���Ƃ������`�悷��
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
