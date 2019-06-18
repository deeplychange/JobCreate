#include <vector>
#include <array>
#include <process.h>
#include "3DMap.h"
#include "SceneMain.h"
#include "AStar.h"
#include "UI.h"
#include "Arrow.h"
#include "Game_lib.h"
#include "Billboard.h"
using namespace DirectX;
using namespace GameLib;

#define X_MAX (ROW*LENGTH)
#define Y_MAX (COLUMN*LENGTH)
#define X_STEP (ROW)
#define Y_STEP (COLUMN)
#define Arrow_Max  (3)


ArrowAnime Arrow[Arrow_Max];
//--------------------------------------------------------------------
//
// DATA_BASE
//
//--------------------------------------------------------------------
//色データ
DirectX::XMFLOAT4 color[]{
	DirectX::XMFLOAT4(0.9882f ,0.9725f ,0.1529f,1.0f),//YELLOW
	DirectX::XMFLOAT4(0.1529f ,0.4078f ,0.9882f,1.0f),//BLUE
	DirectX::XMFLOAT4(0.4471f ,0.9882f ,0.1529f,1.0f),//GREEN
	DirectX::XMFLOAT4(0.9882f ,0.1608f ,0.1529f,1.0f),//RED
	DirectX::XMFLOAT4(1.000f ,1.000f ,1.000f,1.0f), //WHITE
	DirectX::XMFLOAT4(0.000f ,0.000f ,0.000f,1.0f), //BLACK
	DirectX::XMFLOAT4(0.8314f ,0.4078f ,0.08627f,1.0f),    //BROWN
	DirectX::XMFLOAT4(0.7529f ,0.7255f ,0.7255f,1.0f),   //GRAY
};


int CheckMoneyData[AttributeMax]{
	10,//道路
	50,//教師室
	5,//図書館
	5,//自販機
	20,//音楽室
	30,//体育館
	40,//トイレ
};



struct ThreadData {
	DATA3D* map;
	POINT mouse_poin;
	MapData* mapdata;
	MapInformation* Info;
	int turn;
	int* pMapNum;
	int Attribute;
	int* pMoney;
}threadData;





	//マップにいることを保証する
	void EnsureInMap(int& x,int& y){
		if (x <= 0)x = 0;
		if (x >= ROW)x = ROW;
		if (y <= 0)y = 0;
		if (y >= COLUMN)y = COLUMN;
	}

	bool InMapCheck(int& x, int& y) {
		if (x<0 || x>ROW)return false;
		if (y<0 || y>COLUMN)return false;
		return true;
	}

	bool CheckMoney(ThreadData* threadData ) {
		int need = CheckMoneyData[threadData->Attribute];
		if ((*threadData->pMoney) - need < 0)return false;
		else {
			(*threadData->pMoney) -= need;
			return true;
		}
	}

///////////thread関数////////////////
void Move(void* p) {
	threadData = *(ThreadData*) p;
	while (1) {
		if (threadData.mapdata->isClearedFlag)break;
		//DirectX::XMFLOAT3 workPos;
		threadData.mouse_poin = GetClientMousePos();
		if(threadData.Attribute==road)		threadData.map->pos = ChangeToWorldPos(XMFLOAT3(camera.BuildRay(&threadData.mouse_poin).x, ROAD_Y, camera.BuildRay(&threadData.mouse_poin).z));
		else threadData.map->pos = ChangeToWorldPos(XMFLOAT3(camera.BuildRay(&threadData.mouse_poin).x,BUILDING_Y, camera.BuildRay(&threadData.mouse_poin).z));
		//workPos = threadData.map->pos;	
		if (GetAsyncKeyState(VK_LBUTTON) &1) 
		{
			int workPosX = (int)((ChangeToWorldPos(camera.BuildRay(&threadData.mouse_poin)).x - LENGTH / 2) / LENGTH);
			int workPosZ = (int)((ChangeToWorldPos(camera.BuildRay(&threadData.mouse_poin)).z - LENGTH / 2) / LENGTH);
			if(!InMapCheck(workPosX, workPosZ))continue;
			if (threadData.mapdata->buildableArray[threadData.turn]&& threadData.Info[workPosX + workPosZ*ROW].buildable) {
				if (!threadData.Info[workPosX + workPosZ*ROW].buildable)  return;                    //建てれるかのチェック
				if (!CheckMoney(&threadData))continue;    //お金がないとスキップ
				threadData.map->materialColor.w = 1.0f;
				threadData.Info[workPosX + workPosZ*ROW].buildable = false;		
				threadData.Info[workPosX + workPosZ*ROW].canGo = true;
				threadData.Info[workPosX + workPosZ*ROW].Attribute = threadData.Attribute;
				SetAttribute((MapInformation*)&threadData.Info[workPosX + workPosZ*ROW]);
				threadData.mapdata->buildableArray[threadData.turn] = false;
				(*threadData.pMapNum)++;
//-------------------------------------------------------------------------------------------一個目
				//while (1) {
				//	threadData.poin = mScreenToClient();
				//	//マウスの新しい座用を取得
				//	int memoryPosX = (int)(ChangeToWorldPos(camera.BuildRay(&threadData.poin)).x - LENGTH / 2) / LENGTH;
				//	int memoryPosZ = (int)(ChangeToWorldPos(camera.BuildRay(&threadData.poin)).z - LENGTH / 2) / LENGTH;
				//	EnsureInMap(memoryPosX, memoryPosZ);
				//	//作るマップの個数を計算
				//	int nemberOfCreateMapX =abs(memoryPosX - workPosX);
				//	int nemberOfCreateMapZ = abs(memoryPosZ - workPosZ);
				//	//Mapを作る
				//	for (int i = 0; i < nemberOfCreateMapX+ nemberOfCreateMapZ; ++i) {
				//		threadData.map++;
				//		threadData.map->scale = XMFLOAT3(LENGTH, 0.2, LENGTH);
				//		threadData.map->materialColor = color[BROWN];
				//		threadData.map->Id = i;
				//		threadData.map->materialColor.w = 0.5f;
				//		threadData.map->pos = XMFLOAT3(workPos.x + i * 2, workPos.y, workPos.z);
				//		threadData.map->SetPrimitive(new geometric_upside(GetDevice()));
				//	
				//		threadData.mapNum++;
				//	}
				//}

				break;
			}
		}
	}
	threadData.mapdata->isThreadLooping = false;
	threadData.mapdata->isClearedFlag = false;
	_endthread();
};


