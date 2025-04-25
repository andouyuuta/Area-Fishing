#pragma once
#include <DxLib.h>
#include<random>

class Player;
class Rod;
class Fish;

class Dobber
{
public:
    //�E�L�̋���
    static constexpr float UKIDISTANCE = 3000.0f;
    // �C���X�^���X�̐���
    static void CreateInstance(void);
    // �C���X�^���X�̎擾
    static Dobber& GetInstance(void);
    // �R���X�g���N�^(���ƂȂ郂�f���̃n���h��ID)
    Dobber();
    // �f�X�g���N�^
    ~Dobber(void);
    // ������
    void Init(void);
    // �X�V�X�e�b�v
    void Update(void);
    // �`��
    void Draw(void);
    // �������
    void Release(void);
    void Reset(void);           //���Z�b�g
    [[nodiscard]] VECTOR GetAngle(void) const { return rot_; }	//�����̎擾
    [[nodiscard]] int GetUkiModel(void)const { return ukimodelId_; }
    [[nodiscard]] bool GetfishingFlg(void) const { return fishingflg; }
    [[nodiscard]] VECTOR GetUkiPos(void) const { return ukipos_; }
    [[nodiscard]] bool GetIsShot(void) const { return isShot; }
    [[nodiscard]] bool GetLandingFlg(void)const;

    void SetUkiPos(const VECTOR newPos);
    void SetFishingFlag(const bool flg) { fishingflg = flg; }
    void SetIsShotFlag(const bool flg) { isShot = flg; }
    void SetMoveSpeed(const float speed) { DOBBER_MOVE_SPEED = speed; }
    void SetVelocity(const VECTOR Velocity) { velocity_ = Velocity; }
    void SetLandingFlg(const bool flg) { landingflg = flg; }

private:
    // �ÓI�C���X�^���X
    static Dobber* instance_;
    // ���f��ID
    int ukimodelId_;            //�E�L
    bool isShot;                // �E�L�̎g�p�t���O
    bool fishingflg = false;    //�ނ�Ă��邩
    bool landingflg;
    // �e�̔��˃X�s�[�h
    float DOBBER_MOVE_SPEED = 8.0f;
    // ���W
    VECTOR ukipos_;
    // �E�L�̊p�x
    VECTOR rot_;
    // �����l��ς��ĂȂ��ړ��x�N�g��
    VECTOR basicMoveVec;
    // ��]�����ړ��x�N�g��
    VECTOR rotMoveVec;
    // �s���]�����ړ��x�N�g��
    VECTOR transformMoveVec;
    //�d��
    VECTOR gravity_;
    //���x
    VECTOR velocity_;
};