#include "Weapon.h"
#include "Graphics/Graphics.h"
#include "WeaponManager.h"

WeaponManager* manager{};

void Weapon::DrawDebugPrimitive()
{
    DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

    //����Փ˗p�̃f�o�b�O����`��
    debugRenderer->DrawSphere(position, radius, DirectX::XMFLOAT4(0, 0, 0, 1));

    //����Փ˗p�̃f�o�b�O�~����`��
    debugRenderer->DrawCylinder(position, radius, height, DirectX::XMFLOAT4(0, 0, 0, 1));

}

void Weapon::Destroy()
{
    manager->Instance().Remove(this);
}