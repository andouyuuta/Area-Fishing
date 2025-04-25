#include "EffekseerEffect.h"
#include"EffekseerForDXLib.h"

EffekseerEffect::EffekseerEffect(void)
{
}

EffekseerEffect::~EffekseerEffect(void)
{
}

void EffekseerEffect::Init(void)
{
	spawnTimer = 0;

	//�G�t�F�N�V�A�ɕK�v�ȏ�����
	//DirectX11���g�p����悤�ɂ���
	//Effekseer���g�p����ɂ͕K���ݒ肷��
	SetUseDirect3DVersion(DX_DIRECT3D_11);

	//Effekseer������������
	//�����ɂ͂����ʂɕ\������ő�p�[�e�B�N������ݒ肷��
	if (Effekseer_Init(8000) == -1)
	{
		DxLib_End();
		return;
	}

	//�t���X�N���[���E�B���h�E�̐؂�ւ��Ń��\�[�X��������̂�h��
	//Effekseerwo �g�p����ꍇ�͕K���ݒ肷��B
	SetChangeScreenModeGraphicsSystemResetFlag(FALSE);

	//DX���C�u�����̃f�o�C�X���X�Ƃ������̃R�[���o�b�N��ݒ肷��
	//�E�B���h�E�ƃt���X�N���[���̐؂�ւ�����������ꍇ�͕K�����s����
	Effekseer_SetGraphicsDeviceLostCallbackFunctions();

	//�G�t�F�N�g�̓ǂݍ���
	effectResourceHandle = LoadEffekseerEffect("Data/Effect/river.efkefc", 10.0f);
	effectResourceHandle2 = LoadEffekseerEffect("Data/Effect/river.efkefc", 10.0f);
	effectResourceHandle3 = LoadEffekseerEffect("Data/Effect/river.efkefc", 10.0f);
	effectResourceHandle4 = LoadEffekseerEffect("Data/Effect/Falls.efkefc", 10.0f);
	effectResourceHandle5 = LoadEffekseerEffect("Data/Effect/Falls.efkefc", 10.0f);
	effectResourceHandle6 = LoadEffekseerEffect("Data/Effect/mist.efkefc", 10.0f);
	effectResourceHandle7 = LoadEffekseerEffect("Data/Effect/river.efkefc", 10.0f);
	effectResourceHandle8 = LoadEffekseerEffect("Data/Effect/river.efkefc", 10.0f);
	effectResourceHandle9 = LoadEffekseerEffect("Data/Effect/river.efkefc", 10.0f);
	effectResourceHandle10 = LoadEffekseerEffect("Data/Effect/mist.efkefc", 10.0f);
	effectResourceHandle11 = LoadEffekseerEffect("Data/Effect/Falls.efkefc", 10.0f);
	effectResourceHandle12 = LoadEffekseerEffect("Data/Effect/Falls.efkefc", 10.0f);

	//�G�t�F�N�g���Đ�����
	//PlayEffekseer3DEffect(effectResourceHandle);
	playingEffectHandle = PlayEffekseer3DEffect(effectResourceHandle);			//��G�t�F�N�g�P�̃n���h��
	playingEffectHandle2 = PlayEffekseer3DEffect(effectResourceHandle2);		//��G�t�F�N�g�Q�̃n���h��
	playingEffectHandle3 = PlayEffekseer3DEffect(effectResourceHandle3);		//��G�t�F�N�g�R�̃n���h��
	playingEffectHandle4 = PlayEffekseer3DEffect(effectResourceHandle4);		//��G�t�F�N�g�S�̃n���h��
	playingEffectHandle5 = PlayEffekseer3DEffect(effectResourceHandle5);		//��G�t�F�N�g�T�̃n���h��
	playingEffectHandle6 = PlayEffekseer3DEffect(effectResourceHandle6);		//��ڃG�t�F�N�g�U�̃n���h��
	//playingEffectHandle7 = PlayEffekseer3DEffect(effectResourceHandle7);		//��ڃG�t�F�N�g�U�̃n���h��
	//playingEffectHandle8 = PlayEffekseer3DEffect(effectResourceHandle8);		//��ڃG�t�F�N�g�U�̃n���h��
	//playingEffectHandle9 = PlayEffekseer3DEffect(effectResourceHandle9);		//��ڃG�t�F�N�g�U�̃n���h��
	//playingEffectHandle10 = PlayEffekseer3DEffect(effectResourceHandle10);		//��ڃG�t�F�N�g�U�̃n���h��
	//playingEffectHandle11 = PlayEffekseer3DEffect(effectResourceHandle11);		//��ڃG�t�F�N�g�U�̃n���h��
	//playingEffectHandle12 = PlayEffekseer3DEffect(effectResourceHandle12);		//��ڃG�t�F�N�g�U�̃n���h��

	//��P�̑傫���E�ʒu�E�����̐ݒ�
	SetPosPlayingEffekseer3DEffect(playingEffectHandle, 800.0f,/* DX_PI_F / 2*/0.0f, 20000.0f);
	SetRotationPlayingEffekseer3DEffect(playingEffectHandle, 0.0f, DX_PI_F / 2.0f, 0.0f);
	SetScalePlayingEffekseer3DEffect(playingEffectHandle, 10.0f, 10.0f, 12.0f);
	//��Q�̑傫���E�ʒu�E�����̐ݒ�
	SetPosPlayingEffekseer3DEffect(playingEffectHandle2, -800.0f,/* DX_PI_F / 2*/0.0f, 20000.0f);
	SetRotationPlayingEffekseer3DEffect(playingEffectHandle2, 0.0f, DX_PI_F / 2.0f, 0.0f);
	SetScalePlayingEffekseer3DEffect(playingEffectHandle2, 10.0f, 10.0f, 12.0f);
	//��R�̑傫���E�ʒu�E�����̐ݒ�
	SetPosPlayingEffekseer3DEffect(playingEffectHandle3, -800.0f,/* DX_PI_F / 2*/0.0f, 20000.0f);
	SetRotationPlayingEffekseer3DEffect(playingEffectHandle3, 0.0f, DX_PI_F / 2.0f, 0.0f);
	SetScalePlayingEffekseer3DEffect(playingEffectHandle3, 10.0f, 10.0f, 12.0f);
	//��S�̑傫���E�ʒu�E�����̐ݒ�
	SetPosPlayingEffekseer3DEffect(playingEffectHandle4, -200.0f,/* DX_PI_F / 2*/10000.0f, 21000.0f);
	SetRotationPlayingEffekseer3DEffect(playingEffectHandle4, 0.0f, DX_PI_F / 2.0f, 0.0f);
	SetScalePlayingEffekseer3DEffect(playingEffectHandle4, 15.0f, 30.0f, 12.0f);
	//��T�̑傫���E�ʒu�E�����̐ݒ�
	SetPosPlayingEffekseer3DEffect(playingEffectHandle5, -200.0f,/* DX_PI_F / 2*/10000.0f, 21000.0f);
	SetRotationPlayingEffekseer3DEffect(playingEffectHandle5, 0.0f, DX_PI_F / 2.0f, 0.0f);
	SetScalePlayingEffekseer3DEffect(playingEffectHandle5, 15.0f, 30.0f, 12.0f);
	//��T�̑傫���E�ʒu�E�����̐ݒ�
	SetPosPlayingEffekseer3DEffect(playingEffectHandle6, 0.0f,/* DX_PI_F / 2*/100.0f, 21000.0f);
	SetRotationPlayingEffekseer3DEffect(playingEffectHandle6, 0.0f, DX_PI_F / 2.0f, 0.0f);
	SetScalePlayingEffekseer3DEffect(playingEffectHandle6, 15.0f, 10.0f, 15.0f);



	//��P�̑傫���E�ʒu�E�����̐ݒ�
	SetPosPlayingEffekseer3DEffect(playingEffectHandle7, 800.0f,/* DX_PI_F / 2*/0.0f, 22000.0f);
	SetRotationPlayingEffekseer3DEffect(playingEffectHandle7, 0.0f, DX_PI_F / 2.0f, 0.0f);
	SetScalePlayingEffekseer3DEffect(playingEffectHandle7, 10.0f, 10.0f, 12.0f);
	//��Q�̑傫���E�ʒu�E�����̐ݒ�
	SetPosPlayingEffekseer3DEffect(playingEffectHandle8, -800.0f,/* DX_PI_F / 2*/0.0f, 22000.0f);
	SetRotationPlayingEffekseer3DEffect(playingEffectHandle8, 0.0f, DX_PI_F / 2.0f, 0.0f);
	SetScalePlayingEffekseer3DEffect(playingEffectHandle8, 10.0f, 10.0f, 12.0f);
	//��R�̑傫���E�ʒu�E�����̐ݒ�
	SetPosPlayingEffekseer3DEffect(playingEffectHandle9, -800.0f,/* DX_PI_F / 2*/0.0f, 22000.0f);
	SetRotationPlayingEffekseer3DEffect(playingEffectHandle9, 0.0f, DX_PI_F / 2.0f, 0.0f);
	SetScalePlayingEffekseer3DEffect(playingEffectHandle9, 10.0f, 10.0f, 12.0f);

	//��S�̑傫���E�ʒu�E�����̐ݒ�
	SetPosPlayingEffekseer3DEffect(playingEffectHandle11, 200.0f,/* DX_PI_F / 2*/0.0f, 21000.0f);
	SetRotationPlayingEffekseer3DEffect(playingEffectHandle11, 0.0f, DX_PI_F / 2.0f, 0.0f);
	SetScalePlayingEffekseer3DEffect(playingEffectHandle11, 5.0f, 10.0f, 12.0f);
	//��T�̑傫���E�ʒu�E�����̐ݒ�
	SetPosPlayingEffekseer3DEffect(playingEffectHandle12, -200.0f,/* DX_PI_F / 2*/100.0f, 21000.0f);
	SetRotationPlayingEffekseer3DEffect(playingEffectHandle12, 0.0f, DX_PI_F / 2.0f, 0.0f);
	SetScalePlayingEffekseer3DEffect(playingEffectHandle12, 5.0f, 10.0f, 12.0f);
	//��T�̑傫���E�ʒu�E�����̐ݒ�
	SetPosPlayingEffekseer3DEffect(playingEffectHandle10, 0.0f,/* DX_PI_F / 2*/100.0f, 21000.0f);
	SetRotationPlayingEffekseer3DEffect(playingEffectHandle10, 0.0f, DX_PI_F / 2.0f, 0.0f);
	SetScalePlayingEffekseer3DEffect(playingEffectHandle10, 15.0f, 10.0f, 15.0f);
}

