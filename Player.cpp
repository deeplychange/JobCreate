#include "Player.h"
#include "3DMap.h"

Player::Player() {
	//obj = new DATA3D;
	name = "";
	posList.clear();
	stage = 0;
	time = 0;
	GoalX = 0;
	GoalY = 0;
	worldX = 0;
	worldY = 0;
	animeTime = 0;
	learning = rand() % 30;             //0-30�����_���Őݒ�
	communication = rand() % 30;     //�R�~���j�P�[�V����
	spirit = rand() % 30;            //����
	arriveGoal = false;
	Destination = -1;
	stopTime = 0;
	workGoalY = 0;
	workGoalX = 0;
	roadCheckStage = 0;
}


Player::~Player()
{
	//ChenYuezong comment test
}

//Astar�̖ړI�n�X�V
void Player::Update(int &money) {
	DestinationDist = 100;
	int a = 0;
	int b = 0;
	if (this->speed.x >= 0)a = (int)((this->pos.x - 1) / 2.0f);
	else a = (int)((this->pos.x + 1) / 2.0f);
	if (this->speed.y >= 0)b = ROW* (int)((this->pos.z - 1) / 2.0f);
	else b = ROW* (int)((this->pos.z + 1) / 2.0f);
	int work = (a + b);
	//if (mapData.MapInfo[work].canGo) {	//���݂̈ʒu���ʂ��
	switch (updateStage) {
	case setGoalType:
		Destination = rand() % (AttributeMax-1)+1;   //1-max�܂�
		DestinationDist = 100;
		updateStage = 1;
		break;
	case setGoalPos:
		//�ړI�n���W�ݒ�
		for (int i = 0; i < BUILDABLE_MAX; ++i) {
			if (mapData.MapInfo[i].Attribute == Destination)    //�}�X�������������Ă����
			{
				workX = i%ROW;           //�ړI�nX
				workY = i / ROW;           //�ړI�nY
				workX = (int)(workX*LENGTH + LENGTH / 2.0f);
				workY = (int)(workY*LENGTH + LENGTH / 2.0f);
				posList.clear();
				Astar(this->pos.x, this->pos.z, workX, workY, &posList);  //posList�̃T�C�Y�v�Z
				if (posList.size()<DestinationDist)                      //��ԒZ������I��
				{
					workGoalX = workX;
					workGoalY = workY;
					DestinationDist = posList.size();
				}
			}
		}
		if (workGoalY != 0 || workGoalX != 0)updateStage = setAtar;
		else updateStage = setGoalPos;
		break;
	case setAtar:
		//Atar�o�H�T��
		arriveGoal = SetAStar(workGoalX, workGoalY);
		//��������STAGE++
		if (arriveGoal)updateStage = runAtar;
		break;
	case runAtar:
		this->speed = DirectX::XMFLOAT2(0.0f, 0.0f);
		workNum = (int)((this->pos.x - LENGTH / 2.0f) / LENGTH + ROW* (this->pos.z - LENGTH / 2.0f) / LENGTH);
		stopTime++;
		if (stopTime == 10) {
			learning += mapData.MapInfo[workNum].learning;
			spirit += mapData.MapInfo[workNum].spirit;
			communication += mapData.MapInfo[workNum].communication;
			money += mapData.MapInfo[workNum].earnMoney;
		}
		if (stopTime >= 60 * 2) {
			stopTime = 0;
			updateStage = setGoalType;
		}
		break;
	}
	//}

	//else {       //���݂̈ʒu���ʂ�Ȃ�
	//	//���}�b�v�ԍ�
	//	//work = (int)(obj->pos.x /LENGTH) + ROW* (int)((obj->pos.z/ LENGTH));
	//	startPosX = (int)(work%ROW*LENGTH + LENGTH / 2.0f);
	//	startPosZ= (int) (work/ROW*LENGTH + LENGTH / 2.0f);
	//	//���̔ԍ���菬�����̂�T��
	//	for (int i = work; i >0; --i) {
	//		if (mapData.MapInfo[i].Attribute == road) {
	//			workX = i%ROW;           //�ړI�nX
	//			workY = i/ROW;           //�ړI�nY
	//			int compareX = work%ROW;
	//			int compareY = work /ROW;
	//			if ((abs(compareX - workX)+ abs(compareY - workY)) < DestinationDist)
	//			{
	//				DestinationDist = (abs(compareX - workX) + abs(compareY - workY));
	//				workGoalX = (int)(workX*LENGTH + LENGTH / 2.0f);
	//				workGoalY = (int)(workY*LENGTH + LENGTH / 2.0f);
	//			}
	//		}
	//	}
	//	//���̔ԍ����傫���̂�T��
	//	for (int i = work; i < ROW*COLUMN; ++i) {
	//		if (mapData.MapInfo[i].Attribute == road) {
	//			workX = i%ROW;           //�ړI�nX
	//			workY = i/ROW;           //�ړI�nY
	//			int compareX = work%ROW;
	//			int compareY = work/ROW;
	//			if ((abs(compareX - workX) + abs(compareY - workY)) < DestinationDist)
	//			{
	//				DestinationDist = (abs(compareX - workX) + abs(compareY - workY));
	//				workGoalX = (int)(workX*LENGTH + LENGTH / 2.0f);
	//				workGoalY = (int)(workY*LENGTH + LENGTH / 2.0f);
	//			}
	//		}
	//	}
	//	obj->speed.x = (workGoalX - startPosX)*SPEED;   //���x�����ԁ��H��
	//	obj->speed.y = (workGoalY - startPosZ)*SPEED;
	//	obj->pos.x += obj->speed.x;
	//	obj->pos.z += obj->speed.y;
	//	/*if (obj->speed.x >= 0) {
	//		if (obj->pos.x >= workGoalX)obj->pos.x = workGoalX;
	//	}
	//	else {
	//		if (obj->pos.x <= workGoalX)obj->pos.x = workGoalX;
	//	}
	//	if (obj->speed.y >= 0) {
	//		if (obj->pos.z > workGoalY)obj->pos.z = workGoalY;
	//	}
	//	else {
	//		if (obj->pos.y <= workGoalY)obj->pos.z = workGoalY;
	//	}*/
	//}
}



