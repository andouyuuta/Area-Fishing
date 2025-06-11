#pragma once
#include <random>
#include <DxLib.h>

class Random 
{
public:
    // �V���O���g���C���X�^���X�̎擾
    static Random& GetInstance() {
        static Random instance; // �ÓI�C���X�^���X
        return instance;
    }

    // float �^�̃����_���l���擾 (�����Z���k�c�C�X�^)
    float GetRandF(float min, float max) {
        std::uniform_real_distribution<float> dist(min, max);
        return dist(mt);
    }

    // rand() ���g�p���������_���l�擾
    float GetRandY(float min, float max) {
        return min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (max - min)));
    }

private:
    // �R���X�g���N�^ (�O������C���X�^���X���s��)
    Random() : mt(rd()) {}

    // �R�s�[�E���[�u���֎~
    Random(const Random&) = delete;
    Random& operator=(const Random&) = delete;

    std::random_device rd;
    std::mt19937 mt; // �����Z���k�c�C�X�^
};


