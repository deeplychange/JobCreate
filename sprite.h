#pragma once
#include <iostream>
#include <d3d11.h>
//#include "framework.h"
#include <DirectXMath.h>
typedef UINT UINTCOLOR;

class sprite {

	ID3D11VertexShader*  VertexShader;
	ID3D11PixelShader*   PixelShader;
	ID3D11InputLayout*    InptLayout;
	ID3D11Buffer*        VertexBuffer;
	ID3D11RasterizerState* RasterizerState;
	ID3D11ShaderResourceView* SRView;
	D3D11_TEXTURE2D_DESC Desc;
	ID3D11SamplerState* State;
	ID3D11DepthStencilState* DepthSS;
	

public:
	//äÓèÄï`âÊ
	void render(ID3D11DeviceContext*);
	////ÉAÉtÉBÉìïœä∑
	void render(ID3D11DeviceContext*,float dx, float dy, float dw, float dh, float sx, float sy, float sw, float sh, float angle, UINTCOLOR color);
	
	
	sprite(ID3D11Device*,const wchar_t*);
	~sprite();

	
		struct vertex {
			DirectX::XMFLOAT3 position;
			DirectX::XMFLOAT4 color;
			DirectX::XMFLOAT2 texcoord; //wenlizuobiao
		};
};

