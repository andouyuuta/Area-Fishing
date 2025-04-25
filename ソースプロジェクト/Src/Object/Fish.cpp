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
    constexpr float ANIM_SPEED = 0.2f;
    constexpr float ROTATESPEED = 0.05f;
    constexpr float MAX_INTERVAL = 3.0f * 60;
    // ���̔z�u�͈͂�ݒ�
    constexpr float minX = -5500.0f;
    constexpr float maxX = 5500.0f;
    constexpr float minY = -350.0f;
    constexpr float maxY = -350.0f;  // ����y���W�͈�
    constexpr float minZ = -8000.0f;
    constexpr float maxZ = 13000.0f;
    constexpr float MinX = -2500.0f;
    constexpr float MaxX = 2500.0f;
    constexpr float MinY = -340.0f;
    constexpr float MaxY = -350.0f;  // ����y���W�͈�
    constexpr float MinZ = -2800.0f;
    constexpr float MaxZ = 3300.0f;
}

Fish::Fish() 
    :FishPos{0.0f,0.0f,0.0f}, PrevFishPos{0.0f,0.0f,0.0f}, 
      fishmodel(-1), fishInterval(0.0f), intervalcountflg(false), 
      interval(60.0f), leftclickpressflg(false), fishscale(0.25f), angle(0.0f), direction(1), fishnumber(0),
      animAttachNo_(0), animTotalTime_(0.0f), currentAnimTime_(0.0f),
      swimTimer(0.0f), currentDirection(VGet(0.0f, 0.0f, 0.0f)), changeDirectionInterval(0.0f),
      type_(FishType::None), currentAngleY(0.0f)
{
}

Fish::~Fish()
{
}

void Fish::Init(void)
{
}

void Fish::Update(void)
{
    currentAnimTime_ += ANIM_SPEED;

    if (currentAnimTime_ > animTotalTime_)
    {
        currentAnimTime_ = 0.0f;
    }
    MV1SetAttachAnimTime(fishmodel, animAttachNo_, currentAnimTime_);
    MV1SetPosition(fishmodel, FishPos);
}

void Fish::Draw(void) const
{
    MV1DrawModel(fishmodel);
}

void Fish::Release(void)
{
    MV1DeleteModel(fishmodel);
}

void Fish::Move(VECTOR pos, const int fishidx)
{
    Player& player = Player::GetInstance();

    VECTOR playerPos = player.GetPos();
    VECTOR direction = VNorm(VSub(playerPos, FishPos));

    direction = VScale(direction, -1.0f);

    float angleY = atan2f(direction.x, direction.z);
    static float currentAngleY = { 0.0f };
    float rotationSpeed = 0.1f;
    float deltaAngle = angleY - currentAngleY;

    if (deltaAngle > DX_PI_F)deltaAngle -= 2.0f * DX_PI_F;
    if (deltaAngle < -DX_PI_F)deltaAngle += 2.0f * DX_PI_F;

    currentAngleY += deltaAngle * rotationSpeed;

    MV1SetRotationXYZ(fishmodel, VGet(0.0f, currentAngleY + 90.0f, 0.0f));

    if (InputManager::GetInstance().IsClickMouseLeft())
    {
        FishPos = VAdd(FishPos, VScale(VNorm(VSub(player.GetPos(), FishPos)), 7.0f)); // �v���C���[�ɋ߂Â���
        pos = FishPos;
        pos.y = -165.0f;
    }
    else {
        // �}�E�X���L�[��������Ă��Ȃ��ꍇ�Aukimodel����������
        FishPos = VAdd(FishPos, VScale(VNorm(VSub(FishPos, player.GetPos())), 2.0f)); // �v���C���[���牓������
        pos = FishPos;
        pos.y = -165.0f;
    }

    MoveLR();

    MV1SetPosition(fishmodel, FishPos);
    Dobber::GetInstance().SetUkiPos(pos);
}

//�������E�̂ǂ��炩�Ɉړ������邩�����߂�
void Fish::MoveLR(void)
{
    interval--;
    if (interval <= 0)
    {
        direction = Random::GetInstance().GetRandF(0, 1) > 0.5f ? 1 : -1;;
        interval = 60;
    }
    angle += 0.05f * direction;
    FishPos.x += sinf(angle * 2.0f);
}

