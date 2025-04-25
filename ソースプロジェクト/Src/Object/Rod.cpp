#include "../Manager/InputManager.h"
#include "Player.h"
#include "Rod.h"
#include "Dobber.h"

namespace
{
	// アニメーションの再生速度
	constexpr float ANIM_SPEED = 6.0f;
	constexpr float KAMAEANIM_SPEED = 5.0f;
	constexpr float THROWANIM_SPEED = 4.0f;
	constexpr float ANIM_ROD_SPEED = 10.0f;
	//回転スピード
	constexpr float ROT_SPEED_DEG = 1.0f;
}

Rod* Rod::instance_ = nullptr;

void Rod::CreateInstance()
{
	if (instance_ == nullptr)
	{
		instance_ = new Rod();
	}
	instance_->Init();
}

Rod& Rod::GetInstance(void)
{
	return *instance_;
}

Rod::Rod(void)
	:state_(STATE::SPAWN)
{
	animAttachNo_ = 0, animTotalTime_ = 0.0f, animrodAttachNo_ = 0, animrodTotalTime_ = 0.0f, currentAnimTime_ = 0.0f, currentkamaeAnimTime_ = 0.0f,
		currentrodAnimTime_ = 0.0f, currentthrowAnimTime_ = 0.0f, kamaeanimFlg = false, kamaerodAttachNo_ = 0, kamaerodTotalTime_ = 0.0f,
		pos_ = { 0.0f,0.0f,0.0f }, rodFlg = false, rodanimFlg = false, rodmodel_ = -1, rot_ = { 0.0f,0.0f,0.0f }, throwanimAttachNo_ = 0,
		throwanimFlg = false, throwanimTotalTime_ = 0.0f;
}

Rod::~Rod(void)
{
}

void Rod::Init(void)
{
	rodmodel_ = MV1LoadModel("Data/Model/Stage/rod3.mv1");

	pos_ = { 0.0f,50.0f,0.0f };
	rot_ = { 0.0f,0.0f,0.0f };

	MV1SetPosition(rodmodel_, pos_);
	MV1SetRotationXYZ(rodmodel_, rot_);
	MV1SetScale(rodmodel_, scale_);

	rodanimFlg = false;
	rodFlg = false;

	// アニメーションの総時間を取得するためにAttach
	kamaerodAttachNo_ = MV1AttachAnim(rodmodel_, 2, -1, -1);
	animrodAttachNo_ = MV1AttachAnim(rodmodel_, 3, -1, -1);
	throwanimAttachNo_ = MV1AttachAnim(rodmodel_, 4, -1, -1);

	// アニメーション総時間の取得
	animrodTotalTime_ = MV1GetAttachAnimTotalTime(rodmodel_, animrodAttachNo_);
	kamaerodTotalTime_ = MV1GetAttachAnimTotalTime(rodmodel_, kamaerodAttachNo_);
	throwanimTotalTime_ = MV1GetAttachAnimTotalTime(rodmodel_, throwanimAttachNo_);

	// アニメーションの総時間を取得したのでDetach
	MV1DetachAnim(rodmodel_, kamaerodAttachNo_);
	MV1DetachAnim(rodmodel_, animrodAttachNo_);
	MV1DetachAnim(rodmodel_, throwanimAttachNo_);

	currentrodAnimTime_ = 0.0f;
	currentkamaeAnimTime_ = 0.0f;
	currentthrowAnimTime_ = 0.0f;

	// モデルに指定時間のアニメーションを設定する
	MV1SetAttachAnimTime(rodmodel_, animrodAttachNo_, currentAnimTime_);

	ChangeState(STATE::SPAWN);
}

