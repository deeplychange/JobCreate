//******************************************************************************
//
//
//      �Q�[��
//
//
//******************************************************************************

//------< �C���N���[�h >---------------------------------------------------------
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
//  ����������
//--------------------------------
void SceneMain::init()
{
	Scene::init();	    // ���N���X��init���Ă�
	state = 0;         
	isPers = true;        //���s���e
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
//  �X�V����
//--------------------------------
void SceneMain::update()
{
	switch (state) {
	case Init://�����ݒ�
		GameLib::setBlendMode(Blend::BLEND_MODE::ALPHA);   // �ʏ�̃A���t�@����
		//�e�N�X�`��������
		pTextureManager->loadTextures(load_texture);
		//2D������
		pUIManager->init();

		projection = camera.GetProjection();  //�J�����̓��e��ݒ�
		
		//�}�b�v�̏�����
		mapData.InitializeMap();
		//�Ŕ̏�����
		playerManager.Init();

		//GameLib::printf("�|���S���쐬���[�[�[�[�[�[�[�܂��I");            //������debug
		leftFlag = false;

		state++;
		//break;
	case Updata:
		leftFlag = false;
		//���N���b�N�t���O
		KeyFlag(leftFlag);

		//2D�̍X�V
		if (GetAsyncKeyState(VK_LSHIFT) & 1) {SetFrame(120); }
		if (GetAsyncKeyState(VK_RSHIFT) & 1) {SetFrame(60);  }
		//�}�b�v�̍X�V
		mapData.UpdateMap(projection,&mousePos,money, leftFlag);
		//�Ŕ̍X�V
		playerManager.Update(&mousePos,money,leftFlag);
		pUIManager->update(leftFlag, month, day, money);
		//�J�����̍X�V
		//�J�����̃^�Q�b�g���t���[�����X�V
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

		//���Ԑݒ�
		time++;
		if (time >= one_second * 20) {   //20�b��{
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
		//���N���b�N�t���O
		KeyFlag(leftFlag);
		pUIManager->update(leftFlag, month, day,money);
		break;
	}




	//UI�g�̕\���ݒ�
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
//  �`�揈��
//--------------------------------
void SceneMain::draw()
{
	//�J�����̃r���[�擾
	view = camera.SetView();
	
	//*********************************************
	//3D�`��
	//*********************************************
	//���z�`��
	mapData.DrawMap(view, projection, light_dir);
	//�L�����N�^�[�`��
	playerManager.Draw(view, projection, light_dir);
	//�J�����f�B�o�O�p�`��
	camera.Draw();

	//*********************************************
	//2D�`��
	//*********************************************
	//UI
	pUIManager->draw();


	
#if DEBUG_FG
	//font�`��
	char buf[512];
	sprintf_s(buf, "mouseX:%d  mouseY:%d",mousePos.x,mousePos.y);
	drawString(800, 600, buf, 0xFFFFFFFF, 0, 16, 36);
#endif

}

//--------------------------------
//  �I������
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

