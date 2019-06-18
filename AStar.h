#ifndef ASTAR_H
#define	ASTAR_H

#include <vector>
#include <map>
#include <list>
#include "Game_lib.h"

#define SPEED  (1.0f/60.0f)

enum {  //優先度
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
	Node *parent ;   //親Node
	Node(int x,int y);
	Node();
	~Node();
	
	bool Priority;  //優先度 上、下方向の優先度が左、右より高い
	int x, y;
	bool isChecked;    //すでにチェックすみフラグ
	int step;   //歩数(g)
	int dist;   //距離(h)
	int stepAddDist;  //歩数＋距離(f)

	
	//始点からゴールまでの距離を計算する
	//addFeet計算(feet+forGoalDist)
	void SetDist(int _goalX, int _goalY); 
	//座標を設定する
	void SetPos(int _x, int _y);
	//fを取得
	int GetStepAddDist();
	//gを取得
	int GetStep();
	//hを取得
	int GetDist();

	// 自分とParentの座標を読み込んで記録する
	void GetPath(std::list<Point2> *pList);
	void GetPath(std::vector<Point2> *pList);

};

class NodeManager {
public:
	//オープンリスト.
	std::list<Node*> openList;
	// ノードインスタンス管理.
	std::map<int, Node> pool;

	//ゴール座標
	int goalX = 0;
	int goalY = 0;

	NodeManager(int _goalX,int _goalY);
	~NodeManager();
	
	// 指定の座標にあるノードをオープンする.
	Node* OpenNode(int _x,int _y,int cost,Node* parent,bool _priority);

	Node* GetNode(int _x,int _y);

	void OpenAround(Node* _node);
	// 最小スコアのノードを探す
	Node* SeachMinStepNode();
};

bool Astar(float _x, float _y, int _goalX, int _goalY,std::list<Point2>* pList);
#endif