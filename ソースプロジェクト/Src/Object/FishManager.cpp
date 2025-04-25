#include "FishManager.h"
#include "Player.h"
#include "Dobber.h"
#include "Gauge.h"
#include "../Common/Random.h"
#include "../Manager/InputManager.h"

FishManager* FishManager::instance_ = nullptr;

void FishManager::CreateInstance()
{
    if (instance_ == nullptr)
    {
        instance_ = new FishManager();
    }
    instance_->Init();
}

FishManager& FishManager::GetInstance(void)
{
    return *instance_;
}

FishManager::FishManager()
    :ayumodel(-1),hayamodel(-1),hunamodel(-1),iwanamodel(-1),kawamutumodel(-1),medakamodel(-1),
    nizimasumodel(-1),oikawamodel(-1),poriputemodel(-1),uguimodel(-1),uthmodel(-1),wakasagimodel(-1),yamamemodel(-1),
    closestFishindex(-1),randValue(0),fishhitflg(false)
{
}

FishManager::~FishManager()
{
}

void FishManager::Init(void)
{
    ayumodel = MV1LoadModel("Data/Model/Fish/ayu.mv1");
    yamamemodel = MV1LoadModel("Data/Model/Fish/yamame.mv1");
    hayamodel = MV1LoadModel("Data/Model/Fish/haya.mv1");
    hunamodel = MV1LoadModel("Data/Model/Fish/huna.mv1");
    oikawamodel = MV1LoadModel("Data/Model/Fish/oikawa.mv1");
    kawamutumodel = MV1LoadModel("Data/Model/Fish/kawamutu.mv1");
    iwanamodel = MV1LoadModel("Data/Model/Fish/iwana.mv1");
    poriputemodel = MV1LoadModel("Data/Model/Fish/poriputerusu.mv1");
    nizimasumodel = MV1LoadModel("Data/Model/Fish/nizimasu.mv1");
    wakasagimodel = MV1LoadModel("Data/Model/Fish/wakasagi.mv1");
    uguimodel = MV1LoadModel("Data/Model/Fish/ugui.mv1");
    medakamodel = MV1LoadModel("Data/Model/Fish/medaka.mv1");
    uthmodel = MV1LoadModel("Data/Model/Fish/yuthteputeron.mv1");

    Probability();
}

void FishManager::Update(void)
{
    for (Fish& fish : fishlist) {
        fish.Update();
        fish.FishSwimLogic();
    }
    if (Dobber::GetInstance().GetLandingFlg()) {
        FishMove(Dobber::GetInstance().GetUkiPos());
    }
}

void FishManager::Draw(void)
{
    for (Fish& fish : fishlist) {
        fish.Draw();
    }
}

void FishManager::Release(void)
{
    for (Fish& fish : fishlist) {
        fish.Release();
    }
}

//�E�L�������������̋��̏���
void FishManager::FishMove(const VECTOR pos)
{
    Player& player = Player::GetInstance();
    Dobber& dobber = Dobber::GetInstance();
    Gauge& gauge = Gauge::GetInstance();

    // �͈͓��ɂ��鋛�̒��ōł��߂�����������
    if (closestFishindex == -1 && dobber.GetLandingFlg()) {
        float closestDistance = FLT_MAX;

        for (int i = 0; i < MAX_FISH; i++) {
            Fish& fish = fishlist[i];

            // ���ƃE�L�̋������v�Z
            float distance = VSize(VSub(pos, fish.GetFishPos()));

            // �͈͓��ōł��߂�����T��
            if (distance < dobber.UKIDISTANCE && distance < closestDistance) {
                closestDistance = distance;
                closestFishindex = i;
            }
        }
    }

    // �ł��߂��������������ꍇ�A���̋��������ړ�������
    if (closestFishindex != -1) {
        Fish& closestfish = fishlist[closestFishindex];
        MV1SetupCollInfo(closestfish.GetFishModel());
        closestfish.FishCome(closestFishindex);
        // �E�L�Ƌ��̓����蔻��
        auto fishcollision = MV1CollCheck_Sphere(closestfish.GetFishModel(), -1, dobber.GetUkiPos(), 200);
        //�������Ă�����
        if (fishcollision.HitNum >= 1) {
            //�E�L�Ƌ����������Ă�����ނ�J�n
            //�ނꂽ��
            fishhitflg = true;
            closestfish.FishingInterval(dobber);

            if (closestfish.Getleftclickpressflg()) {
                fishhitflg = false;
                MV1SetupCollInfo(dobber.GetUkiModel());
                MV1SetupCollInfo(player.GetModel());
                auto playercollision = MV1CollCheck_Sphere(dobber.GetUkiModel(), -1, player.GetPos(), 100);
                //�E�L�ƃv���C���[���������Ă����烊�U���g���
                if (playercollision.HitNum >= 1)
                {
                    dobber.SetFishingFlag(true);
                    gauge.SetGaugeFlg(false);
                }
                else {
                    gauge.SetGaugeFlg(true);
                    gauge.UpdateGauge();
                    closestfish.Move(dobber.GetUkiPos(), closestFishindex);
                }
                MV1CollResultPolyDimTerminate(playercollision);
            }
        }
        else {
            //�����ނ�Ă��Ȃ��Ƃ��ɍ��N���b�N����������
            if (InputManager::GetInstance().IsTrgMouseLeft()) {
                //�E�L��߂��ďI������
                dobber.Reset();
                dobber.SetLandingFlg(false);
                closestFishindex = -1;
                fishhitflg = false;
            }
        }
        MV1CollResultPolyDimTerminate(fishcollision);
    }
}

