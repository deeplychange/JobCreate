#include "Camera.h"
#include "Game_lib.h"
#include "3DMap.h"
using namespace DirectX;
using namespace GameLib;
//毎回回転角度
#define angle   (1)

//	コンストラクタ
Camera::Camera()
{
	//	ビュー定義
	pos = XMFLOAT3(-1.0f, 35.0f, -15.0f);
	target = DirectX::XMFLOAT3(10.0f, 0.0f, 10.0f);
	dir= DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	gaze = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);

	//	投影行列定義
	width = (float)GameLib::system::SCREEN_WIDTH; 			
	height = (float)GameLib::system::SCREEN_HEIGHT; 			
	aspect = width / height;				
	fovY = DirectX::XMConvertToRadians(30.0f);	//	視野角(30度)			
	znear = 0.1f; 							
	zfar = 1000.0f; 								
	SetXMMatrixPerspectiveFovLH(); //透視投影
	//SetXMMatrixOrthographicLH();    //平行投影
	up= DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f);    //カメラーを設定
	memory.x = (LONG)0.0l;
	memory.y = (LONG)0.0l;
	gaze.x = target .x- pos.x;
	gaze.y = target .y- pos.y;
	gaze.z = target .z- pos.z;
	angleX = 0.0f;
	angleY = 0.0f;
}

void Camera::SetDistance() {
	distance = sqrt((pos.x - target.x)*(pos.x - target.x) + (pos.y - target.y)*(pos.y - target.y) + (pos.z - target.z)*(pos.z - target.z));
}

////ビュー変換              // 視図変換
DirectX::XMMATRIX Camera::SetView() {
	//DirectX::XMMATRIX V;
	DirectX::XMVECTOR cp, ct, cup;
	cp = DirectX::XMVectorSet(pos.x, pos.y, pos.z, 1.0f);
	ct = DirectX::XMVectorSet(target.x, target.y, target.z, 1.0f);
	cup = DirectX::XMVectorSet(up.x,up.y,up.z,0.0f);
	//V = DirectX::XMMatrixLookAtLH(cp, ct, cup);
	view= DirectX::XMMatrixLookAtLH(cp, ct, cup);//RH:右手系座標
	//視点から位置までの距離
	return view;
}

DirectX::XMMATRIX Camera::GetProjection() {
	return projection;
}

void Camera::Update(const GameLib::DATA3D& t,POINT* point) {
	//マウスの座標を保存する
	memory = *point;
	//	ターゲット修正
	target.x = t.pos.x;
	target.y = t.pos.y + 1.0f;	//	高さ調整
	target.z = t.pos.z+ LENGTH/2;
	SetDistance();


	//マウスの座標を保存する
	*point = mScreenToClient();
	
	
	//移動したかどうかのチェック
	if (GetAsyncKeyState(VK_RBUTTON) !=0) {
		XMFLOAT3 FromPosToTargetF;
		FromPosToTargetF.x =t.pos.x- (-1.0f); FromPosToTargetF.y = t.pos.y- 35.0f, FromPosToTargetF.z = t.pos.z-(-15.0f);
		XMVECTOR FromPosToTargetV = XMLoadFloat3(&FromPosToTargetF);
		XMVECTOR Dis = XMVector3Length(FromPosToTargetV);
		XMStoreFloat(&distance, Dis);
		if (memory.x != point->x || memory.y != point->y) {
			//スクリーンのX軸
			if (memory.x != point->x) {
				//移動差計算
				memory.x -= point->x;
				//最大回転角度は１０度
				if (memory.x >= 10)memory.x = 10;
				if (memory.x <= -10)memory.x = -10;
				//移動角度を計算
				angleX += memory.x;
				pos.z = distance*cosf(angleX / 180 * PI);
				pos.x = distance*sinf(angleX / 180 * PI);
			}
			////スクリーンのY軸
			//if (memory.y != point->y) {
			//	memory.y -= point->y;
			//	if (memory.y >= 10)memory.y = 10;
			//	if (memory.y <= -10)memory.y = -10;
			//	angleY += memory.y;
			//	pos.z = distance* cosf(angleY / 180 * PI);
			//	pos.y = distance* sinf(angleY / 180 * PI);
			//}

		}
	}
	if (angleX >= 360)angleX = 0;
	if (angleX <= -360)angleX = 0;
	if (angleY >= 360)angleY = 0;
	if (angleY <= -360)angleY = 0;
	if (GetAsyncKeyState('A') & 1) {
		angleX += angle;
		rotate(&pos.x, &pos.z, angleX / 180 * PI, t.pos.x, t.pos.z);
	}
	if (GetAsyncKeyState('D') & 1) {
		angleX -= angle;
		rotate(&pos.x, &pos.z, angleX / 180 * PI, t.pos.x, t.pos.z);
	}
	if (GetAsyncKeyState('W') & 1) {
		angleY += angle;
		rotate(&pos.x, &pos.z, angleY / 180 * PI, t.pos.x, t.pos.z);

	}
	if (GetAsyncKeyState('S') & 1) {
		angleY -= angle;
		rotate(&pos.x, &pos.z, angleY / 180 * PI, t.pos.x, t.pos.z);

	}
	if (GetAsyncKeyState('Q') & 1) {
		pos.x *= 1.1f;
		pos.y *= 1.1f;
		pos.z *= 1.1f;
	}
	if (GetAsyncKeyState('E') & 1) {
		pos.x /= 1.1f;
		pos.y /= 1.1f;
		pos.z /= 1.1f;
	}
	if (GetAsyncKeyState(VK_SPACE) & 1) {
		pos = XMFLOAT3(-1.0f, 35.0f, -15.0f);
	}
}



