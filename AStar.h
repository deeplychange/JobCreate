#ifndef ASTAR_H
#define	ASTAR_H

#include <vector>
#include <map>
#include <list>
#include "Game_lib.h"

#define SPEED  (1.0f/60.0f)

enum {  //�D��x
	LOW,
	HIGH,
};

class Point2 {
public: 
	int x, y;
	Point2(int _x, int _y) {
		x=_x ;
		y=_y ;
	}

	void set(int _x, int _y) {
		x=_x ;
		y=_y ;
	}
};


class Node {
	
public:
	Node *parent ;   //�eNode
	Node(int x,int y);
	Node();
	~Node();
	
	bool Priority;  //�D��x ��A�������̗D��x�����A�E��荂��
	int x, y;
	bool isChecked;    //���łɃ`�F�b�N���݃t���O
	int step;   //����(g)
	int dist;   //����(h)
	int stepAddDist;  //�����{����(f)

	
	//�n�_����S�[���܂ł̋������v�Z����
	//addFeet�v�Z(feet+forGoalDist)
	void SetDist(int _goalX, int _goalY); 
	//���W��ݒ肷��
	void SetPos(int _x, int _y);
	//f���擾
	int GetStepAddDist();
	//g���擾
	int GetStep();
	//h���擾
	int GetDist();

	// ������Parent�̍��W��ǂݍ���ŋL�^����
	void GetPath(std::list<Point2> *pList);
	void GetPath(std::vector<Point2> *pList);

};

class NodeManager {
public:
	//�I�[�v�����X�g.
	std::list<Node*> openList;
	// �m�[�h�C���X�^���X�Ǘ�.
	std::map<int, Node> pool;

	//�S�[�����W
	int goalX = 0;
	int goalY = 0;

	NodeManager(int _goalX,int _goalY);
	~NodeManager();
	
	// �w��̍��W�ɂ���m�[�h���I�[�v������.
	Node* OpenNode(int _x,int _y,int cost,Node* parent,bool _priority);

	Node* GetNode(int _x,int _y);

	void OpenAround(Node* _node);
	// �ŏ��X�R�A�̃m�[�h��T��
	Node* SeachMinStepNode();
};

bool Astar(float _x, float _y, int _goalX, int _goalY,std::list<Point2>* pList);
#endif