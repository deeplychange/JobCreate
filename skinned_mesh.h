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
	//ID3D11Buffer*        VBuffer;  //頂点バッファ
	//ID3D11Buffer*        IBuffer;  //インデックスバッファ
	ID3D11Buffer*        CBuffer;  //定数バッファ
	ID3D11RasterizerState* RSSolid;  //線描画
	ID3D11RasterizerState* RSWireframe;  //塗りつぶし描画
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
		ID3D11DeviceContext* Context,     //デバイスコンテキスト
		const DirectX::XMFLOAT4X4 & world_view_projection,      //ワールド.ビュー.プロジェクション合成行列
		const DirectX::XMFLOAT4X4 & world,   //ワールド変更行列
		const DirectX::XMFLOAT4   & material_color,  //材質色 
		const DirectX::XMFLOAT4   &light_direction,  //ライト進行方向
		bool full                          //線.塗りつぶし描画部フラグ
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
		//今回は１つの頂点が影響を受けるボーンを最大四つまでとする
	};

	//材質情報
	struct material {
		DirectX::XMFLOAT4 color = { 0.8f,0.8f,0.8f,1.0f };
		ID3D11ShaderResourceView *shader_resource_view;
	};


	//material diffuse;
	//メッシュに複数の材質を描画(自分の色とテクスチャ両方持ったら)
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
		//Unit19追加
		// 1,0,0,0     0,1,0,0    0,0,1,0    0,0,0,1
		DirectX::XMFLOAT4X4 global_transform = { 1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1 };
	}; 
	std::vector<mesh> meshes;

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
		//ボーン
		//Unit21
		DirectX::XMFLOAT4X4  bone_transforms[MAX_BONES];
	};
	//Unit21
	//FBXの右手座標系から左手に変換する
	//convert coordinate system from'Up:+Z Front:+Y Right -HAND' to 'UP:+Y Front:+Z LEFT -HAND'
	DirectX::XMFLOAT4X4 coordinate_conversion = {
		1,0,0,0,
		0,0,1,0,
		0,1,0,0,
		0,0,0,1
	}; //Y軸とZ軸を入れ替える
	void update();
	void BufferSet(ID3D11Device* Device, vertex *vertices, u_int *indices, int VerticesNumber, int indicesNumber,ID3D11Buffer **vertex_buffer, ID3D11Buffer **index_buffer);
};
