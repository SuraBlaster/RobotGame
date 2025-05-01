#pragma once

#include <wrl.h>
#include <d3d11.h>
#include <DirectXMath.h>

// �`��^�[�Q�b�g�o�b�t�@
class RenderTarget
{
	// �`��֘A
public: 
	void Render();
private:
	// ���_�f�[�^
	struct Vertex
	{
		DirectX::XMFLOAT3	position;
		DirectX::XMFLOAT2	texcoord;
	};
	Microsoft::WRL::ComPtr<ID3D11InputLayout>	inputLayout;
	Microsoft::WRL::ComPtr<ID3D11Buffer>		vertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11VertexShader>	vertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>	pixelShader;

public:
	ID3D11RenderTargetView* org_rtv[4];
	ID3D11DepthStencilView* org_dsv;
	void Begin();
	void End();
	void SetPSSlot(int slot);

public:
	RenderTarget(ID3D11Device* device, UINT width, UINT height, DXGI_FORMAT format, bool depth = false);
	~RenderTarget() {}

	// �V�F�[�_�[���\�[�X�r���[�擾
	const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& GetShaderResourceView(){ return shaderResourceView; }

	// �����_�[�^�[�Q�b�g�r���[�擾
	const Microsoft::WRL::ComPtr<ID3D11RenderTargetView>& GetRenderTargetView(){ return renderTargetView; }

	// �e�N�X�`���̏��擾
	D3D11_TEXTURE2D_DESC GetTexture2dDesc(){ return texture2dDesc; }

	// �e�N�X�`�����擾
	inline int GetWidth() const { return texture2dDesc.Width; }

	// �e�N�X�`�������擾
	inline int GetHeight() const { return texture2dDesc.Height; }

private:
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	shaderResourceView;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>		renderTargetView;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView>		depthStencilView;
	D3D11_TEXTURE2D_DESC								texture2dDesc;
};

