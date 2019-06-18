#ifndef INCLUDED_GAME
#define	INCLUDED_GAME
//*****************************************************************************************************************************
//
//		���C���V�[��
//
//*****************************************************************************************************************************

#include "Scene.h"
#include "template.h"
#include <DirectXMath.h>
#include "Game_lib.h"
#include "Camera.h"
#include "Billboard.h"
class	SceneMain : public Scene, public Singleton<SceneMain>
{
private:
	int state;
	bool leftFlag;
	int time;
	int month;
	int day;            //��{�@���{�@���{
	int frame;
	//�r���[
	DirectX::XMMATRIX view;
	
	//���e
	DirectX::XMMATRIX projection;
	//guangyuan sheding
	DirectX::XMFLOAT4 light_dir;
	
	int money;

	bool canUIDraw;

	bool isPers;  //true:���s���e�@�@false:�������e
public:
	
	void init();
	void update();
	void draw();
	void uninit();
	int GetMoney();
	void PlusMoney(int plusMoney);
	void MinusMoney(int MinusMoney);
};


extern Camera camera;

extern Billboard billBoard;

#define SCENE_MAIN      (SceneMain::getInstance())

//******************************************************************************

#endif // !INCLUDED_GAME

