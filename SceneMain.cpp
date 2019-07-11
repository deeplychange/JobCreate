//******************************************************************************
//
//
//      ゲーム
//
//
//******************************************************************************

//------< インクルード >---------------------------------------------------------
#include "playerManager.h"
#include "SceneMain.h"
#include "Blend.h"
#include "Texture.h"
#include "Player.h"
#include "SpriteData.h"
#include "skinned_mesh.h"
#include "3DMap.h"
#include "UI.h"

using namespace GameLib;
using namespace DirectX;
Camera camera;
MapData mapData;
POINT mousePos;
PlayerManager playerManager;

static const int one_second = 60;
static const int one_year = 12;
static const int init_money = 1234;
//--------------------------------
//  初期化処理
//--------------------------------
void SceneMain::init()
{
	Scene::init();	    // 基底クラスのinitを呼ぶ
	state = 0;         
	isPers = true;        //平行投影
	light_dir = DirectX::XMFLOAT4(0, 1, 0, 0);
	camera.SetDistance();
	canUIDraw = false;
	state = 0;
	mousePos = mScreenToClient();
	leftFlag = false;
	money = init_money;
	time = 0;
	month = 1;
	day = 0;
	frame = 60;
}

//--------------------------------
//  更新処理
//--------------------------------
void SceneMain::update()
{
	switch (state) {
	case Init://初期設定
		GameLib::setBlendMode(Blend::BLEND_MODE::ALPHA);   // 通常のアルファ処理
		//テクスチャ初期化
		pTextureManager->loadTextures(load_texture);
		//2D初期化
		pUIManager->init();

		projection = camera.GetProjection();  //カメラの投影を設定
		
		//マップの初期化
		mapData.InitializeMap();
		//看板の初期化
		playerManager.Init();

		//GameLib::printf("ポリゴン作成しーーーーーーーます！");            //すごいdebug
		leftFlag = false;

		state++;
		//break;
	case Updata:
		leftFlag = false;
		//左クリックフラグ
		KeyFlag(leftFlag);

		//2Dの更新
		if (GetAsyncKeyState(VK_LSHIFT) & 1) {SetFrame(120); }
		if (GetAsyncKeyState(VK_RSHIFT) & 1) {SetFrame(60);  }
		//マップの更新
		mapData.UpdateMap(projection,&mousePos,money, leftFlag);
		//看板の更新
		playerManager.Update(&mousePos,money,leftFlag);
		pUIManager->update(leftFlag, month, day, money);
		//カメラの更新
		//カメラのタゲット毎フレームを更新
		{
			//static GameLib::DATA3D map = mapData.GetMap(190);
			//camera.Update(map, &mousePos);
			camera.Update(&mousePos);
		}
		
		mousePos = mScreenToClient();
		if (mousePos.x <= 0)mousePos.x = 0;
		if (mousePos.x >= GameLib::system::SCREEN_WIDTH)mousePos.x = GameLib::system::SCREEN_WIDTH;
		if (mousePos.y <= 0)mousePos.y = 0;
		if (mousePos.y >= GameLib::system::SCREEN_HEIGHT)mousePos.y = GameLib::system::SCREEN_HEIGHT;

		//時間設定
		time++;
		if (time >= one_second * 20) {   //20秒一旬
			day++;
			time = 0;
		}
		if (day >= DateMax) {
			month++;
			if(month<=one_year)day = 0;
			if(month>one_year)state++;
		}
		break;
	case Event:
		leftFlag = false;
		//左クリックフラグ
		KeyFlag(leftFlag);
		pUIManager->update(leftFlag, month, day,money);
		break;
	}




	//UI枠の表示設定
	if (MousePosCheck(mousePos,0, 0, 64, 64*3)) {
		UIchooseFrame.drawFlag = true;
		if (MousePosCheck(mousePos,FRAME_SIZE* 0, FRAME_SIZE *0, FRAME_SIZE *1, FRAME_SIZE * 1))UIchooseFrame.posNum = UIFrame::eUIBuilding;
		if (MousePosCheck(mousePos,FRAME_SIZE* 0, FRAME_SIZE *1, FRAME_SIZE *1, FRAME_SIZE * 2))UIchooseFrame.posNum = UIFrame::eUIEvent;
		if (MousePosCheck(mousePos,FRAME_SIZE* 0, FRAME_SIZE *2, FRAME_SIZE *1, FRAME_SIZE * 3))UIchooseFrame.posNum = UIFrame::eUIHelp;

	}
	else UIchooseFrame.drawFlag = false;

	if (UISecondary.drawFlag) {
	//	if (MousePosCheck(mousePos, 64*1, 0,140+32, 64 * 4)) {
			UIchooseFrame.drawFlag = true;
			if (MousePosCheck(mousePos, FRAME_SIZE*1, FRAME_SIZE * 0, FRAME_SIZE * 2, FRAME_SIZE * 1))UIchooseFrame.posNum = UIFrame::eUIRoad;
			if (MousePosCheck(mousePos, FRAME_SIZE*1, FRAME_SIZE * 1, FRAME_SIZE * 2, FRAME_SIZE * 2))UIchooseFrame.posNum = UIFrame::eUITeachroom;
			if (MousePosCheck(mousePos, FRAME_SIZE*1, FRAME_SIZE * 2, FRAME_SIZE * 2, FRAME_SIZE * 3))UIchooseFrame.posNum = UIFrame::eUILibrary;
			if (MousePosCheck(mousePos, FRAME_SIZE*1, FRAME_SIZE * 3, FRAME_SIZE * 2, FRAME_SIZE * 4))UIchooseFrame.posNum = UIFrame::eUIVendingMachine;
			if (MousePosCheck(mousePos, ARROW_RIGHT_POS_X, ARROW_RIGHT_POS_Y, ARROW_RIGHT_POS_X + ARROW_SIZE, ARROW_RIGHT_POS_Y + ARROW_SIZE))UIchooseFrame.posNum = UIFrame::eUIArrowRight;
			if (MousePosCheck(mousePos, ARROW_LEFT_POS_X, ARROW_LEFT_POS_Y, ARROW_RIGHT_POS_X + ARROW_SIZE, ARROW_LEFT_POS_Y+ARROW_SIZE))UIchooseFrame.posNum = UIFrame::eUIArrowLeft;
		
	//	}
	}
}

//--------------------------------
//  描画処理
//--------------------------------
void SceneMain::draw()
{
	//カメラのビュー取得
	view = camera.SetView();
	
	//*********************************************
	//3D描画
	//*********************************************
	//建築描画
	mapData.DrawMap(view, projection, light_dir);
	//キャラクター描画
	playerManager.Draw(view, projection, light_dir);
	//カメラディバグ用描画
	camera.Draw();

	//*********************************************
	//2D描画
	//*********************************************
	//UI
	pUIManager->draw();


	
#if DEBUG_FG
	//font描画
	char buf[512];
	sprintf_s(buf, "mouseX:%d  mouseY:%d",mousePos.x,mousePos.y);
	drawString(800, 600, buf, 0xFFFFFFFF, 0, 16, 36);
#endif

}

//--------------------------------
//  終了処理
//--------------------------------
void SceneMain::uninit()
{
	pTextureManager->releaseTexture();
	pUIManager->Delete();

}

int SceneMain::GetMoney() {
	return money;
}

void SceneMain::PlusMoney(int plusMoney) {
	money += plusMoney;
}

void SceneMain::MinusMoney(int minusMoney) {
	money -= minusMoney;
}

//******************************************************************************