MapData::MapData() {
	for (int i = 0; i < ROW*COLUMN; ++i) {
		Map[i] = new DATA3D;
	}
	for (int i = 0; i < BUILDABLE_MAX; ++i) {
		Building[i] = new DATA3D;
	}
	
	mapNum = 0;
	for (int i = 0; i < BUILDABLE_MAX; ++i) {
		buildableArray[i] = true;
	}
	isThreadLooping = false;
	isClearedFlag =false;
	canCreate = NONE;
}

MapData::~MapData() {
	for (int i = 0; i < ROW*COLUMN; i++) {
		if (Map[i]) {
			delete Map[i];
			Map[i] = nullptr;
		}
	}
	for (int i = 0; i < BUILDABLE_MAX; i++) {
		if (Building[i]) {
			delete Building[i];
			Building[i] = nullptr;
		}
	}
	//delete[] Map;

}

//建築配列から建築できる番号を読み込む
int MapData::GetBuildableNum() {
	for (int i = 0; i < BUILDABLE_MAX; ++i) {
		if (Building[i]->obj==nullptr) {
			return i;
		}
	}
	return -1;
}

void MapData::InitializeMap() {
	int work = 0;       //建てる番号用
	//マップ情報の初期化
	for (int i = 0; i < ROW*COLUMN; ++i) {
		MapInfo[i].buildable = true;
		MapInfo[i].canGo = false;
		MapInfo[i].clearable = true;
		MapInfo[i].learning = 0;
		MapInfo[i].communication = 0;
		MapInfo[i].spirit = 0;
		MapInfo[i].Attribute = NONE;
		MapInfo[i].earnMoney = 0;
		MapInfo[i].stage = 0;
		MapInfo[i].AttrUpFlag = false;
		MapInfo[i].MomentFlag = false;
		MapInfo[i].TeacherRoomFlag = false;
		MapInfo[i].MusicRoomFlag = false;
		for (int j = 0; j < 3; ++j) {
			MapInfo[i].AttrMemory[j]= 0;
		}
	}

	//マップ作成
	for (int i = 0; i < COLUMN; i++) { //行
		for (int j = 0; j < ROW; j++) { //列
			Map[j+i*ROW]->SetPrimitive(roadDraw);
			Map[j+i*ROW]->scale = XMFLOAT3(LENGTH, HIGHT, LENGTH);
			Map[j+i*ROW]->pos = XMFLOAT3(LENGTH*(float)j + 0.5f*LENGTH, 0.0f,LENGTH*(float)i + 0.5f*LENGTH);
			Map[j+i*ROW]->angle = XMFLOAT3(0, 0, 0);
			Map[j+i*ROW]->materialColor = color[GREEN];
			Map[j+i*ROW]->materialColor.w = 0.7f;
			Map[j+i*ROW]->bWireframe = false;
			Map[j+i*ROW]->Id = -1;
		}
	}
	
	//道作成(横)
	for (int i = 0; i < 10; ++i) {
		//Building[i]->SetPrimitive(geometric_upside(GetDevice()));
		Building[i]->SetPrimitive(roadDraw);
		Building[i]->scale = XMFLOAT3(LENGTH, HIGHT, LENGTH);
		Building[i]->pos = XMFLOAT3(LENGTH*(float)i+ 0.5f*LENGTH, ROAD_Y, LENGTH*5.0f + 0.5f*LENGTH);
		Building[i]->angle = XMFLOAT3(0, 0, 0);
		Building[i]->materialColor = color[BROWN];
		Building[i]->bWireframe = false;
		Building[i]->Id = i;
		Building[i]->Attribute = road;
		buildableArray[i] = false;
		//マップ情報設定
		MapInfo[5*ROW+i].buildable = false;
		MapInfo[5*ROW+i].canGo = true;
		MapInfo[5*ROW+i].communication = 0;
		MapInfo[5*ROW+i].spirit = 0;
		MapInfo[5*ROW+i].learning = 0;
		MapInfo[5*ROW+i].Attribute = road;
		mapNum++;
	}
	////道作成(縦)
	work = GetBuildableNum();
	for (int i = 0; i < 5; i++) {
		Building[work+i]->SetPrimitive(roadDraw);
		Building[work+i]->scale = XMFLOAT3(LENGTH, HIGHT, LENGTH);
		Building[work+i]->pos = XMFLOAT3(1, ROAD_Y, 1+LENGTH*i);
		Building[work+i]->angle = XMFLOAT3(0, 0, 0);
		Building[work+i]->materialColor = color[BROWN];
		Building[work+i]->bWireframe = false;
		Building[work+i]->Id = work + i;
		Building[i]->Attribute = road;
		buildableArray[work+i] = false;
		//マップ情報設定
		int workNum = (int)((Building[work + i]->pos.x - 1) / LENGTH + ROW*(Building[work + i]->pos.z - 1) / LENGTH);
		MapInfo[workNum].buildable = false;
		MapInfo[workNum].canGo = true;
		MapInfo[workNum].communication = 0;
		MapInfo[workNum].spirit = 0;
		MapInfo[workNum].learning = 0;
		MapInfo[workNum].Attribute = road;
		mapNum++;
	}
	//壁作る
	//下壁前半
	work = GetBuildableNum();
	for (int i = 0; i < 4; ++i) {      
		Building[work+i]->SetPrimitive(roadDraw);
		Building[work+i]->scale = XMFLOAT3(0.5, WALLHIGH, LENGTH);
		Building[work+i]->pos = XMFLOAT3(3, WALL_Y,3+LENGTH*i);
		Building[work+i]->angle = XMFLOAT3(0, 0, 0);
		Building[work+i]->materialColor = color[GRAY];
		Building[work+i]->bWireframe = false;
		Building[work+i]->Id = work + i;
		buildableArray[work + i] = false;
		//マップ情報設定
		int workNum = (int)((Building[work + i]->pos.x-1)/LENGTH + ROW*(Building[work + i]->pos.z-1)/LENGTH);
		MapInfo[workNum].buildable = false;
		MapInfo[workNum].canGo = false;
		MapInfo[workNum].communication = 0;
		MapInfo[workNum].spirit = 0;
		MapInfo[workNum].clearable = false;
		MapInfo[workNum].learning = 0;
		mapNum++;
	}
	//下壁後半
	work = GetBuildableNum();
	for (int i = 0; i < ROW-4-3; ++i) {       
		Building[work + i]->SetPrimitive(wallDraw);
		Building[work + i]->scale = XMFLOAT3(0.5, WALLHIGH, LENGTH);
		Building[work + i]->pos = XMFLOAT3(3, WALL_Y, 1+LENGTH*6 + LENGTH*i);
		Building[work + i]->angle = XMFLOAT3(0, 0, 0);
		Building[work + i]->materialColor = color[GRAY];
		Building[work + i]->bWireframe = false;
		Building[work + i]->Id = work + i;
		buildableArray[work + i] = false;
		//マップ情報設定
		int workNum = (int)((Building[work + i]->pos.x - 1) / LENGTH + ROW*(Building[work + i]->pos.z - 1) / LENGTH);
		MapInfo[workNum].buildable = false;
		MapInfo[workNum].canGo = false;
		MapInfo[workNum].communication = 0;
		MapInfo[workNum].clearable = false;
		MapInfo[workNum].spirit = 0;
		MapInfo[workNum].learning = 0;
		mapNum++;
	}
	//右壁
	work = GetBuildableNum();
	for (int i = 0; i < COLUMN -2; ++i) {      
		Building[work + i]->SetPrimitive(wallDraw);
		Building[work + i]->scale = XMFLOAT3(LENGTH, 0.5, 0.5);
		Building[work + i]->pos = XMFLOAT3(3+LENGTH*i, WALL_Y, 3);
		Building[work + i]->angle = XMFLOAT3(0, 0, 0);
		Building[work + i]->materialColor = color[GRAY];
		Building[work + i]->bWireframe = false;
		Building[work + i]->Id = work + i;
		buildableArray[work + i] = false;
		//マップ情報設定
		int workNum = (int)((Building[work + i]->pos.x - 1) / LENGTH + ROW*(Building[work + i]->pos.z - 1) / LENGTH);
		MapInfo[workNum].buildable = false;
		MapInfo[workNum].canGo = false;
		MapInfo[workNum].communication = 0;
		MapInfo[workNum].spirit = 0;
		MapInfo[workNum].clearable = false;
		MapInfo[workNum].learning = 0;
		mapNum++;
	}
	//左壁
	work = GetBuildableNum();
	for (int i = 0; i < ROW -2; ++i) {       
		Building[work + i]->SetPrimitive(wallDraw);
		Building[work + i]->scale = XMFLOAT3(LENGTH, WALLHIGH, 0.5);
		Building[work + i]->pos = XMFLOAT3(3 + LENGTH*i, WALL_Y, ROW*LENGTH - 3);
		Building[work + i]->angle = XMFLOAT3(0, 0, 0);
		Building[work + i]->materialColor = color[GRAY];
		Building[work + i]->bWireframe = false;
		Building[work + i]->Id = work + i;
		buildableArray[work + i] = false;
		//マップ情報設定
		int workNum = (int)((Building[work + i]->pos.x - 1) / LENGTH + ROW*(Building[work + i]->pos.z - 1) / LENGTH);
		MapInfo[workNum].buildable = false;
		MapInfo[workNum].canGo = false;
		MapInfo[workNum].communication = 0;
		MapInfo[workNum].spirit = 0;
		MapInfo[workNum].clearable = false;
		MapInfo[workNum].learning = 0;
		mapNum++;
	}
	work = GetBuildableNum();
	//左壁
	for (int i = 0; i < COLUMN - 2; ++i) {      
		Building[work + i]->SetPrimitive(wallDraw);
		Building[work + i]->scale = XMFLOAT3(0.5, WALLHIGH, LENGTH);
		Building[work + i]->pos = XMFLOAT3(ROW*LENGTH - 3, WALL_Y, 3 + LENGTH*i);
		Building[work + i]->angle = XMFLOAT3(0, 0, 0);
		Building[work + i]->materialColor = color[GRAY];
		Building[work + i]->bWireframe = false;
		Building[work + i]->Id = work + i;
		buildableArray[work + i] = false;
		//マップ情報設定
		int workNum = (int)((Building[work + i]->pos.x - 1) / LENGTH + ROW*(Building[work + i]->pos.z - 1) / LENGTH);
		MapInfo[workNum].buildable = false;
		MapInfo[workNum].canGo = false;
		MapInfo[workNum].communication = 0;
		MapInfo[workNum].spirit = 0;
		MapInfo[workNum].clearable = false;
		MapInfo[workNum].learning = 0;
		mapNum++;
	}

	//教師室
	{
		work = GetBuildableNum();
		Create(Building[work],teacherRoom, L"./Data/picture/teacherroom.png", 1 + LENGTH * 7, BUILDING_Y, 1 + LENGTH * 6, 0, 0, 64, 64);
		Building[work]->Id = work;
		Building[work]->materialColor = color[RED];
		Building[work]->materialColor.w = 1.0f;
		buildableArray[work] = false;
		//マップ情報設定
		int workNum = (int)((Building[work]->pos.x - 1) / LENGTH + ROW*(Building[work]->pos.z - 1) / LENGTH);
		MapInfo[workNum].buildable = false;
		MapInfo[workNum].canGo = true;
		MapInfo[workNum].clearable = true;
		MapInfo[workNum].Attribute = teacherRoom;
		SetAttribute(&MapInfo[workNum]);
		mapNum++;
	}	

	//図書館
	{
		work = GetBuildableNum();
		Create(Building[work], library, L"./Data/picture/library.png", 5, BUILDING_Y, 9, 0, 0, 64, 64);
		Building[work]->Id = work;
		Building[work]->materialColor = color[YELLOW];
		Building[work]->materialColor.w = 1.0f;
		buildableArray[work] = false;
		//マップ情報設定
		int workNum = (int)((Building[work]->pos.x - 1) / LENGTH + ROW*(Building[work]->pos.z - 1) / LENGTH);
		MapInfo[workNum].buildable = false;
		MapInfo[workNum].canGo = true;
		MapInfo[workNum].clearable = true;
		MapInfo[workNum].Attribute = library;
		SetAttribute(&MapInfo[workNum]);
		mapNum++;
	}
	//自販機
	{
		work = GetBuildableNum();
		Create(Building[work], autoShop, L"./Data/picture/autoShop.png", 9, BUILDING_Y, 9, 0, 0, 64, 64);
		Building[work]->Id = work;
		Building[work]->materialColor.w = 1.0f;
		buildableArray[work] = false;
		//マップ情報設定
		int workNum = (int)((Building[work]->pos.x - 1) / LENGTH + ROW*(Building[work]->pos.z - 1) / LENGTH);
		MapInfo[workNum].buildable = false;
		MapInfo[workNum].canGo = true;
		MapInfo[workNum].clearable = true;
		MapInfo[workNum].Attribute = autoShop;
		SetAttribute(&MapInfo[workNum]);
		mapNum++;
	}
}



