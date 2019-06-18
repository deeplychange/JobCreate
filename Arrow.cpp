#include "Arrow.h"
#include "Game_lib.h"
#include "3DMap.h"
using namespace DirectX;
using namespace GameLib;

ArrowAnime::ArrowAnime() {
	time = 0;
	stage = 0;
	obj = new DATA3D;
}


ArrowAnime::~ArrowAnime() {
	delete obj;
	obj = nullptr;
}

void ArrowAnime::Clear() {
	time = 0;
	stage = 0;
	obj->obj = nullptr;
}

void ArrowAnime::Init(wchar_t *_filename, float posX, float posY, float posZ, float _sx, float _sy, float _sw, float _sh){
	obj->sx = _sx;obj->sy = _sy;obj->sw = _sw;obj->sh = _sh;
	obj->SetPrimitive(playerDraw,false,_filename);
	obj->pos = ChangeToWorldPos(XMFLOAT3(posX, posY, posZ));
	obj->tex = false;
	obj->scale = XMFLOAT3(1.5f, 1.5f, 1.5f);
	obj->speed = XMFLOAT2(0.0f, 0.0f);
}

void ArrowAnime::Update(MapInformation* Map) {
	switch (stage) {
	case 0:
		switch (Map->stage) {
		case AttrUpAni:
			obj->pos.y += 0.01f;
			time++;
			if (time >= 1*60)stage = StandBy;
			break;
		case AttrDownAni:
			obj->pos.y -= 0.01f;
			time++;
			if (time>=1*60)stage = Delete;
			break;
		}
		break;
	case StandBy:
		Map->stage = StandBy;
		Clear();
		break;
	case Delete:
		Map->AttrUpFlag = false;
		Map->stage = 0;
		Clear();
		break;
	}
	
}

void ArrowAnime::AniDown(MapInformation* Map) {
	switch (stage) {
	case 0:
		obj->pos.y -= 0.01f;
		time++;
		if (time >= 1 * 60)stage = Delete;
		break;
	case Delete:
		Map->AttrUpFlag = false;
		Map->stage = 0;
		Clear();
		break;
	}
}
void ArrowAnime::AniUp(MapInformation* Map) {
	switch (stage) {
	case 0:
		obj->pos.y += 0.01f;
		time++;
		if (time >= 1 * 60)stage = StandBy;
		break;
	case StandBy:
		Map->stage = StandBy;
		Clear();
		break;
	}
}
void ArrowAnime::Draw(const DirectX::XMMATRIX& view,
	const DirectX::XMMATRIX& projection,
	const DirectX::XMFLOAT4& light_dir) {
	
	obj->Draw(view, projection, XMLoadFloat4(&light_dir), true, obj->sx,obj->sy, obj->sw, obj->sh);

;}

