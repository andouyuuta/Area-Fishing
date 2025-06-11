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
	// カメラの高さ
	constexpr float HEIGHT = 200.0f;

	// 追従対象からカメラまでの距離
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

	// カメラの初期位置
	pos_ = { 0.0f,0.0f,0.0f };

	//追従対象からのローカル座標設定
	localPosFromThirdPlayer_ = { 0.0f,HEIGHT,-DIS_FOLLOW2CAMERA };
	localPosFromFirstPlayer_ = { 0.0f,180.0f,-250.0f };
	//追従対象からのローカル座標の回転値
	localRotFromPlayer_ = { 0.0f,0.0f,0.0f };

	// カメラの初期角度
	angles_ = { 0.0f, 0.0f, 0.0f };

	SetMouseDispFlag(FALSE); // カーソルを非表示にする

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

		//カメラモードが切り替わった時
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
	// 現在のマウス座標を取得
	int mouseX, mouseY;
	GetMousePoint(&mouseX, &mouseY);

	// 画面中央の座標を計算
	int centerX =Application::SCREEN_SIZE_X / 2;
	int centerY = Application::SCREEN_SIZE_Y / 2;

	// マウスの移動量を計算
	int deltaX = mouseX - centerX;
	int deltaY = mouseY - centerY;

	// マウスカーソルを画面中央に再設定
	SetMousePoint(centerX, centerY);

	// マウスの移動量に応じてカメラの回転角度を更新
	localRotFromPlayer_.x += deltaY * sensitivity; // ピッチ（上下）
	localRotFromPlayer_.y += deltaX * sensitivity; // ヨー（左右）

	// ピッチ角度を制限してカメラの反転を防止
	if (localRotFromPlayer_.x > 0.5f) localRotFromPlayer_.x = 0.5f;
	if (localRotFromPlayer_.x < -0.3f) localRotFromPlayer_.x = -0.3f;

	// 回転行列を作成
	MATRIX mat = MGetIdent(); // 単位行列
	MATRIX matRotX = MGetRotX(localRotFromPlayer_.x);
	MATRIX matRotY = MGetRotY(localRotFromPlayer_.y);

	// 回転行列を合成
	mat = MMult(mat, matRotX);
	mat = MMult(mat, matRotY);

	// プレイヤーからの相対位置を回転
	VECTOR localPos = VTransform(localPosFromThirdPlayer_, mat);

	// プレイヤーの位置を取得（仮の関数）
	VECTOR playerPos = player_->GetPos(); // 実際のプレイヤー位置取得関数に置き換えてください

	// カメラのワールド座標を計算
	pos_ = VAdd(playerPos, localPos);

	// カメラの位置と注視点を設定
	VECTOR upVec = { 0.0f, 1.0f, 0.0f };
	SetCameraPositionAndTargetAndUpVec(pos_, playerPos, upVec);
}

void Camera::FirstCamera(void)
{
	// 現在のマウス座標を取得
	int mouseX, mouseY;
	GetMousePoint(&mouseX, &mouseY);

	// 画面中央の座標を計算
	int centerX = Application::SCREEN_SIZE_X / 2;
	int centerY = Application::SCREEN_SIZE_Y / 2;

	// マウスの移動量を計算
	int deltaX = mouseX - centerX;
	int deltaY = mouseY - centerY;

	// マウスカーソルを画面中央に再設定
	SetMousePoint(centerX, centerY);

	// マウスの移動量に応じてカメラの回転角度を更新
	localRotFromPlayer_.x += deltaY * sensitivity; // ピッチ（上下）
	localRotFromPlayer_.y+= deltaX * sensitivity; // ヨー（左右）

	// ピッチ角度を制限してカメラの反転を防止
	if (localRotFromPlayer_.x > 0.5f) localRotFromPlayer_.x = 0.5f;
	if (localRotFromPlayer_.x < -0.3f) localRotFromPlayer_.x = -0.3f;

	// 回転行列を作成
	MATRIX mat = MGetIdent(); // 単位行列
	MATRIX matRotX = MGetRotX(localRotFromPlayer_.x);
	MATRIX matRotY = MGetRotY(localRotFromPlayer_.y);

	// 回転行列を合成
	mat = MMult(mat, matRotX);
	mat = MMult(mat, matRotY);

	// プレイヤーからの相対位置を回転
	VECTOR localPos = VTransform(localPosFromThirdPlayer_, mat);

	// プレイヤーの位置を取得（仮の関数）
	VECTOR playerPos = player_->GetPos(); // 実際のプレイヤー位置取得関数に置き換えてください

	// カメラのワールド座標を計算
	pos_ = VAdd(playerPos, localPos);

	// カメラの位置と注視点を設定
	VECTOR upVec = { 0.0f, 1.0f, 0.0f };
	SetCameraPositionAndTargetAndUpVec(pos_, playerPos, upVec);
}