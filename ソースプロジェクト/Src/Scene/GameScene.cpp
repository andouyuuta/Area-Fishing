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

namespace
{
	static constexpr const char* BACKGROUND_PATH = "Data/Image/result.png";
	static constexpr const char* FISHON_PATH = "Data/Image/FishOn.png";
	static constexpr const char* ROD_SPAWN_TEXT_PATH = "Data/Image/RodSpawn.png";
	static constexpr const char* UKI_SPAWN_TEXT_PATH = "Data/Image/UkiSpawn.png";

	// �E�B���h�E�T�C�Y
	static constexpr int SCREEN_WIDTH = Application::SCREEN_SIZE_X;
	static constexpr int SCREEN_HEIGHT = Application::SCREEN_SIZE_Y;

	// ������\���ʒu�Ȃ�
	static constexpr int FISH_NAME_FONT_SIZE = 48;
	static constexpr int INFO_FONT_SIZE = 16;
	static constexpr int INFO_MARGIN_BOTTOM = 30;

	// �����\����
	struct FishInfo 
	{
		float scale;
		const char* name;
		bool isRare = false;
	};

	// ����񃊃X�g
	static constexpr FishInfo fishInfos[] = 
	{
		{0.0f, ""}, // 0�Ԗځi���g�p�j
		{0.75f, "�A��"},
		{0.75f, "���}��"},
		{0.75f, "�n��"},
		{0.5f,  "�t�i"},
		{0.5f,  "�I�C�J��"},
		{5.0f,  "���_�J"},
		{0.75f, "�J�����c"},
		{0.6f,  "�C���i"},
		{0.2f,  "�|���v�e���X"},
		{0.3f,  "�j�W�}�X"},
		{0.5f,  "�E�O�C"},
		{0.5f,  "���J�T�M"},
		{0.3f,  "���[�X�e�m�v�e����", true}
	};

	// �\���T�C�Y�萔
	static constexpr int FISHON_X = SCREEN_WIDTH / 2 - 400;
	static constexpr int FISHON_Y = SCREEN_HEIGHT / 2 - 150;
	static constexpr int FISHON_WIDTH = 600;
	static constexpr int FISHON_HEIGHT = 300;
	static constexpr int GRAPH_Y = 50;
	static constexpr int GRAPH_WEIGHT_ = 250;
	static constexpr int GRAPH_HEIGHT = 100;
	static constexpr int ROD_Y = 50;
	static constexpr int UKI_TEXT_Y = 130;
	static constexpr unsigned int TEXT_COLOR = 0xff0000;
	static constexpr int TEXT_X = 10;
	static constexpr int TEXT_Y = SCREEN_HEIGHT - 30;
	static constexpr int CLEAR_TEXT_Y = SCREEN_HEIGHT - 15;
	static constexpr int MAX_COLOR = 255;
}

GameScene::GameScene(void) 
	: SceneBase(), 
	sunHandle_(-1),
	backgroundImg_(-1),
	isDraw_(false),
	isClear_(false),
	effect_(nullptr),
	fishColor(0),
	fishModel(-1),
	fishName_(""),
	fishOnImg_(-1),
	fishScale_(0.0f),
	rodSpawnImg_(-1),
	ukiSpawnImg_(-1),
	water_(nullptr)
{
}

GameScene::~GameScene(void)
{
}

void GameScene::Init(void)
{
	backgroundImg_ = LoadGraph(BACKGROUND_PATH);
	fishOnImg_ = LoadGraph(FISHON_PATH);
	rodSpawnImg_ = LoadGraph(ROD_SPAWN_TEXT_PATH);
	ukiSpawnImg_ = LoadGraph(UKI_SPAWN_TEXT_PATH);
	// ����������
	camera_ = new Camera();
	stage_ = new Stage();
	player_ = new Player();
	rod_ = new Rod();
	dobber_ = new Dobber();
	fishmng_ = new FishManager();
	effect_ = new EffekseerEffect();
	water_ = new Water();
	gauge_ = new Gauge();

	stage_->Init();
	player_->Init(camera_);
	rod_->Init(player_, dobber_);
	dobber_->Init(player_, rod_, fishmng_, stage_);
	fishmng_->Init(player_, dobber_, gauge_);
	effect_->Init();
	water_->Init();
	gauge_->Init(player_, dobber_, rod_, fishmng_);
	camera_->Init(rod_, player_);

	SceneManager& sceneManager = SceneManager::GetInstance();
}

