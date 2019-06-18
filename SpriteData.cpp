#include "SpriteData.h"
#include "Game_lib.h"
using namespace GameLib;



// 2D画像ロードデータ
LoadTexture load_texture[] = {
	{ Player, "./Data/004_cube.png" },
	{ -1, NULL }
};


SPRITE_DATA spr_player = SPRITE_DATA(Player,VECTOR2(0, 0), VECTOR2(1296, 758));