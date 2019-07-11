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
// UIManager�N���X
class UIManager : public OBJ2DManager, public Singleton<UIManager>
{
public:
	void init();
	bool isAllive();				// �����`�F�b�N
	void Delete();

};

// �ړ��A���S���Y��
class UIListMain : public MoveAlg2D
{
public:
	int stage=0;
	POINT point;
	void move(OBJ2D* obj,bool leftFlag,int month,int day, int money = 0);
};
// �ړ��A���S���Y��
class UIListSecondary : public MoveAlg2D
{
public:
	bool drawFlag;
	int stage=0;
	int drawNO = 0;   //���`��̔ԍ�
	POINT point;
	void move(OBJ2D* obj,bool leftFlag,int month,int day, int money = 0);
};
// �ړ��A���S���Y��
class UIListAttribute : public MoveAlg2D
{
public:
	bool drawFlag;
	int learning;      //�v���O�����\��
	int communication; //�R�~���j�P�[�V����
	int spirit;        //����
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


#define    DIGIT_MAX         (4)    //  �\�������̍ő�
#define    DIGIT_SIZE        (64)   //  �����摜�� 1 �����̑傫��
#define    NUM_DIGIT_1LINE   (8)    //  �����摜�� 1 ��ɕ���ł��鐔
#define    DigitDispSize     (20)   //���l�\���̍ۂ̕����T�C�Y
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
	BYTE   digit[DIGIT_MAX];           //  �e���̒l
	float    digitDispSize;             //  ���l�\���̍ۂ̕����T�C�Y
	void SetDigitDispSize(int size);      //  �����T�C�Y�ύX�֐�
	void SetNumber(int num);       //  ���l�̐ݒ�
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