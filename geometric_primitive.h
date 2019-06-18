#pragma once
#include <iostream>
#include <d3d11.h>
//#include "framework.h"
#include <DirectXMath.h>
typedef UINT UINTCOLOR;
enum {
	CUBE,
	CYLINDER,
	POLYGON
};
enum {
	UP,
	FRONT,
};
class geometric_primitive {
protected:	
	ID3D11VertexShader*  GVertexShader;
	ID3D11PixelShader*   GPixelShader;
	ID3D11InputLayout*   GInptLayout;
	ID3D11Buffer*        VBuffer;  //頂点バッファ
	ID3D11Buffer*        IBuffer;  //インデックスバッファ
	ID3D11Buffer*        CBuffer;  //定数バッファ
	ID3D11RasterizerState* RSSolid;  //塗りつぶし描画
	ID3D11RasterizerState* RSWireframe;  //線描画
	ID3D11DepthStencilState *GDepthSS;
	//テクスチャ用
	ID3D11SamplerState* State;
	ID3D11ShaderResourceView* SRView;
	D3D11_TEXTURE2D_DESC Desc;

	int                    numIndices;
	int					   face;
public:
	int					   type;


	virtual void render(
		ID3D11DeviceContext* Context,     //デバイスコンテキスト
		const DirectX::XMFLOAT4X4 & world_view_projection,      //ワールド.ビュー.プロジェクション合成行列
		const DirectX::XMFLOAT4X4 & world,   //ワールド変更行列
		const DirectX::XMFLOAT4   & material_color,  //材質色 
		const DirectX::XMFLOAT4   &light_direction,  //ライト進行方向
		bool full                          //線.塗りつぶし描画部フラグ
		);

	virtual void geometric_primitive::renderBillboard
		(
			ID3D11DeviceContext*Context,
			const DirectX::XMFLOAT4X4 & world_view_projection,
			const DirectX::XMFLOAT4X4 & world,
			const DirectX::XMFLOAT4   & material_color,
			const DirectX::XMFLOAT4 &light_direction,
			bool line,float sx,float sy,float sw,float sh);

	virtual void geometric_primitive::renderTexture
		(
			ID3D11DeviceContext*Context,
			const DirectX::XMFLOAT4X4 & world_view_projection,
			const DirectX::XMFLOAT4X4 & world,
			const DirectX::XMFLOAT4   & material_color,
			const DirectX::XMFLOAT4 &light_direction,
			bool line, float sx, float sy, float sw, float sh);

	geometric_primitive() {}	//fbx継承のために		
	geometric_primitive(ID3D11Device*);
	geometric_primitive(ID3D11Device*,wchar_t *_filename);
	virtual ~geometric_primitive();
	

	struct vertex 
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT4 color;
		DirectX::XMFLOAT3 normal;
		DirectX::XMFLOAT2 tecoord;  //0-1の間
	};
	vertex *VectexMemory; //RayPick用 vertex座標を保存
 	struct cbuffer
	{
		//ワールド。ビュー、プロジェクション合成行列
		DirectX::XMFLOAT4X4 world_view_projection;
		//ワールド変換行列
		DirectX::XMFLOAT4X4 world;
		//caizhise
		DirectX::XMFLOAT4   material_color;
		//ライト進行方向
		DirectX::XMFLOAT4   light_direction;

	};
	
	int VerticesNumber, indicesNumber;
	void BufferSet(ID3D11Device* Device, vertex *vertices, u_int *indices,int VerticesNumber,int indicesNumber);
	void SetVectexPos(vertex *vertexs);
	DirectX::XMFLOAT3 GetVertexPos(int Num);
};



//立方体クラス
class geometric_cube :public geometric_primitive {
public:
	geometric_cube(ID3D11Device* Device);
};

//円クラス
class geometric_cylinder :public geometric_primitive {
public:
	const int NUM;
	geometric_cylinder(ID3D11Device* Device, int x = 3, float r = 0.5, float h = 1.0);
};

//面クラス
class geometric_polygon :public geometric_primitive {
public:
	geometric_polygon(ID3D11Device* Device);
	geometric_polygon(ID3D11Device* Device,int _type, wchar_t *_filename);
};

class geometric_upside :public geometric_primitive {
public:
	geometric_upside(ID3D11Device* Device);

};

