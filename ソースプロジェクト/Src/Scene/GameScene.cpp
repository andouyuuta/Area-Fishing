#include <cmath>
#include <DxLib.h>
#include "GameScene.h"
#include "../Manager/Camera.h"
#include "../Manager/InputManager.h"
#include "../Manager/SceneManager.h"
#include "../Object/Dobber.h"
#include "../Object/EffekseerEffect.h"
#include "../Object/FishManager.h"
#include "../Object/Player.h"
#include "../Object/Rod.h"
#include "../Object/Stage.h"
#include "../Object/water.h"
#include "../Object/Gauge.h"
#include "../Utility/Utility.h"
#include "../Common/Random.h"
#include "../Application.h"

GameScene::GameScene(void) : SceneBase()
{
	SunHundle = -1, backgroundimg = -1, drawflg = false, clearflg = false, effect_ = nullptr, fishcolor = 0, fishmodel = -1, fishname = "", fishonimg = -1,
		fishscale = 0.0f, rodspawnimg = -1, ukispawnimg = -1, water_ = nullptr;
}

GameScene::~GameScene(void)
{
}

void GameScene::Init(void)
{
	backgroundimg = LoadGraph("Data/Image/result.png");
	fishonimg = LoadGraph("Data/Image/FishOn.png");
	rodspawnimg = LoadGraph("Data/Image/RodSpawn.png");
	ukispawnimg = LoadGraph("Data/Image/UkiSpawn.png");
	// ����������
	Stage::CreateInstance();
	Stage::GetInstance().Init();
	Player::CreateInstance();
	Player::GetInstance().Init();
	Rod::CreateInstance();
	Rod::GetInstance().Init();
	FishManager::CreateInstance();
	FishManager::GetInstance().Init();
	Dobber::CreateInstance();
	Dobber::GetInstance().Init();
	effect_ = new EffekseerEffect();
	effect_->Init();
	water_ = new Water();
	water_->Init();
	Gauge::CreateInstance();
	Gauge::GetInstance().Init();
	SceneManager& sceneManager = SceneManager::GetInstance();
	Camera* camera = sceneManager.GetCamera();
}

void GameScene::Update(void)
{
	if (Dobber::GetInstance().GetfishingFlg())
	{
		SetFish(FishManager::GetInstance().GetClosestFishNumber());
		if (clearflg) {
			if (InputManager::GetInstance().IsTrgDown(KEY_INPUT_SPACE)) {
				SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE);
			}
		}
		else {
			if (InputManager::GetInstance().IsTrgDown(KEY_INPUT_SPACE)) {
				Initialize();
			}
		}
	}
	else {
		//�X�V����
		Stage::GetInstance().Update();
		Player::GetInstance().Update();
		FishManager::GetInstance().Update();
		Rod::GetInstance().Update();
		Dobber::GetInstance().Update();
		effect_->Update();
		water_->Update();
	}
}

void GameScene::Draw(void)
{
	//�ނ�Ă��烂�f�������\��
	if (Dobber::GetInstance().GetfishingFlg())
	{
		//�����A�ꂽ��
		DrawGraph(0, 0, backgroundimg, true);
		MV1DrawModel(fishmodel);
		// ���̖��O
		SetFontSize(48);
		DrawFormatString(0, 0, GetColor(0, 0, 0), fishname);
		SetFontSize(16);
		if (clearflg) {
			DrawString(10, Application::SCREEN_SIZE_Y - 30, "���߂łƂ��I�����A���u���[�X�e�m�v�e�����v��ނ邱�Ƃɐ��������I", GetColor(255, 255, 255)); // ��
			DrawString(10, Application::SCREEN_SIZE_Y - 15, "SPACE�Ń^�C�g����ʂɖ߂�", GetColor(255, 255, 255)); // ��
		}
		else {
			DrawString(10, Application::SCREEN_SIZE_Y - 50, "SPACE�ŃQ�[����ʂɖ߂�", GetColor(255, 255, 255)); // ��
		}
	}
	else {
		//�A��ĂȂ��ꍇ
		//�`�揈��
		Stage::GetInstance().Draw();
		Rod::GetInstance().Draw();
		effect_->Draw();
		FishManager::GetInstance().Draw();
		Dobber::GetInstance().Draw();
		water_->Draw();
		Player::GetInstance().Draw();
		Gauge::GetInstance().Draw();
		DrawString(0, 0, "�ڕW�F�����A���u���[�X�e�m�v�e�����v��ނ낤�I", GetColor(0, 0, 0));
	}
	Text();
}