void EffekseerEffect::Update(void)
{
	//DX���C�u�����̃J������Effekseer�̃J�����̓���������
	Effekseer_Sync3DSetting();

	//Effekseer�ɂ��Đ����̃G�t�F�N�g���X�V����
	UpdateEffekseer3D();

	//��P�̃G�t�F�N�g���Đ����I������������x�Đ�
	if (IsEffekseer3DEffectPlaying(playingEffectHandle) != 0)
	{
		playingEffectHandle = PlayEffekseer3DEffect(effectResourceHandle);

		//�傫���E�ʒu�E�����̍Đݒ�
		SetScalePlayingEffekseer3DEffect(playingEffectHandle, 20.0f, 15.0f, 20.0f);
		SetRotationPlayingEffekseer3DEffect(playingEffectHandle, 0.0f, DX_PI_F / 2.0f, 0.0f);
		SetPosPlayingEffekseer3DEffect(playingEffectHandle, 1000.0f,-162.0f, 14000.0f);
	}

	//��Q�̃G�t�F�N�g���Đ����I������������x�Đ�
	if (IsEffekseer3DEffectPlaying(playingEffectHandle2) != 0)
	{
		playingEffectHandle2 = PlayEffekseer3DEffect(effectResourceHandle2);

		//�傫���E�ʒu�E�����̍Đݒ�
		SetScalePlayingEffekseer3DEffect(playingEffectHandle2, 20.0f, 15.0f, 20.0f);
		SetRotationPlayingEffekseer3DEffect(playingEffectHandle2, 0.0f, DX_PI_F / 2.0f, 0.0f);
		SetPosPlayingEffekseer3DEffect(playingEffectHandle2, -1000.0f, -162.0f, 14000.0f);
	}

	//��R�̃G�t�F�N�g���Đ����I������������x�Đ�
	if (IsEffekseer3DEffectPlaying(playingEffectHandle3) != 0)
	{
		playingEffectHandle3 = PlayEffekseer3DEffect(effectResourceHandle3);

		//�傫���E�ʒu�E�����̍Đݒ�
		SetScalePlayingEffekseer3DEffect(playingEffectHandle3, 20.0f, 15.0f, 20.0f);
		SetRotationPlayingEffekseer3DEffect(playingEffectHandle3, 0.0f, DX_PI_F / 2.0f, 0.0f);
		SetPosPlayingEffekseer3DEffect(playingEffectHandle3, 0.0f, -162.0f, 14000.0f);
	}
	//��S�̃G�t�F�N�g���Đ����I������������x�Đ�
	if (IsEffekseer3DEffectPlaying(playingEffectHandle4) != 0)
	{
		playingEffectHandle4 = PlayEffekseer3DEffect(effectResourceHandle4);

		//�傫���E�ʒu�E�����̍Đݒ�
		SetScalePlayingEffekseer3DEffect(playingEffectHandle4, 35.0f, 35.0f, 35.0f);
		SetRotationPlayingEffekseer3DEffect(playingEffectHandle4, 0.0f, DX_PI_F / 2.0f, 0.0f);
		SetPosPlayingEffekseer3DEffect(playingEffectHandle4, 200.0f, 355.0f, 15000.0f);
	}
	//��T�̃G�t�F�N�g���Đ����I������������x�Đ�
	if (IsEffekseer3DEffectPlaying(playingEffectHandle5) != 0)
	{
		playingEffectHandle5 = PlayEffekseer3DEffect(effectResourceHandle5);

		//�傫���E�ʒu�E�����̍Đݒ�
		SetScalePlayingEffekseer3DEffect(playingEffectHandle5, 35.0f, 35.0f, 30.0f);
		SetRotationPlayingEffekseer3DEffect(playingEffectHandle5, 0.0f, DX_PI_F / 2.0f, 0.0f);
		SetPosPlayingEffekseer3DEffect(playingEffectHandle5, -200.0f, 355.0f, 15000.0f);
	}
	//��T�̃G�t�F�N�g���Đ����I������������x�Đ�
	if (IsEffekseer3DEffectPlaying(playingEffectHandle6) != 0)
	{
		playingEffectHandle6 = PlayEffekseer3DEffect(effectResourceHandle6);

		//�傫���E�ʒu�E�����̍Đݒ�
		SetScalePlayingEffekseer3DEffect(playingEffectHandle6, 15.0f, 25.0f, 30.0f);
		SetRotationPlayingEffekseer3DEffect(playingEffectHandle6, 0.0f, DX_PI_F / 2.0f, 0.0f);
		SetPosPlayingEffekseer3DEffect(playingEffectHandle6, 0.0f, 5.0f, 15000.0f);
	}

	if (IsEffekseer3DEffectPlaying(playingEffectHandle7) != 0)
	{
		spawnTimer++;
		if (spawnTimer < spawnInterval) {
			return;
		}
		playingEffectHandle7 = PlayEffekseer3DEffect(effectResourceHandle7);

		//�傫���E�ʒu�E�����̍Đݒ�
		SetScalePlayingEffekseer3DEffect(playingEffectHandle7, 25.0f, 25.0f, 20.0f);
		SetRotationPlayingEffekseer3DEffect(playingEffectHandle7, 0.0f, DX_PI_F / 2.0f, 0.0f);
		SetPosPlayingEffekseer3DEffect(playingEffectHandle7, 1000.0f, -165.0f,16000.0f);
	}

	//��Q�̃G�t�F�N�g���Đ����I������������x�Đ�
	if (IsEffekseer3DEffectPlaying(playingEffectHandle8) != 0)
	{
		spawnTimer++;
		if (spawnTimer < spawnInterval) {
			return;
		}
		playingEffectHandle8 = PlayEffekseer3DEffect(effectResourceHandle8);

		//�傫���E�ʒu�E�����̍Đݒ�
		SetScalePlayingEffekseer3DEffect(playingEffectHandle8, 25.0f, 25.0f, 20.0f);
		SetRotationPlayingEffekseer3DEffect(playingEffectHandle8, 0.0f, DX_PI_F / 2.0f, 0.0f);
		SetPosPlayingEffekseer3DEffect(playingEffectHandle8, -1000.0f, -165.0f, 15000.0f);
	}

	//��R�̃G�t�F�N�g���Đ����I������������x�Đ�
	if (IsEffekseer3DEffectPlaying(playingEffectHandle9) != 0)
	{
		spawnTimer++;
		if (spawnTimer < spawnInterval) {
			return;
		}
		playingEffectHandle9 = PlayEffekseer3DEffect(effectResourceHandle9);

		//�傫���E�ʒu�E�����̍Đݒ�
		SetScalePlayingEffekseer3DEffect(playingEffectHandle9, 25.0f, 25.0f, 20.0f);
		SetRotationPlayingEffekseer3DEffect(playingEffectHandle9, 0.0f, DX_PI_F / 2.0f, 0.0f);
		SetPosPlayingEffekseer3DEffect(playingEffectHandle9, 0.0f, -165.0f, 15000.0f);
	}
	if (IsEffekseer3DEffectPlaying(playingEffectHandle10) != 0)
	{
		spawnTimer++;
		if (spawnTimer < spawnInterval2) {
			return;
		}
		playingEffectHandle10 = PlayEffekseer3DEffect(effectResourceHandle10);

		//�傫���E�ʒu�E�����̍Đݒ�
		SetScalePlayingEffekseer3DEffect(playingEffectHandle10, 35.0f, 30.0f, 30.0f);
		SetRotationPlayingEffekseer3DEffect(playingEffectHandle10, 0.0f, DX_PI_F / 2.0f, 0.0f);
		SetPosPlayingEffekseer3DEffect(playingEffectHandle10, 0.0f, 5.0f, 15000.0f);
	}

	//��S�̃G�t�F�N�g���Đ����I������������x�Đ�
	if (IsEffekseer3DEffectPlaying(playingEffectHandle11) != 0)
	{
		spawnTimer++;
		if (spawnTimer < spawnInterval) {
			return;
		}
		playingEffectHandle11 = PlayEffekseer3DEffect(effectResourceHandle11);

		//�傫���E�ʒu�E�����̍Đݒ�
		SetScalePlayingEffekseer3DEffect(playingEffectHandle11, 35.0f, 35.0f, 35.0f);
		SetRotationPlayingEffekseer3DEffect(playingEffectHandle11, 0.0f, DX_PI_F / 2.0f, 0.0f);
		SetPosPlayingEffekseer3DEffect(playingEffectHandle11, 200.0f, -165.0f, 15000.0f);
	}
	//��T�̃G�t�F�N�g���Đ����I������������x�Đ�
	if (IsEffekseer3DEffectPlaying(playingEffectHandle12) != 0)
	{
		spawnTimer++;
		if (spawnTimer < spawnInterval) {
			return;
		}
		playingEffectHandle12 = PlayEffekseer3DEffect(effectResourceHandle12);

		//�傫���E�ʒu�E�����̍Đݒ�
		SetScalePlayingEffekseer3DEffect(playingEffectHandle12, 35.0f, 35.0f, 35.0f);
		SetRotationPlayingEffekseer3DEffect(playingEffectHandle12, 0.0f, DX_PI_F / 2.0f, 0.0f);
		SetPosPlayingEffekseer3DEffect(playingEffectHandle12, -200.0f, -165.0f, 15500.0f);
	}
	//��T�̃G�t�F�N�g���Đ����I������������x�Đ�

}

void EffekseerEffect::Draw(void)
{
	DrawEffekseer3D();
}

void EffekseerEffect::Release(void)
{
	DeleteEffekseerEffect(effectResourceHandle);
	DeleteEffekseerEffect(effectResourceHandle2);
	DeleteEffekseerEffect(effectResourceHandle3);
	DeleteEffekseerEffect(effectResourceHandle4);
	DeleteEffekseerEffect(effectResourceHandle5);
	DeleteEffekseerEffect(effectResourceHandle6);
	DeleteEffekseerEffect(effectResourceHandle7);
	DeleteEffekseerEffect(effectResourceHandle8);
	DeleteEffekseerEffect(effectResourceHandle9);
	DeleteEffekseerEffect(effectResourceHandle10);
	DeleteEffekseerEffect(effectResourceHandle11);
	DeleteEffekseerEffect(effectResourceHandle12);





	Effkseer_End();
}
