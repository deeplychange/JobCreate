#ifndef CAMERA_H
#define CAMERA_H

#include "Game_lib.h"
#include "Player.h"


enum
{
	CAMERA_WATCHING = 0,		//	監視カメラ
	CAMERA_DIRECTIONAL,			//	位置固定カメラ
	CAMERA_TPS,					//	TPSカメラ
	CAMERA_FPS,					//	FPSカメラ

	CAMERA_MAX
};

class Camera {
private:
	/*void	Watching(const Player& t);
	void	Directional(const Player& t);
	void	TPS(const Player& t);
	void	FPS(const Player& t);*/
public:
	DirectX::XMFLOAT3	pos;				//	座標
	DirectX::XMFLOAT3	target;				//	注視点
	DirectX::XMMATRIX	projection;			//	投影行列
	DirectX::XMFLOAT3	dir;                //カメラ毎回の移動分
	DirectX::XMFLOAT3   gaze;               //カメラの視線
	DirectX::XMMATRIX   view;                 //ビュー座標
	DirectX::XMFLOAT3   up;                  //カメラの上ベクトル
	Camera();
	DirectX::XMFLOAT3 RayZ0;   //RayPick
	DirectX::XMFLOAT3 v;       //RayPick
	DirectX::XMFLOAT3 RayZ1;   //RayPick
	DirectX::XMFLOAT3 Ray;
	DirectX::XMFLOAT3 PickRayDir, PickRayOrigZ1, PickRayOrigZ0;
	DirectX::XMVECTOR dirNormalize;
	float Raydistance;
	float fdot;      //ポリゴンとRayの角度


	POINT memory;
	

	int		mode;
	float   angleX;    //X軸でカメラの回転角度
	float   angleY;    //Y軸でカメラの回転角度
	float   distance; //ターゲットから位置までの距離
	

	void	Update(const GameLib::DATA3D& t,POINT *point);
	void    Update(POINT *point);
	void    Draw();


	float	width;     //画面幅
	float	height;    //画面高
	float	aspect;    //width/height
	float	fovY;      //視野角
	float	znear;     //最近点   Nearクリップ値
	float	zfar;      //最遠点   Farクリップ値

	//ビュー変換
	DirectX::XMMATRIX  SetView();
	//投影設定/計算
	DirectX::XMMATRIX  GetProjection();

	//平行投影
	void	SetXMMatrixOrthographicLH();
	//透視投影
	void	SetXMMatrixPerspectiveFovLH();
	//Projectionを取得
	DirectX::XMMATRIX	GetProjectionMatrix() { return  projection; }
	//ワールド座標を取得
	DirectX::XMMATRIX Camera::GetWorldPos();
	//ビューを取得
	DirectX::XMMATRIX  GetViewPos() { return view; }
	//距離を計算する(r)
	void   SetDistance();
	//RayPick
	void Pick(POINT *point,GameLib::DATA3D& mesh, GameLib::DATA3D& fakemesh, float* memoryDistance );
	//Rayの座標を取得
	DirectX::XMFLOAT3 BuildRay(POINT *point);
	//RayとMeshの当たり判定
	bool IntersectTriangle(GameLib::DATA3D *mesh,GameLib::DATA3D * fakemesh,const DirectX::XMVECTOR& orig, const DirectX::XMVECTOR& dir,
		DirectX::XMFLOAT3& v0, DirectX::XMFLOAT3& v1, DirectX::XMFLOAT3& v2,bool clockwise,float* memoryDistance);
};

// (x,y)の点を(mx,my)を中心にang角回転する
//cameraX, cameraZ, ROTATE_SPEED, targetX, targetZ
void rotate(float *x, float *y, const float ang, const float mx, const float my);

extern Camera camera;

#endif //