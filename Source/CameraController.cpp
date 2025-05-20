//#include "CameraController.h"
//#include "Camera.h"
//#include "Input/Input.h"
//
//
//void CameraController::Update(float elapsedTime)
//{
//    GamePad& gamePad = Input::Instance().GetGamePad();
//    float ax = gamePad.GetAxisRX();
//    float ay = gamePad.GetAxisRY();
//
//    float speed = rollSpeed * elapsedTime;
//
//    if (gamePad.GetAxisRX())
//    {
//       //X����]����
//       angle.y += ax * speed;
//    }
//
//    if (gamePad.GetAxisRY())
//    {
//        //Y����]����
//        angle.x += ay * speed;
//    }
//
//    if (angle.x > maxAngleX)
//    {
//        angle.x = maxAngleX;
//    }
//
//    if (angle.x < minAngleX)
//    {
//        angle.x = minAngleX;
//    }
//
//    if (angle.y < -DirectX::XM_PI)
//    {
//        angle.y += DirectX::XM_2PI;
//    }
//
//    if (angle.y > DirectX::XM_PI)
//    {
//        angle.y -= DirectX::XM_2PI;
//    }
//
//    DirectX::XMMATRIX Transform = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);
//
//    DirectX::XMVECTOR Front = Transform.r[2];
//    DirectX::XMFLOAT3 front;
//    DirectX::XMStoreFloat3(&front,Front);
//
//    DirectX::XMFLOAT3 eye;
//    eye.x = target.x - front.x * range;
//    eye.y = target.y - front.y * range;
//    eye.z = target.z - front.z * range;
//
//    Camera::Instance().SetLookAt(eye, target, DirectX::XMFLOAT3(0, 1, 0));
//}

#include "CameraController.h"
#include "Camera.h"
#include "Input/Input.h"
#include <Player.h>

void CameraController::Update(float elapsedTime)
{
    if (cameraEffect_Death.IsActive()) {
        cameraEffect_Death.Update(elapsedTime);
        return;
    }

    if (cameraEffect_Clear.IsActive()) {
        cameraEffect_Clear.Update(elapsedTime);
        return;
    }



    Mouse& mouse = Input::Instance().GetMouse();
    float ax = static_cast<float>(mouse.GetPositionX() - mouse.GetOldPositionX());
    float ay = static_cast<float>(mouse.GetPositionY() - mouse.GetOldPositionY());

    if (isZeroClear)
    {
        ax = ay = 0;
        isZeroClear = false;
    }

    //X,y���̉�]
    float sensitive = 0.001f;
    angle.y -= static_cast<float>(ax) * sensitive;
    angle.x -= static_cast<float>(ay) * sensitive;

    if (angle.x < maxAngleX)
    {
        angle.x = maxAngleX;
    }
    if (angle.x > minAngleX)
    {
        angle.x = minAngleX;
    }

    if (angle.y < -DirectX::XM_PI)
    {
        angle.y += DirectX::XM_2PI;
    }
    if (angle.y > DirectX::XM_PI)
    {
        angle.y -= DirectX::XM_2PI;
    }

    //�J�����̉�]�l����]�s��ɕϊ�
    DirectX::XMMATRIX transform = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);

    //��]�s�񂩂�O�����x�N�g�������o��
    DirectX::XMVECTOR Front = transform.r[2];
    DirectX::XMFLOAT3 front;
    DirectX::XMStoreFloat3(&front, Front);

    //���ӓ_����x�N�g�������Ɉ�苗�����ꂽ�J�������_�����߂�
    DirectX::XMFLOAT3 eye;
    eye.x = target.x + front.x * range;
    eye.y = target.y + front.y * range;
    eye.z = target.z + front.z * -range;

    //�J�������C�L���X�g
   /* if (!Debug::instance().getIsDebugMode())
    {
        DirectX::XMFLOAT3 start = target;
        DirectX::XMFLOAT3 end = { eye.x + front.x,eye.y + front.y,eye.z - front.z };
        HitResult hit;
        if (Stage::Instance().rayCast(start, end, hit))
        {
            range -= DirectX::XMVectorGetX(DirectX::XMVector3Length(
                DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&hit.position), DirectX::XMLoadFloat3(&end))));
            if (range < 0.001f)
            {
                range = 0.001f;
            }
            eye.x = target.x + front.x * range;
            eye.y = target.y + front.y * range;
            eye.z = target.z + front.z * -range;
        }
        else
        {
            _range = _beforRange;
        }
    }*/
    Camera::Instance().SetLookAt(eye, target, DirectX::XMFLOAT3(0, 1, 0));
}

