#include <string>
#include "../Utility/Utility.h"
#include "../Manager/SceneManager.h"
#include "../Manager/InputManager.h"
#include "../Manager/Camera.h"
#include "TitleScene.h"

namespace
{
	constexpr int MANUAL_PAGE_COUNT = 5;
	constexpr int SELECT_COUNT = 3;

	// UI�ʒu�E�T�C�Y
	constexpr int TITLE_POS_X = 0;
	constexpr int TITLE_POS_Y = 0;

	constexpr int TITLE_LOGO_POS_X = 160;
	constexpr int TITLE_LOGO_POS_Y = 10;

	constexpr int SELECT_GUIDE_POS_X = 100;
	constexpr int SELECT_GUIDE_POS_Y1 = 300;
	constexpr int SELECT_GUIDE_POS_Y2 = 325;

	constexpr int SELECT_TEXT_START_Y = 400;
	constexpr int SELECT_TEXT_INTERVAL = 75;

	constexpr int PAGE_TEXT_OFFSET_X = 70;
	constexpr int PAGE_TEXT_OFFSET_Y = 30;

	constexpr int PAGE_HINT_LEFT_X = 0;
	constexpr int PAGE_HINT_LEFT_Y = 0;

	constexpr int PAGE_HINT_RIGHT_OFFSET_X = 350;
	constexpr int PAGE_HINT_RIGHT_Y = 0;

	// �F
	constexpr int COLOR_SELECTED_R = 255;
	constexpr int COLOR_SELECTED_G = 0;
	constexpr int COLOR_SELECTED_B = 0;

	constexpr int COLOR_NORMAL_R = 0;
	constexpr int COLOR_NORMAL_G = 0;
	constexpr int COLOR_NORMAL_B = 0;

	constexpr int FONT_SIZE_TITLE = 24;
	constexpr int FONT_SIZE_MANUAL = 16;

	// �t�@�C���p�X
	constexpr const char* TITLE_IMAGE_PATH = "Data/Image/Title.png";
	constexpr const char* TITLE_LOGO_IMAGE_PATH = "Data/Image/titlelogo.png";

	constexpr const char* TEXTBOX_IMAGE_PATH = "Data/Image/textbox.png";
	constexpr const char* SELECTTEXT_IMAGE_PATH = "Data/Image/selecttext.png";

	constexpr const char* SOUND_PATH = "Data/Sound/download_c3cd42c09c.mp3";

	constexpr const char* FONT_PATH = "Data/Font/Nikumaru.otf";
	constexpr const char* FONT_NAME = "07�ɂ��܂�t�H���g";

	// ����֘A
	constexpr const char* MANUAL_HINT = "���L�[�F�߂�@�@���L�[�F�i��";
	constexpr const char* MANUAL_HINT_RETURN = "ENTER�L�[�Ń��[�h�I���ɖ߂�";
	constexpr const char* SELECT_GUIDE_TEXT1 = "�@�����L�[�ňړ�";
	constexpr const char* SELECT_GUIDE_TEXT2 = "Enter�L�[��Select";

	// �}�j���A���\�L
	constexpr const char* MANUAL_IMAGE_PATHS[MANUAL_PAGE_COUNT] = {
		"Data/Image/manual1-1.png",
		"Data/Image/manual2-1.png",
		"Data/Image/manual4-1.png",
		"Data/Image/manual3-1.png",
		"Data/Image/manual5-1.png"
	};

	// �y�[�W�\�L
	constexpr const char* MANUAL_PAGE_TEXT[MANUAL_PAGE_COUNT] = {
		"�P/�T", "�Q/�T", "�R/�T", "�S/�T", "�T/�T"
	};

	// ���j���[�I��
	constexpr const char* MENU_TEXT[SELECT_COUNT] = {
		"1. �Q�[���X�^�[�g",
		"2. ����ݒ�",
		"3. �I��"
	};
}

// �R���X�g���N�^
TitleScene::TitleScene()
	: titleImg_(-1),
	titlePos_{ 0.0f, 0.0f, 0.0f },
	selection_(0),
	isSelect_(true),
	isManual_(false),
	manualSelect_(0),
	mainSoundHandle_(-1),
	fontHandle(-1),
	textBoxImg_(-1),
	selectTextImg_(-1)
{
	for (int& img : manualImage_) img = -1;
}

// �f�X�g���N�^
TitleScene::~TitleScene() {}

// ������
void TitleScene::Init()
{
	titleImg_ = LoadGraph(TITLE_IMAGE_PATH);

	for (int i = 0; i < MANUAL_PAGE_COUNT; ++i)
	{
		manualImage_[i] = LoadGraph(MANUAL_IMAGE_PATHS[i]);
	}

	mainSoundHandle_ = LoadSoundMem(SOUND_PATH);
	textBoxImg_ = LoadGraph(TEXTBOX_IMAGE_PATH);
	selectTextImg_ = LoadGraph(SELECTTEXT_IMAGE_PATH);

	AddFontResourceEx(FONT_PATH, FR_PRIVATE, NULL);
}