void MapData::UpdateMap(const DirectX::XMMATRIX &_projection, POINT *point, int &money, bool leftflag) {



	//Cキーで道を建築する
	if (GetAsyncKeyState('C') & 1) {
		CreateBuilding(_projection, point, road, money);
	}

	//UIから建築する
	if (canCreate >= 0) {
		CreateBuilding(_projection, point, canCreate, money);
		canCreate = NONE;
	}

	if (GetAsyncKeyState('V') & 1) {
		for (int i = 0; i < ROW*COLUMN; i++) {
			if (Map[i]->obj)Map[i]->bWireframe = !Map[i]->bWireframe;
		}
		for (int i = 0; i < +BUILDABLE_MAX; i++) {
			if (Building[i]->obj)Building[i]->bWireframe = !Building[i]->bWireframe;
		}
	}
	if (GetAsyncKeyState('Z') & 1) {
		ClearBuilding(point);
	}
	if (leftflag) {
		if (!isThreadLooping)PickUIFrame(point);
	}

	const int MAX_ELEMENTS = 3;     //相性になれる建築の個数
	//相性チェック
	for (int i = 0; i < ROW*COLUMN; ++i) {
		///////////////////////teacherRoom,library,autoShop/////////////////////////
		bool check[MAX_ELEMENTS] = { false,false,false };
		Attribute types[MAX_ELEMENTS] = { teacherRoom,library,autoShop };
		int out[MAX_ELEMENTS] = { 0 };
		if (MapInfo[i].Attribute == teacherRoom|| MapInfo[i].TeacherRoomFlag) {
			//属性チェック関数
			if (CheckAttr(i, 0, 0, MAX_ELEMENTS, check, types, out)) {
				MapInfo[i].TeacherRoomFlag = true;
				for (int j = 0; j < MAX_ELEMENTS; ++j) {
					MapInfo[i].AttrMemory[j] = out[j];
				}
				//矢印アニメと属性アップ関数
				AttributeUp(types, out[0], out[1], out[2]);
			}
			if (MapInfo[i].stage== StandBy) {
				if (!CheckAttr(i, 0, 0, MAX_ELEMENTS, check, types, out)) {
					if (MapInfo[i].MomentFlag)MapInfo[i].stage = AttrDown;
					AttributeUp(types, MapInfo[i].AttrMemory[0], MapInfo[i].AttrMemory[1], MapInfo[i].AttrMemory[2]);

				}
			}
		}
	
		///////////////////////musicRoom,gymnasium,toilet/////////////////////////
		check[0] = false; check[1] = false; check[2] = false;
		types[0] = musicRoom; types[1] = gymnasium; types[2] = toilet;
		out[0] = 0; out[1] = 0; out[2] = 0;
		if (MapInfo[i].Attribute == musicRoom|| MapInfo[i].MusicRoomFlag) {
			Attribute types[MAX_ELEMENTS] = { musicRoom,gymnasium,toilet };
			bool check[MAX_ELEMENTS] = { false,false,false };
			int out[MAX_ELEMENTS] = { 0 };
			if (CheckAttr(i, 0, 0, MAX_ELEMENTS, check, types, out)) {
				for (int j = 0; j < MAX_ELEMENTS; ++j) {
					MapInfo[i].AttrMemory[j] = out[j];
				}
				MapInfo[i].MusicRoomFlag = true;
				AttributeUp(types, out[0], out[1], out[2]);
			}
			if (MapInfo[i].stage==StandBy) {
				if (!CheckAttr(i, 0, 0, MAX_ELEMENTS, check, types, out)) {
					if (MapInfo[i].MomentFlag)MapInfo[i].stage = AttrDown;
					AttributeUp(types, MapInfo[i].AttrMemory[0], MapInfo[i].AttrMemory[1], MapInfo[i].AttrMemory[2]);

				}
			}
		}
		
	} //相性チェックのfor文over
	
	//建築を透明化
	//Transparent();

}

