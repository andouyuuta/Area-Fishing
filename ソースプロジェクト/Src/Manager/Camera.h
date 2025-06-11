#pragma once
#include <DxLib.h>

class Player;
class Rod;

class Camera
{

public:
	enum CameraMode {
		THIRD_PERSON,
		FIRST_PERSON,
	};

	CameraMode currentMode_;


	Camera(void);
	~Camera(void);

	void Init(Rod* rod, Player* player);
	void Update(void);
	void Draw(void);
	void Release(void);

	//�J������]�p�a�擾
	VECTOR GetAngles(void);
	VECTOR GetPos(void) { return pos_; }

private:

	CameraMode prevMode_;

	// �J�����̈ʒu
	VECTOR pos_;

	// �J�����̊p�x(rad)
	VECTOR angles_;

	//�J�����̉�]��
	VECTOR cameraRot = { 0.0f,0.0f,0.0f };
	//�J�����̂̒��S�̍��W
	VECTOR rotCameraCenterPos = { 0.0f,0.0f,0.0f };
	//��]����J�����̍��W
	VECTOR rotCameraPos = { 50.0f,0.0f,100.0f };

	//�Ǐ]�Ώۂ���̃��[�J�����W
	VECTOR localPosFromThirdPlayer_;
	VECTOR localPosFromFirstPlayer_;
	//�Ǐ]�Ώۂ���̃��[�J�����W�̉�]�l
	VECTOR localRotFromPlayer_;

	// �}�E�X�̑O��ʒu��ێ�
	int lastMouseX = 0;
	int lastMouseY = 0;

	// �}�E�X���x
	const float sensitivity = 0.005f;

	void ThirdCamera(void);
	void FirstCamera(void);

	Player* player_;
	Rod* rod_;
};

