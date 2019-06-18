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
	ID3D11Buffer*        VBuffer;  //���_�o�b�t�@
	ID3D11Buffer*        IBuffer;  //�C���f�b�N�X�o�b�t�@
	ID3D11Buffer*        CBuffer;  //�萔�o�b�t�@
	ID3D11RasterizerState* RSSolid;  //�h��Ԃ��`��
	ID3D11RasterizerState* RSWireframe;  //���`��
	ID3D11DepthStencilState *GDepthSS;
	//�e�N�X�`���p
	ID3D11SamplerState* State;
	ID3D11ShaderResourceView* SRView;
	D3D11_TEXTURE2D_DESC Desc;

	int                    numIndices;
	int					   face;
public:
	int					   type;


	virtual void render(
		ID3D11DeviceContext* Context,     //�f�o�C�X�R���e�L�X�g
		const DirectX::XMFLOAT4X4 & world_view_projection,      //���[���h.�r���[.�v���W�F�N�V���������s��
		const DirectX::XMFLOAT4X4 & world,   //���[���h�ύX�s��
		const DirectX::XMFLOAT4   & material_color,  //�ގ��F 
		const DirectX::XMFLOAT4   &light_direction,  //���C�g�i�s����
		bool full                          //��.�h��Ԃ��`�敔�t���O
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

	geometric_primitive() {}	//fbx�p���̂��߂�		
	geometric_primitive(ID3D11Device*);
	geometric_primitive(ID3D11Device*,wchar_t *_filename);
	virtual ~geometric_primitive();
	

	struct vertex 
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT4 color;
		DirectX::XMFLOAT3 normal;
		DirectX::XMFLOAT2 tecoord;  //0-1�̊�
	};
	vertex *VectexMemory; //RayPick�p vertex���W��ۑ�
 	struct cbuffer
	{
		//���[���h�B�r���[�A�v���W�F�N�V���������s��
		DirectX::XMFLOAT4X4 world_view_projection;
		//���[���h�ϊ��s��
		DirectX::XMFLOAT4X4 world;
		//caizhise
		DirectX::XMFLOAT4   material_color;
		//���C�g�i�s����
		DirectX::XMFLOAT4   light_direction;

	};
	
	int VerticesNumber, indicesNumber;
	void BufferSet(ID3D11Device* Device, vertex *vertices, u_int *indices,int VerticesNumber,int indicesNumber);
	void SetVectexPos(vertex *vertexs);
	DirectX::XMFLOAT3 GetVertexPos(int Num);
};



//�����̃N���X
class geometric_cube :public geometric_primitive {
public:
	geometric_cube(ID3D11Device* Device);
};

//�~�N���X
class geometric_cylinder :public geometric_primitive {
public:
	const int NUM;
	geometric_cylinder(ID3D11Device* Device, int x = 3, float r = 0.5, float h = 1.0);
};

//�ʃN���X
class geometric_polygon :public geometric_primitive {
public:
	geometric_polygon(ID3D11Device* Device);
	geometric_polygon(ID3D11Device* Device,int _type, wchar_t *_filename);
};

class geometric_upside :public geometric_primitive {
public:
	geometric_upside(ID3D11Device* Device);

};

