#ifndef _ENEMY_H_
#define	_ENEMY_H_

#include "OBJ2D.h"
#include "template.h"
//******************************************************************************
//
//
//		Enemyクラス
//
//
//******************************************************************************
//==============================================================================
//
//		移動アルゴリズム
//
//==============================================================================

// 移動アルゴリズム
class Enemy1 : public MoveAlg2D
{
public:
	void move(OBJ2D* obj);
};

class Enemy2 : public MoveAlg2D
{
public:
	void move(OBJ2D* obj);
};

class EnemyOut : public MoveAlg2D
{
public:
	void move(OBJ2D* obj);
};

extern Enemy1 enemy1;
extern Enemy2 enemy2;
extern EnemyOut enemyOut;

void enemy_out_set(OBJ2D *obj);

//==============================================================================
//
//		消去アルゴリズム
//
//==============================================================================

// 消去アルゴリズム
class EraseEnemy : public EraseAlg2D
{
	void erase(OBJ2D* obj);
};

//==============================================================================
//
//		EnemyManagerクラス
//
//==============================================================================

// EnemyManagerクラス
class EnemyManager : public OBJ2DManager, public Singleton<EnemyManager>
{
};

#define pEnemyManager		(EnemyManager::getInstance())
#define ENEMY_BEGIN			(pEnemyManager->getList()->begin())
#define ENEMY_END			(pEnemyManager->getList()->end())

//******************************************************************************
#endif // !_ENEMY_H_
