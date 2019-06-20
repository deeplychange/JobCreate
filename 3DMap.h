#ifndef MAP_3D_H
#define MAP_3D_H

#include "Game_lib.h"
#include "3DMap.h"
#include "NumberName.h"

using namespace GameLib;

#define ROW (20)    //��
#define COLUMN  (20)    //�s
#define LENGTH (2.0f)   //����
#define HIGHT (0.2f)   //����
#define BUILDING_X  ( 1.0f)
#define BUILDING_Y  ( 0.2f)
#define BUILDING_Z  ( 1.0f)
#define BUILDING_TEX_POS_X  (0.0f)
#define BUILDING_TEX_POS_Y  (64.0f)
#define BUILDING_TEX_LENGTH (64.0f)
#define BUILDING_TEX_HIGHT  (64.0f)
#define ROAD_Y  ( 0.005f)
#define WALL_Y     (0.2f)
#define MAP_MAX  (ROW*COLUMN)
#define BUILDABLE_MAX (ROW*COLUMN)
#define WALLHIGH  (0.5)



DirectX::XMFLOAT4 color[];//�F�f�[�^


struct MapInformation {
	bool buildable; //���Ă��邩�ǂ��t���O
	bool canGo;    //�s���邩�ǂ��t���O
	int learning;      //�v���O�����\��
	int communication; //�R�~���j�P�[�V����
	int spirit;     //����
	bool clearable;  //�폜�\�t���O
	int Attribute;   //����
	int earnMoney;
	int stage;       //�����A�b�v�X�e�[�W
	bool AttrUpFlag; //�����A�b�v�������ǂ���
	bool MomentFlag;        //AttrDown��Stage�p�@�u�ԃt���O
	bool TeacherRoomFlag;  //�����̑���͋��t��
	bool MusicRoomFlag;    //����͉��y��
	int AttrMemory[3];
};
void SetAttribute(MapInformation* MapInfo);

class MapData {
public:
	DATA3D* Map[ROW*COLUMN];
	DATA3D* Building[BUILDABLE_MAX];
	MapInformation MapInfo[ROW*COLUMN];
	MapData();
	~MapData();

	int mapNum;      //�}�b�v�ԍ�
	int canCreate;   //can�N���G�C�g�t���O
	//static int textNum;

	bool isThreadLooping;
	bool buildableArray[BUILDABLE_MAX];    //���Ă�z��
	bool isClearedFlag;    //���������t���O


	void InitializeMap();

	void UpdateMap(const DirectX::XMMATRIX &_projection, POINT *point,int &money,bool leftflag);

	void DrawMap(const DirectX::XMMATRIX& view,const DirectX::XMMATRIX& projection,const DirectX::XMFLOAT4& light_dir);

	void CreateBuilding(const DirectX::XMMATRIX &_projection, POINT *point,int canCreate,int &money);
	void ClearBuilding(POINT *point);

	DATA3D GetMap(int x);    //�}�b�v�̔ԍ����擾

	void PickUIFrame(POINT* point);

	int GetBuildableNum();


	//MapInfo��No,attribute
	void CheckCompatibility(int No,int Attribute);
	//�����ɂ���đ������A�b�v����֐�
	void AttributeUp(Attribute array[], int NoOne, int NoTwo, int NoThree);
	void AttributeUp(int attr, int NoOne, int NoTwo, int NoThree);
	//�����̍ċA�֐�
	bool CheckAttr(int mapNO, int pos, int attrNO, const int MAX_ELEMENTS, bool check[], Attribute types[], int out[]);
};

extern MapData mapData;

void Create(int num,GameLib::DATA3D* building,int Attribute, wchar_t *_filename, float posX, float posY, float posZ, float _sx, float _sy, float _sw, float _sh);

DirectX::XMFLOAT3 ChangeToWorldPos(DirectX::XMFLOAT3 _pos);

#endif// !MAP_3D_H