// �X�V
void TitleScene::Update()
{
	InputManager& ins = InputManager::GetInstance();

	if (CheckSoundMem(mainSoundHandle_) == 0)
	{
		PlaySoundMem(mainSoundHandle_, DX_PLAYTYPE_LOOP);
	}

	ChangeFont(FONT_NAME);

	if (isSelect_) UpdateSelect();
	if (isManual_) UpdateManual();

	if (ins.IsTrgDown(KEY_INPUT_RETURN))
	{
		switch (selection_)
		{
		case 0:
			SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAME);
			break;
		case 1:
			isManual_ = !isManual_;
			isSelect_ = true;
			break;
		case 2:
			DxLib_End();
			break;
		}
	}
}

// �`��
void TitleScene::Draw()
{
	if (isSelect_) DrawSelect();
	if (isManual_) DrawManual();
}


// ���
void TitleScene::Release()
{
	for (int i = 0; i < MANUAL_PAGE_COUNT; ++i)
	{
		DeleteGraph(manualImage_[i]);
	}
	DeleteSoundMem(mainSoundHandle_);
	RemoveFontResourceEx(FONT_PATH, FR_PRIVATE, NULL);
}

// ���j���[�I���X�V
void TitleScene::UpdateSelect()
{
	InputManager& ins = InputManager::GetInstance();

	if (isSelect_)
	{
		if (ins.IsTrgDown(KEY_INPUT_DOWN))
		{
			selection_ = (selection_ + 1) % SELECT_COUNT;
		}
		else if (ins.IsTrgDown(KEY_INPUT_UP))
		{
			selection_ = (selection_ + SELECT_COUNT - 1) % SELECT_COUNT;
		}
	}
}

// ���j���[�I��`��
void TitleScene::DrawSelect()
{
	DrawGraph(TITLE_POS_X, TITLE_POS_Y, titleImg_, true);

	SetFontSize(FONT_SIZE_TITLE);

	DrawString(SELECT_GUIDE_POS_X, SELECT_GUIDE_POS_Y1, SELECT_GUIDE_TEXT1, GetColor(COLOR_SELECTED_R, COLOR_SELECTED_G, COLOR_SELECTED_B));
	DrawString(SELECT_GUIDE_POS_X, SELECT_GUIDE_POS_Y2, SELECT_GUIDE_TEXT2, GetColor(COLOR_SELECTED_R, COLOR_SELECTED_G, COLOR_SELECTED_B));

	for (int i = 0; i < SELECT_COUNT; ++i)
	{
		int y = SELECT_TEXT_START_Y + i * SELECT_TEXT_INTERVAL;
		int color = (selection_ == i)
			? GetColor(COLOR_SELECTED_R, COLOR_SELECTED_G, COLOR_SELECTED_B)
			: GetColor(COLOR_NORMAL_R, COLOR_NORMAL_G, COLOR_NORMAL_B);
		DrawString(SELECT_GUIDE_POS_X, y, MENU_TEXT[i], color);
	}

}

// ��������X�V
void TitleScene::UpdateManual()
{
	InputManager& ins = InputManager::GetInstance();

	if (isManual_)
	{
		if (ins.IsTrgDown(KEY_INPUT_RIGHT))
		{
			manualSelect_ = (manualSelect_ + 1) % MANUAL_PAGE_COUNT;
		}
		else if (ins.IsTrgDown(KEY_INPUT_LEFT))
		{
			manualSelect_ = (manualSelect_ + MANUAL_PAGE_COUNT - 1) % MANUAL_PAGE_COUNT;
		}
	}
}

// ��������`��
void TitleScene::DrawManual()
{
	DrawGraph(0, 0, manualImage_[manualSelect_], true);

	DrawString(
		Application::SCREEN_SIZE_X - PAGE_TEXT_OFFSET_X,
		Application::SCREEN_SIZE_Y - PAGE_TEXT_OFFSET_Y,
		MANUAL_PAGE_TEXT[manualSelect_],
		GetColor(COLOR_NORMAL_R, COLOR_NORMAL_G, COLOR_NORMAL_B)
	);

	DrawString(PAGE_HINT_LEFT_X, PAGE_HINT_LEFT_Y, MANUAL_HINT, GetColor(COLOR_NORMAL_R, COLOR_NORMAL_G, COLOR_NORMAL_B));

	DrawString(
		Application::SCREEN_SIZE_X - PAGE_HINT_RIGHT_OFFSET_X,
		PAGE_HINT_RIGHT_Y,
		MANUAL_HINT_RETURN,
		GetColor(COLOR_NORMAL_R, COLOR_NORMAL_G, COLOR_NORMAL_B)
	);
}

// �t�H���g�`��p
void TitleScene::DrawText(int x, int y, const char* text, int color)
{
	DrawStringToHandle(x, y, text, color, fontHandle);
}
