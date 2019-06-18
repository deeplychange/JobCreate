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
		std::string name;      //名前
		int stage;              //Astar用ステージ
		int roadCheckStage;
		int updateStage;       //upDataステージ
		int time;              //移動速度用時間
		int GoalX, GoalY;       //目的地座標
		float worldX, worldY;   //Listの座標を保存
		int animeTime;         //アニメ用時間
		int learning;             //プログラム能力
		int communication;     //コミュニケーション
		int spirit;            //根性
		int Destination;       //目的地
		unsigned int DestinationDist;   //目的地までの距離
		int workX, workY;         //マップ情報座標転換用  
		int workGoalX, workGoalY;//マップ情報座標転換用  
		int startPosX, startPosZ;//道がない時自分の今の位置を記録する用
		int stopTime;
		int workNum;
		bool arriveGoal;
		
		//void Init();
		void Update(int &money);
		void Animation();
		bool SetAStar(int goalX, int goalY);
	};

	int GetPlayerNum();

	//DATA3Dデータ,FRONT/UP,画像ファイル,座標X,座標Y,座標Z,読み込んでる画像のX、Y、SX、SW
	//void SetPlayer(Player* pPlayer, wchar_t *_filename, float posX = 3.0f, float posY = 0.2f, float posZ = 1.0, float _sx = 0.0f, float _sy = 0.0f, float _sw = 64.0f, float _sh = 64.0f);

#endif