void Rod::Update(void)
{
	Player& player = Player::GetInstance();
	static Player::PlayerMode lastMode = player.GetCurrentMode();

	//プレイヤーモードが変更されたときに処理を行う
	if (lastMode != player.GetCurrentMode()) {
		lastMode = player.GetCurrentMode();

		if (lastMode == Player::PlayerMode::FIRST_PERSON) {
			//一人称になった時に竿を初期化
			rodFlg = true;
			rodanimFlg = false;

			//プレイヤーの向きを取得して竿の角度を設定
			rot_ = player.GetAngle();
			MV1SetRotationXYZ(rodmodel_, rot_);

			//竿の初期位置をセット
			currentAnimTime_ = 0.0f;
			animrodAttachNo_ = MV1AttachAnim(rodmodel_, 3, -1, -1);
			UpdateRodmodel();
		}
		else if(lastMode==Player::PlayerMode::THIRD_PERSON) {
			//三人称の時に竿を非表示
			rodFlg = false;
			rodanimFlg = false;

			//アニメーションをリセット
			MV1DetachAnim(rodmodel_, kamaerodAttachNo_);
			MV1DetachAnim(rodmodel_, animrodAttachNo_);
			MV1DetachAnim(rodmodel_, throwanimAttachNo_);

			currentAnimTime_ = 0.0f;
			currentkamaeAnimTime_ = 0.0f;
			currentthrowAnimTime_ = 0.0f;

			ChangeState(STATE::SPAWN);

			// ウキの状態もリセット
			Dobber::GetInstance().Reset();
		}
	}

	//竿が表示されていたら更新処理を行う
	if (rodFlg) {
		HandleInput();
		UpdateAnimation();
		RodProcess();
	}
}

void Rod::SpawnUpdate(void)
{
	if (rodanimFlg == true)
	{
		currentAnimTime_ += ANIM_SPEED;
		currentrodAnimTime_ += ANIM_SPEED;
		MV1SetAttachAnimTime(rodmodel_, animrodAttachNo_, currentAnimTime_);
	}

	if (currentAnimTime_ > animrodTotalTime_)
	{
		currentAnimTime_ = 0.0f;
		rodanimFlg = false;
	}
}

void Rod::KamaeUpdate(void)
{
	if (kamaeanimFlg)
	{
		currentkamaeAnimTime_ += KAMAEANIM_SPEED;
		MV1SetAttachAnimTime(rodmodel_, kamaerodAttachNo_, currentkamaeAnimTime_);

		if (currentkamaeAnimTime_ > kamaerodTotalTime_)
		{
			currentkamaeAnimTime_ = 0.0f;
			kamaeanimFlg = false;

			// アニメーション終了時のモデル回転を更新
			MV1SetRotationXYZ(rodmodel_, rot_);
		}
	}
}

void Rod::ThrowUpdate(void)
{
	if (throwanimFlg)
	{
		currentthrowAnimTime_ += THROWANIM_SPEED;
		MV1SetAttachAnimTime(rodmodel_, throwanimAttachNo_, currentthrowAnimTime_);

		if (currentthrowAnimTime_ > throwanimTotalTime_)
		{
			currentthrowAnimTime_ = 0.0f;
			throwanimFlg = false;

			// アニメーション終了時のモデル回転を更新
			MV1SetRotationXYZ(rodmodel_, rot_);
		}
	}
}

void Rod::AwaseUpdate(void)
{
}

void Rod::Draw(void)
{
	if (rodFlg) {
		MV1DrawModel(rodmodel_);
	}
}

void Rod::Release(void)
{
	MV1DeleteModel(rodmodel_);
}
void Rod::UpdateAnimation(void)
{
	switch (state_)
	{
	case Rod::STATE::SPAWN:
		SpawnUpdate();
		break;
	case Rod::STATE::KAMAE:
		KamaeUpdate();
		break;
	case Rod::STATE::THROW:
		ThrowUpdate();
		break;
	case Rod::STATE::AWASE:
		AwaseUpdate();
		break;
	default:
		break;
	}
}

void Rod::RodProcess(void)
{
	auto& input = InputManager::GetInstance();
	Player& player = Player::GetInstance();

	//三人称だったら竿を出現させない
	if (player.GetCurrentMode() == Player::PlayerMode::THIRD_PERSON) {
		return;
	}

	if ((!rodanimFlg) && input.IsTrgMouseRight()) {
		ToggleRod();
	}

	//竿が出ている場合はモデルを更新
	if (rodFlg == true) {
		UpdateAnimation();
		RodMove();
		UpdateRodmodel();
	}
}

