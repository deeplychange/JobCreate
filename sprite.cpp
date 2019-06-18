#include <direct.h>
#include "WICTextureLoader.h"
#include "sprite.h"
#include "ResourceManger.h"
#include "Blend.h"
#include "Game_lib.h"

inline  DirectX::XMFLOAT4 toNDColor(UINTCOLOR a_inColor)
{
	float red, green, blue, alph;
	/*red		= (a_inColor >> 24 & 0xFF) / 255.0f;
	green	= (a_inColor >> 16 & 0xFF) / 255.0f;
	blue	= (a_inColor >> 8 & 0xFF) / 255.0f;
	alph	= (a_inColor & 0xFF) / 255.0f;*/

	/*red = a_inColor >> 24 & 0xFF;
	green = a_inColor >> 16 & 0xFF;
	blue = a_inColor >> 8 & 0xFF;
	alph = a_inColor & 0xFF;
	red /= 255.0f;
	green /= 255.0f;
	blue /= 255.0f;
	alph /= 255.0f;*/

	red = (a_inColor & 0xFF000000) / (float)0xFF000000;
	green = (a_inColor & 0xFF0000) / (float)0xFF0000;
	blue = (a_inColor & 0xFF00) / (float)0xFF00;
	alph = (a_inColor & 0xFF) / (float)0xFF;

	return DirectX::XMFLOAT4(red, green, blue, alph);
}

sprite::sprite(ID3D11Device* Device, const wchar_t* texName) {


	HRESULT hr = S_OK;
	//4-1
	//Create buffer
	vertex vertices[] =
	{
		{ DirectX::XMFLOAT3(-0.5, +0.5, 0),DirectX::XMFLOAT4(1, 1, 1, 1),DirectX::XMFLOAT2(0,0) },
		{ DirectX::XMFLOAT3(+0.5, +0.5, 0),DirectX::XMFLOAT4(1, 0, 0, 1),DirectX::XMFLOAT2(0,1) },
		{ DirectX::XMFLOAT3(-0.5, -0.5, 0),DirectX::XMFLOAT4(0, 1, 0, 1),DirectX::XMFLOAT2(1,0) },
		{ DirectX::XMFLOAT3(+0.5, -0.5, 0),DirectX::XMFLOAT4(0, 0, 1, 1),DirectX::XMFLOAT2(1,1) },
	};
	//4-2-1
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(vertex) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	//4-2-2
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = vertices;

	//4-2-3
	hr = Device->CreateBuffer(&bd, &InitData, &VertexBuffer);
	if (FAILED(hr)) {
		MessageBox(NULL, L"sprite::sprite():Create Buffer failed", 0, 0);
		return;
	}

	//VertexShader    InputLayout
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 4 * 3, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 4 * 7, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = ARRAYSIZE(layout);
	ResourceManager::LoadCreateVertexShaders(Device, "sprite_vs.cso",layout,numElements, &VertexShader, &InptLayout);
	ResourceManager::LoadCreatePixelShader(Device, &PixelShader, "sprite_ps.cso");


	//4-6-1  ラスタライザーステートの設定
	D3D11_RASTERIZER_DESC rsDesc;
	ZeroMemory(&rsDesc, sizeof(D3D11_RASTERIZER_DESC));
	rsDesc.FillMode = D3D11_FILL_SOLID;
	rsDesc.CullMode = D3D11_CULL_NONE;
	rsDesc.DepthClipEnable = FALSE;
	hr = Device->CreateRasterizerState(&rsDesc, &RasterizerState);
	if (FAILED(hr)) return;

	ResourceManager::LoadShaderResourceView(Device, texName, &SRView, &Desc);

	D3D11_DEPTH_STENCIL_DESC dsDesc;
	// Depth test parameters
	ZeroMemory(&dsDesc, sizeof(dsDesc));
	dsDesc.DepthEnable = false;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;        // D3D11_DEPTH_WRITE_MASK_ALL
	dsDesc.DepthFunc = D3D11_COMPARISON_ALWAYS;//LESS
	// Stencil test parameters
	dsDesc.StencilEnable = false;
	// Create depth stencil state
	hr = Device->CreateDepthStencilState(&dsDesc, &DepthSS);
	if (FAILED(hr)) return;
///////////////////////////////////////////////////////////////////////////
//////
///////////////////////////////////////////////////////////////////////////
	
	//	//5-4
	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof(samplerDesc));
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDesc.MipLODBias = 0;
	samplerDesc.MaxAnisotropy = 16;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	hr = Device->CreateSamplerState(&samplerDesc, &State);
	if (FAILED(hr)) 
	{
		return ;
	}
}

#define RELEASE_IF(x) if(x){x->Release();}
sprite::~sprite() {
	
	RELEASE_IF(DepthSS);
	RELEASE_IF(State);
	ResourceManager::ReleaseShaderResourceView(SRView);
	RELEASE_IF(VertexBuffer);
	RELEASE_IF(RasterizerState);
	ResourceManager::ReleaseCreatePixelShader(PixelShader);
	ResourceManager::ReleaseCreateVertexShaders(VertexShader, InptLayout);
}
#undef RELEASE_IF


