

#ifndef OBJ2D_H_
#define	OBJ2D_H_
#include <list>
#include "Game_lib.h"
#include "NumberName.h"
using namespace std;
//******************************************************************************
//
//
//		OBJ2D�N���X
//
//
//******************************************************************************

enum {
	frame=0,
	character,
	build,
};

// �O���錾
class OBJ2D;
// �ړ��A���S���Y���N���X�i���ۃN���X�j
class MoveAlg2D
{
public:
	virtual void move(OBJ2D* obj, bool leftFlag,int month,int day,int money) = 0;  // �������z�֐������̂ŁAMoveAlg�^�̃I�u�W�F�N�g�͐錾�ł��Ȃ��i�h�������Ďg���j
};

// �����A���S���Y���N���X�i���ۃN���X�j
class EraseAlg2D
{
public:
	virtual void erase(OBJ2D* obj) = 0; // ��Ɠ��l
};


class OBJ2D {
public:
	GameLib::SPRITE_DATA*	data;			// �摜�f�[�^
	
	sprite** sprs;
	sprite* current_spr;
	int		max;

	VECTOR2		pos;			// ���W
	VECTOR2		speed;			// ���x
	VECTOR2		size;			// �傫��
	UINTCOLOR texColor;         //�e�N�X�`���F
	VECTOR2 texPos;              //�e�N�X�`�����W
	VECTOR2 texSize;            //�e�N�X�`���傫��
	float Texangle;
	bool canDraw;              //�`��ł��邩�Ƃ��t���O
	int  type;                 //�\�͕`��p����  �@�L�����N�^�[�Ȃ當����`�悷��

	//�`��p�\��
	int learning;
	int spirit;
	int communication;
	int earnMoney;
	bool earnMoneyDarwFlag;

	MoveAlg2D*		mvAlg;			// �ړ��A���S���Y��
	EraseAlg2D*		eraseAlg;		// �����A���S���Y��
	int				id;
	VECTOR2		    delta;			// �ړ��ʂ̍���
	int				command;		// ���̃I�u�W�F�N�g����̃R�}���h
	bool			xflip;			// �摜���E���](�������Ă��Ȃ�)
	OBJ2D();						// �R���X�g���N�^
	~OBJ2D();
	void SetSprite(wchar_t *name, int no = 0, int max = 1);
	void ChangeSprite(int no);
	void clear();					// �����o�ϐ��̃N���A
	void move(bool leftFlag,int month,int day,int money);		
																	// �ړ�
	void draw();					// �`��
};

// OBJ2DManager�N���X
class OBJ2DManager
{
protected:
	list<OBJ2D>				objList;			// ���X�g
public:
	void init();								// �������i���ׂď����j
	OBJ2D* add(MoveAlg2D* mvAlg, VECTOR2 pos = VECTOR2(0, 0), int id = 0);		// ���X�g�֒ǉ�
	void update(bool leftFlag,int month,int day,int money);								// �X�V
	void draw();							// �`��
	list<OBJ2D>* getList() { return &objList; }
};

//******************************************************************************
#endif // !_OBJ2D_H_