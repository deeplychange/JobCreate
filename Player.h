#ifndef PLAYER_H
#define PLAYER_H

#include "Game_lib.h"
#include "AStar.h"

#define PLAYER_MAX      (20)
#define PLAYER_START_X  (1.0f)
#define PLAYER_START_Y  (0.2f)
#define PLAYER_START_Z  (11.0f)

//1.0f, 0.2f, 11.0f, 0.0f, 64.0f, 64.0f, 64.0f


	class Player:public GameLib::DATA3D
	{
	public:
		Player();
		~Player();
		std::list <Point2> posList;
		std::string name;      //���O
		int stage;              //Astar�p�X�e�[�W
		int roadCheckStage;
		int updateStage;       //upData�X�e�[�W
		int time;              //�ړ����x�p����
		int GoalX, GoalY;       //�ړI�n���W
		float worldX, worldY;   //List�̍��W��ۑ�
		int animeTime;         //�A�j���p����
		int learning;             //�v���O�����\��
		int communication;     //�R�~���j�P�[�V����
		int spirit;            //����
		int Destination;       //�ړI�n
		unsigned int DestinationDist;   //�ړI�n�܂ł̋���
		int workX, workY;         //�}�b�v�����W�]���p  
		int workGoalX, workGoalY;//�}�b�v�����W�]���p  
		int startPosX, startPosZ;//�����Ȃ��������̍��̈ʒu���L�^����p
		int stopTime;
		int workNum;
		bool arriveGoal;
		
		//void Init();
		void Update(int &money);
		void Animation();
		bool SetAStar(int goalX, int goalY);
	};

	int GetPlayerNum();

	//DATA3D�f�[�^,FRONT/UP,�摜�t�@�C��,���WX,���WY,���WZ,�ǂݍ���ł�摜��X�AY�ASX�ASW
	//void SetPlayer(Player* pPlayer, wchar_t *_filename, float posX = 3.0f, float posY = 0.2f, float posZ = 1.0, float _sx = 0.0f, float _sy = 0.0f, float _sw = 64.0f, float _sh = 64.0f);

#endif