void Fish::FishIntervalCount(void)
{
    fishInterval++;
    intervalcountflg = true;
}

//�������Ԉȓ��ɍ��N���b�N����������ނ���J�n����
void Fish::FishingInterval(Dobber& dobber)
{
    if (fishInterval < MAX_INTERVAL && InputManager::GetInstance().IsTrgMouseLeft()) {
        leftclickpressflg = true;
        intervalcountflg = false;
    }
    else {
        //���Ԑ����ȓ��ɍ��N���b�N�����Ȃ������烊�Z�b�g
        if (!leftclickpressflg) {
            FishIntervalCount();
            if (fishInterval >= MAX_INTERVAL) {
                //fishInterval = MAX_INTERVAL;
                dobber.Reset();
                Reset();
            }
        }
    }
}

void Fish::FishCome(int& fishidx)
{
    //���̈ʒu��uki�̈ʒu�ɋ߂Â���
    Dobber& uki = Dobber::GetInstance();
    VECTOR ukipos = uki.GetUkiPos();
    VECTOR direction = VNorm(VSub(ukipos, FishPos)); // uki�̕���
    FishPos.x += direction.x * 2.0f; // x���W���X�V
    FishPos.z += direction.z * 2.0f; // z���W���X�V

    //���̉�]����
    VECTOR targetDirection = VNorm(VSub(uki.GetUkiPos(), FishPos));
    if (VSize(targetDirection) > 0.01f) {
        float angleY = atan2f(targetDirection.x, targetDirection.z);
        currentAngleY = { 0.0f };

        //������������]������
        float deltaAngle = angleY - currentAngleY;
        if (deltaAngle > 180.0f)deltaAngle -= 360.0f;
        if (deltaAngle < -180.0f)deltaAngle += 360.0f;

        currentAngleY += deltaAngle * ROTATESPEED;

        MV1SetRotationXYZ(fishmodel, VGet(0.0f, currentAngleY + 90.0f, 0.0f));
    }

    MV1SetPosition(fishmodel, FishPos); // �V�����ʒu��ݒ�
}

//�������R�ɓ���
void Fish::FishSwimLogic(void)
{
    if (!(Dobber::GetInstance().GetfishingFlg())) {
        //������(�ŏ�����)
        if (swimTimer == 0.0f && changeDirectionInterval == 0.0f) {
            currentDirection = VNorm(VGet(Random::GetInstance().GetRandF(-100.0f, 100.0f), Random::GetInstance().GetRandF(-0.5f, 0.5f), Random::GetInstance().GetRandF(-100.0f, 100.0f)));
            changeDirectionInterval = Random::GetInstance().GetRandF(50.0f, 200.0f);
        }

        //�^�C�}�[�𑝉�
        swimTimer += 1.0f;

        // ��莞�Ԃ��Ƃɉj�������������_���ɕύX
        if (swimTimer >= changeDirectionInterval) {
            // �����_���ȕ�����ݒ�
            currentDirection.x = Random::GetInstance().GetRandF(-100.0f, 100.0f);  // X����
            currentDirection.y = Random::GetInstance().GetRandF(-0.5f, 0.5f);  // Y�����i�����㉺�����ɉj���j
            currentDirection.z = Random::GetInstance().GetRandF(-100.0f, 100.0f);  // Z����

            // �x�N�g���𐳋K�����āA����������
            currentDirection = VNorm(currentDirection);

            //���̕ύX�Ԋu�������_����
            changeDirectionInterval = Random::GetInstance().GetRandF(50.0f, 200.0f);
            swimTimer = 0.0f;            // �^�C�}�[�����Z�b�g
        }

        float swimSpeed = Random::GetInstance().GetRandF(1.5f, 3.0f);

        //���̈ړ�
        PrevFishPos = FishPos;
        FishPos = VAdd(FishPos, VScale(currentDirection, swimSpeed));
        SwimRogicRotate();
        MV1SetPosition(fishmodel, FishPos);
    }
}

