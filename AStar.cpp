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
	
	//���݂��Ă����
	if (pool.find(idx) != pool.end())return &pool[idx];
	//���݂��Ă��Ȃ��Ȃ�ۑ�����
	Node node(_x,_y);
	// �q���[���X�e�B�b�N�E�R�X�g���v�Z����.
	node.SetDist( goalX, goalY);
	
	pool[idx] = node;
	return &pool[idx];
}

Node* NodeManager::OpenNode(int _starX,int _starY,int cost,Node* _parent,bool _priority) {
	Node *node;
	// ���W���`�F�b�N
	if (_starX > ROW || _starY > COLUMN) return nullptr;
	if (_starX < 0 || _starY < 0)  return nullptr;

	if (!mapData.MapInfo[_starX + _starY * ROW].canGo)return nullptr;
	//if (mapData.MapInfo[_starX + _starY * ROW].attribute)return nullptr;
	// �m�[�h���擾����
	node = GetNode(_starX, _starY);
	if (node->isChecked) return nullptr;
	
	node->Priority = _priority;
	node->step = cost;
	node->parent = _parent;
	node->isChecked = true;
	node->stepAddDist = node->step + node->dist;
	openList.push_back(node); // �m�[�h���I�[�v�����X�g�ɒǉ�����.
	return node;
}

void NodeManager::OpenAround(Node* _node) {
	//���̍��W�Acost���L�^����
	
	int x = _node->x;
	int y = _node->y;
	int step = _node->step;
	step += 1;
	//�l������Open����
	OpenNode(x - 1, y, step, _node,LOW); // �� .
	OpenNode(x, y - 1, step, _node,HIGH); // ��.
	OpenNode(x + 1, y, step, _node,LOW); // �E.
	OpenNode(x, y + 1, step, _node,HIGH); // ��.
}

Node* NodeManager::SeachMinStepNode() {
	// �ŏ��X�R�A
	int min = 9999;
	// �ŏ����R�X�g
	int minCost = 9999;

	Node* minNode=nullptr ;
	for each(Node *node in openList) {
		int score = node->GetStepAddDist();
		if (score > min) {
			// �X�R�A���傫��
			continue;
		}
		//�D��x�̒���
		if (score == min && node->step >= minCost/*&&!node->Priority*/) {
			// �X�R�A�������Ƃ��͎��R�X�g����r����
			continue;
		}
		// �ŏ��l�X�V.
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
	//���[���h���W����Point2�ɕϊ�
	int AStarStartX = (int)(_x / LENGTH);
	int AStarStartY = (int)(_y / LENGTH);
	int AStarGoalX = (int)(_goalX / LENGTH);
	int AStarGoalY = (int)(_goalY / LENGTH);
	
	//�S�[���ݒ�
	NodeManager mgr = NodeManager(AStarGoalX, AStarGoalY);

	//�N�_�ݒ�
	Node* node = mgr.OpenNode(AStarStartX, AStarStartY,0,nullptr,HIGH);
	if (!node)return false;
	//Astar�o�H���s
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
		// �l�������J��
		mgr.OpenAround(node);
		
		// �ŏ��X�R�A�̃m�[�h��T��
		node = mgr.SeachMinStepNode();
		//�G���[�`�F�b�N
		if (node == nullptr)break;
		//�S�[���`�F�b�N
		if (node->x == AStarGoalX && node->y == AStarGoalY) {
			//openlist���獡�̃m�[�h���폜����
			mgr.openList.clear();
			// �p�X���擾����
			node->GetPath(pList);
			//���]����
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