void CameraController::StartDeath()
{
    // ���݂̎��_�ƃ^�[�Q�b�g��ۑ����ăX�^�[�g
    Player& player = Player::Instance();
    cameraEffect_Death.Start({player.GetPosition()},Camera::Instance().GetEye() );
}

void CameraController::StartClear()
{
    // ���݂̎��_�ƃ^�[�Q�b�g��ۑ����ăX�^�[�g
    Player& player = Player::Instance();
    cameraEffect_Clear.Start({player.GetPosition()},Camera::Instance().GetEye() );
}


//void CameraController::UpdateNormal(float elapsedTime)
//{
//    GamePad& gamePad = Input::instance().getGamePad();
//
//    float ax = gamePad.GetAxisRX();
//    float ay = gamePad.GetAxisRY();
//    //X,y���̉�]
//    float speed = _normalRollSpeed * elapsedTime;
//    _angle.y -= ax * speed;
//    _angle.x += ay * speed;
//
//    if (_angle.x < _maxAngleX)
//    {
//        _angle.x = _maxAngleX;
//    }
//    if (_angle.x > _minAngleX)
//    {
//        _angle.x = _minAngleX;
//    }
//
//    if (_angle.y < -DirectX::XM_PI)
//    {
//        _angle.y += DirectX::XM_2PI;
//    }
//    if (_angle.y > DirectX::XM_PI)
//    {
//        _angle.y -= DirectX::XM_2PI;
//    }
//
//    //�J�����̉�]�l����]�s��ɕϊ�
//    DirectX::XMMATRIX transform = DirectX::XMMatrixRotationRollPitchYaw(_angle.x, _angle.y, _angle.z);
//
//    //��]�s�񂩂�O�����x�N�g�������o��
//    DirectX::XMVECTOR Front = transform.r[2];
//    DirectX::XMFLOAT3 front;
//    DirectX::XMStoreFloat3(&front, Front);
//
//    //���ӓ_����x�N�g�������Ɉ�苗�����ꂽ�J�������_�����߂�
//    DirectX::XMFLOAT3 eye;
//    eye.x = _target.x + front.x * _range;
//    eye.y = _target.y + front.y * _range;
//    eye.z = _target.z + front.z * -_range;
//
//    //�J�������C�L���X�g
//    if (!Debug::instance().getIsDebugMode())
//    {
//        DirectX::XMFLOAT3 start = _target;
//        DirectX::XMFLOAT3 end = { eye.x + front.x,eye.y + front.y,eye.z - front.z };
//        HitResult hit;
//        if (Stage::instance().rayCast(start, end, hit))
//        {
//            _range -= DirectX::XMVectorGetX(DirectX::XMVector3Length(
//                DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&hit.position), DirectX::XMLoadFloat3(&end))));
//            //�J�������߂Â��߂��Ȃ��悤��
//            if (_range < 0.001f)
//            {
//                _range = 0.001f;
//            }
//            eye.x = _target.x + front.x * _range;
//            eye.y = _target.y + front.y * _range;
//            eye.z = _target.z + front.z * -_range;
//        }
//        else
//        {
//            _range = _beforRange;
//        }
//    }
//
//    Camera::instance().setLookAt(eye, _target, DirectX::XMFLOAT3(0, 1, 0));
//}
//
//void CameraController::updateNormal(float elapsedTime, float range)
//{
//    GamePad& gamePad = Input::instance().getGamePad();
//
//    float ax = gamePad.GetAxisRX();
//    float ay = gamePad.GetAxisRY();
//    //X,y���̉�]
//    float speed = _normalRollSpeed * elapsedTime;
//    _angle.y -= ax * speed;
//    _angle.x += ay * speed;
//
//    if (_angle.x < _maxAngleX)
//    {
//        _angle.x = _maxAngleX;
//    }
//    if (_angle.x > _minAngleX)
//    {
//        _angle.x = _minAngleX;
//    }
//
//    if (_angle.y < -DirectX::XM_PI)
//    {
//        _angle.y += DirectX::XM_2PI;
//    }
//    if (_angle.y > DirectX::XM_PI)
//    {
//        _angle.y -= DirectX::XM_2PI;
//    }
//
//    //�J�����̉�]�l����]�s��ɕϊ�
//    DirectX::XMMATRIX transform = DirectX::XMMatrixRotationRollPitchYaw(_angle.x, _angle.y, _angle.z);
//
//    //��]�s�񂩂�O�����x�N�g�������o��
//    DirectX::XMVECTOR Front = transform.r[2];
//    DirectX::XMFLOAT3 front;
//    DirectX::XMStoreFloat3(&front, Front);
//
//    //���ӓ_����x�N�g�������Ɉ�苗�����ꂽ�J�������_�����߂�
//    DirectX::XMFLOAT3 eye;
//    eye.x = _target.x + front.x * range;
//    eye.y = _target.y + front.y * range;
//    eye.z = _target.z + front.z * -range;
//
//    //�J�������C�L���X�g
//    if (!Debug::instance().getIsDebugMode())
//    {
//        DirectX::XMFLOAT3 start = _target;
//        DirectX::XMFLOAT3 end = { eye.x + front.x,eye.y + front.y,eye.z - front.z };
//        HitResult hit;
//        if (Stage::instance().rayCast(start, end, hit))
//        {
//            range -= DirectX::XMVectorGetX(DirectX::XMVector3Length(
//                DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&hit.position), DirectX::XMLoadFloat3(&end))));
//            //�J�������߂Â��߂��Ȃ��悤��
//            if (range < 0.001f)
//            {
//                range = 0.001f;
//            }
//            eye.x = _target.x + front.x * range;
//            eye.y = _target.y + front.y * range;
//            eye.z = _target.z + front.z * -range;
//        }
//        else
//        {
//            range = _beforRange;
//        }
//    }
//
//    Camera::instance().setLookAt(eye, _target, DirectX::XMFLOAT3(0, 1, 0));
//}

//void CameraController::DrawDebugGUI()
//{
//#ifdef USE_IMGUI
//    ImGui::SetNextWindowPos(ImVec2(961, 201), ImGuiCond_FirstUseEver);
//    ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);
//    if (ImGui::Begin("CameraContollor", nullptr, ImGuiWindowFlags_None))
//    {
//        if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
//        {
//            //�ʒu
//            //ImGui::InputFloat3("Target", &target.x);
//            ImGui::SliderFloat3("Target", &_target.x, -100, +100);
//            //��]
//            DirectX::XMFLOAT3 a;
//            a.x = DirectX::XMConvertToDegrees(_angle.x);
//            a.y = DirectX::XMConvertToDegrees(_angle.y);
//            a.z = DirectX::XMConvertToDegrees(_angle.z);
//            ImGui::InputFloat3("angle", &a.x);
//            _angle.x = DirectX::XMConvertToRadians(a.x);
//            _angle.y = DirectX::XMConvertToRadians(a.y);
//            _angle.z = DirectX::XMConvertToRadians(a.z);
//
//            //�X�P�[��
//            //ImGui::InputFloat3("scale", &scale.x);
//        }
//    }
//    ImGui::End();
//#endif
//}
