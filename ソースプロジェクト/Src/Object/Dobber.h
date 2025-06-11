#pragma once
#include <DxLib.h>
#include <random>

class Player;
class Rod;
class FishManager;
class Stage;

class Dobber
{
public:
    static constexpr float UKIDISTANCE = 3000.0f;

    // �R���X�g���N�^
    Dobber(void);
    // �f�X�g���N�^
    ~Dobber(void);
    void Init(Player* player, Rod* rod, FishManager* fishmng, Stage* stage);		// ����������
    void Update(void);		// �X�V����
    void Draw(void);		// �`�揈��
    void Release(void);		// �������
    void Reset(void);       // ���Z�b�g
    [[nodiscard]] int GetUkiModel(void)const { return ukiModelId_; }        // ���f���̎擾
    [[nodiscard]] bool GetFishingFlg(void) const { return isFishing; }      // �ނ�Ă��邩
    [[nodiscard]] VECTOR GetUkiPos(void) const { return ukiPos_; }          // ���W�̎擾
    [[nodiscard]] bool GetIsShot(void) const { return isShot_; }            // �V���b�g������
    [[nodiscard]] bool GetLandingFlg(void)const;                            // �n�ʂɂ�����
    [[nodiscard]] float GetUkiDistance(void)const { return UKIDISTANCE; }

    void SetUkiPos(const VECTOR newPos);
    void SetFishingFlg(const bool flg) { isFishing = flg; }
    void SetLandingFlg(const bool flg) { isLanding = flg; }

private:

    int ukiModelId_;            // �E�L���f��
    bool isShot_;               // �E�L���g�p���Ă��邩
    bool isFishing;             // �ނ�Ă��邩
    bool isLanding;             // �������Ă��邩
    float moveSpeed_;           // �e�̔��˃X�s�[�h

    VECTOR ukiPos_;             // ���W
    VECTOR rot_;                // �E�L�̊p�x
    VECTOR scale_;              // �E�L�̑傫��
    VECTOR basicMoveVec_;       // �����l��ς��ĂȂ��ړ��x�N�g��
    VECTOR rotMoveVec_;         // ��]�����ړ��x�N�g��
    VECTOR transformMoveVec_;   // �s���]�����ړ��x�N�g��
    VECTOR gravity_;            // �d��
    VECTOR velocity_;           // ���x

    Player* player_;
    Stage* stage_;
    Rod* rod_;
    FishManager* fishmng_;
};