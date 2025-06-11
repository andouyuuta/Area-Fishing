#include "../Manager/InputManager.h"
#include "Player.h"
#include "Rod.h"
#include "Dobber.h"

namespace
{
	// アニメーション速度
	constexpr float SPAWN_ANIM_SPEED = 6.0f;
	constexpr float KAMAE_ANIM_SPEED = 5.0f;
	constexpr float THROW_ANIM_SPEED = 4.0f;
	constexpr float ANIM_ROD_SPEED = 10.0f;

	// 回転・速度
	constexpr float HALF_ROTATION_DEG = 180.0f;				// 半回転（度数）
	constexpr float FULL_ROTATION_RAD = 2.0f * DX_PI_F;		// 一回転（ラジアン）
	constexpr float ROT_SPEED_DEG = 1.0f;					// 回転速度

	// 竿の相対座標
	constexpr VECTOR ROD_RELATIVE_POS = { 30.0f, 0.0f, 100.0f };

	// モデル関連
	constexpr VECTOR DEFAULT_SCALE = { 0.25f, 0.25f, -0.25f };
	constexpr const char* ROD_MODEL_PATH = "Data/Model/Stage/rod3.mv1";

	// アニメーションIDと速度の対応テーブル
	const std::map<Rod::AnimationType, int> ANIM_INDEX_MAP = 
	{
		{ Rod::AnimationType::RodSpawn, 3 },
		{ Rod::AnimationType::RodKamae, 2 },
		{ Rod::AnimationType::RodThrow, 4 },
	};

	const std::map<Rod::AnimationType, float> ANIM_SPEED_MAP = 
	{
		{ Rod::AnimationType::RodSpawn, SPAWN_ANIM_SPEED },
		{ Rod::AnimationType::RodKamae, KAMAE_ANIM_SPEED },
		{ Rod::AnimationType::RodThrow, THROW_ANIM_SPEED },
	};
}

Rod::Rod(void)
	:state_(STATE::SPAWN),
	rodModel_(-1),
	isDraw_(false),
	scale_{ DEFAULT_SCALE },
	pos_{ 0.0f, 0.0f, 0.0f },
	rot_{ 0.0f, 0.0f, 0.0f }
{
}

Rod::~Rod(void)
{
}

void Rod::Init(Player* player,Dobber* dobber)
{
	player_ = player;
	dobber_ = dobber;

	rodModel_ = MV1LoadModel(ROD_MODEL_PATH);
	MV1SetScale(rodModel_, scale_);
	MV1SetPosition(rodModel_, pos_);
	MV1SetRotationXYZ(rodModel_, rot_);

	// アニメーションの初期化
	for (auto& [type, animIndex] : ANIM_INDEX_MAP)
	{
		AnimationData data{};
		data.attachIndex_ = MV1AttachAnim(rodModel_, animIndex, -1, -1);
		data.totalTime_ = MV1GetAttachAnimTotalTime(rodModel_, data.attachIndex_);
		data.currentTime_ = 0.0f;
		data.isPlaying_ = false;
		animations_[type] = data;
	}

	ChangeState(STATE::SPAWN);
}

void Rod::Update(void)
{
	static Player::PlayerMode lastMode = player_->GetCurrentMode();

	// プレイヤーモードが変更されたときに処理を行う
	if (lastMode != player_->GetCurrentMode()) 
	{
		lastMode = player_->GetCurrentMode();

		if (lastMode == Player::PlayerMode::FIRST_PERSON) 
		{
			// 一人称になった時に竿を初期化
			isDraw_ = true;

			// プレイヤーの向きを取得して竿の角度を設定
			rot_ = player_->GetAngle();
			MV1SetRotationXYZ(rodModel_, rot_);

			// 竿の初期位置をセット
			for (auto& [animType, animData] : animations_)
			{
				if (animData.attachIndex_ >= 0)
					MV1DetachAnim(rodModel_, animData.attachIndex_);
				animData.attachIndex_ = MV1AttachAnim(rodModel_, static_cast<int>(animType), -1, -1);
				animData.currentTime_ = 0.0f;
				animData.isPlaying_ = false;
			}

			ChangeState(STATE::SPAWN);
		}
		else if(lastMode==Player::PlayerMode::THIRD_PERSON) 
		{
			// 三人称の時に竿を非表示
			isDraw_ = false;

			// すべてのアニメーションをデタッチして停止
			for (auto& [_, animData] : animations_)
			{
				if (animData.attachIndex_ >= 0)
					MV1DetachAnim(rodModel_, animData.attachIndex_);
				animData.isPlaying_ = false;
				animData.currentTime_ = 0.0f;
			}

			ChangeState(STATE::SPAWN);

			//  ウキの状態もリセット
			dobber_->Reset();
		}
	}

	// 竿が表示されていたら更新処理を行う
	if (isDraw_) 
	{
		HandleInput();
		UpdateAnimation();
		RodProcess();
	}
}

void Rod::Draw(void)
{
	if (isDraw_) 
	{
		MV1DrawModel(rodModel_);
	}
}

void Rod::Release(void)
{
	for (auto& [type, animData] : animations_)
	{
		MV1DetachAnim(rodModel_, animData.attachIndex_);
	}
	MV1DeleteModel(rodModel_);
}

// アニメーション更新
void Rod::UpdateAnimation(void)
{
	for (auto& [animType, animData] : animations_)
	{
		if (!animData.isPlaying_)
			continue;

		float speed = ANIM_SPEED_MAP.at(animType);  // 速度マップから再生速度取得
		animData.currentTime_ += speed;

		if (animData.currentTime_ >= animData.totalTime_)
		{
			animData.currentTime_ = animData.totalTime_;
			animData.isPlaying_ = false;
		}
		MV1SetAttachAnimTime(rodModel_, animData.attachIndex_, animData.currentTime_);
	}
}