//基準描画
void sprite::render(ID3D11DeviceContext*Context) {
	UINT stride = sizeof(vertex);
	UINT offset = 0;

	//Set vertex buffer
	//頂点
	Context->IASetVertexBuffers(0, 1, &VertexBuffer, &stride, &offset);

	//5-2    Set primitive topology
	Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//5-3   Set the input layout
	Context->IASetInputLayout(InptLayout);

	//5-4
	Context->RSSetState(RasterizerState);
	//5-5   Render a triangle
	Context->VSSetShader(VertexShader, NULL, 0);
	Context->PSSetShader(PixelShader, NULL, 0);
	//UNIT04
	//2Dのみ
	Context->PSSetShaderResources(0, 1, &SRView);

	//2Dのみ
	Context->PSSetSamplers(0, 1, &State);

	//UNIT6深度テストを設定する
	Context->OMSetDepthStencilState(DepthSS, 1);

	Context->Draw(4, 0);
}

void sprite::render(ID3D11DeviceContext*Context, float dx, float dy, float dw, float dh, float sx, float sy, float sw, float sh, float angle, UINTCOLOR color)
{
	DirectX::XMFLOAT4 colorNDC;
	colorNDC = toNDColor(color);
	
	vertex verticesScreen[] = {
		{ DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT4(colorNDC.x,colorNDC.y,colorNDC.z,colorNDC.w) ,DirectX::XMFLOAT2(sx,sy) },
		{ DirectX::XMFLOAT3(1, 0, 0), DirectX::XMFLOAT4(colorNDC.x,colorNDC.y,colorNDC.z,colorNDC.w) ,DirectX::XMFLOAT2(sx + sw,sy) },
		{ DirectX::XMFLOAT3(0, 1, 0), DirectX::XMFLOAT4(colorNDC.x,colorNDC.y,colorNDC.z,colorNDC.w) ,DirectX::XMFLOAT2(sx,sy + sh) },
		{ DirectX::XMFLOAT3(1, 1, 0), DirectX::XMFLOAT4(colorNDC.x,colorNDC.y,colorNDC.z,colorNDC.w) ,DirectX::XMFLOAT2(sx + sw,sy + sh) },
	};
	//アフィン変換
	//拡大縮小
	for (int i = 0; i < 4; i++)
	{
		verticesScreen[i].position.x = dw*verticesScreen[i].position.x;
		verticesScreen[i].position.y = dh*verticesScreen[i].position.y;
	}
	//回転
	//中心点
	float tenX, tenY;
	tenX = dw / 2 /*+ dx*/;
	tenY = dh / 2 /*+ dy*/;
	for (int i = 0; i < 4; i++) {
		float X, Y;
		X = verticesScreen[i].position.x;
		Y = verticesScreen[i].position.y;
		verticesScreen[i].position.x = (X - tenX)*cosf(angle*0.01745f) - (Y - tenY)*sinf(angle*0.01745f) + tenX;
		verticesScreen[i].position.y = (X - tenX)*sinf(angle*0.01745f) + (Y - tenY)*cosf(angle*0.01745f) + tenY;
	}

	//平行移動
	for (int i = 0; i < 4; i++) {
		verticesScreen[i].position.x = dx + verticesScreen[i].position.x;
		verticesScreen[i].position.y = dy + verticesScreen[i].position.y;

	}

	//NDC変換	
	vertex verticesNDC[4];
	for (int i = 0; i < 4; i++) {
		verticesNDC[i].position.x = (verticesScreen[i].position.x /GameLib::system::SCREEN_WIDTH * 2 - 1);
		verticesNDC[i].position.y = ((verticesScreen[i].position.y / GameLib::system::SCREEN_HEIGHT) * (-2)) + 1;
		verticesNDC[i].position.z = verticesScreen[i].position.z;
		verticesNDC[i].color = verticesScreen[i].color;
		verticesNDC[i].texcoord.x = verticesScreen[i].texcoord.x / Desc.Width;
		verticesNDC[i].texcoord.y = verticesScreen[i].texcoord.y / Desc.Height;
	}

	D3D11_MAPPED_SUBRESOURCE sub0;
	Context->Map(VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &sub0);
	memcpy(sub0.pData, verticesNDC, sizeof(verticesNDC));
	Context->Unmap(VertexBuffer, 0);

	UINT stride = sizeof(vertex);
	UINT offset = 0;

	//5-1   Set the input layout
	Context->IASetVertexBuffers(0, 1, &VertexBuffer, &stride, &offset);

	//5-2    Set primitive topology
	Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//5-3   Set the input layout
	Context->IASetInputLayout(InptLayout);

	//線描画か塗りつぶし描画かの設定
	Context->RSSetState(RasterizerState);
	//5-5   Render a triangle
	Context->VSSetShader(VertexShader, NULL, 0);
	Context->PSSetShader(PixelShader, NULL, 0);
	
	//2Dのみ
	Context->PSSetShaderResources(0, 1, &SRView);

	//2Dのみ
	Context->PSSetSamplers(0, 1, &State);

	//深度テストを設定する　
	Context->OMSetDepthStencilState(DepthSS, 1);

	Context->Draw(4, 0);

}