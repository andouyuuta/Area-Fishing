#include "FishManager.h"
#include "Player.h"
#include "Dobber.h"
#include "Gauge.h"
#include "../Common/Random.h"
#include "../Manager/InputManager.h"

namespace
{
    // ���f���E�p�X�֘A
    static constexpr const char* AYU_MODEL_PATH = "Data/Model/Fish/ayu.mv1";                // �A��
    static constexpr const char* YAMAME_MODEL_PATH = "Data/Model/Fish/yamame.mv1";          // ���}��
    static constexpr const char* HAYA_MODEL_PATH = "Data/Model/Fish/haya.mv1";              // �n��
    static constexpr const char* HUNA_MODEL_PATH = "Data/Model/Fish/huna.mv1";              // �t�i
    static constexpr const char* OIKAWA_MODEL_PATH = "Data/Model/Fish/oikawa.mv1";          // �I�C�J��
    static constexpr const char* KAWAMUTU_MODEL_PATH = "Data/Model/Fish/kawamutu.mv1";      // �J�����c
    static constexpr const char* IWANA_MODEL_PATH = "Data/Model/Fish/iwana.mv1";            // �C���i
    static constexpr const char* PORIPUTE_MODEL_PATH = "Data/Model/Fish/poriputerusu.mv1";  // �|���v�e���X
    static constexpr const char* NIZIMASU_MODEL_PATH = "Data/Model/Fish/nizimasu.mv1";      // �j�W�}�X
    static constexpr const char* WAKASAGI_MODEL_PATH = "Data/Model/Fish/wakasagi.mv1";      // ���J�T�M
    static constexpr const char* UGUI_MODEL_PATH = "Data/Model/Fish/ugui.mv1";              // �E�O�C
    static constexpr const char* MEDAKA_MODEL_PATH = "Data/Model/Fish/medaka.mv1";          // ���_�J
    static constexpr const char* UTH_MODEL_PATH = "Data/Model/Fish/yuthteputeron.mv1";      // ���[�X�e�m�v�e����

    constexpr float MIN_PROBABILITY = 1.0f;
    constexpr float MAX_PROBABILITY = 100.0f;

    constexpr float AYU_MIN = 1.0f;
    constexpr float AYU_MAX = 10.0f;

    constexpr float YAMAME_MIN = 11.0f;
    constexpr float YAMAMW_MAX = 20.0f;

    constexpr float HAYA_MIN = 21.0f;
    constexpr float HAYA_MAX = 30.0f;

    constexpr float HUNA_MIN = 31.0f;
    constexpr float HUNA_MAX = 40.0f;

    constexpr float OIKAWA_MIN = 41.0f;
    constexpr float OIKAWA_MAX = 50.0f;

    constexpr float MEDAKA_MIN = 51.0f;
    constexpr float MEDAKA_MAX = 58.0f;

    constexpr float KAWAMUTU_MIN = 59.0f;
    constexpr float KAWAMUTU_MAX = 65.0f;

    constexpr float IWANA_MIN = 66.0f;
    constexpr float IWANA_MAX = 75.0f;

    constexpr float PORIPUTE_MIN = 76.0f;
    constexpr float PORIPUTE_MAX = 78.0f;

    constexpr float NIZIMASU_MIN = 79.0f;
    constexpr float NIZIMASU_MAX = 81.0f;

    constexpr float UGUI_MIN = 82.0f;
    constexpr float UGUI_MAX = 89.0f;

    constexpr float WAKASAGI_MIN = 90.0f;
    constexpr float WAKASAGI_MAX = 98.0f;

    constexpr float UTH_MIN = 99.0f;
    constexpr float UTH_MAX = 100.0f;
}

FishManager::FishManager(void)
    :closestFishIndex_(-1),
    isFishHit_(false)
{
}

FishManager::~FishManager(void)
{
}