void Rod::RodMove(void)
{
	//入力制御取得
	InputManager& ins = InputManager::GetInstance();

	//回転スピードをデグリー→ラジアンに変更
	float rotPow = ROT_SPEED_DEG * DX_PI_F / 180.0f;

	//上下左右キーでカメラの座標を変更する
	if (ins.IsNew(KEY_INPUT_RIGHT)) { rot_.y += rotPow; }
	if (ins.IsNew(KEY_INPUT_LEFT)) { rot_.y -= rotPow; }

	if (rot_.y >= 360.0f)
	{
		rot_.y -= 360.0f;
	}

	VECTOR playerpos_ = Player::GetInstance().GetPos();

	MATRIX rotMatrix = MGetRotY(rot_.y);

	VECTOR relativePos = { 30.0f,0.0f,100.0f };

	VECTOR newRodPos = VTransform(relativePos, rotMatrix);

	pos_ = VAdd(playerpos_, newRodPos);

	MV1SetPosition(rodmodel_, pos_);
	MV1SetRotationXYZ(rodmodel_, rot_);
}

void Rod::ChangeState(const STATE state)
{
	if (state_ == state)return;

	state_ = state;

	//現在のアニメーションをすべてデタッチさせる
	MV1DetachAnim(rodmodel_, kamaerodAttachNo_);
	MV1DetachAnim(rodmodel_, animrodAttachNo_);
	MV1DetachAnim(rodmodel_, throwanimAttachNo_);

	//アニメーション時間リセット
	currentAnimTime_ = 0.0f;
	currentkamaeAnimTime_ = 0.0f;
	currentthrowAnimTime_ = 0.0f;

	switch (state_)
	{
	case Rod::STATE::SPAWN:
		rodanimFlg = true;
		animrodAttachNo_ = MV1AttachAnim(rodmodel_, 3, -1, -1);
		break;
	case Rod::STATE::KAMAE:
		kamaeanimFlg = true;
		kamaerodAttachNo_ = MV1AttachAnim(rodmodel_, 2, -1, -1);
		break;
	case Rod::STATE::THROW:
		throwanimFlg = true;
		throwanimAttachNo_ = MV1AttachAnim(rodmodel_, 4, -1, -1);
		break;
	case Rod::STATE::AWASE:
		break;
	default:
		break;
	}
}

void Rod::HandleInput()
{
	auto& input = InputManager::GetInstance();

	if (input.IsTrgMouseRight()) {
		ToggleRod();
	}

	if (rodFlg && !rodanimFlg) {
		if (input.IsTrgDown(KEY_INPUT_SPACE)) {
			ChangeState(STATE::KAMAE);
		}
		else if (input.IsTrgMouseLeft()) {
			ChangeState(STATE::THROW);
		}
	}
}

void Rod::UpdateRodmodel()const
{
	MV1SetPosition(rodmodel_, pos_);
	MV1SetRotationXYZ(rodmodel_, rot_);
}

void Rod::ToggleRod()
{
	Player& player = Player::GetInstance();
	Player::PlayerMode currentMode = player.GetCurrentMode();

	//三人称だったら竿を表示させない
	if (currentMode == Player::PlayerMode::THIRD_PERSON) {
		return;
	}

	if (rodFlg) {
		//竿を収納する
		MV1DetachAnim(rodmodel_, kamaerodAttachNo_);

		rodFlg = false;
		rodanimFlg = false;
		kamaeanimFlg = false;
		throwanimFlg = false;

		//アニメーション時間をリセット
		currentAnimTime_ = 0.0f;
		currentkamaeAnimTime_ = 0.0f;
		currentthrowAnimTime_ = 0.0f;

		ChangeState(STATE::SPAWN);

		//ウキの状態をリセット
		Dobber::GetInstance().Reset();
	}
	else {
		//竿を出す
		rodFlg = true;
		rodanimFlg = true;

		//アニメーションリセット
		MV1DetachAnim(rodmodel_, kamaerodAttachNo_);
		MV1DetachAnim(rodmodel_, animrodAttachNo_);
		MV1DetachAnim(rodmodel_, throwanimAttachNo_);

		currentAnimTime_ = 0.0f;

		//プレイヤーの向きを取得して竿に設定する
		rot_ = player.GetAngle();
		MV1SetRotationXYZ(rodmodel_, rot_);

		//竿の出しアニメーションを設定
		animrodAttachNo_ = MV1AttachAnim(rodmodel_, 3, -1, -1);

		//状態をSpawnに変更
		ChangeState(STATE::SPAWN);
	}
}

void Rod::Reset(void)
{
	rodFlg = false;
}