bool Player::SetAStar(int goalX, int goalY) {
	switch (this->stage) {
	case Clear:
		this->stage = 0;
		this->time = 0;
		this->worldX = 0;
		this->worldY = 0;
		this->GoalX = 0;
		this->GoalY = 0;
		this->speed = DirectX::XMFLOAT2(0.0f, 0.0f);
		//break;
	case SetGoal:
		this->GoalX = goalX;
		this->GoalY = goalY;
		this->stage++;
		//break;
	case StartList1:
		this->posList.clear();
		Astar(this->pos.x, this->pos.z, this->GoalX, this->GoalY, &this->posList);
		this->stage = SetSpeed1;
		//break;
	case SetSpeed1:
		if (this->posList.size() == 0)    //List���Ȃ��Ȃ�����@TRUE
		{
			this->stage = Clear;
			return true;
			break;
		}
		this->worldX = this->posList.begin()->x*LENGTH + 1; //���[���h���W�ɕϊ�
		this->worldY = this->posList.begin()->y*LENGTH + 1; //���[���h���W�ɕϊ�
		this->speed.x = (this->worldX - this->pos.x)*SPEED;   //���x�����ԁ��H��
		this->speed.y = (this->worldY - this->pos.z)*SPEED;
		this->stage = Run;
		//break;
	case Run:
		this->pos.x += this->speed.x;
		this->pos.z += this->speed.y;
		this->time++;
		if (this->time >= 60) {                     //��b��g
			this->pos.x = this->worldX;
			this->pos.z = this->worldY;
			this->posList.erase(this->posList.begin());
			this->time = 0;
			this->stage = StartList1;
		}
		break;
	}
	return false;
}




//�A�j���[�V����
void Player::Animation() {
	this->animeTime++;
	if (this->speed.x < 0 || this->speed.y > 0) {    //�摜�̐؂�ւ�
		this->sy = 64 * 2;
	}
	if (this->speed.x > 0 || this->speed.y <0) {
		this->sy = 64 * 3;
	}
	if (this->speed.x == 0 && this->speed.y == 0) {
		if (this->sy == 64 * 2)this->sy = 64 * 0;
		if (this->sy == 64 * 3)this->sy = 64 * 1;
	}
	if (this->animeTime % 10 == 0) {
		this->sx += 64;
		if (this->sx >= 64 * 3)this->sx = 0;
		this->animeTime = 0;
	}
}