//相性アップ　判定とアニメ
void MapData::AttributeUp(Attribute array[], int NoOne, int NoTwo, int NoThree) {
	
		switch (MapInfo[NoOne].stage)
		{
		case AttrUp: {
			if (array[0] == teacherRoom&&array[1] == library&&array[2] == autoShop) {
				MapInfo[NoOne].communication += 7;
				MapInfo[NoOne].spirit += 7;
				MapInfo[NoOne].learning += 7;
				MapInfo[NoTwo].communication += 5;
				MapInfo[NoTwo].spirit += 5;
				MapInfo[NoTwo].learning += 5;
				MapInfo[NoThree].communication += 2;
				MapInfo[NoThree].spirit += 2;
				MapInfo[NoThree].learning += 2;
			}
			else if (array[0] == musicRoom&&array[1] == gymnasium&&array[2] == toilet) {
				MapInfo[NoOne].communication += 10;
				MapInfo[NoOne].spirit += 10;
				MapInfo[NoOne].learning += 10;
				MapInfo[NoTwo].communication += 7;
				MapInfo[NoTwo].spirit += 7;
				MapInfo[NoTwo].learning +=7;
				MapInfo[NoThree].communication += 3;
				MapInfo[NoThree].spirit += 3;
				MapInfo[NoThree].learning += 3;
			}
			XMFLOAT3 posM[3];
			posM[0].x = (NoOne%ROW)*2.0f+1.0f;   posM[0].y = 0.8f, posM[0].z = (NoOne   / 20.0f)*2.0f+1.0f;
			posM[1].x = (NoTwo%ROW)*2.0f+1.0f;   posM[1].y = 0.8f, posM[1].z = (NoTwo   / 20.0f)*2.0f+1.0f;
			posM[2].x = (NoThree%ROW)*2.0f+1.0f; posM[2].y = 0.8f, posM[2].z = (NoThree / 20.0f)*2.0f+1.0f;

			for (int i = 0; i < 3; ++i) {
				Arrow[i].Init(L"./Data/picture/arrowUp.png",
					posM[i].x, posM[i].y, posM[i].z, 0, 0, 64, 64);
			}
			MapInfo[NoOne].AttrUpFlag = true;
			MapInfo[NoOne].MomentFlag = true;
			MapInfo[NoOne].stage = AttrUpAni;
		}
			break;
		case AttrDown: {
			if (array[0] == teacherRoom&&array[1] == library&&array[2] == autoShop) {
				MapInfo[NoOne].communication -= 7;
				MapInfo[NoOne].spirit -= 7;
				MapInfo[NoOne].learning -= 7;
				MapInfo[NoTwo].communication -= 5;
				MapInfo[NoTwo].spirit -= 5;
				MapInfo[NoTwo].learning -= 5;
				MapInfo[NoThree].communication -= 2;
				MapInfo[NoThree].spirit -= 2;
				MapInfo[NoThree].learning -= 2;
			}
			else if (array[0] == musicRoom&&array[1] == gymnasium&&array[2] == toilet) {
				MapInfo[NoOne].communication -= 10;
				MapInfo[NoOne].spirit -= 10;
				MapInfo[NoOne].learning -= 10;
				MapInfo[NoTwo].communication -= 7;
				MapInfo[NoTwo].spirit -= 7;
				MapInfo[NoTwo].learning -= 7;
				MapInfo[NoThree].communication -= 3;
				MapInfo[NoThree].spirit -= 3;
				MapInfo[NoThree].learning -= 3;
			}
			XMFLOAT3 posM[3];
			posM[0].x = (NoOne%ROW) * 2.0f + 1.0f;   posM[0].y = 2.3f, posM[0].z = (NoOne / 20.0f) * 2.0f + 1.0f;
			posM[1].x = (NoTwo%ROW) * 2.0f + 1.0f;   posM[1].y = 2.3f, posM[1].z = (NoTwo / 20.0f) * 2.0f + 1.0f;
			posM[2].x = (NoThree%ROW) * 2.0f + 1.0f; posM[2].y = 2.3f, posM[2].z = (NoThree / 20) * 2.0f + 1.0f;
			for (int i = 0; i < 3; ++i) {
				if (!Arrow[i].obj->obj) {
					Arrow[i].Init(L"./Data/picture/arrowDown.png",
						posM[i].x, posM[i].y, posM[i].z, 0, 0, 64, 64);
				}
			}
			MapInfo[NoOne].MomentFlag = false;
			MapInfo[NoOne].TeacherRoomFlag = false;
			MapInfo[NoOne].MusicRoomFlag = false;
			MapInfo[NoOne].stage = AttrDownAni;
		}
			break;
		case AttrUpAni://上げるアニメ
			for (int i = 0; i < 3; ++i) {
				Arrow[i].Update(&MapInfo[NoOne]);
			}
			break;
		case AttrDownAni://降りるアニメ
			for (int i = 0; i < 3; ++i) {
				Arrow[i].Update(&MapInfo[NoOne]);
			}
			break;
		case StandBy:
			break;
		}
}