// ����������
void FishManager::Init(Player* player, Dobber* dobber, Gauge* gauge)
{
    player_ = player;
    dobber_ = dobber;
    gauge_ = gauge;

    fishModelPaths_ = 
    {
        {FishType::Ayu,AYU_MODEL_PATH},
        {FishType::Yamame, YAMAME_MODEL_PATH},
        {FishType::Haya, HAYA_MODEL_PATH},
        {FishType::Huna, HUNA_MODEL_PATH},
        {FishType::Oikawa, OIKAWA_MODEL_PATH},
        {FishType::Kawamutu, KAWAMUTU_MODEL_PATH},
        {FishType::Iwana, IWANA_MODEL_PATH},
        {FishType::Poriputerusu, PORIPUTE_MODEL_PATH},
        {FishType::Nizimasu, NIZIMASU_MODEL_PATH},
        {FishType::Wakasagi, WAKASAGI_MODEL_PATH},
        {FishType::Ugui, UGUI_MODEL_PATH},
        {FishType::Medaka, MEDAKA_MODEL_PATH},
        {FishType::Uthtenoputeron, UTH_MODEL_PATH}
    };

    // ���f���ǂݍ��݁i�}�b�v�Ɋi�[�j
    for (const auto& pair : fishModelPaths_) 
    {
        fishModels_[pair.first] = MV1LoadModel(pair.second.c_str());
    }

    // �m���ŋ��𐶐�
    Probability();      
}

// �X�V����
void FishManager::Update(void)
{
    for (Fish& fish : fishList) 
    {
        fish.Update();
    }
    // �E�L���n�ʂɂ����Ƃ��̏���
    if (dobber_->GetLandingFlg())
    {
        TouchDownMove(dobber_->GetUkiPos());
    }
}

void FishManager::Draw(void)
{
    for (Fish& fish : fishList) 
    {
        fish.Draw();
    }
}

void FishManager::Release(void)
{
    for (Fish& fish : fishList) 
    {
        fish.Release();
    }
}

// �E�L�������������̋��̏���
void FishManager::TouchDownMove(const VECTOR pos)
{
    // �͈͓��ɂ��鋛�̒��ōł��߂�����������
    if (closestFishIndex_ == -1 && dobber_->GetLandingFlg()) 
    {
        float closestDistance = FLT_MAX;

        // �͈͓��ɓ����Ă��邩
        for (int i = 0; i < MAX_FISH; i++) 
        {
            Fish& fish = fishList[i];

            // ���ƃE�L�̋������v�Z
            float distance = VSize(VSub(pos, fish.GetFishPos()));

            // �͈͓��ōł��߂�����T��
            if (distance < dobber_->GetUkiDistance() && distance < closestDistance) 
            {
                closestDistance = distance;
                closestFishIndex_ = i;
            }
        }
    }

    // �ł��߂��������������ꍇ�A���̋��������ړ�������
    if (closestFishIndex_ != -1) 
    {
        Fish& closestfish = fishList[closestFishIndex_];
        MV1SetupCollInfo(closestfish.GetFishModel());
        closestfish.FishCome(closestFishIndex_);
        // �E�L�Ƌ��̓����蔻��
        auto fishcollision = MV1CollCheck_Sphere(closestfish.GetFishModel(), -1, dobber_->GetUkiPos(), 200);
        // �������Ă�����
        if (fishcollision.HitNum >= 1) 
        {
            // �E�L�Ƌ����������Ă�����ނ�J�n
            // �ނꂽ��
            isFishHit_ = true;
            closestfish.FishingInterval();

            if (closestfish.GetLeftClickPressFlg()) 
            {
                isFishHit_ = false;
                MV1SetupCollInfo(dobber_->GetUkiModel());
                MV1SetupCollInfo(player_->GetModel());
                auto playercollision = MV1CollCheck_Sphere(dobber_->GetUkiModel(), -1, player_->GetPos(), 100);
                // �E�L�ƃv���C���[���������Ă����烊�U���g���
                if (playercollision.HitNum >= 1)
                {
                    dobber_->SetFishingFlg(true);
                    gauge_->SetGaugeFlg(false);
                }
                else
                {
                    gauge_->SetGaugeFlg(true);
                    gauge_->UpdateGauge();
                    closestfish.Move(dobber_->GetUkiPos(), closestFishIndex_);
                }
                MV1CollResultPolyDimTerminate(playercollision);
            }
        }
        else 
        {
            // �����ނ�Ă��Ȃ��Ƃ��ɍ��N���b�N����������
            if (InputManager::GetInstance().IsTrgMouseLeft()) 
            {
                // �E�L��߂��ďI������
                dobber_->Reset();
                dobber_->SetLandingFlg(false);    // ���n���Ă��Ȃ��ɐݒ�
                closestFishIndex_ = -1;
                isFishHit_ = false;
            }
        }
        MV1CollResultPolyDimTerminate(fishcollision);
    }
}

