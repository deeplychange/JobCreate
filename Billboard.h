
#ifndef BILLBOARD_H
#define BILLBOARD_H

#include "Game_lib.h"
#include "AStar.h"
#include <cstring>
#include "Player.h"

class Billboard {
public:
	~Billboard();
	int playerNum;
	int stage = 0;
	int memoryPlayerNum;
	bool addPlayFlag;
	void Init();
	void Update(POINT*point, int &money, bool flag);
	void Draw(const DirectX::XMMATRIX& view,
		const DirectX::XMMATRIX& projection,
		const DirectX::XMFLOAT4& light_dir);


};

void Transparent();


#endif// !MAP_3D_H