void Camera::Update(POINT *point) {
	//マウスの座標を保存する
	memory = *point;
	if (GetAsyncKeyState('D') & 1) {
		pos.x += 2.0f;
	}
	if (GetAsyncKeyState('A') & 1) {
		pos.x -= 2.0f;
	}
	if (GetAsyncKeyState('W') & 1) {
		pos.z += 2.0f;
	}
	if (GetAsyncKeyState('S') & 1) {
		pos.z -= 2.0f;
	}
	if (GetAsyncKeyState('Q') & 1) {
		pos.x *= 1.1f;
		pos.y *= 1.1f;
		pos.z *= 1.1f;
	}
	if (GetAsyncKeyState('E') & 1) {
		pos.x /= 1.1f;
		pos.y /= 1.1f;
		pos.z /= 1.1f;
	}
	if (GetAsyncKeyState(VK_SPACE) & 1) {
		pos = XMFLOAT3(-1.0f, 35.0f, -15.0f);
	}

	//マウス現在の座標を取得する
	//GetCursorPos(point);
	*point=mScreenToClient();

	//移動したかどうかのチェック
	if (GetAsyncKeyState(VK_RBUTTON)!=0) {
		if (memory.x != point->x || memory.y != point->y) {
			//スクリーンのX軸
			if (memory.x != point->x) {
				//移動差を計算
				memory.x = (long)((memory.x-point->x)*0.1);
			//	memory.x *= 0.1;
				//最大回転角度は１０度
				if (memory.x >= 3)memory.x = 3;
				if (memory.x <= -3)memory.x = -3;
				pos.x += memory.x;
			}
			//スクリーンのY軸
			if (memory.y != point->y) {
				memory.y = (long)((memory.y - point->y)*0.1);
				//memory.y -= point->y;
				//memory.y *=  0.1;
				if (memory.y >= 3)memory.y = 3;
				if (memory.y <= -3)memory.y = -3;
				pos.z += memory.y*-1;
			}
		}
	}
	

	target.x = pos.x + gaze.x;
	target.y = pos.y + gaze.y;
	target.z = pos.z + gaze.z;

}

void Camera::Draw() {
	//Debug用
}

//	平行投影
void Camera::SetXMMatrixOrthographicLH (){
	projection= DirectX::XMMatrixOrthographicLH(width,height, znear,zfar);
}

//	透視投影
void Camera::SetXMMatrixPerspectiveFovLH() {
	//projection = DirectX::XMMatrixOrthographicLH(2 * aspect, 2, 0.1f, 200.0f);
	projection = DirectX::XMMatrixPerspectiveFovLH(fovY, aspect, znear, zfar);
}

//ワールド座標を取得
DirectX::XMMATRIX Camera::GetWorldPos() {
	//ワールド変換
	DirectX::XMMATRIX S, Rx, Ry, Rz, R, T, W;
	S = DirectX::XMMatrixScaling(1.0f,1.0f,1.0f);
	Rx = DirectX::XMMatrixRotationX(0);
	Ry = DirectX::XMMatrixRotationY(0);
	Rz = DirectX::XMMatrixRotationZ(0);
	R = Rz*Rx*Ry;
	T = DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z);//オフセット行列
	W = S*R*T;
	return W;
}




