#ifndef CAMERA_H
#define CAMERA_H

#include "Game_lib.h"
#include "Player.h"


enum
{
	CAMERA_WATCHING = 0,		//	�Ď��J����
	CAMERA_DIRECTIONAL,			//	�ʒu�Œ�J����
	CAMERA_TPS,					//	TPS�J����
	CAMERA_FPS,					//	FPS�J����

	CAMERA_MAX
};

class Camera {
private:
	/*void	Watching(const Player& t);
	void	Directional(const Player& t);
	void	TPS(const Player& t);
	void	FPS(const Player& t);*/
public:
	DirectX::XMFLOAT3	pos;				//	���W
	DirectX::XMFLOAT3	target;				//	�����_
	DirectX::XMMATRIX	projection;			//	���e�s��
	DirectX::XMFLOAT3	dir;                //�J��������̈ړ���
	DirectX::XMFLOAT3   gaze;               //�J�����̎���
	DirectX::XMMATRIX   view;                 //�r���[���W
	DirectX::XMFLOAT3   up;                  //�J�����̏�x�N�g��
	Camera();
	DirectX::XMFLOAT3 RayZ0;   //RayPick
	DirectX::XMFLOAT3 v;       //RayPick
	DirectX::XMFLOAT3 RayZ1;   //RayPick
	DirectX::XMFLOAT3 Ray;
	DirectX::XMFLOAT3 PickRayDir, PickRayOrigZ1, PickRayOrigZ0;
	DirectX::XMVECTOR dirNormalize;
	float Raydistance;
	float fdot;      //�|���S����Ray�̊p�x


	POINT memory;
	

	int		mode;
	float   angleX;    //X���ŃJ�����̉�]�p�x
	float   angleY;    //Y���ŃJ�����̉�]�p�x
	float   distance; //�^�[�Q�b�g����ʒu�܂ł̋���
	

	void	Update(const GameLib::DATA3D& t,POINT *point);
	void    Update(POINT *point);
	void    Draw();


	float	width;     //��ʕ�
	float	height;    //��ʍ�
	float	aspect;    //width/height
	float	fovY;      //����p
	float	znear;     //�ŋߓ_   Near�N���b�v�l
	float	zfar;      //�ŉ��_   Far�N���b�v�l

	//�r���[�ϊ�
	DirectX::XMMATRIX  SetView();
	//���e�ݒ�/�v�Z
	DirectX::XMMATRIX  GetProjection();

	//���s���e
	void	SetXMMatrixOrthographicLH();
	//�������e
	void	SetXMMatrixPerspectiveFovLH();
	//Projection���擾
	DirectX::XMMATRIX	GetProjectionMatrix() { return  projection; }
	//���[���h���W���擾
	DirectX::XMMATRIX Camera::GetWorldPos();
	//�r���[���擾
	DirectX::XMMATRIX  GetViewPos() { return view; }
	//�������v�Z����(r)
	void   SetDistance();
	//RayPick
	void Pick(POINT *point,GameLib::DATA3D& mesh, GameLib::DATA3D& fakemesh, float* memoryDistance );
	//Ray�̍��W���擾
	DirectX::XMFLOAT3 BuildRay(POINT *point);
	//Ray��Mesh�̓����蔻��
	bool IntersectTriangle(GameLib::DATA3D *mesh,GameLib::DATA3D * fakemesh,const DirectX::XMVECTOR& orig, const DirectX::XMVECTOR& dir,
		DirectX::XMFLOAT3& v0, DirectX::XMFLOAT3& v1, DirectX::XMFLOAT3& v2,bool clockwise,float* memoryDistance);
};

// (x,y)�̓_��(mx,my)�𒆐S��ang�p��]����
//cameraX, cameraZ, ROTATE_SPEED, targetX, targetZ
void rotate(float *x, float *y, const float ang, const float mx, const float my);

extern Camera camera;

#endif //