//RayPick
void MapData::PickUIFrame(POINT* point) {
	//PLAYERを選択
	DATA3D* fakeMesh = new DATA3D;
	float memoyDist = 10000.0f;//交線と各MESHの最短距離用
	for (int i = 0; i < BUILDABLE_MAX; i++) {
		if (Building[i]->obj) {      //画像あれば
			camera.Pick(point, *Building[i], *fakeMesh, &memoyDist);
		}
	}
	if (fakeMesh->obj) {
		for (int i = 0; i < BUILDABLE_MAX; i++) {
			if (Building[i]->obj == fakeMesh->obj)
			{
				if (Building[i]->Attribute == NONE|| Building[i]->Attribute==road)continue;
				if (!UIAttribute.drawFlag) {
					UIAttribute.drawFlag = true;
					//RayPick時　選んだブロック番号の一時的な保存 
					int memoryPos = (int)((Building[i]->pos.x - LENGTH / 2) / LENGTH + ROW*(Building[i]->pos.z - LENGTH / 2) / LENGTH);
					UIAttribute.learning = MapInfo[memoryPos].learning;
					UIAttribute.spirit = MapInfo[memoryPos].spirit;
					UIAttribute.communication = MapInfo[memoryPos].communication;
					UIAttribute.earnMoneyDarwFlag = true;
					UIAttribute.earnMoney = MapInfo[memoryPos].earnMoney;
					UIAttribute.PlayerOrBillboard = BILLBOARD;
				}
			}
		}
	}
	fakeMesh = nullptr;
	delete fakeMesh;
	
}

void MapData::DrawMap(const DirectX::XMMATRIX& view,
	const DirectX::XMMATRIX& projection,
	const DirectX::XMFLOAT4& light_dir
	) {

	//建築の描画
	for (int i = 0; i <BUILDABLE_MAX; i++) {
		if (Building[i]->obj) {
			Building[i]->Draw(view, projection, XMLoadFloat4(&light_dir));
		}
	}
	
	//床の描画
	for (int i = 0; i < ROW*COLUMN; i++) {
		if (Map[i]->obj) {
			Map[i]->Draw(view, projection, XMLoadFloat4(&light_dir));
		}
	}
	
	//相性用矢印のアニメ描画
	for (int i = 0; i < Arrow_Max; ++i) {
		if (Arrow[i].obj)Arrow[i].Draw(view,projection, light_dir);
	}
}


DATA3D MapData::GetMap(int x) {
	if (x > ROW*COLUMN)x = ROW*COLUMN;
	if(Map[x]->obj)return *Map[x];
	else return *Map[0];
}  

