#include <stdlib.h>
#include "RenderTarget.h"
#include "Graphics.h"
#include "GpuResourceUtils.h"
#include "Misc.h"
#include "DefaultLitShader.h"

void RenderTarget::Begin()
{
	ID3D11DeviceContext* dc = Graphics::Instance().GetDeviceContext();

	// ��ʃN���A�������_�[�^�[�Q�b�g�ݒ�
	ID3D11RenderTargetView* rtv[] = {
		GetRenderTargetView().Get(),
	};

	// �W���̃^�[�Q�b�g�擾
	dc->OMGetRenderTargets(4, org_rtv, &org_dsv);
	if (org_dsv == nullptr) return;

	// �g�p����f�v�X�o�b�t�@
	ID3D11DepthStencilView* dsv = org_dsv;
	if (depthStencilView.Get() != nullptr) dsv = depthStencilView.Get();

	// �N���A
	FLOAT color[] = { 0.0f, 0.7f, 1.0f, 1.0f };	// RGBA(0.0�`1.0)
	dc->ClearRenderTargetView(rtv[0], color);
	dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	dc->OMSetRenderTargets(1, rtv, dsv);

	// �r���[�|�[�g�̐ݒ�
	D3D11_VIEWPORT	vp = {};
	vp.Width = static_cast<float>(GetWidth());
	vp.Height = static_cast<float>(GetHeight());
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	dc->RSSetViewports(1, &vp);
}

void RenderTarget::End()
{
	ID3D11DeviceContext* dc = Graphics::Instance().GetDeviceContext();
	dc->OMSetRenderTargets(4, org_rtv, org_dsv);
}

// �V�F�[�_�[�X���b�g�ɓo�^
void RenderTarget::SetPSSlot(int slot)
{
	ID3D11DeviceContext* dc = Graphics::Instance().GetDeviceContext();
	dc->PSSetShaderResources(slot, 1, shaderResourceView.GetAddressOf());
}

// �`����s
void RenderTarget::Render()
{
	ID3D11DeviceContext* dc = Graphics::Instance().GetDeviceContext();

	// �T���v���X�e�[�g�ݒ�
	ID3D11SamplerState* samplerStates[] =
	{
		Graphics::Instance().GetRenderState()->GetSamplerState(SamplerState::LinearWrap)
	};
	dc->PSSetSamplers(0, _countof(samplerStates), samplerStates);

	// GPU�ɕ`�悷�邽�߂̃f�[�^��n��
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	dc->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);
	dc->IASetInputLayout(inputLayout.Get());
	dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	dc->VSSetShader(vertexShader.Get(), nullptr, 0);
	dc->PSSetShader(pixelShader.Get(), nullptr, 0);
	dc->PSSetShaderResources(0, 1, shaderResourceView.GetAddressOf());

	// �`��
	dc->Draw(4, 0);

	// slot����͂���
	ID3D11ShaderResourceView* dummy = nullptr;
	dc->PSSetShaderResources(0, 1, &dummy);
}




RenderTarget::RenderTarget(ID3D11Device* device, UINT width, UINT height, DXGI_FORMAT format, bool depth)
{
	texture2dDesc = {};
	texture2dDesc.Width = width;
	texture2dDesc.Height = height;
	texture2dDesc.MipLevels = 1;
	texture2dDesc.ArraySize = 1;
	texture2dDesc.Format = format;
	texture2dDesc.SampleDesc.Count = 1;
	texture2dDesc.SampleDesc.Quality = 0;
	texture2dDesc.Usage = D3D11_USAGE_DEFAULT;
	texture2dDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	texture2dDesc.CPUAccessFlags = 0;
	texture2dDesc.MiscFlags = 0;

	Microsoft::WRL::ComPtr<ID3D11Texture2D>	texture2d;
	HRESULT hr = device->CreateTexture2D( &texture2dDesc, 0, texture2d.ReleaseAndGetAddressOf() );
	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

	D3D11_SHADER_RESOURCE_VIEW_DESC shader_resource_view_desc	= {};
	shader_resource_view_desc.Format = texture2dDesc.Format;
	shader_resource_view_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shader_resource_view_desc.Texture2D.MipLevels = 1;
	hr = device->CreateShaderResourceView(texture2d.Get(), &shader_resource_view_desc, shaderResourceView.ReleaseAndGetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

	D3D11_RENDER_TARGET_VIEW_DESC render_target_view_desc{};
	render_target_view_desc.Format = texture2dDesc.Format;
	render_target_view_desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	hr = device->CreateRenderTargetView(texture2d.Get(), &render_target_view_desc, renderTargetView.ReleaseAndGetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));


	depthStencilView = nullptr;
	if (depth)
	{
		// �[�x�X�e���V�������������ނ��߂̃e�N�X�`�����쐬����B
		Microsoft::WRL::ComPtr<ID3D11Texture2D> texture2d;
		D3D11_TEXTURE2D_DESC texture2dDesc;
		texture2dDesc.Width = width;
		texture2dDesc.Height = height;
		texture2dDesc.MipLevels = 1;
		texture2dDesc.ArraySize = 1;
		texture2dDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		texture2dDesc.SampleDesc.Count = 1;
		texture2dDesc.SampleDesc.Quality = 0;
		texture2dDesc.Usage = D3D11_USAGE_DEFAULT;
		texture2dDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		texture2dDesc.CPUAccessFlags = 0;
		texture2dDesc.MiscFlags = 0;
		hr = device->CreateTexture2D(&texture2dDesc, nullptr, texture2d.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

		// �[�x�X�e���V���e�N�X�`���ւ̏������݂ɑ����ɂȂ�[�x�X�e���V���r���[���쐬����B
		hr = device->CreateDepthStencilView(texture2d.Get(), nullptr, depthStencilView.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}

	// �`��p�f�[�^(�X�N���[�����WSTRIP)
	Vertex v[4] = {
		{ {-1, 1, 0}, {0, 0} },
		{ { 1, 1, 0}, {1, 0} },
		{ {-1,-1, 0}, {0, 1} },
		{ { 1,-1, 0}, {1, 1} },
	};
	// ���_�o�b�t�@�̐���
	{
		// ���_�o�b�t�@���쐬���邽�߂̐ݒ�I�v�V����
		D3D11_BUFFER_DESC buffer_desc = {};
		buffer_desc.ByteWidth = sizeof(Vertex) * 4;
		buffer_desc.Usage = D3D11_USAGE_DYNAMIC;
		buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		buffer_desc.MiscFlags = 0;
		buffer_desc.StructureByteStride = 0;
		D3D11_SUBRESOURCE_DATA data = {};
		data.pSysMem = v;

		// ���_�o�b�t�@�I�u�W�F�N�g�̐���
		hr = device->CreateBuffer(&buffer_desc, &data, vertexBuffer.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}

	// ���_�V�F�[�_�[
	{
		// ���̓��C�A�E�g
		D3D11_INPUT_ELEMENT_DESC inputElementDesc[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		hr = GpuResourceUtils::LoadVertexShader(
			device,
			"Shader/PostEffectVS.cso",
			inputElementDesc,
			ARRAYSIZE(inputElementDesc),
			inputLayout.GetAddressOf(),
			vertexShader.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

	}

	// �s�N�Z���V�F�[�_�[
	{
		hr = GpuResourceUtils::LoadPixelShader(
			device,
			"Shader/PostEffectPS.cso",
			pixelShader.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}

}
