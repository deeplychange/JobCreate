#ifndef MAP_3D_H
#define MAP_3D_H

#include "Game_lib.h"
#include "3DMap.h"
#include "NumberName.h"

using namespace GameLib;

#define ROW (20)    //列
#define COLUMN  (20)    //行
#define LENGTH (2.0f)   //長さ
#define HIGHT (0.2f)   //高さ
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



DirectX::XMFLOAT4 color[];//色データ


struct MapInformation {
	bool buildable; //建てられるかどかフラグ
	bool canGo;    //行けるかどかフラグ
	int learning;      //プログラム能力
	int communication; //コミュニケーション
	int spirit;     //根性
	bool clearable;  //削除可能フラグ
	int Attribute;   //属性
	int earnMoney;
	int stage;       //属性アップステージ
	bool AttrUpFlag; //相性アップしたかどうか
	bool MomentFlag;        //AttrDownのStage用　瞬間フラグ
	bool TeacherRoomFlag;  //相性の相手は教師室
	bool MusicRoomFlag;    //相手は音楽室
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

	int mapNum;      //マップ番号
	int canCreate;   //canクリエイトフラグ
	//static int textNum;

	bool isThreadLooping;
	bool buildableArray[BUILDABLE_MAX];    //建てる配列
	bool isClearedFlag;    //道生成中フラグ


	void InitializeMap();

	void UpdateMap(const DirectX::XMMATRIX &_projection, POINT *point,int &money,bool leftflag);

	void DrawMap(const DirectX::XMMATRIX& view,const DirectX::XMMATRIX& projection,const DirectX::XMFLOAT4& light_dir);

	void CreateBuilding(const DirectX::XMMATRIX &_projection, POINT *point,int canCreate,int &money);
	void ClearBuilding(POINT *point);

	DATA3D GetMap(int x);    //マップの番号を取得

	void PickUIFrame(POINT* point);

	int GetBuildableNum();


	//MapInfoのNo,attribute
	void CheckCompatibility(int No,int Attribute);
	//相性によって属性をアップする関数
	void AttributeUp(Attribute array[], int NoOne, int NoTwo, int NoThree);
	void AttributeUp(int attr, int NoOne, int NoTwo, int NoThree);
	//相性の再帰関数
	bool CheckAttr(int mapNO, int pos, int attrNO, const int MAX_ELEMENTS, bool check[], Attribute types[], int out[]);
};

extern MapData mapData;

void Create(int num,GameLib::DATA3D* building,int Attribute, wchar_t *_filename, float posX, float posY, float posZ, float _sx, float _sy, float _sw, float _sh);

DirectX::XMFLOAT3 ChangeToWorldPos(DirectX::XMFLOAT3 _pos);

#endif// !MAP_3D_H