void MapData::CreateBuilding(const DirectX::XMMATRIX &_projection,POINT  *point,int _canCreate,int &money) {
	if (isThreadLooping) return;     //Thread関数実行チェック
		if (mapNum <= BUILDABLE_MAX) {
			for (int i = 0; i < BUILDABLE_MAX; ++i) {
				if (buildableArray[i]) {
					int workPosX = (int)((ChangeToWorldPos(camera.BuildRay(point)).x - LENGTH / 2) / LENGTH);
					int workPosZ = (int)((ChangeToWorldPos(camera.BuildRay(point)).z - LENGTH / 2) / LENGTH);
					switch (_canCreate) {
					case road:
						Building[i]->SetPrimitive(roadDraw);
						Building[i]->tex = false;
						Building[i]->scale = XMFLOAT3(LENGTH, HIGHT, LENGTH);
						Building[i]->pos = ChangeToWorldPos(XMFLOAT3(camera.BuildRay(point).x, ROAD_Y,camera.BuildRay(point).z));
						Building[i]->materialColor = color[BROWN];
						Building[i]->Id = i;
						Building[i]->materialColor.w = 0.5f;
						Building[i]->Attribute = road;
						threadData.Attribute = road;
						break;
					case teacherRoom:  
						Create(Building[i],teacherRoom, L"./Data/picture/teacherroom.png", BUILDING_X, BUILDING_Y, BUILDING_Z, BUILDING_TEX_POS_X, BUILDING_TEX_POS_Y, BUILDING_TEX_LENGTH, BUILDING_TEX_HIGHT);
						Building[i]->Id = i;
						threadData.Attribute = teacherRoom;
						break;
					case library:
						Create(Building[i],library, L"./Data/picture/library.png", BUILDING_X, BUILDING_Y, BUILDING_Z, BUILDING_TEX_POS_X, BUILDING_TEX_POS_Y, BUILDING_TEX_LENGTH, BUILDING_TEX_HIGHT);
						Building[i]->Id = i;
						threadData.Attribute = library;
						break;
					case autoShop:
						Create(Building[i], autoShop, L"./Data/picture/autoShop.png", BUILDING_X, BUILDING_Y, BUILDING_Z, BUILDING_TEX_POS_X, BUILDING_TEX_POS_Y, BUILDING_TEX_LENGTH, BUILDING_TEX_HIGHT);
						Building[i]->Id = i;
						threadData.Attribute = autoShop;
						break;
					case musicRoom:
						Create(Building[i], musicRoom, L"./Data/picture/musicroom.png", BUILDING_X, BUILDING_Y, BUILDING_Z, BUILDING_TEX_POS_X, BUILDING_TEX_POS_Y, BUILDING_TEX_LENGTH, BUILDING_TEX_HIGHT);
						Building[i]->Id = i;
						threadData.Attribute = musicRoom;
						break;
					case gymnasium:
						Create(Building[i], gymnasium, L"./Data/picture/gymnasium.png", BUILDING_X, BUILDING_Y, BUILDING_Z, BUILDING_TEX_POS_X, BUILDING_TEX_POS_Y, BUILDING_TEX_LENGTH, BUILDING_TEX_HIGHT);
						Building[i]->Id = i;
						threadData.Attribute = gymnasium;
						break;
					case toilet:
						Create(Building[i], toilet, L"./Data/picture/toilet.png", BUILDING_X, BUILDING_Y, BUILDING_Z, BUILDING_TEX_POS_X, BUILDING_TEX_POS_Y, BUILDING_TEX_LENGTH, BUILDING_TEX_HIGHT);
						Building[i]->Id = i;
						threadData.Attribute = toilet;
						break;

					}//switch over
						mapNum++;
						isClearedFlag = false;
						threadData.map = Building[i];
						threadData.mapdata = this;
						threadData.turn = i;
						threadData.Info = MapInfo;
						threadData.pMapNum = &mapNum;
						threadData.pMoney = &money;
						isThreadLooping = true;
						_beginthread(Move, 0, (void*)&threadData);
						break;
			}//if(buildableArray[i])
				//マウスをウィンドの真ん中に強制移動
				//POINT mouse;
				//mouse = SetClientToScreen(system::SCREEN_WIDTH/2, system::SCREEN_HEIGHT/2 + 40);
				//SetCursorPos(mouse.x, mouse.y);
		}//for
	}
}

void MapData::ClearBuilding(POINT *point) {
	//マウス座標を取得
	XMFLOAT3 pos = ChangeToWorldPos(camera.BuildRay(point));
	for (int i = 0; i < BUILDABLE_MAX; i++) {
		if (Building[i]->obj) {
			if (Building[i]->pos.x == pos.x&&Building[i]->pos.z == pos.z)
			{
				int workPosX = (int)((ChangeToWorldPos(camera.BuildRay(point)).x - LENGTH / 2) / LENGTH);
				int workPosZ = (int)((ChangeToWorldPos(camera.BuildRay(point)).z - LENGTH / 2) / LENGTH);
				int plus = workPosX + workPosZ*ROW;
				if (!MapInfo[workPosX + workPosZ*ROW].clearable)break;
				buildableArray[Building[i]->Id] = true;
				delete Building[i]->obj;
				Building[i]->obj = nullptr;
			//	Building[i] = new DATA3D;
				MapInfo[plus].canGo = false;
				MapInfo[plus].buildable = true;
				MapInfo[plus].communication = 0;
				MapInfo[plus].spirit = 0;
				MapInfo[plus].learning = 0;
				MapInfo[plus].clearable = true;
				MapInfo[plus].Attribute = NONE;
				//MapInfo[workPosX + workPosZ*ROW].stage = AttrDown;
				//MapInfo[workPosX + workPosZ*ROW].AttrUpFlag = false;
				isClearedFlag = true;
				mapNum--;
				
			}
		}
	}
}

//ブロックの中心座標に変換
XMFLOAT3 ChangeToWorldPos(XMFLOAT3 _pos) {
	XMFLOAT3 pos;
	pos.x =(float)(int)_pos.x ;
	int work=0;
	work = (int)pos.x;
	work = work % 2;
	if(work ==0)pos.x += LENGTH / 2;
	
	pos.y = _pos.y;
	pos.z = (float)(int)_pos.z;
	work = (int)pos.z;
	work = work % 2;
	if (work == 0)pos.z += LENGTH / 2;
	
	return pos;
}


