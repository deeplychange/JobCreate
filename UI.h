#ifndef UI_H_
#define	UI_H_
#include "template.h"
#include "OBJ2D.h"


#define FRAME_SIZE (64)
#define ARROW_TEX_SIZE (64)
#define ARROW_SIZE  (32)
#define ARROW_RIGHT_POS_X (140)
#define ARROW_RIGHT_POS_Y (90)
#define ARROW_LEFT_POS_X (140)
#define ARROW_LEFT_POS_Y (ARROW_RIGHT_POS_Y+ARROW_TEX_SIZE)
// UIManagerクラス
class UIManager : public OBJ2DManager, public Singleton<UIManager>
{
public:
	void init();
	bool isAllive();				// 生存チェック
	void Delete();

};

// 移動アルゴリズム
class UIListMain : public MoveAlg2D
{
public:
	int stage=0;
	POINT point;
	void move(OBJ2D* obj,bool leftFlag,int month,int day, int money = 0);
};
// 移動アルゴリズム
class UIListSecondary : public MoveAlg2D
{
public:
	bool drawFlag;
	int stage=0;
	int drawNO = 0;   //今描画の番号
	POINT point;
	void move(OBJ2D* obj,bool leftFlag,int month,int day, int money = 0);
};
// 移動アルゴリズム
class UIListAttribute : public MoveAlg2D
{
public:
	bool drawFlag;
	int learning;      //プログラム能力
	int communication; //コミュニケーション
	int spirit;        //根性
	int earnMoney;
	bool earnMoneyDarwFlag;
	int stage=0;
	int PlayerOrBillboard;  
	POINT point;
	void move(OBJ2D* obj, bool leftFlag, int month, int day, int money = 0);
};

class UIListDay : public MoveAlg2D
{
public:
	int stage=0;
	void move(OBJ2D* obj, bool leftFlag, int month, int day, int money = 0);
};
class UIListMonth : public MoveAlg2D
{
public:
	int stage=0;
	void move(OBJ2D* obj, bool leftFlag, int month, int day, int money = 0);
};
class UIListMonthNo : public MoveAlg2D
{
public:
	int stage=0;
	void move(OBJ2D* obj, bool leftFlag, int month, int day, int money = 0);
};
class UIListHelp : public MoveAlg2D
{
public:
	bool drawFlag;
	int stage=0;
	POINT point;
	void move(OBJ2D* obj, bool leftFlag, int month, int day, int money = 0);
};
class UIListChooseFrame : public MoveAlg2D
{
public:
	bool drawFlag;
	int posNum;
	int stage=0;
	POINT point;
	void move(OBJ2D* obj, bool leftFlag, int month, int day, int money = 0);
};


#define    DIGIT_MAX         (4)    //  表示桁数の最大
#define    DIGIT_SIZE        (64)   //  数字画像の 1 文字の大きさ
#define    NUM_DIGIT_1LINE   (8)    //  数字画像の 1 列に並んでいる数
#define    DigitDispSize     (20)   //数値表示の際の文字サイズ
#define    NUMBER_POS_X      (1180.0f)
#define    NUMBER_POS_Y      (40.0f)
class UIListMoney : public MoveAlg2D
{
public:
	int stage;
	void move(OBJ2D* obj, bool leftFlag, int month, int day, int money = 0);
};
class UIListMoney1 : public MoveAlg2D
{
public:
	bool drawFlag;
	int stage;
	POINT point;
	float numberPosX;
	float numberPosY;
	BYTE   digit[DIGIT_MAX];           //  各桁の値
	float    digitDispSize;             //  数値表示の際の文字サイズ
	void SetDigitDispSize(int size);      //  文字サイズ変更関数
	void SetNumber(int num);       //  数値の設定
	virtual void move(OBJ2D* obj, bool leftFlag, int month, int day, int money = 0);
};

class UIListMoney2 :public UIListMoney1
{
public:
	float numberPosX;
	void move(OBJ2D* obj, bool leftFlag, int month, int day, int money = 0);
};

class UIListMoney3 :  public UIListMoney1
{
public:
	float numberPosX;
	void move(OBJ2D* obj, bool leftFlag, int month, int day, int money = 0);
};

class UIListMoney4 : public UIListMoney1
{
public:
	float numberPosX;
	void move(OBJ2D* obj, bool leftFlag, int month, int day, int money = 0);
};

class UIListArrowLeft : public MoveAlg2D
{
public:
	int stage=0;
	POINT point;
	void move(OBJ2D* obj, bool leftFlag, int month, int day, int money = 0);
};
class UIListArrowRight : public MoveAlg2D
{
public:
	POINT point;
	int stage=0;
	void move(OBJ2D* obj, bool leftFlag, int month, int day, int money = 0);
};
bool MousePosCheck(POINT point, int x, int y, int x2, int y2);

#define pUIManager		(UIManager::getInstance())
#define UI_BEGIN		(pUIManager->getList()->begin())
#define UI_END			(pUIManager->getList()->end())

extern UIListMain		UIMain;
extern UIListSecondary      UISecondary;
extern UIListAttribute      UIAttribute;
extern UIListChooseFrame      UIchooseFrame;
//******************************************************************************
#endif // !_PLAYER_H_