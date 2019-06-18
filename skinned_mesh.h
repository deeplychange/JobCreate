#include <iostream>
#include <d3d11.h>
//#include "framework.h"
#include <DirectXMath.h>
#include <vector>
#include <wrl/client.h>  //Unit19
#include "geometric_primitive.h"
#define MAX_BONE_INFLUENCES (4)
#define MAX_BONES (32)

class skinned_mesh :public geometric_primitive {
protected:
	ID3D11VertexShader*  GVertexShader;
	ID3D11PixelShader*   GPixelShader;
	ID3D11InputLayout*   GInptLayout;
	//ID3D11Buffer*        VBuffer;  //���_�o�b�t�@
	//ID3D11Buffer*        IBuffer;  //�C���f�b�N�X�o�b�t�@
	ID3D11Buffer*        CBuffer;  //�萔�o�b�t�@
	ID3D11RasterizerState* RSSolid;  //���`��
	ID3D11RasterizerState* RSWireframe;  //�h��Ԃ��`��
	ID3D11DepthStencilState *GDepthSS;
	ID3D11SamplerState    *GState;
	//ID3D11ShaderResourceView* GSRView;
	D3D11_TEXTURE2D_DESC GDesc;
	int numIndices;
	int face;
public:
	DirectX::XMFLOAT3 pos;
	bool zahyouFlag;
	void render(
		ID3D11DeviceContext* Context,     //�f�o�C�X�R���e�L�X�g
		const DirectX::XMFLOAT4X4 & world_view_projection,      //���[���h.�r���[.�v���W�F�N�V���������s��
		const DirectX::XMFLOAT4X4 & world,   //���[���h�ύX�s��
		const DirectX::XMFLOAT4   & material_color,  //�ގ��F 
		const DirectX::XMFLOAT4   &light_direction,  //���C�g�i�s����
		bool full                          //��.�h��Ԃ��`�敔�t���O
		);
	skinned_mesh(ID3D11Device*Device,const char *fbx_filename);
	virtual ~skinned_mesh();
	
	struct vertex
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT3 normal;
		DirectX::XMFLOAT2 texcoord;
		//Unit20
		FLOAT bone_weights[MAX_BONE_INFLUENCES] = {0,0,0,0 };
		INT bone_indices[MAX_BONE_INFLUENCES] = {};
		//����͂P�̒��_���e�����󂯂�{�[�����ő�l�܂łƂ���
	};

	//�ގ����
	struct material {
		DirectX::XMFLOAT4 color = { 0.8f,0.8f,0.8f,1.0f };
		ID3D11ShaderResourceView *shader_resource_view;
	};


	//material diffuse;
	//���b�V���ɕ����̍ގ���`��(�����̐F�ƃe�N�X�`��������������)
	struct subset {
		u_int index_start = 0;
		u_int index_count = 0;
		material diffuse;
	};
	//Unit19 mesh class
	struct mesh {
		Microsoft::WRL::ComPtr<ID3D11Buffer> vertex_buffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer> index_buffer;
		std::vector<subset> subsets;
		//Unit19�ǉ�
		// 1,0,0,0     0,1,0,0    0,0,1,0    0,0,0,1
		DirectX::XMFLOAT4X4 global_transform = { 1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1 };
	}; 
	std::vector<mesh> meshes;

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
		//�{�[��
		//Unit21
		DirectX::XMFLOAT4X4  bone_transforms[MAX_BONES];
	};
	//Unit21
	//FBX�̉E����W�n���獶��ɕϊ�����
	//convert coordinate system from'Up:+Z Front:+Y Right -HAND' to 'UP:+Y Front:+Z LEFT -HAND'
	DirectX::XMFLOAT4X4 coordinate_conversion = {
		1,0,0,0,
		0,0,1,0,
		0,1,0,0,
		0,0,0,1
	}; //Y����Z�������ւ���
	void update();
	void BufferSet(ID3D11Device* Device, vertex *vertices, u_int *indices, int VerticesNumber, int indicesNumber,ID3D11Buffer **vertex_buffer, ID3D11Buffer **index_buffer);
};