void Create(GameLib::DATA3D* building, int Attribute,wchar_t *_filename, float posX, float posY, float posZ, float _sx, float _sy, float _sw, float _sh) {
	building->sx = _sx; building->sy = _sy; building->sw = _sw; building->sh = _sh;
	building->SetPrimitive(buildingDraw,false,_filename);
	building->pos = ChangeToWorldPos(XMFLOAT3(posX, posY, posZ));
	building->tex = true;
	building->scale = XMFLOAT3(LENGTH, 1.2f, LENGTH);	
	building->speed = XMFLOAT2(0.0f, 0.0f);
	building->materialColor.w = 0.5f;
	building->Attribute = Attribute;
}

//周囲の二ブロックをチェック
//        x
//      x x x
//    x x o x x
//      x x x
//        x
//再帰で相性をチェック
bool MapData::CheckAttr(int mapNO, int pos, int attrNO, const int MAX_ELEMENTS,bool check[], Attribute types[], int out[])
{
	if (MapInfo[mapNO].Attribute == types[attrNO] && pos < MAX_ELEMENTS) {
		out[attrNO] = mapNO;
		check[attrNO] = true;
	}
	else {
		return false;
	}
	bool result = true;
	for (int i = 0; i < MAX_ELEMENTS; ++i) {
		if (!check[i]) {
			if (CheckAttr(mapNO - ROW, pos + 1, i, MAX_ELEMENTS, check, types, out)) {//下
				return true;
			}
			if (CheckAttr(mapNO + ROW, pos + 1, i, MAX_ELEMENTS, check, types, out)) {//上
				return true;
			}
			if (CheckAttr(mapNO + 1, pos + 1, i, MAX_ELEMENTS, check, types, out)) {//右
				return true;
			}
			if (CheckAttr(mapNO - 1, pos + 1, i, MAX_ELEMENTS, check, types, out)) {//左
				return true;
			}
		}
		result = result && check[i];
	}

	return result;
}

int AttributeData[Attribute::AttributeMax][4]{
	//learning communication spirit earnMoney
	{ 0,0,0,0 },//道路
	{ 12,3,2,0 },//教師室
	{ 5,0,5,0 },//図書館
	{ 0,2,5,3 },//自販機
	{ 4,1,2,0 },//音楽室
	{ 0,1,7,25 },//体育館
	{ -2,6,1,0 },//トイレ
};

enum {
	learning = 0,
	communication,
	spirit,
	earnMoney
};


//属性設定
void SetAttribute(MapInformation* MapInfo) {

	if (MapInfo->Attribute <= NONE && MapInfo->Attribute >= AttributeMax)return;
	int* data = AttributeData[MapInfo->Attribute];

	MapInfo->learning		= data[learning];
	MapInfo->communication	= data[communication];
	MapInfo->spirit			= data[spirit];
	MapInfo->earnMoney		= data[earnMoney];
	
	//if (MapInfo->Attribute == teacherRoom) {
	//	MapInfo->learning = 12;
	//	MapInfo->communication = 3;
	//	MapInfo->spirit = 2;
	//}
	//else if (MapInfo->Attribute == library) {
	//	MapInfo->learning = 5;
	//	MapInfo->communication = 0;
	//	MapInfo->spirit = 5;
	//}
	//else if (MapInfo->Attribute == autoShop) {
	//	MapInfo->learning = 0;
	//	MapInfo->communication = 2;
	//	MapInfo->spirit = 5;
	//	MapInfo->earnMoney = 3;
	//}
	//else if (MapInfo->Attribute == musicRoom) {
	//	MapInfo->spirit = 4;
	//	MapInfo->learning = 1;
	//	MapInfo->communication = 2;
	//	MapInfo->earnMoney =0;
	//}
	//else if (MapInfo->Attribute == gymnasium) {
	//	MapInfo->learning = 0;
	//	MapInfo->communication = 1;
	//	MapInfo->spirit = 7;
	//	MapInfo->earnMoney = 25;
	//}
	//else if (MapInfo->Attribute == toilet) {
	//	MapInfo->learning = -2;
	//	MapInfo->communication = 6;
	//	MapInfo->spirit = 1;
	//	MapInfo->earnMoney = 0;
	//}
}