XMFLOAT3 ScreenToWorld(float x, float y, float z)
{
	XMMATRIX Inverse;

	XMMATRIX  ViewPort;
	XMMATRIX  CameraProj = camera.GetProjection();
	XMMATRIX  CameraView = camera.GetViewPos();

	XMFLOAT4X4 vp;
	ViewPort = XMMatrixIdentity();
	XMStoreFloat4x4(&vp, ViewPort);
	vp._11 =  GameLib::system::SCREEN_WIDTH  / 2.0f;
	vp._22 = -GameLib::system::SCREEN_HEIGHT / 2.0f;
	vp._41 =  GameLib::system::SCREEN_WIDTH  / 2.0f;
	vp._42 =  GameLib::system::SCREEN_HEIGHT / 2.0f;
	ViewPort = XMLoadFloat4x4(&vp);

	Inverse = XMMatrixInverse(NULL, CameraView*CameraProj*ViewPort);

	XMFLOAT3 pOut;
	XMVECTOR work, outW;
	work = XMLoadFloat3(&XMFLOAT3(x, y, z));
	outW = XMVector3TransformCoord(work, Inverse);

	XMStoreFloat3(&pOut, outW);
	return pOut;
}

//Rayの座標を取得
XMFLOAT3 Camera::BuildRay(POINT *point) {
	//カメラのProject座標を取得
	XMMATRIX projection = camera.GetProjection();
	//FLOAT4X4型に変換
	XMFLOAT4X4 Proj4x4;
	XMStoreFloat4x4(&Proj4x4, projection);

	//カメラビュー座標を取得
	XMMATRIX  CameraView = camera.GetViewPos();
	//カメラワールド座標を取得
	XMMATRIX  CameraPos = camera.GetWorldPos();

	////////////////////////////////////////////////////////////////////////
	//奥本先生の
	//////////////////////////////////////////////////////////////////////
	//XMFLOAT3 p0 = ScreenToWorld(point->x, point->y, 0.0f);
	//XMFLOAT3 p1 = ScreenToWorld(point->x, point->y, 1.0f);
	//XMFLOAT3 v;
	//v.x = p1.x - p0.x;	v.y = p1.y - p0.y;	v.z = p1.z - p0.z;

	//XMVECTOR p = XMLoadFloat3(&p0);
	//XMVECTOR vec = XMLoadFloat3(&v);
	//vec = XMVector3Normalize(vec);
	//XMStoreFloat3(&v, vec);

	//XMVECTOR N = XMLoadFloat3(&XMFLOAT3(0, 1.0f, 0));				//	法線

	//XMFLOAT3 q = XMFLOAT3(0.0f, 0.0f, 0.0f);
	//float dot;
	//XMStoreFloat(&dot, XMVector3Dot(vec, N));
	//if (dot < 0)
	//{
	//	float LP0;		XMStoreFloat(&LP0, XMVector3Dot(-p, N));
	//	q.x = p0.x + LP0 / dot * v.x;
	//	q.y = p0.y + LP0 / dot * v.y;
	//	q.z = p0.z + LP0 / dot * v.z;
	//}
	//return q;
	////////////////////////////////////////////////////////////////////////////////////////
	
	XMMATRIX WorldView = CameraView;
	XMMATRIX InverseWorldView;  //MATRIX型ワールドビューの逆行列
	InverseWorldView =XMMatrixInverse(NULL,WorldView);
	XMFLOAT4X4 m;  //FLOAT4X4型ワールドビューの逆行列
	XMStoreFloat4x4(&m, InverseWorldView);

	v.x = (((2.0f * (point->x)) / GameLib::system::SCREEN_WIDTH) - 1) / Proj4x4._11;
	v.y = -(((2.0f * (point->y)) / GameLib::system::SCREEN_HEIGHT) - 1)/ Proj4x4._22;
	v.z = 1.0f;

	PickRayDir.x = v.x * m._11 + v.y * m._21 +v.z * m._31;
	PickRayDir.y = v.x * m._12 + v.y * m._22 +v.z * m._32;
	PickRayDir.z = v.x * m._13 + v.y * m._23 +v.z * m._33;
	PickRayOrigZ0.x = m._41;
	PickRayOrigZ0.y = m._42;
	PickRayOrigZ0.z = m._43;
	dirNormalize =XMLoadFloat3(&PickRayDir);
	dirNormalize =XMVector3Normalize(dirNormalize);
	XMStoreFloat3(&PickRayDir, dirNormalize);

	//∵q.y=0;
	//∴q.y=p.y+v.y*t->t=-p.y/v.y->q=p+v*(-p.y/v.y)
	XMFLOAT3 q = XMFLOAT3(0.0f, 0.0f, 0.0f);
	q.x = PickRayOrigZ0.x + PickRayDir.x*((-PickRayOrigZ0.y) / PickRayDir.y);
	q.y = PickRayOrigZ0.y + PickRayDir.y*((-PickRayOrigZ0.y) / PickRayDir.y);
	q.z = PickRayOrigZ0.z + PickRayDir.z*((-PickRayOrigZ0.y) / PickRayDir.y);
	return q;
}

