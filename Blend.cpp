#include"Blend.h"
using namespace GameLib;
ID3D11BlendState* Blend::BlendState[MODE_MAX];
bool Blend::bLoad = false;
Blend::BLEND_MODE Blend::enumMode;

bool Blend::Initialize(ID3D11Device* Device) {
	HRESULT  hr = S_OK;
	D3D11_BLEND_DESC BlendDesc;
	ZeroMemory(&BlendDesc, sizeof(BlendDesc));
	BlendDesc.AlphaToCoverageEnable = false;
	BlendDesc.IndependentBlendEnable = false;
	BlendDesc.RenderTarget[0].BlendEnable = true;

	//NONE
	BlendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE; // Color_Fsrc  
	BlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO; // Color_Fdst  
	BlendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD; // Color_Operation  
	BlendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE; // Alpha_Fsrc  
	BlendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO; // Alpha_Fdst  
	BlendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD; // Alpha_Operation  
	BlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	hr = Device->CreateBlendState(&BlendDesc, &BlendState[NONE]);
	if (FAILED(hr))return false;
	//alpha
	BlendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA; // Color_Fsrc  
	BlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA; // Color_Fdst  
	BlendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD; // Color_Operation  
	BlendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE; // Alpha_Fsrc  
	BlendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO; // Alpha_Fdst  
	BlendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD; // Alpha_Operation  
	BlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	hr = Device->CreateBlendState(&BlendDesc, &BlendState[ALPHA]);
	if (FAILED(hr))return false;



	//add
	BlendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA; // Color_Fsrc  
	BlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE; // Color_Fdst  
	BlendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD; // Color_Operation  
	BlendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE; // Alpha_Fsrc  
	BlendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO; // Alpha_Fdst  
	BlendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD; // Alpha_Operation  
	BlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	hr = Device->CreateBlendState(&BlendDesc, &BlendState[ADD]);
	if (FAILED(hr))return false;


	//subtract
	BlendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA; // Color_Fsrc  
	BlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE; // Color_Fdst  
	BlendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_SUBTRACT; // Color_Operation  
	BlendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE; // Alpha_Fsrc  
	BlendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO; // Alpha_Fdst  
	BlendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD; // Alpha_Operation  
	BlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	hr = Device->CreateBlendState(&BlendDesc, &BlendState[SUB]);
	if (FAILED(hr))return false;


	//replace
	BlendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA; // Color_Fsrc  
	BlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO; // Color_Fdst  
	BlendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD; // Color_Operation  
	BlendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE; // Alpha_Fsrc  
	BlendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO; // Alpha_Fdst  
	BlendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD; // Alpha_Operation  
	BlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	hr = Device->CreateBlendState(&BlendDesc, &BlendState[REPLACE]);

	//multiply
	BlendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ZERO; // Color_Fsrc  
	BlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_SRC_COLOR /*D3D11_BLEND_SRC_COLOR*/; // Color_Fdst  
	BlendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD; // Color_Operation  
	BlendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO; // Alpha_Fsrc  
	BlendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_SRC_ALPHA; // Alpha_Fdst  
	BlendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD; // Alpha_Operation  
	BlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	hr = Device->CreateBlendState(&BlendDesc, &BlendState[MULTIPLY]);


	//lighten
	BlendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE; // Color_Fsrc  
	BlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE; // Color_Fdst  
	BlendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_MAX; // Color_Operation  
	BlendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE; // Alpha_Fsrc  
	BlendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE; // Alpha_Fdst  
	BlendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_MAX; // Alpha_Operation  
	BlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	hr = Device->CreateBlendState(&BlendDesc, &BlendState[LIGHTEN]);

	//darken
	BlendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE; // Color_Fsrc  
	BlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE; // Color_Fdst  
	BlendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_MIN; // Color_Operation  
	BlendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE; // Alpha_Fsrc  
	BlendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE; // Alpha_Fdst  
	BlendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_MIN; // Alpha_Operation  
	BlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	hr = Device->CreateBlendState(&BlendDesc, &BlendState[DARKEN]);

	//screen
	BlendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA; // Color_Fsrc  
	BlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_COLOR; // Color_Fdst  
	BlendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD; // Color_Operation  
	BlendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE; // Alpha_Fsrc  
	BlendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_SRC_ALPHA; // Alpha_Fdst  
	BlendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD; // Alpha_Operation  
	BlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	hr = Device->CreateBlendState(&BlendDesc, &BlendState[SCREEN]);
	if (FAILED(hr))return false;

	bLoad = true;
	return true;
}