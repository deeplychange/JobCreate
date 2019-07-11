
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

//SceneMain Updata用
enum {
	Init,
	Updata,
	Event
};


enum Attribute {
	NONE = -1,
	road = 0,	//道路
	teacherRoom,//教師室
	library,    //図書館
	autoShop,   //自販機
	musicRoom,  //音楽室
	gymnasium,  //体育館
	toilet,     //トイレ
	AttributeMax,
};

//色番号
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
	T_L_A,     //教師室と図書館と自販機
	G_M_T,       //音楽室と体育館とトイレ
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
	eUIVendingMachine, //自販機
	eUIArrowLeft,
	eUIArrowRight,

};
#endif