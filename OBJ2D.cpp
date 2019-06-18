#include "OBJ2D.h"
#include "NumberName.h"
using namespace GameLib;
// �R���X�g���N�^
OBJ2D::OBJ2D()
{
	clear();
}

OBJ2D::~OBJ2D() {
	clear();
	
}

void OBJ2D::clear()
{
	if (data) {
		delete data;
		data = nullptr;
	}

	if (sprs)
	{
		for (int i = 0; i < max; ++i) {
			if (sprs[i]) {
				delete sprs[i];
				sprs[i] = nullptr;
			}
		}
		delete[] sprs;
		sprs = nullptr;
	}
	xflip = true;
	canDraw = false;
	pos = VECTOR2(0, 0);
	speed = VECTOR2(0, 0);
	size = VECTOR2(0, 0);
	texColor = 0xFFFFFFFF;
	texPos = VECTOR2(0, 0);
	texSize=VECTOR2(0,0);
	Texangle=0;
	mvAlg = 0;
	eraseAlg = 0;
	delta = VECTOR2(0, 0);
	command=0;
	id=0;
	learning = 0;
	spirit = 0;
	communication = 0;
}

// �ړ�
void OBJ2D::move(bool leftFlag,int month,int day,int money)
{
	if (mvAlg) mvAlg->move(this,leftFlag,month,day,money);
	if (eraseAlg) eraseAlg->erase(this);
}


// �`��
void OBJ2D::draw()
{
	char buf[512];
	if (canDraw) {
		//Loadtexture����`��
		if (data)
		{
			DrawSprite(pos, size, data, xflip);
		}
		//���̂܂ܕ`��
		if (current_spr) {
			DrawSpr(pos, size, texPos, texSize, Texangle, texColor, current_spr, xflip);
			if (type == character) {
				sprintf_s(buf, "Learning:%d",learning);
				drawString(40, 368, buf, 0x000000FF, 0, 16, 16);
				sprintf_s(buf, "Communication:%d", communication);
				drawString(40, 398, buf, 0x000000FF, 0, 16, 16);
				sprintf_s(buf, "Spirit:%d", spirit);
				drawString(40, 428, buf, 0x000000FF, 0, 16, 16);
				if (earnMoneyDarwFlag) {
					sprintf_s(buf, "EarnMoney:%d", earnMoney);
					drawString(40, 458, buf, 0x000000FF, 0, 16, 16);
				}
				
			}
		}
	}
}

void OBJ2D::SetSprite(wchar_t* name, int no, int max) {
	if (no >= max)	return;

	bool flg = false;
	if (!sprs)
	{
		sprs = new sprite*[max];
		this->max = max;
		flg = true;
	}
	sprs[no] = new sprite(GetDevice(), name);
	if (flg)	ChangeSprite(no);
}

void OBJ2D::ChangeSprite(int no)
{
	current_spr = sprs[no];
}


///////////////////////////////
//OBJ2D Manager         
///////////////////////////////

void OBJ2DManager::init()
{
	objList.clear();						// ���X�g�̃N���A
}

// ���X�g�֒ǉ�
OBJ2D* OBJ2DManager::add(MoveAlg2D* mvAlg, VECTOR2 pos, int id)
{
	OBJ2D	item;
	item.mvAlg = mvAlg;						// �ړ��A���S���Y���ݒ�
	item.pos = pos;							// ���W�̐ݒ�
	item.id = id;
	objList.push_back(item);				// ���X�g�֒ǉ�
	return &(*objList.rbegin());
}

// �X�V
void OBJ2DManager::update(bool leftFlag,int month,int day,int money)
{
	// �ړ�
	for (auto& item : objList) {
		VECTOR2	old = item.pos;
		item.move(leftFlag,month,day,money);

		item.delta = item.pos - old;
		item.command = 0;
	}
	// ��m�[�h�̍폜
	auto it = objList.begin();
	while (it != objList.end()) {
		if (it->mvAlg) {
			it++;
		}
		else {
			it = objList.erase(it);
		}
	}
}



// �`��
void OBJ2DManager::draw()
{
	for (auto& item : objList) {
		item.draw();
	}
}
