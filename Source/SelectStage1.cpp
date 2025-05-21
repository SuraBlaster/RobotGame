#include "SelectStage1.h"
#include "StageManager.h"
#include "SceneSelect.h"
#include <Input/Input.h>
#include <Graphics/Graphics.h>
#include <Camera.h>

//�R���X�g���N�^
SelectStage1::SelectStage1()
{
    model = new Model("Data/Model/SelectStage/SelectMap1.mdl");

    scale.x = scale.y = scale.z = 0.005f;

    CursorFlag = false;
}

//�f�X�g���N�^
SelectStage1::~SelectStage1()
{
    //�X�e�[�W���f����j��
    delete model;
}

//�X�V����
void SelectStage1::Update(float elapsedTime)
{
	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();

	RenderContext rc;

	//�J���������ݒ�
	Camera& camera = Camera::Instance();
	rc.view = camera.GetView();
	rc.projection = camera.GetProjection();

	GetMouseToStage(dc, rc.view, rc.projection);

    CursorFlag = StageManager::Instance().GetCursorFlag1();

    if (CursorFlag)
    {
        turnSpeed = DirectX::XMConvertToRadians(3);
    }
    else
    {
        turnSpeed = DirectX::XMConvertToRadians(1);
   
    }
 
    angle.y += turnSpeed;
    
    position.y += sinf(angle.y * 0.5f) * 0.025f;

    UpdateTransform();

    model->UpdateTransform(transform);
}

void SelectStage1::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    //�V�F�[�_�[�Ƀ��f����`�悵�Ă��炤
    shader->Draw(dc, model);
}

bool SelectStage1::RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit)
{
    return Collision::IntersectRayVsModel(start, end, model, hit);
}

void SelectStage1::GetMouseToStage(ID3D11DeviceContext* dc, const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection)
{
    // �r���[�|�[�g�擾
    D3D11_VIEWPORT viewport;
    UINT numViewports = 1;
    dc->RSGetViewports(&numViewports, &viewport);

    // �s��ϊ��̏���
    DirectX::XMMATRIX View = DirectX::XMLoadFloat4x4(&view);
    DirectX::XMMATRIX Projection = DirectX::XMLoadFloat4x4(&projection);
    DirectX::XMMATRIX World = DirectX::XMMatrixIdentity();

    // �}�E�X���͎擾
    Mouse& mouse = Input::Instance().GetMouse();

    // �}�E�X�X�N���[�����W�iz=0 �� �j�A�N���b�v�ʁj
    DirectX::XMFLOAT3 screenPosition = {
        static_cast<float>(mouse.GetPositionX()),
        static_cast<float>(mouse.GetPositionY()),
        0.0f
    };

    // �j�A�� �� ���[���h���W
    DirectX::XMVECTOR nearPoint = DirectX::XMVector3Unproject(
        DirectX::XMLoadFloat3(&screenPosition),
        viewport.TopLeftX, viewport.TopLeftY,
        viewport.Width, viewport.Height,
        0.0f, 1.0f,
        Projection, View, World
    );

    // �t�@�[�ʁiz = 1.0�j�� ���[���h���W
    screenPosition.z = 1.0f;
    DirectX::XMVECTOR farPoint = DirectX::XMVector3Unproject(
        DirectX::XMLoadFloat3(&screenPosition),
        viewport.TopLeftX, viewport.TopLeftY,
        viewport.Width, viewport.Height,
        0.0f, 1.0f,
        Projection, View, World
    );

    // ���C�̎n�_�ƕ���
    DirectX::XMFLOAT3 start;
    DirectX::XMStoreFloat3(&start, nearPoint);

    DirectX::XMFLOAT3 endTemp;
    DirectX::XMStoreFloat3(&endTemp, farPoint);
    DirectX::XMFLOAT3 direction = {
        endTemp.x - start.x,
        endTemp.y - start.y,
        endTemp.z - start.z
    };

    // ���C�����𐳋K�����A������1000�ɉ���
    DirectX::XMVECTOR dirVec = DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&direction));
    DirectX::XMVECTOR startVec = DirectX::XMLoadFloat3(&start);
    DirectX::XMVECTOR extendedEndVec = DirectX::XMVectorAdd(startVec, DirectX::XMVectorScale(dirVec, 10000.0f));

    DirectX::XMFLOAT3 end;
    DirectX::XMStoreFloat3(&end, extendedEndVec);

    // �q�b�g����
    HitResult hit;
    if (RayCast(start, end, hit))
    {
        // �����������̏����i��F�X�P�[���ύX�j
        scale = { 0.006f, 0.006f, 0.006f };

        if (mouse.GetButtonDown() & Mouse::BTN_LEFT)
        { 
            StageManager::Instance().SetCursorFlag1(true);
        }
        
    }
    else
    {
        scale = { 0.005f, 0.005f, 0.005f };
    }
    
}