//RayPick
void Camera::Pick(POINT *point,DATA3D& mesh, DATA3D& _fakeMesh, float* memoryDistance){
	BuildRay(point);	
	
	XMVECTOR RayOrigin, RayDir;
	RayOrigin = XMLoadFloat3(&PickRayOrigZ0);
	RayDir = XMLoadFloat3(&PickRayDir);

	XMFLOAT3 v0;
	XMFLOAT3 v1;
	XMFLOAT3 v2;
	
	bool clockwise = false;  //時計回りフラグ
	
	for (int i = 0; i < mesh.obj->indicesNumber / 3; i++) {                      //１２個面
		if (i % 2 == 0) {
			v0.x = (mesh.obj->GetVertexPos(2 * i / 2 + 0).x *mesh.scale.x + mesh.pos.x);
			v0.y = (mesh.obj->GetVertexPos(2 * i / 2 + 0).y *mesh.scale.y + mesh.pos.y);
			v0.z = (mesh.obj->GetVertexPos(2 * i / 2 + 0).z *mesh.scale.z + mesh.pos.z);
			v1.x = (mesh.obj->GetVertexPos(2 * i / 2 + 1).x *mesh.scale.x + mesh.pos.x);
			v1.y = (mesh.obj->GetVertexPos(2 * i / 2 + 1).y *mesh.scale.y + mesh.pos.y);
			v1.z = (mesh.obj->GetVertexPos(2 * i / 2 + 1).z *mesh.scale.z + mesh.pos.z);
			v2.x = (mesh.obj->GetVertexPos(2 * i / 2 + 2).x *mesh.scale.x + mesh.pos.x);
			v2.y = (mesh.obj->GetVertexPos(2 * i / 2 + 2).y *mesh.scale.y + mesh.pos.y);
			v2.z = (mesh.obj->GetVertexPos(2 * i / 2 + 2).z *mesh.scale.z + mesh.pos.z);
			clockwise = true;
		}								 
		else {							 
			v0.x = (mesh.obj->GetVertexPos(2 * (i - 1)  + 1).x *mesh.scale.x + mesh.pos.x);
			v0.y = (mesh.obj->GetVertexPos(2 * (i - 1)  + 1).y *mesh.scale.y + mesh.pos.y);
			v0.z = (mesh.obj->GetVertexPos(2 * (i - 1)  + 1).z *mesh.scale.z + mesh.pos.z);
			v1.x = (mesh.obj->GetVertexPos(2 * (i - 1)  + 2).x *mesh.scale.x + mesh.pos.x);
			v1.y = (mesh.obj->GetVertexPos(2 * (i - 1)  + 2).y *mesh.scale.y + mesh.pos.y);
			v1.z = (mesh.obj->GetVertexPos(2 * (i - 1)  + 2).z *mesh.scale.z + mesh.pos.z);
			v2.x = (mesh.obj->GetVertexPos(2 * (i - 1)  + 3).x *mesh.scale.x + mesh.pos.x);
			v2.y = (mesh.obj->GetVertexPos(2 * (i - 1)  + 3).y *mesh.scale.y + mesh.pos.y);
			v2.z = (mesh.obj->GetVertexPos(2 * (i - 1)  + 3).z *mesh.scale.z + mesh.pos.z);
			clockwise = false;
		}//else over
		if (IntersectTriangle(&mesh, &_fakeMesh, RayOrigin, RayDir, v0, v1, v2, clockwise, memoryDistance))
		{
			break; //当たったらfor文から抜ける
		}
	}//for over
}

