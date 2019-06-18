
#include "OBJ3D.h"
#include "Game_lib.h"
using namespace DirectX;
using namespace GameLib;

//DATA_3D
namespace GameLib {
	//コンストラクタ
	DATA_3D::DATA_3D() {
		pos = DirectX::XMFLOAT3(0, 0, 0);
		scale = DirectX::XMFLOAT3(1, 1, 1);
		angle = DirectX::XMFLOAT3(0, 0, 0);
		material_col = DirectX::XMFLOAT4(1, 1, 1, 1);
		billboard = false;
		bWireframe = false;
		speed = 0;
	}

	//セット3Dモデル 
	//primitiveのnew,テクスチャの貼り付け
	void DATA_3D::SetPrimitive(geometric_primitive* _primitive, bool _billboard, bool _bWireframe)
	{
		obj = _primitive;
		bLoad = true;
		billboard = _billboard;
		bWireframe = _bWireframe;
	}

	//削除
	void	DATA_3D::Release()
	{
		if (obj && bLoad)
		{
			delete	obj;
		}
		obj = nullptr;
		bLoad = false;
	}

	DirectX::XMMATRIX DATA_3D::WorldChange()
	{
		//ワールド変換
		DirectX::XMMATRIX S, Rx, Ry, Rz, R, T, W;
		S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
		DirectX::XMFLOAT3 _angle;
		_angle.x = angle.x * (PI / 180);
		_angle.y = angle.y * (PI / 180);
		_angle.z = angle.z * (PI / 180);
		Rx = DirectX::XMMatrixRotationX(_angle.x);
		Ry = DirectX::XMMatrixRotationY(_angle.y);
		Rz = DirectX::XMMatrixRotationZ(_angle.z);
		R = Rz*Rx*Ry;
		T = DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z);
		W = S*R*T;
		return W;
	}

	//描画
	void XM_CALLCONV  DATA_3D::Draw(
		DirectX::FXMMATRIX view,
		DirectX::FXMMATRIX projection,
		DirectX::FXMVECTOR light_dir,
		bool billboard
		)
	{
		//	モデルがなければ描画しない
		if (!obj)		return;

		//	ワールド変換行列取得
		DirectX::XMMATRIX world = WorldChange();


		//逆行列を取得
		if (billboard) {
			DirectX::XMFLOAT4X4 workView, work;
			DirectX::XMStoreFloat4x4(&workView, view);
			work._11 = workView._11*scale.x;	work._12 = workView._21*scale.x;	work._13 = workView._31*scale.x;	work._14 = 0.0f;
			work._21 = workView._12*scale.y;	work._22 = workView._22*scale.y;	work._23 = workView._32*scale.y;	work._24 = 0.0f;
			work._31 = workView._13*scale.z;	work._32 = workView._23*scale.z;	work._33 = workView._33*scale.z;	work._34 = 0.0f;
			work._41 = pos.x;			work._42 = pos.y;			work._43 = pos.z;			work._44 = 1.0f;
			world = DirectX::XMLoadFloat4x4(&work);
		}
		// 次にオフセットさせます。
		//	//FLoat4X4型に変換する
		DirectX::XMFLOAT4X4 w, wvp;
		DirectX::XMStoreFloat4x4(&wvp, (world*view*projection));
		DirectX::XMStoreFloat4x4(&w, world);
		DirectX::XMFLOAT4 lightColor;
		XMStoreFloat4(&lightColor, light_dir);
		//テクスチャ貼ってない描画
		if (!billboard)
		{
			obj->render(m.Context, wvp, w, material_col, lightColor, bWireframe);
		}
		//看板描画　テクスチャあり
		else obj->renderBillboard(m.Context, wvp, w, material_col, lightColor, bWireframe);
	}


	//そのまま4x4型を代入
	//使っていない
	void DATA_3D::Draw(const DirectX::XMFLOAT4X4 & world_view_projection,      //ワールド.ビュー.プロジェクション合成行列
		const DirectX::XMFLOAT4X4 & world,   //ワールド変更行列
		const DirectX::XMFLOAT4   & material_color,  //材質色 
		const DirectX::XMFLOAT4   &light_direction  //ライト進行方向
		)
	{
		//	モデルが無ければ描画処理を一切行わない
		if (!obj)		return;

		if (!billboard)obj->render(m.Context, world_view_projection, world, material_color, light_direction, bWireframe);
	}
}