void GameScene::Update(void)
{
	if (dobber_->GetFishingFlg())
	{
		// �����ނꂽ���ނꂽ��������`�悳����
		SetFish(fishmng_->GetClosestFishNumber());
		fishmng_->ClosestFishAnimation();
		camera_->Update();
		if (isClear_) 
		{
			if (InputManager::GetInstance().IsTrgDown(KEY_INPUT_SPACE))
			{
				SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE);
			}
		}
		else
		{
			if (InputManager::GetInstance().IsTrgDown(KEY_INPUT_SPACE)) 
			{
				// ������
				Reset();
			}
		}
	}
	else 
	{
		// �X�V����
		stage_->Update();
		player_->Update();
		fishmng_->Update();
		rod_->Update();
		dobber_->Update();
		effect_->Update();
		water_->Update();
		camera_->Update();
	}
}

void GameScene::Draw(void)
{
	//�@�ނ�Ă��烂�f�������\��
	if (dobber_->GetFishingFlg())
	{
		//�@�����A�ꂽ��
		DrawGraph(0, 0, backgroundImg_, true);
		MV1DrawModel(fishModel);
		// ���̖��O
		SetFontSize(FISH_NAME_FONT_SIZE);
		DrawFormatString(0, 0, GetColor(0, 0, 0), fishName_);
		SetFontSize(INFO_FONT_SIZE);
		if (isClear_)
		{
			DrawString(TEXT_X, CLEAR_TEXT_Y,
				"���߂łƂ��I�����A���u���[�X�e�m�v�e�����v��ނ邱�Ƃɐ��������I", GetColor(MAX_COLOR, MAX_COLOR, MAX_COLOR));
			DrawString(TEXT_X, TEXT_Y,
				"SPACE�Ń^�C�g����ʂɖ߂�", GetColor(MAX_COLOR, MAX_COLOR, MAX_COLOR));
		}
		else
		{
			DrawString(TEXT_X, TEXT_Y,
				"SPACE�ŃQ�[����ʂɖ߂�", GetColor(MAX_COLOR, MAX_COLOR, MAX_COLOR));
		}
	}
	else 
	{
		// �ނ�Ă��Ȃ��ꍇ
		// �`�揈��
		stage_->Draw();
		rod_->Draw();
		effect_->Draw();
		fishmng_->Draw();
		dobber_->Draw();
		water_->Draw();
		player_->Draw();
		gauge_->Draw();
		DrawString(0, 0, "�ڕW�F�����A���u���[�X�e�m�v�e�����v��ނ낤�I", GetColor(0, 0, 0));
	}
	Text();
}

void GameScene::Release(void)
{
	// �������
	stage_->Release();
	player_->Release();
	rod_->Release();
	fishmng_->Release();
	dobber_->Release();
	effect_->Release();
	delete effect_;
	water_->Release();
	delete water_;
	DeleteGraph(backgroundImg_);
	camera_->Release();
}

//�Q�[�����̃��Z�b�g����
void GameScene::Reset(void)
{
	// ���֘A
	fishmng_->Reset();
	fishmng_->Probability();
	// �E�L�֘A
	dobber_->Reset();
	// �Ɗ֘A
	rod_->Reset();
	// �Q�[�W�֘A
	gauge_->Reset();
	// �v���C���[���O�l�̂ɖ߂�
	player_->SetCurrentMode(Player::THIRD_PERSON);
}

void GameScene::SetFish(int number)
{
	if (number <= 0 || number >= static_cast<int>(std::size(fishInfos)))
	{
		// �͈͊O�̔ԍ��Ȃ牽�����Ȃ�
		return;
	}

	const FishInfo& fish = fishInfos[number];							// �ԍ��ɑΉ������������擾
	fishScale_ = fish.scale;											// ���̑傫����ݒ�
	fishName_ = fish.name;												// ���̖��O��ݒ�
	isClear_ = fish.isRare;												// �����A���ǂ����̃t���O���Z�b�g
	fishModel = fishmng_->GetClosestFishModel();						// ����3D���f�����擾
	MV1SetPosition(fishModel, player_->GetPos());						// �����f���̈ʒu���v���C���[�ʒu�ɍ��킹��
	MV1SetScale(fishModel, { fishScale_, fishScale_, fishScale_ });		// �����f���̃X�P�[����ݒ�
}

void GameScene::Text(void)
{
	if (rod_->GetFlg())
	{
		if (dobber_->GetIsShot())
		{
			if (fishmng_->GetFishHitFlg())
			{
				DrawExtendGraph(
					FISHON_X,
					FISHON_Y,
					FISHON_WIDTH, FISHON_HEIGHT,
					fishOnImg_,
					true);
			}
		}
		else
		{
			DrawExtendGraph(0, GRAPH_Y, GRAPH_WEIGHT_, GRAPH_HEIGHT, ukiSpawnImg_, true);
			DrawString(0, UKI_TEXT_Y, "���L�[�ŃE�L�𒲐�", 0xff0000);
		}
	}
	else
	{
		DrawExtendGraph(0, GRAPH_Y, GRAPH_WEIGHT_, GRAPH_HEIGHT, rodSpawnImg_, true);
	}
}