// ���Z�b�g
void FishManager::Reset(void) 
{
    Fish& closestfish = fishList[closestFishIndex_];
    int closestmodel = closestfish.GetFishModel();
    MV1DeleteModel(closestmodel);
    for (Fish& fish : fishList) 
    {
        fish.SetInterval(0.0f);
        fish.SetLeftClickPressFlg(false);
    }
}

// ���ׂĂ̋��̃��f�����폜
void FishManager::FishAllDelete(void)
{
    for (Fish& fish : fishList)
    {
        if (fish.GetFishModel() != -1)
        {
            MV1DeleteModel(fish.GetFishModel());
        }
    }
    fishList.clear();
}

// �m���ŋ��𐶐�������
void FishManager::Probability(void)
{
    FishAllDelete();

    struct FishProbability
    {
        FishType type;
        float min, max;
    };

    const std::vector<FishProbability> probabilities =
    {
        {FishType::Ayu, AYU_MIN, AYU_MAX},
        {FishType::Yamame, YAMAME_MIN, YAMAMW_MAX},
        {FishType::Haya, HAYA_MIN, HAYA_MAX},
        {FishType::Huna, HUNA_MIN,HAYA_MAX},
        {FishType::Oikawa, OIKAWA_MIN,OIKAWA_MAX},
        {FishType::Medaka, MEDAKA_MIN, MEDAKA_MAX},
        {FishType::Kawamutu, KAWAMUTU_MIN, KAWAMUTU_MAX},
        {FishType::Iwana, IWANA_MIN, IWANA_MAX},
        {FishType::Poriputerusu, PORIPUTE_MIN, PORIPUTE_MAX},
        {FishType::Nizimasu, NIZIMASU_MIN, NIZIMASU_MAX},
        {FishType::Ugui, UGUI_MIN, UGUI_MAX},
        {FishType::Wakasagi, WAKASAGI_MIN, WAKASAGI_MAX},
        {FishType::Uthtenoputeron, UTH_MIN, UTH_MAX}
    };

    for (int i = 0; i < MAX_FISH; ++i)
    {
        int randVal = static_cast<int>(Random::GetInstance().GetRandF(MIN_PROBABILITY, MAX_PROBABILITY));
        FishType selectedType = FishType::None;

        for (const auto& p : probabilities)
        {
            if (randVal >= p.min && randVal <= p.max)
            {
                selectedType = p.type;
                break;
            }
        }

        fishList.emplace_back();
        fishList[i].Init(player_, dobber_, this);
        fishList[i].SetFishManager(this);
        fishList[i].ChangeFish(selectedType);
    }
}

int FishManager::GetFishModel(Fish::FishType type) const
{
    auto model = fishModels_.find(type);
    return (model != fishModels_.end()) ? model->second : -1;
}

void FishManager::ClosestFishAnimation(void)
{
    for (Fish& fish : fishList)
    {
        fishList[closestFishIndex_].FishAnimation();
    }
}
