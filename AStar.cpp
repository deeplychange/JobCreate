#include <algorithm>
#include "AStar.h"
#include "3DMap.h"



using namespace DirectX;

Node::Node(int _x,int _y) {
	parent = nullptr;
	x = _x;
	y = _y;
	isChecked = false;
	step = -1;
	dist = 0;
	stepAddDist = 10000;
	Priority = LOW;
}

Node::Node() {
	isChecked = false;
	step = -1;
	dist = 0;
	stepAddDist = 10000;
}
Node::~Node() {
	
}


void Node::SetPos(int _x, int _y) {
	x = _x;
	y = _y;
}

int Node::GetStep() {
	return step;
}
int Node::GetStepAddDist() {
	return stepAddDist;
}

int Node::GetDist() {
	return dist;
}

void Node::SetDist(int _goalX,int _goalY) {
	int distX = abs(_goalX - x);
	int distY = abs(_goalY - y);
	dist = distX + distY;
}

void Node::GetPath(std::list<Point2>* pList) {
	pList->push_back(Point2(x, y));
	if (parent != nullptr) {
		parent->GetPath(pList);
	}

}

void Node::GetPath(std::vector<Point2> *pList) {
	pList->push_back(Point2(x, y));
	if (parent != nullptr) {
		parent->GetPath(pList);
	}
}


//-----------------------------------------------------
//NodeManager
//-----------------------------------------------------
NodeManager::NodeManager(int _goalX, int _goalY) {
	goalX=_goalX ;
	goalY=_goalY ;
	openList =  std::list<Node*>();
	pool = std::map<int, Node>();
}


NodeManager::~NodeManager() {
	
	for (auto it = openList.begin(); it != openList.end(); it++) {
		delete (*it);
	}
}

Node* NodeManager::GetNode(int _x, int _y) {
	int idx=  _x + (_y * COLUMN);
	
	//存在していると
	if (pool.find(idx) != pool.end())return &pool[idx];
	//存在していないなら保存する
	Node node(_x,_y);
	// ヒューリスティック・コストを計算する.
	node.SetDist( goalX, goalY);
	
	pool[idx] = node;
	return &pool[idx];
}

Node* NodeManager::OpenNode(int _starX,int _starY,int cost,Node* _parent,bool _priority) {
	Node *node;
	// 座標をチェック
	if (_starX > ROW || _starY > COLUMN) return nullptr;
	if (_starX < 0 || _starY < 0)  return nullptr;

	if (!mapData.MapInfo[_starX + _starY * ROW].canGo)return nullptr;
	//if (mapData.MapInfo[_starX + _starY * ROW].attribute)return nullptr;
	// ノードを取得する
	node = GetNode(_starX, _starY);
	if (node->isChecked) return nullptr;
	
	node->Priority = _priority;
	node->step = cost;
	node->parent = _parent;
	node->isChecked = true;
	node->stepAddDist = node->step + node->dist;
	openList.push_back(node); // ノードをオープンリストに追加する.
	return node;
}

void NodeManager::OpenAround(Node* _node) {
	//今の座標、costを記録する
	
	int x = _node->x;
	int y = _node->y;
	int step = _node->step;
	step += 1;
	//四方向をOpenする
	OpenNode(x - 1, y, step, _node,LOW); // 左 .
	OpenNode(x, y - 1, step, _node,HIGH); // 上.
	OpenNode(x + 1, y, step, _node,LOW); // 右.
	OpenNode(x, y + 1, step, _node,HIGH); // 下.
}

Node* NodeManager::SeachMinStepNode() {
	// 最小スコア
	int min = 9999;
	// 最小実コスト
	int minCost = 9999;

	Node* minNode=nullptr ;
	for each(Node *node in openList) {
		int score = node->GetStepAddDist();
		if (score > min) {
			// スコアが大きい
			continue;
		}
		//優先度の調整
		if (score == min && node->step >= minCost/*&&!node->Priority*/) {
			// スコアが同じときは実コストも比較する
			continue;
		}
		// 最小値更新.
		min = score;
		minCost = node->step;
		minNode = node;
	}
	return minNode;
}


////////////////////////////////////////////////////////
//Astar
/////////////////////////////////////////////////////////


bool Astar(float _x,float _y, int _goalX,int _goalY, std::list<Point2>* pList) {
	//ワールド座標からPoint2に変換
	int AStarStartX = (int)(_x / LENGTH);
	int AStarStartY = (int)(_y / LENGTH);
	int AStarGoalX = (int)(_goalX / LENGTH);
	int AStarGoalY = (int)(_goalY / LENGTH);
	
	//ゴール設定
	NodeManager mgr = NodeManager(AStarGoalX, AStarGoalY);

	//起点設定
	Node* node = mgr.OpenNode(AStarStartX, AStarStartY,0,nullptr,HIGH);
	if (!node)return false;
	//Astar経路実行
	int cnt = 0;
	while (cnt<100) {
		if(!node)break;
		auto i= mgr.openList.begin(), end_= mgr.openList.end();
		while(i!=end_){
			if ((*i) == node) {
				i=mgr.openList.erase(i);
				continue;
			}
			++i;
		}
		//mgr.openList.erase(node);
		// 四方向を開く
		mgr.OpenAround(node);
		
		// 最小スコアのノードを探す
		node = mgr.SeachMinStepNode();
		//エラーチェック
		if (node == nullptr)break;
		//ゴールチェック
		if (node->x == AStarGoalX && node->y == AStarGoalY) {
			//openlistから今のノードを削除する
			mgr.openList.clear();
			// パスを取得する
			node->GetPath(pList);
			//反転する
			//pList.reverse();

			std::reverse(pList->begin(),pList->end());
			break;
		}
		cnt++;
	}
	if (pList->size()==0)return false;
	pList->erase(pList->begin());
	
	return true;
}

