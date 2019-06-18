

#ifndef OBJ2D_H_
#define	OBJ2D_H_
#include <list>
#include "Game_lib.h"
#include "NumberName.h"
using namespace std;
//******************************************************************************
//
//
//		OBJ2Dクラス
//
//
//******************************************************************************

enum {
	frame=0,
	character,
	build,
};

// 前方宣言
class OBJ2D;
// 移動アルゴリズムクラス（抽象クラス）
class MoveAlg2D
{
public:
	virtual void move(OBJ2D* obj, bool leftFlag,int month,int day,int money) = 0;  // 純粋仮想関数を持つので、MoveAlg型のオブジェクトは宣言できない（派生させて使う）
};

// 消去アルゴリズムクラス（抽象クラス）
class EraseAlg2D
{
public:
	virtual void erase(OBJ2D* obj) = 0; // 上と同様
};


class OBJ2D {
public:
	GameLib::SPRITE_DATA*	data;			// 画像データ
	
	sprite** sprs;
	sprite* current_spr;
	int		max;

	VECTOR2		pos;			// 座標
	VECTOR2		speed;			// 速度
	VECTOR2		size;			// 大きさ
	UINTCOLOR texColor;         //テクスチャ色
	VECTOR2 texPos;              //テクスチャ座標
	VECTOR2 texSize;            //テクスチャ大きさ
	float Texangle;
	bool canDraw;              //描画できるかとかフラグ
	int  type;                 //能力描画用判定  　キャラクターなら文字を描画する

	//描画用能力
	int learning;
	int spirit;
	int communication;
	int earnMoney;
	bool earnMoneyDarwFlag;

	MoveAlg2D*		mvAlg;			// 移動アルゴリズム
	EraseAlg2D*		eraseAlg;		// 消去アルゴリズム
	int				id;
	VECTOR2		    delta;			// 移動量の差分
	int				command;		// 他のオブジェクトからのコマンド
	bool			xflip;			// 画像左右反転(実装していない)
	OBJ2D();						// コンストラクタ
	~OBJ2D();
	void SetSprite(wchar_t *name, int no = 0, int max = 1);
	void ChangeSprite(int no);
	void clear();					// メンバ変数のクリア
	void move(bool leftFlag,int month,int day,int money);		
																	// 移動
	void draw();					// 描画
};

// OBJ2DManagerクラス
class OBJ2DManager
{
protected:
	list<OBJ2D>				objList;			// リスト
public:
	void init();								// 初期化（すべて消去）
	OBJ2D* add(MoveAlg2D* mvAlg, VECTOR2 pos = VECTOR2(0, 0), int id = 0);		// リストへ追加
	void update(bool leftFlag,int month,int day,int money);								// 更新
	void draw();							// 描画
	list<OBJ2D>* getList() { return &objList; }
};

//******************************************************************************
#endif // !_OBJ2D_H_