void Fish::SwimRogicRotate(void) const
{
    VECTOR direction = VSub(FishPos, PrevFishPos);

    if (VSize(direction) <= 0.01f) return;
    direction = VNorm(direction); // ���K�����ʂ�ۑ�

    float angleY = atan2f(direction.x, direction.z); // ��]�p�x���v�Z�i���W�A���j
    static float currentAngleY =  0.0f ;
    float deltaAngle = angleY - currentAngleY;

    // ��]�p�x�� -�΁`�� �Ɏ��߂�
    if (deltaAngle > DX_PI_F) deltaAngle -= 2.0f * DX_PI_F;
    if (deltaAngle < -DX_PI_F) deltaAngle += 2.0f * DX_PI_F;
    //��]���x
    currentAngleY += deltaAngle * ROTATESPEED;

    MV1SetRotationXYZ(fishmodel, VGet(0.0f, currentAngleY + 90.0f, 0.0f));
}

void Fish::ChangeFish(FishType newtype)
{
    if (fishmodel != -1) {
        MV1DeleteModel(fishmodel);  // �������f�����폜
    }

    type_ = newtype;

    // �V�������f�������[�h
    switch (type_) {
    case FishType::Ayu:          fishmodel = MV1DuplicateModel(FishManager::GetInstance().GetAyuModel()); fishnumber = 1; break;
    case FishType::Yamame:       fishmodel = MV1DuplicateModel(FishManager::GetInstance().GetYamameModel()); fishnumber = 2; break;
    case FishType::Haya:         fishmodel = MV1DuplicateModel(FishManager::GetInstance().GetHayaModel()); fishnumber = 3; break;
    case FishType::Huna:         fishmodel = MV1DuplicateModel(FishManager::GetInstance().GetHunaModel()); fishnumber = 4; break;
    case FishType::Oikawa:       fishmodel = MV1DuplicateModel(FishManager::GetInstance().GetOikawaModel()); fishnumber = 5; break;
    case FishType::Medaka:       fishmodel = MV1DuplicateModel(FishManager::GetInstance().GetMedakaModel()); fishnumber = 6; break;
    case FishType::Kawamutu:     fishmodel = MV1DuplicateModel(FishManager::GetInstance().GetKawamutuModel()); fishnumber = 7; break;
    case FishType::Iwana:        fishmodel = MV1DuplicateModel(FishManager::GetInstance().GetIwanaModel()); fishnumber = 8; break;
    case FishType::Poriputerusu: fishmodel = MV1DuplicateModel(FishManager::GetInstance().GetPoriputeModel()); fishnumber = 9; break;
    case FishType::Nizimasu:     fishmodel = MV1DuplicateModel(FishManager::GetInstance().GetNizimasuModel()); fishnumber = 10; break;
    case FishType::Ugui:         fishmodel = MV1DuplicateModel(FishManager::GetInstance().GetUguiModel()); fishnumber = 11; break;
    case FishType::Wakasagi:     fishmodel = MV1DuplicateModel(FishManager::GetInstance().GetWakasagiModel()); fishnumber = 12; break;
    case FishType::Uthtenoputeron: fishmodel = MV1DuplicateModel(FishManager::GetInstance().GetUthModel()); fishnumber = 13; break;
    default: fishmodel = -1; break;
    }

    // ���f���̈ʒu��X�P�[����ݒ�
    if (fishmodel != -1) {
        FishPos= {
             Random::GetInstance().GetRandY(minX, maxX),  // X���W
             Random::GetInstance().GetRandY(minY, maxY),  // Y���W
             Random::GetInstance().GetRandY(minZ, maxZ)   // Z���W
        };
        MV1SetPosition(fishmodel, FishPos);
        MV1SetScale(fishmodel, { fishscale,fishscale,fishscale });

        // �A�j���[�V�����ݒ�
        animAttachNo_ = MV1AttachAnim(fishmodel, 0,-1,-1);
        animTotalTime_ = MV1GetAttachAnimTotalTime(fishmodel, animAttachNo_);
        currentAnimTime_ = 0.0f;
        MV1SetAttachAnimTime(fishmodel, animAttachNo_, currentAnimTime_);
    }
}

void Fish::Reset(void)
{
    intervalcountflg = false;
    fishInterval = 0.0f;
    MV1DeleteModel(fishmodel);
    leftclickpressflg = false;
}
