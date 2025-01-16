#include "Mathf.h"
#include <random>

float Mathf::Lerp(float a, float b, float t)
{
    return a * (1.0f - t) + (b * t);
}

//指定範囲内のランダム値を計算する
float Mathf::RandomRange(float min, float max)
{
    static std::random_device rd;                  // 乱数生成の種
    static std::mt19937 gen(rd());                 // メルセンヌ・ツイスタープロジェクター
    std::uniform_real_distribution<float> dis(min, max); // 指定範囲の乱数分布

    return dis(gen);
}
