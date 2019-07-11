
#ifndef NUMBER_H
#define NUMBER_H

#define DEBUG_FG  (true)

enum {
	Clear,
	SetGoal,
	StartList1,
	StartList2,
	SetSpeed1,
	SetSpeed2,
	Run
};

//SceneMain Updata�p
enum {
	Init,
	Updata,
	Event
};


enum Attribute {
	NONE = -1,
	road = 0,	//���H
	teacherRoom,//���t��
	library,    //�}����
	autoShop,   //���̋@
	musicRoom,  //���y��
	gymnasium,  //�̈��
	toilet,     //�g�C��
	AttributeMax,
};

//�F�ԍ�
enum {
	YELLOW = 0,
	BLUE,
	GREEN,
	RED,
	WHITE,
	BLACK,
	BROWN,
	GRAY,
};


enum {
	setGoalType,
	setGoalPos,
	setAtar,
	runAtar
};

enum {
	setPlayer,
	addPlayer,
	playerMove
};
enum {
	PLAYER=1,
	BILLBOARD,
};
enum Compatibility{
	T_L_A,     //���t���Ɛ}���قƎ��̋@
	G_M_T,       //���y���Ƒ̈�قƃg�C��
	Max,
};				 


enum ForSetPrimitive {
	playerDraw=0,
	roadDraw,
	wallDraw,
	buildingDraw,
};

enum UIDate {
	DateEarly = 0,
	DateMid,
	DateLate,
	DateMax
};

enum UI {
	Secondary_rood,
	Secondary_gymnasium,
	Secondary_max,
};

enum UIFrame {
	eUIBuilding,
	eUIEvent,
	eUIHelp,
	eUIRoad,
	eUITeachroom,
	eUILibrary,
	eUIVendingMachine, //���̋@
	eUIArrowLeft,
	eUIArrowRight,

};
#endif