//相性アップのバカ方法
//使っていない
void MapData::CheckCompatibility(int No, int Attribute) {
	bool check[2] = { false,false };
	if (Attribute == teacherRoom) {
		//libraryらな
		//上を調べる
		if (MapInfo[No + ROW].Attribute == library) {
			//上の上　上の左　上の右　左　右　下　
			if (MapInfo[No + ROW * 2].Attribute          == autoShop)AttributeUp(teacherRoom, No, No + ROW, No + ROW * 2);
			else if (MapInfo[No + ROW * 1 - 1].Attribute == autoShop)AttributeUp(teacherRoom, No, No + ROW, No + ROW * 1 - 1);
			else if (MapInfo[No + ROW * 1 + 1].Attribute == autoShop)AttributeUp(teacherRoom, No, No + ROW, No + ROW * 1 + 1);
			else if (MapInfo[No - 1].Attribute           == autoShop)AttributeUp(teacherRoom, No, No + ROW, No - 1);
			else if (MapInfo[No + 1].Attribute           == autoShop)AttributeUp(teacherRoom, No, No + ROW, No + 1);
			else if(MapInfo[No - ROW * 1].Attribute      == autoShop)AttributeUp(teacherRoom, No, No + ROW, No - ROW * 1);
		}
		//左を調べる
		if (MapInfo[No - 1].Attribute == library) {
			//左の左　左の上  左の下　下　　右　　左
			if (MapInfo[No - 2].Attribute            == autoShop)AttributeUp(teacherRoom,No,No-1, No - 2);
			else if (MapInfo[No + ROW].Attribute     == autoShop)AttributeUp(teacherRoom,No,No-1, No + ROW);
			else if (MapInfo[No - ROW - 1].Attribute == autoShop)AttributeUp(teacherRoom,No,No-1, No - ROW - 1);
			else if (MapInfo[No - ROW].Attribute     == autoShop)AttributeUp(teacherRoom,No,No-1, No -ROW);
			else if (MapInfo[No + 1].Attribute       == autoShop)AttributeUp(teacherRoom,No,No-1, No + 1);
			else if (MapInfo[No - 1].Attribute       == autoShop)AttributeUp(teacherRoom, No, No - 1, No -1);
		}
		//下を調べる
		if (MapInfo[No - ROW].Attribute == library) {
			//下の下　　下の左　下の右　　右  左　上
			if (MapInfo[No - ROW*2].Attribute        == autoShop)AttributeUp(teacherRoom, No, No - ROW, No - ROW*2);
			else if (MapInfo[No - ROW-1].Attribute   == autoShop)AttributeUp(teacherRoom, No, No - ROW, No - ROW-1);
			else if (MapInfo[No - ROW + 1].Attribute == autoShop)AttributeUp(teacherRoom, No, No - ROW, No - ROW + 1);
			else if (MapInfo[No + 1].Attribute       == autoShop)AttributeUp(teacherRoom, No, No - ROW, No + 1);
			else if (MapInfo[No - 1].Attribute       == autoShop)AttributeUp(teacherRoom, No, No - ROW, No - 1);
			else if (MapInfo[No + ROW].Attribute     == autoShop)AttributeUp(teacherRoom, No, No - ROW, No + ROW);
		}
		
		//右を調べる
		if (MapInfo[No +1].Attribute == library) {
			//右の右　　右の上　　右の下　上　下　左
			if (MapInfo[No +2].Attribute             == autoShop)AttributeUp(teacherRoom, No, No + 1, No +2);
			else if (MapInfo[No + ROW + 1].Attribute == autoShop)AttributeUp(teacherRoom, No, No + 1, No + ROW + 1);
			else if (MapInfo[No - ROW + 1].Attribute == autoShop)AttributeUp(teacherRoom, No, No + 1, No - ROW + 1);
			else if (MapInfo[No + ROW].Attribute     == autoShop)AttributeUp(teacherRoom, No, No + 1, No + ROW);
			else if (MapInfo[No - ROW].Attribute     == autoShop)AttributeUp(teacherRoom, No, No + 1, No - ROW);
			else if (MapInfo[No - 1].Attribute       == autoShop)AttributeUp(teacherRoom, No, No + 1, No -1);
		}
		//autoShopなら	
		//上を調べる
		if (MapInfo[No + ROW].Attribute == autoShop) {
			//上の上　上の左　上の右　左　右　下　
			if (MapInfo[No + ROW * 2].Attribute          == library)AttributeUp(teacherRoom, No, No + ROW, No + ROW * 2);
			else if (MapInfo[No + ROW * 1 - 1].Attribute == library)AttributeUp(teacherRoom, No, No + ROW, No + ROW * 1 - 1);
			else if (MapInfo[No + ROW * 1 + 1].Attribute == library)AttributeUp(teacherRoom, No, No + ROW, No + ROW * 1 + 1);
			else if (MapInfo[No - 1].Attribute           == library)AttributeUp(teacherRoom, No, No + ROW, No - 1);
			else if (MapInfo[No + 1].Attribute           == library)AttributeUp(teacherRoom, No, No + ROW, No + 1);
			else if(MapInfo[No - ROW * 1].Attribute      == library)AttributeUp(teacherRoom, No, No + ROW, No - ROW * 1);
		}
		//左を調べる
		if (MapInfo[No - 1].Attribute == autoShop) {
			//左の左　左の上  左の下　下　　右　　左
			if (MapInfo[No - 2].Attribute            == library)AttributeUp(teacherRoom,No,No-1, No - 2);
			else if (MapInfo[No + ROW].Attribute     == library)AttributeUp(teacherRoom,No,No-1, No + ROW);
			else if (MapInfo[No - ROW - 1].Attribute == library)AttributeUp(teacherRoom,No,No-1, No - ROW - 1);
			else if (MapInfo[No - ROW].Attribute     == library)AttributeUp(teacherRoom,No,No-1, No -ROW);
			else if (MapInfo[No + 1].Attribute       == library)AttributeUp(teacherRoom,No,No-1, No + 1);
			else if (MapInfo[No - 1].Attribute       == library)AttributeUp(teacherRoom, No, No - 1, No -1);
		}
		//下を調べる
		if (MapInfo[No - ROW].Attribute == autoShop) {
			//下の下　　下の左　下の右　　右  左　上
			if (MapInfo[No - ROW*2].Attribute        == library)AttributeUp(teacherRoom, No, No - ROW, No - ROW*2);
			else if (MapInfo[No - ROW-1].Attribute   == library)AttributeUp(teacherRoom, No, No - ROW, No - ROW-1);
			else if (MapInfo[No - ROW + 1].Attribute == library)AttributeUp(teacherRoom, No, No - ROW, No - ROW + 1);
			else if (MapInfo[No + 1].Attribute       == library)AttributeUp(teacherRoom, No, No - ROW, No + 1);
			else if (MapInfo[No - 1].Attribute       == library)AttributeUp(teacherRoom, No, No - ROW, No - 1);
			else if (MapInfo[No + ROW].Attribute     == library)AttributeUp(teacherRoom, No, No - ROW, No + ROW);
		}
		//右を調べる
		if (MapInfo[No +1].Attribute == autoShop) {
			//右の右　　右の上　　右の下　上　下　左
			if (MapInfo[No +2].Attribute             == library)AttributeUp(teacherRoom, No, No + 1, No +2);
			else if (MapInfo[No + ROW + 1].Attribute == library)AttributeUp(teacherRoom, No, No + 1, No + ROW + 1);
			else if (MapInfo[No - ROW + 1].Attribute == library)AttributeUp(teacherRoom, No, No + 1, No - ROW + 1);
			else if (MapInfo[No + ROW].Attribute     == library)AttributeUp(teacherRoom, No, No + 1, No + ROW);
			else if (MapInfo[No - ROW].Attribute     == library)AttributeUp(teacherRoom, No, No + 1, No - ROW);
			else if (MapInfo[No - 1].Attribute       == library)AttributeUp(teacherRoom, No, No + 1, No -1);
		}
	}
}

//使っていない
void MapData::AttributeUp(int attr, int NoOne, int NoTwo, int NoThree) {
	if (attr==teacherRoom) {
		MapInfo[NoOne].communication += 3;
		MapInfo[NoOne].spirit += 3;
		MapInfo[NoOne].learning += 3;
		MapInfo[NoTwo].communication += 3;
		MapInfo[NoTwo].spirit += 3;
		MapInfo[NoTwo].learning += 3;
		MapInfo[NoThree].communication += 3;
		MapInfo[NoThree].spirit += 3;
		MapInfo[NoThree].learning += 3;
		MapInfo[NoOne].stage++;
	}
}