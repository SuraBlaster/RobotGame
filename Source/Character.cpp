#include "Character.h"
#include "Mathf.h"
#include "StageManager.h"
//行列更新処理
void Character::UpdateTransform()
{
    //スケール行列を作成
    DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
    //回転行列を作成
    //DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);
    DirectX::XMMATRIX X = DirectX::XMMatrixRotationX(angle.x);
    DirectX::XMMATRIX Y = DirectX::XMMatrixRotationY(angle.y);
    DirectX::XMMATRIX Z = DirectX::XMMatrixRotationZ(angle.z);
    DirectX::XMMATRIX R = Y * X * Z;
    //位置行列を作成
    DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
    //3つの行列を組み合わせ、ワールド行列を作成
    DirectX::XMMATRIX W = S * R * T;
    //計算したワールド座標を取り出す
    DirectX::XMStoreFloat4x4(&transform, W);
}

bool Character::ApplyDamage(int damege)
{

    //死亡している場合は健康状態を変更しない
    if (health <= 0)return false;

    if (invincibleTime <= 0)
    {
        health -= damege;
        invincibleTime += 0.5f;
    }

    if (health <= 0)
    {
        OnDead();
    }
    else
    {
        OnDamaged();
    }

    return true;
}

void Character::AddImpulse(const DirectX::XMFLOAT3& impulse)
{
    velocity.x += impulse.x;
    velocity.y += impulse.y;
    velocity.z += impulse.z;

}

void Character::Move(float vx, float vz, float speed)
{
    // 移動処理
   /* speed *= elapsedTime;
    position.x += vx * speed;
    position.z += vz * speed;*/

    moveVecX = vx;
    moveVecZ = vz;

    maxMoveSpeed = speed;

}

void Character::Turn(float elapsedTime, float vx, float vz, float speed)
{
    speed *= elapsedTime;

    //長さが小さい場合は計算しない
    float length = sqrtf(vx * vx + vz * vz);
    if (length < 0.001f)return;

    //単位ベクトル化
    vx /= length;
    vz /= length;

    //前方向を求める
    float frontX = sinf(angle.y);
    float frontZ = cosf(angle.y);

    //内積
    float dot = (vx * frontX) + (vz * frontZ);

    //補正値
    float rot = 1.0 - dot;

    //回転速度調整
    //speed *= rot;
    if (rot > speed)rot = speed;

    //単位ベクトルの外積
    float cross = (vz * frontX) - (vx * frontZ);

    if (cross < 0.0f)
    {
        //angle.y += speed;
        angle.y += rot;
    }
    else
    {
        //angle.y -= speed;
        angle.y -= rot;
    }

}

//ジャンプ処理
void Character::Jump(float speed)
{
    velocity.y += speed;
}

//速度更新処理
void Character::UpdateVelocity(float elapsedTime)
{
    //経過フレーム
    float elapsedFrame = 60.0f * elapsedTime;

    //垂直速力更新処理
    UpdateVerticalVelocity(elapsedFrame);

    //水平走力更新処理
    UpdateHorizontalVelocity(elapsedFrame);

    //垂直移動更新処理
    UpdateVerticalMove(elapsedTime);

    //水平移動更新処理
    UpdateHorizontalMove(elapsedTime);
}

void Character::UpdateInvincibleTimer(float elapsedTime)
{
    if (invincibleTime > 0.0f)
    {
        invincibleTime -= elapsedTime;
    }
}

void Character::UpdateVerticalVelocity(float elapsedFrame)
{
    //重力処理
    velocity.y += gravity * elapsedFrame;
}

void Character::UpdateVerticalMove(float elapsedTime)
{
    float my = velocity.y * elapsedTime;

    DirectX::XMFLOAT3 normal = { 0,1,0 };

    if (my < 0.0f)
    {
        DirectX::XMFLOAT3 start = { position.x, position.y + stepOffset, position.z };

        DirectX::XMFLOAT3 end = { position.x, position.y + my, position.z };

        HitResult hit;
        if (StageManager::Instance().RayCast(start, end, hit))
        {
            position = hit.position;
            normal = hit.normal;
            


            if (!isGround)
            {
                OnLanding();
                
            }
            isGround = true;
            velocity.y = 0.0f;

            angle.x += hit.rotation.x;
            angle.y += hit.rotation.y;
            angle.z += hit.rotation.z;

        }
        else
        {
            position.y += my;
            isGround = false;
        }

        
    }
    else if (my > 0.0f)
    {
        position.y += my;
        isGround = false;
    }

    //地面の向きに沿うようにXZ軸回転
    {
        //Y軸が法線ベクトル方向に向くようにオイラー角回転を算出する
        DirectX::XMFLOAT3 test = { angle.x,angle.y,angle.z };

        test.x = atan2(normal.z, normal.y);
        test.z = atan2(-normal.x, normal.y);

    
        //線形補完で滑らかに回転する
        angle.x = Mathf::Lerp(angle.x, test.x, 0.1f);
        angle.z = Mathf::Lerp(angle.z, test.z, 0.1f);

    }
}

