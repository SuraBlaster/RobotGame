#pragma once

#include <wrl.h>
#include <d3d11.h>
#include <DirectXMath.h>

// 描画ターゲットバッファ
class RenderTarget
{
	// 描画関連
public: 
	void Render();
private:
	// 頂点データ
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

	// シェーダーリソースビュー取得
	const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& GetShaderResourceView(){ return shaderResourceView; }

	// レンダーターゲットビュー取得
	const Microsoft::WRL::ComPtr<ID3D11RenderTargetView>& GetRenderTargetView(){ return renderTargetView; }

	// テクスチャの情報取得
	D3D11_TEXTURE2D_DESC GetTexture2dDesc(){ return texture2dDesc; }

	// テクスチャ幅取得
	inline int GetWidth() const { return texture2dDesc.Width; }

	// テクスチャ高さ取得
	inline int GetHeight() const { return texture2dDesc.Height; }

private:
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	shaderResourceView;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>		renderTargetView;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView>		depthStencilView;
	D3D11_TEXTURE2D_DESC								texture2dDesc;
};