void GameScene::Release(void)
{
	//�������
	Stage::GetInstance().Release();
	Player::GetInstance().Release();
	Rod::GetInstance().Release();
	FishManager::GetInstance().Release();
	Dobber::GetInstance().Release();
	effect_->Release();
	delete effect_;
	water_->Release();
	delete water_;
	DeleteGraph(backgroundimg);
}

//�Q�[�����̃��Z�b�g����
void GameScene::Initialize(void)
{
	//���֘A
	FishManager::GetInstance().Reset();
	FishManager::GetInstance().Probability();
	//�E�L�֘A
	Dobber::GetInstance().Reset();
	//�Ɗ֘A
	Rod::GetInstance().Reset();
	//�Q�[�W�֘A
	Gauge::GetInstance().Reset();
	//�v���C���[���O�l�̂ɖ߂�
	Player::GetInstance().SetCurrentMode(Player::THIRD_PERSON);
}

void GameScene::SetFish(int number)
{
	switch (number) {
	case 1:
		fishscale = 0.75f;
		fishname = "�A��";
		break;
	case 2:
		fishscale = 0.75f;
		fishname = "���}��";
		break;
	case 3:
		fishscale = 0.75f;
		fishname = "�n��";
		break;
	case 4:
		fishscale = 0.5f;
		fishname = "�t�i";
		break;
	case 5:
		fishscale = 0.5f;
		fishname = "�I�C�J��";
		break;
	case 6:
		fishscale = 5.0f;
		fishname = "���_�J";
		break;
	case 7:
		fishscale = 0.75f;
		fishname = "�J�����c";
		break;
	case 8:
		fishscale = 0.6f;
		fishname = "�C���i";
		break;
	case 9:
		fishscale = 0.2f;
		fishname = "�|���v�e���X";
		break;
	case 10:
		fishscale = 0.3f;
		fishname = "�j�W�}�X";
		break;
	case 11:
		fishscale = 0.5f;
		fishname = "�E�O�C";
		break;
	case 12:
		fishscale = 0.5f;
		fishname = "���J�T�M";
		break;
	case 13:
		fishscale = 0.3f;
		fishname = "���[�X�e�m�v�e����";
		clearflg = true;
		break;
	default:
		break;
	}

	// ���f���Ɋւ��邱��
	fishmodel = FishManager::GetInstance().GetClosestFishModel();
	VECTOR playerpos = Player::GetInstance().GetPos();
	MV1SetPosition(fishmodel, playerpos);
	MV1SetScale(fishmodel, { fishscale,fishscale,fishscale });
}

void GameScene::Text(void)
{
	if (Rod::GetInstance().GetFlg()) {
		if (Dobber::GetInstance().GetIsShot()) {
			if (FishManager::GetInstance().GetFishHitFlg()) {
				DrawExtendGraph(Application::SCREEN_SIZE_X / 2 - 400, Application::SCREEN_SIZE_Y / 2 - 150, 600, 300, fishonimg, true);
			}
		}
		else {
			DrawExtendGraph(0, 50, 250, 100, ukispawnimg, true);
			DrawString(0, 130, "���L�[�ŃE�L�𒲐�", 0xff0000);
		}
	}
	else {
		DrawExtendGraph(0, 50, 250, 100, rodspawnimg, true);
	}
}