void FishManager::Reset() {
    Fish& closestfish = fishlist[closestFishindex];
    int closestmodel = closestfish.GetFishModel();
    MV1DeleteModel(closestmodel);
    for (Fish& fish : fishlist) {
        fish.SetInterval(0.0f);
        fish.Setleftclickpressflg(false);
    }
}

void FishManager::FishAllDelete(void)
{
    // ���ׂĂ̋��̃��f�����폜
    for (Fish& fish : fishlist)
    {
        int model = fish.GetFishModel();
        if (model != -1)
        {
            MV1DeleteModel(model);
        }
    }
    fishlist.clear();
}

//��ԋ߂����̃��f��
int FishManager::GetClosestFishModel(void) const
{
    return fishlist[closestFishindex].GetFishModel();
}

//��ԋ߂����̔ԍ�
int FishManager::GetClosestFishNumber(void)
{
    return fishlist[closestFishindex].GetFishNumber();
}

//��ԋ߂������ƂɃE�L�ɂ������Ƃ��̃t���O(Interval�b)
bool FishManager::GetClosestIntervalFlg(void) const
{
    return fishlist[closestFishindex].GetIntervalFlg();
}

void FishManager::SetClosestFishInterval(float interval)
{
    for (Fish& fish : fishlist) 
    {
        fish.SetInterval(interval);
    }
}

void FishManager::SetCountFlg(bool flg)
{
    for (Fish& fish : fishlist)
    {
        fish.SetCountFlg(flg);
    }
}

//�m���ŋ��𐶐�������
void FishManager::Probability()
{
    FishAllDelete();

    for (int i = 0; i < MAX_FISH; i++) {
        // �����_���ȑI�����擾
        randValue = Random::GetInstance().GetRandF(1, 100); // 1����100�܂ł̃����_���Ȑ����𐶐�

        //��m���Ń|���v�e���X�A�j�W�}�X�A���[�X�e�m�v�e����
        //���j���Ń��_�J�A�J�����c�A�E�O�C�A���J�T�M
        //���m���ł���ȊO�̋�
        Fish::FishType type = Fish::FishType::None;
        if (randValue >= 1 && randValue <= 10) type = Fish::FishType::Ayu;
        else if (randValue >= 11 && randValue <= 20) type = Fish::FishType::Yamame;
        else if (randValue >= 21 && randValue <= 30) type = Fish::FishType::Haya;
        else if (randValue >= 31 && randValue <= 40) type = Fish::FishType::Huna;
        else if (randValue >= 41 && randValue <= 50) type = Fish::FishType::Oikawa;
        else if (randValue >= 51 && randValue <= 58) type = Fish::FishType::Medaka;
        else if (randValue >= 59 && randValue <= 65) type = Fish::FishType::Kawamutu;
        else if (randValue >= 66 && randValue <= 75) type = Fish::FishType::Iwana;
        else if (randValue >= 76 && randValue <= 78) type = Fish::FishType::Poriputerusu;
        else if (randValue >= 79 && randValue <= 81) type = Fish::FishType::Nizimasu;
        else if (randValue >= 82 && randValue <= 89) type = Fish::FishType::Ugui;
        else if (randValue >= 90 && randValue <= 98) type = Fish::FishType::Wakasagi;
        else if (randValue >= 99 && randValue <= 100) type = Fish::FishType::Uthtenoputeron;

        fishlist.emplace_back();
        fishlist[i].ChangeFish(type);
    }
}