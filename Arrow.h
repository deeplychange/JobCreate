#ifndef ARROW_H
#define ARROW_H
#include  "Game_lib.h"
#include  "Billboard.h"
#include  "3DMap.h"
enum {
	AttrUp,  //属性アップ
	AttrDown,//属性ダウン
	AttrUpAni,//属性アップのアニメ
	AttrDownAni,//属性ダウンのアニメ
	StandBy,   //待機
	Delete,
};

class ArrowAnime{
public:
	int time;
	int stage;
	GameLib::DATA3D* obj;
	ArrowAnime();
	~ArrowAnime();

	void Clear();
	void Init(wchar_t *_filename, float posX, float posY, float posZ, float _sx, float _sy, float _sw, float _sh);
	void Update(MapInformation* Map);
	void Draw(const DirectX::XMMATRIX& view,
		const DirectX::XMMATRIX& projection,
		const DirectX::XMFLOAT4& light_dir);
	void AniDown(MapInformation* Map);
	void AniUp(MapInformation* Map);
};



#endif // !ARROW_H
