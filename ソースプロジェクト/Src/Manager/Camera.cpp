#include "Camera.h"
#include "../Application.h"
#include "../Manager/InputManager.h"
#include "../Manager/SceneManager.h"
#include "../Utility/Utility.h"
#include"../Object/Player.h"
#include"../Object/Rod.h"
#include"../Object/Rod.h"

namespace
{
	// �J�����̍���
	constexpr float HEIGHT = 200.0f;

	// �Ǐ]�Ώۂ���J�����܂ł̋���
	constexpr float DIS_FOLLOW2CAMERA = 400.0f;

	constexpr float ROT_SPEED_DEG = 1.0f;
}

Camera::Camera(void)
{
}

Camera::~Camera(void)
{
}

void Camera::Init(Rod* rod, Player* player)
{
	rod_ = rod;
	player_ = player;

	// �J�����̏����ʒu
	pos_ = { 0.0f,0.0f,0.0f };

	//�Ǐ]�Ώۂ���̃��[�J�����W�ݒ�
	localPosFromThirdPlayer_ = { 0.0f,HEIGHT,-DIS_FOLLOW2CAMERA };
	localPosFromFirstPlayer_ = { 0.0f,180.0f,-250.0f };
	//�Ǐ]�Ώۂ���̃��[�J�����W�̉�]�l
	localRotFromPlayer_ = { 0.0f,0.0f,0.0f };

	// �J�����̏����p�x
	angles_ = { 0.0f, 0.0f, 0.0f };

	SetMouseDispFlag(FALSE); // �J�[�\�����\���ɂ���

	currentMode_ = CameraMode::THIRD_PERSON;
	prevMode_ = CameraMode::THIRD_PERSON;;
}

void Camera::Update(void)
{
	auto& ins = InputManager::GetInstance();

	if (SceneManager::GetInstance().GetSceneID() == SceneManager::SCENE_ID::GAME)
	{
		if (rod_->GetFlg()) {
			currentMode_ = CameraMode::FIRST_PERSON;
		}
		else {
			currentMode_ = CameraMode::THIRD_PERSON;
		}

		//�J�������[�h���؂�ւ������
		if (currentMode_ != prevMode_) {
			localRotFromPlayer_ = player_->GetAngle();
			prevMode_ = currentMode_;
		}

		switch (currentMode_)
		{
		case THIRD_PERSON:
			ThirdCamera();
			break;
		case FIRST_PERSON:
			FirstCamera();
			break;
		default:
			ThirdCamera();
			break;
		}
	}
}

void Camera::Draw(void)
{
}

void Camera::Release(void)
{
}

VECTOR Camera::GetAngles(void)
{
	return localRotFromPlayer_;
}

void Camera::ThirdCamera(void)
{
	// ���݂̃}�E�X���W���擾
	int mouseX, mouseY;
	GetMousePoint(&mouseX, &mouseY);

	// ��ʒ����̍��W���v�Z
	int centerX =Application::SCREEN_SIZE_X / 2;
	int centerY = Application::SCREEN_SIZE_Y / 2;

	// �}�E�X�̈ړ��ʂ��v�Z
	int deltaX = mouseX - centerX;
	int deltaY = mouseY - centerY;

	// �}�E�X�J�[�\������ʒ����ɍĐݒ�
	SetMousePoint(centerX, centerY);

	// �}�E�X�̈ړ��ʂɉ����ăJ�����̉�]�p�x���X�V
	localRotFromPlayer_.x += deltaY * sensitivity; // �s�b�`�i�㉺�j
	localRotFromPlayer_.y += deltaX * sensitivity; // ���[�i���E�j

	// �s�b�`�p�x�𐧌����ăJ�����̔��]��h�~
	if (localRotFromPlayer_.x > 0.5f) localRotFromPlayer_.x = 0.5f;
	if (localRotFromPlayer_.x < -0.3f) localRotFromPlayer_.x = -0.3f;

	// ��]�s����쐬
	MATRIX mat = MGetIdent(); // �P�ʍs��
	MATRIX matRotX = MGetRotX(localRotFromPlayer_.x);
	MATRIX matRotY = MGetRotY(localRotFromPlayer_.y);

	// ��]�s�������
	mat = MMult(mat, matRotX);
	mat = MMult(mat, matRotY);

	// �v���C���[����̑��Έʒu����]
	VECTOR localPos = VTransform(localPosFromThirdPlayer_, mat);

	// �v���C���[�̈ʒu���擾�i���̊֐��j
	VECTOR playerPos = player_->GetPos(); // ���ۂ̃v���C���[�ʒu�擾�֐��ɒu�������Ă�������

	// �J�����̃��[���h���W���v�Z
	pos_ = VAdd(playerPos, localPos);

	// �J�����̈ʒu�ƒ����_��ݒ�
	VECTOR upVec = { 0.0f, 1.0f, 0.0f };
	SetCameraPositionAndTargetAndUpVec(pos_, playerPos, upVec);
}

void Camera::FirstCamera(void)
{
	// ���݂̃}�E�X���W���擾
	int mouseX, mouseY;
	GetMousePoint(&mouseX, &mouseY);

	// ��ʒ����̍��W���v�Z
	int centerX = Application::SCREEN_SIZE_X / 2;
	int centerY = Application::SCREEN_SIZE_Y / 2;

	// �}�E�X�̈ړ��ʂ��v�Z
	int deltaX = mouseX - centerX;
	int deltaY = mouseY - centerY;

	// �}�E�X�J�[�\������ʒ����ɍĐݒ�
	SetMousePoint(centerX, centerY);

	// �}�E�X�̈ړ��ʂɉ����ăJ�����̉�]�p�x���X�V
	localRotFromPlayer_.x += deltaY * sensitivity; // �s�b�`�i�㉺�j
	localRotFromPlayer_.y+= deltaX * sensitivity; // ���[�i���E�j

	// �s�b�`�p�x�𐧌����ăJ�����̔��]��h�~
	if (localRotFromPlayer_.x > 0.5f) localRotFromPlayer_.x = 0.5f;
	if (localRotFromPlayer_.x < -0.3f) localRotFromPlayer_.x = -0.3f;

	// ��]�s����쐬
	MATRIX mat = MGetIdent(); // �P�ʍs��
	MATRIX matRotX = MGetRotX(localRotFromPlayer_.x);
	MATRIX matRotY = MGetRotY(localRotFromPlayer_.y);

	// ��]�s�������
	mat = MMult(mat, matRotX);
	mat = MMult(mat, matRotY);

	// �v���C���[����̑��Έʒu����]
	VECTOR localPos = VTransform(localPosFromThirdPlayer_, mat);

	// �v���C���[�̈ʒu���擾�i���̊֐��j
	VECTOR playerPos = player_->GetPos(); // ���ۂ̃v���C���[�ʒu�擾�֐��ɒu�������Ă�������

	// �J�����̃��[���h���W���v�Z
	pos_ = VAdd(playerPos, localPos);

	// �J�����̈ʒu�ƒ����_��ݒ�
	VECTOR upVec = { 0.0f, 1.0f, 0.0f };
	SetCameraPositionAndTargetAndUpVec(pos_, playerPos, upVec);
}