//RayとMeshの当たり判定
bool Camera::IntersectTriangle(DATA3D* mesh, DATA3D*  _fakemesh,const XMVECTOR& _orig, const XMVECTOR& _dir,
	XMFLOAT3& v0, XMFLOAT3& v1, XMFLOAT3& v2, bool _clockwise,float* _memoryDistance)
{
	bool Flag01 = false, Flag02 = false, Flag12 = false;//内点判断用
	XMVECTOR orig = _orig;
	XMVECTOR dir = _dir;
	XMVECTOR v0Vec = XMLoadFloat3(&v0);
	XMVECTOR v1Vec = XMLoadFloat3(&v1);
	XMVECTOR v2Vec = XMLoadFloat3(&v2);
	
	XMFLOAT3 edge1;         //ポリゴンの線1
	edge1.x = v1.x - v0.x;    edge1.y = v1.y - v0.y;   edge1.z = v1.z - v0.z;
	XMFLOAT3 edge2;         //ポリゴンの線2
	edge2.x = v2.x - v0.x;   edge2.y = v2.y - v0.y;    edge2.z = v2.z - v0.z;
	//XMFLOAT3からVECTORに変換
	XMVECTOR edge1Vec,edge2Vec;
	edge1Vec=XMLoadFloat3(&edge1);  
	edge2Vec=XMLoadFloat3(&edge2);

	XMVECTOR n;  //ポリゴンの法線
	if (_clockwise) {
		n = XMVector3Cross(edge1Vec, edge2Vec);
	}
	else {
		n = XMVector3Cross(edge2Vec, edge1Vec);
	}
	n= XMVector3Normalize(n);
	//向けの判断する　
	
	dir=XMVector3Normalize(dir);            //光線の方向単位ベクトル
	XMVECTOR vdot = XMVector3Dot(dir, n);
	XMStoreFloat(&fdot, vdot);
	
	if (fdot >0) return false;              //　＋なら裏向き

	XMVECTOR A = XMVector3Dot(dir, n);     //交線の方向ベクトルと三角形Nの射影長

	XMVECTOR v = XMVectorSubtract(v0Vec, orig); //頂点V0－交線始点
	XMVECTOR B = XMVector3Dot(v, n);       //射影長
	float a, b;
	XMStoreFloat(&a, A);
	XMStoreFloat(&b, B);
	Raydistance = b / a;
	XMVECTOR q;    //交点
	q = orig + dir*Raydistance;

	
	//内点判定
	XMVECTOR qv0 = XMVectorSubtract(v0Vec, q);  //q->v0のベクトル
	XMVECTOR qv1 = XMVectorSubtract(v1Vec, q);  //q->v1のベクトル
	XMVECTOR qv2 = XMVectorSubtract(v2Vec, q);  //q->v2のベクトル
	XMVECTOR Cross01, Cross02, Cross12;
	if (_clockwise) {
		Cross01 = XMVector3Cross(qv0, qv1);
		Cross02 = XMVector3Cross(qv1, qv2);
		Cross12 = XMVector3Cross(qv2, qv0);
	}
	else {
		Cross01 = XMVector3Cross(qv0, qv2);
		Cross02 = XMVector3Cross(qv2, qv1);
		Cross12 = XMVector3Cross(qv1, qv0);
	}
	XMVECTOR dot01 = XMVector3Dot(Cross01, n);
	XMVECTOR dot02 = XMVector3Dot(Cross02, n);
	XMVECTOR dot12 = XMVector3Dot(Cross12, n);
	float Direction01, Direction02, Direction12;
	XMStoreFloat(&Direction01, dot01);
	XMStoreFloat(&Direction02, dot02);
	XMStoreFloat(&Direction12, dot12);

	if (Direction01 >0)Flag01 = true;
	if (Direction02 >0)Flag02 = true;
	if (Direction12 >0)Flag12 = true;
	if (Flag01&&Flag02&&Flag12) {
		if (Raydistance < *_memoryDistance) {
		*_memoryDistance = Raydistance;
		*_fakemesh = *mesh;
		}
		return true;
	}
	return false;
}

// (x,y)の点を(mx,my)を中心にang角回転する
void rotate(float *x, float *y, const float ang, const float moveX, const float moveY) {
	const float ox = *x - moveX, oy = *y - moveY;
	*x = ox * cos(ang) + oy * sin(ang);
	*y = -ox * sin(ang) + oy * cos(ang);
	*x += moveX;
	*y += moveY;
}