// 竿全体の動き
void Rod::RodProcess(void)
{
	auto& input = InputManager::GetInstance();

	// 三人称だったら竿を出現させない
	if (player_->GetCurrentMode() == Player::PlayerMode::THIRD_PERSON) 
	{
		return;
	}

	if (!IsCurrentAnimationPlaying() && input.IsTrgMouseRight()) 
	{
		ToggleRod();
	}

	// 竿が出ている場合はモデルを更新
	if (isDraw_) 
	{
		UpdateAnimation();
		RodMove();
		MV1SetPosition(rodModel_, pos_);
		MV1SetRotationXYZ(rodModel_, rot_);
	}
}

void Rod::RodMove(void)
{
	// 入力制御取得
	InputManager& ins = InputManager::GetInstance();

	// 回転スピードをデグリー→ラジアンに変更
	float rotPow = ROT_SPEED_DEG * DX_PI_F / HALF_ROTATION_DEG;

	// 上下左右キーでカメラの座標を変更する
	if (ins.IsNew(KEY_INPUT_RIGHT))  rot_.y += rotPow;
	if (ins.IsNew(KEY_INPUT_LEFT)) rot_.y -= rotPow;

	// 360度以上になったら戻す
	if (rot_.y >= FULL_ROTATION_RAD)
	{
		rot_.y -= FULL_ROTATION_RAD;
	}
	else if (rot_.y < 0.0f)
	{
		rot_.y += FULL_ROTATION_RAD;
	}

	// プレイヤーの位置を取得
	const VECTOR playerPos = player_->GetPos();

	// Y軸回転行列を作成
	const MATRIX rotMatrix = MGetRotY(rot_.y);

	// ロッドの新しい位置 = プレイヤー位置 + 回転行列適用した相対位置
	const VECTOR newRodPos = VTransform(ROD_RELATIVE_POS, rotMatrix);

	pos_ = VAdd(playerPos, newRodPos);

	// モデルの座標と回転を設定
	MV1SetPosition(rodModel_, pos_);
	MV1SetRotationXYZ(rodModel_, rot_);
}

void Rod::ChangeState(STATE state)
{
	if (state_ == state)
		return;

	// まず全アニメーション停止
	for (auto& [_, animData] : animations_)
	{
		animData.isPlaying_ = false;
		animData.currentTime_ = 0.0f;
		MV1DetachAnim(rodModel_, animData.attachIndex_);
		animData.attachIndex_ = -1; // 忘れずにリセット
	}

	state_ = state;

	// 状態に応じてアニメーション再生開始
	AnimationType playAnim;
	switch (state_)
	{
	case STATE::SPAWN: playAnim = AnimationType::RodSpawn; break;
	case STATE::KAMAE: playAnim = AnimationType::RodKamae; break;
	case STATE::THROW: playAnim = AnimationType::RodThrow; break;
	default: return;
	}

	animations_[playAnim].attachIndex_ = MV1AttachAnim(rodModel_, ANIM_INDEX_MAP.at(playAnim), -1, -1);
	animations_[playAnim].isPlaying_ = true;
	animations_[playAnim].currentTime_ = 0.0f;
}

void Rod::HandleInput()
{
	auto& input = InputManager::GetInstance();

	if (input.IsTrgMouseRight()) 
	{
		ToggleRod();
	}

	if (isDraw_ && !IsCurrentAnimationPlaying())
	{
		if (input.IsTrgDown(KEY_INPUT_SPACE)) 
		{
			ChangeState(STATE::KAMAE);
		}
		else if (input.IsTrgMouseLeft()) 
		{
			ChangeState(STATE::THROW);
		}
	}
}

void Rod::ToggleRod()
{
	Player::PlayerMode currentMode = player_->GetCurrentMode();

	// 三人称だったら竿を表示させない
	if (currentMode == Player::PlayerMode::THIRD_PERSON) 
	{
		return;
	}

	if (isDraw_) 
	{
		for (auto& [_, animData] : animations_)
		{
			if (animData.attachIndex_ >= 0)
				MV1DetachAnim(rodModel_, animData.attachIndex_);
			animData.isPlaying_ = false;
			animData.currentTime_ = 0.0f;
		}

		ChangeState(STATE::SPAWN);

		// ウキの状態をリセット
		dobber_->Reset();
	}
	else 
	{
		// 竿を出す
		isDraw_ = true;

		rot_ = player_->GetAngle();
		MV1SetRotationXYZ(rodModel_, rot_);

		for (auto& [animType, animData] : animations_)
		{
			animData.attachIndex_ = MV1AttachAnim(rodModel_, static_cast<int>(animType), -1, -1);
			animData.currentTime_ = 0.0f;
			animData.isPlaying_ = false;
		}

		// 状態をSpawnに変更
		ChangeState(STATE::SPAWN);
	}
}

bool Rod::IsCurrentAnimationPlaying() const
{
	switch (state_)
	{
	case STATE::SPAWN:
		return animations_.at(AnimationType::RodSpawn).isPlaying_;
	case STATE::KAMAE:
		return animations_.at(AnimationType::RodKamae).isPlaying_;
	case STATE::THROW:
		return animations_.at(AnimationType::RodThrow).isPlaying_;
	default:
		return false;
	}
}

void Rod::Reset(void)
{
	isDraw_ = false;
}
