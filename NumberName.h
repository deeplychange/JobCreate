
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

//SceneMain Updatap
enum {
	Init,
	Updata,
	Event
};


enum Attribute {
	NONE = -1,
	road = 0,	//ΉH
	teacherRoom,//³tΊ
	library,    //}Ω
	autoShop,   //©Μ@
	musicRoom,  //ΉyΊ
	gymnasium,  //ΜηΩ
	toilet,     //gC
	AttributeMax,
};

//FΤ
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
	T_L_A,     //³tΊΖ}ΩΖ©Μ@
	G_M_T,       //ΉyΊΖΜηΩΖgC
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
	eUIVendingMachine, //©Μ@
	eUIArrowLeft,
	eUIArrowRight,

};
#endif