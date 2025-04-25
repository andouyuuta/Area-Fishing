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
		TEST_,
	};

	CameraMode currentMode_;

	// カメラの高さ
	static constexpr float HEIGHT = 200.0f;

	// 追従対象からカメラまでの距離
	static constexpr float DIS_FOLLOW2CAMERA = 400.0f;

	static constexpr float ROT_SPEED_DEG = 1.0f;

	Camera(void);
	~Camera(void);

	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);


	//カメラ回転角殿取得
	VECTOR GetAngles(void);
	VECTOR GetPos(void) { return pos_; }

private:

	CameraMode prevMode_;

	// カメラの位置
	VECTOR pos_;

	// カメラの角度(rad)
	VECTOR angles_;

	//カメラの回転量
	VECTOR cameraRot = { 0.0f,0.0f,0.0f };
	//カメラのの中心の座標
	VECTOR rotCameraCenterPos = { 0.0f,0.0f,0.0f };
	//回転するカメラの座標
	VECTOR rotCameraPos = { 50.0f,0.0f,100.0f };

	//追従対象からのローカル座標
	VECTOR localPosFromThirdPlayer_;
	VECTOR localPosFromFirstPlayer_;
	//追従対象からのローカル座標の回転値
	VECTOR localRotFromPlayer_;

	// マウスの前回位置を保持
	int lastMouseX = 0;
	int lastMouseY = 0;

	// マウス感度
	const float sensitivity = 0.005f;

	void ThirdCamera(void);
	void FirstCamera(void);
};