void Character::UpdateHorizontalVelocity(float elapsedFrame)
{

    float length = sqrtf(velocity.x * velocity.x + velocity.z * velocity.z);

    if (length > 0.0f)
    {
        float friction = this->friction * elapsedFrame;

        //空中にいるときは摩擦力を減らす
        //if (!&IsGround) friction *= airControl;

        if (length > friction)
        {
            float vx = velocity.x / length;
            float vz = velocity.z / length;

            velocity.x -= vx * friction;
            velocity.z -= vz * friction;
        }
        else
        {
            velocity.x = 0.0f;
            velocity.z = 0.0f;
        }
    }

    //XZ平面の走力加速
    if (length <= maxMoveSpeed)
    {
        float moveVecLength = sqrtf(moveVecX * moveVecX + moveVecZ * moveVecZ);

        if (moveVecLength > 0.0f)
        {
            //加速力
            float acceleration = this->acceleration * elapsedFrame;

            //空中にいるときは加速力を減らす
            //if (!&IsGround) friction *= airControl;

            //移動ベクトルによる加速処理
            velocity.x += moveVecX * acceleration;
            velocity.z += moveVecZ * acceleration;

            //最大速度制限
            float length = sqrtf(velocity.x * velocity.x + velocity.z * velocity.z);

            if (length > maxMoveSpeed)
            {
                float vx = velocity.x / length;
                float vz = velocity.z / length;

                velocity.x = vx * maxMoveSpeed;
                velocity.z = vz * maxMoveSpeed;
            }
        }
    }
    moveVecX = 0.0f;
    moveVecZ = 0.0f;
}

void Character::UpdateHorizontalMove(float elapsedTime)
{
    /*position.x += velocity.x * elapsedTime;
    position.z += velocity.z * elapsedTime;*/

    //水平速力量計算
    float velocityLengthXZ = (velocity.x * velocity.x + velocity.z * velocity.z);
    if (velocityLengthXZ > 0.0f)
    {
        //水平移動値
        float mx = velocity.x * elapsedTime;
        float mz = velocity.z * elapsedTime;

        //レイの開始位置と終点位置
        DirectX::XMFLOAT3 start = { position.x,position.y + stepOffset,position.z };
        DirectX::XMFLOAT3 end = { position.x + mx,position.y,position.z + mz };

        //レイキャストによる壁判定
        HitResult hit;
        if (StageManager::Instance().RayCast(start, end, hit))
        {
            //壁までのベクトル
            DirectX::XMVECTOR Start = DirectX::XMLoadFloat3(&hit.position);
            DirectX::XMVECTOR End = DirectX::XMLoadFloat3(&end);
            DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(End,Start);

            //壁の法線
            DirectX::XMVECTOR Normal = DirectX::XMLoadFloat3(&hit.normal);

            //入射ベクトルを法線に射影
            DirectX::XMVECTOR Dot = DirectX::XMVector3Dot(Vec, DirectX::XMVectorNegate(Normal));

            //補正位置の計算
            /*DirectX::XMVECTOR P = DirectX::XMLoadFloat3(&hit.position);

            DirectX::XMVECTOR PB = DirectX::XMVectorSubtract(End, P);

            DirectX::XMVECTOR N = DirectX::XMVector3Normalize(Normal);

            DirectX::XMVECTOR a = DirectX::XMVector3Dot(PB, N);

            DirectX::XMVECTOR R = */

            float a = DirectX::XMVectorGetX(Dot);
            a *= 1.1;

            DirectX::XMVECTOR R = DirectX::XMVectorAdd(Vec, DirectX::XMVectorScale(Normal, a));

            DirectX::XMVECTOR O = DirectX::XMVectorAdd(End, DirectX::XMVectorScale(Normal, a));

            DirectX::XMFLOAT3 o;
            DirectX::XMStoreFloat3(&o, O);
            position.x = o.x;
            position.z = o.z;

        }
        else
        {
            //移動
            position.x += mx;
            position.z += mz;
        }
    }


}
