#pragma once

#include <DirectXMath.h>

class Character
{
public:
    Character() = default;
    virtual ~Character() = default;

    void UpdateTransform();

    //位置取得
    const DirectX::XMFLOAT3& GetPosition() const { return position; }

    //位置設定
    void SetPosition(const DirectX::XMFLOAT3& position) { this->position = position; }

    //回転取得
    const DirectX::XMFLOAT3& GetAngle() const { return angle; }

    //回転設定
    void SetAngle(const DirectX::XMFLOAT3& angle) { this->angle = angle; }

    //スケール設定
    const DirectX::XMFLOAT3& GetScale() const { return scale; }

    //スケール取得
    void SetScale(const DirectX::XMFLOAT3& scale) { this->scale = scale; }

    float GetRadius() const { return radius; }

    //地面に設置しているか
    bool IsGround() const { return isGround; }

    //ダメージを与える
    bool ApplyDamage(int damege);

    //高さ取得
    float GetHeight() const { return height; }

    //衝撃を与える
    void AddImpulse(const DirectX::XMFLOAT3& impulse);

    //健康状態を取得
    int GetHealth() const { return health; }

    //最大健康状態を取得
    int GetMaxHealth() const { return maxHealth; }

protected:
    //移動処理
    void Move(float vx, float vz, float speed);

    //回転処理
    void Turn(float elapsedTime, float vx, float vz, float speed);

    //ジャンプ処理
    void Jump(float speed);

    //走力処理更新
    void UpdateVelocity(float elapsedTime);

    void UpdateInvincibleTimer(float elapsedTime);
    //着地したときに呼ばれる
    virtual void OnLanding() {}

    //ダメージを受けた時に呼ばれる
    virtual void OnDamaged(){}

    //死亡時に呼ばれる
    virtual void OnDead(){}

private:
    //垂直速力更新処理
    void UpdateVerticalVelocity(float elapsedFrame);

    //垂直移動更新処理
    void UpdateVerticalMove(float elapsedTime);

    //水平速力更新処理
    void UpdateHorizontalVelocity(float elapsedFrame);

    //水平移動更新処理
    void UpdateHorizontalMove(float elapsedTime);
protected:
    DirectX::XMFLOAT3 position = { 0, 0, 0 };
    DirectX::XMFLOAT3 angle    = { 0, 0, 0 };
    DirectX::XMFLOAT3 scale    = { 1, 1, 1 };
    DirectX::XMFLOAT4X4 transform{
        1,0,0,0,
        0,1,0,0,
        0,0,1,0,
        0,0,0,1
    };
    float radius = 0.5f;
    
    float gravity = -1.0f;

    DirectX::XMFLOAT3 velocity = { 0,0,0 };

    bool isGround = false;

    float height = 2.0f;

    int health = 5;

    int maxHealth = 5;

    float invincibleTimer = 1.0f;

    float invincibleTime = 0.0f;

    float friction = 0.5f;

    float acceleration = 1.0f;

    float maxMoveSpeed = 5.0f;

    float moveVecX = 0.0f;

    float moveVecZ = 0.0f;

    float airControl = 0.3f;

    float stepOffset = 1.0f;

    float slopeRate = 1.0f;
};



