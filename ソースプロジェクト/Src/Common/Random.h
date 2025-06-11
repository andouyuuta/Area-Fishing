#pragma once
#include <random>
#include <DxLib.h>

class Random 
{
public:
    // シングルトンインスタンスの取得
    static Random& GetInstance() {
        static Random instance; // 静的インスタンス
        return instance;
    }

    // float 型のランダム値を取得 (メルセンヌツイスタ)
    float GetRandF(float min, float max) {
        std::uniform_real_distribution<float> dist(min, max);
        return dist(mt);
    }

    // rand() を使用したランダム値取得
    float GetRandY(float min, float max) {
        return min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (max - min)));
    }

private:
    // コンストラクタ (外部からインスタンス化不可)
    Random() : mt(rd()) {}

    // コピー・ムーブを禁止
    Random(const Random&) = delete;
    Random& operator=(const Random&) = delete;

    std::random_device rd;
    std::mt19937 mt; // メルセンヌツイスタ
};


