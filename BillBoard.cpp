#include <process.h>
#include "Billboard.h"
#include "3DMap.h"
#include "Camera.h"
#include "UI.h"
#include "NumberName.h"

using namespace DirectX;
using namespace GameLib;


Player player[PLAYER_MAX];

std::string Name[]{
	"A","B","C","D","E","F","G"
};//7個


//歩数
int Step(int pos,int step) {
	return (int)(pos+step*LENGTH);
}





void SetPlayer(Player* player, wchar_t *_filename, float posX, float posY, float posZ, float _sx, float _sy, float _sw, float _sh) {
	static int nameNO = 0;
	
	player->sx = _sx; player->sy = _sy; player->sw = _sw; player->sh = _sh;
	player->SetPrimitive(playerDraw, false, _filename);
	player->pos = ChangeToWorldPos(XMFLOAT3(posX, posY, posZ));
	player->tex = false;
	player->scale = XMFLOAT3(0.7f, 1.5f, 1.0f);
	player->speed = XMFLOAT2(0.0f, 0.0f);
}


int GetPlayerNum() {
	for (int i = 0; i < PLAYER_MAX; i++) {
		if (!player[i].obj)return i;
		if (i == PLAYER_MAX&&player[i].obj)return 20;
	}
	return -1;
}

//建築を透明化
void Transparent() {
	for (int i = 0; i < BUILDABLE_MAX; ++i) {
		for (int j = 0; j < PLAYER_MAX; ++j) {
			if (player[j].obj&&mapData.Building[i]->Attribute!=road) {
				if (player[j].pos.x == mapData.Building[i]->pos.x&&player[j].pos.z == mapData.Building[i]->pos.z) {
					mapData.Building[i]->materialColor.w = 0.3f;
				}
				else  mapData.Building[i]->materialColor.w = 1.0f;

			}
		}
	}
}
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////
/////////////////////////        Billboardクラスの実装
/////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

Billboard::~Billboard() {
	for (int i = 0; i < PLAYER_MAX; ++i) {
		if (player[i].obj) {
			delete player[i].obj;
			player[i].obj = nullptr;
		}
	}
}



void Billboard::Init() {
	for (int i = 0; i < PLAYER_MAX; ++i) {
		player[i].Id = i;
	}
	playerNum = 0;
	stage = 0;
	addPlayFlag = false;
	memoryPlayerNum = 0;
	//一人作る
	SetPlayer(&player[0], L"./Data/char/Player.png", PLAYER_START_X, PLAYER_START_Y, PLAYER_START_Z, 0.0f, 64.0f, 64.0f, 64.0f);
	stage = playerMove;
}

//Player設定
void Billboard::Update(POINT* point, int &money,bool leftflag) {
	static int nameNO = 0;
	switch (stage) {
	case addPlayer:   //新規プレイヤー
		playerNum = GetPlayerNum();
		if (playerNum >= PLAYER_MAX) { 
			stage++; 
		}
		SetPlayer(&player[playerNum], L"./Data/char/Player.png", PLAYER_START_X, PLAYER_START_Y, PLAYER_START_Z, 0.0f,64.0f, 64.0f, 64.0f);
		stage++;
	case playerMove:   //Atart経路移動
		for (int i = 0; i < PLAYER_MAX; i++) {
			if (player[i].obj) {
				player[i].Animation();
				player[i].Update(money);
			}
		}	
		if (addPlayFlag) {
			stage = addPlayer;
			addPlayFlag = false;
		}
		if (GetAsyncKeyState('Y') & 1) {
			stage = addPlayer;
		}
		break;
	}

	{//Pick Player
		//PLAYERを選択
		if (leftflag) {            //左クリックフラグチェック
			DATA3D* fakeMesh = new DATA3D;
			float memoyDist = 10000.0f;//交線と各MESHの最短距離用
			for (int i = 0; i < PLAYER_MAX; i++) {
				if (player[i].obj) {      //画像あれば
					camera.Pick(point, player[i], *fakeMesh, &memoyDist);
				}
			}
			if (fakeMesh->obj) {
				for (int i = 0; i < PLAYER_MAX; i++) {
					if (player[i].obj == fakeMesh->obj)
					{
						memoryPlayerNum = i;
						UIAttribute.drawFlag = true;
						UIAttribute.PlayerOrBillboard = PLAYER;
						UIAttribute.learning = player[i].learning;
						UIAttribute.spirit = player[i].spirit;
						UIAttribute.communication = player[i].communication;
						UIAttribute.earnMoneyDarwFlag = false;
					}
				}
			}
			fakeMesh = nullptr;
			delete fakeMesh;
		}
	}
	
	//UIがPlayerのデータを読み込みしたらずっと更新する
	if (UIAttribute.drawFlag&&UIAttribute.PlayerOrBillboard==PLAYER) {
		UIAttribute.learning = player[memoryPlayerNum].learning;
		UIAttribute.spirit = player[memoryPlayerNum].spirit;
		UIAttribute.communication = player[memoryPlayerNum].communication;
	}
}



//描画
void Billboard::Draw(const DirectX::XMMATRIX& view,
	const DirectX::XMMATRIX& projection,
	const DirectX::XMFLOAT4& light_dir) {

	for (int i = 0; i < PLAYER_MAX; ++i) {
		if (player[i].obj) {
		 	player[i].Draw(view, projection, XMLoadFloat4(&light_dir), true, player[i].sx, player[i].sy, player[i].sw, player[i].sh);
		}
	}
}



