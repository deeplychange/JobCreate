#ifndef GAME_LIB
#define GAME_LIB

#include <stdio.h>
#include <windows.h>
#include <tchar.h>
#include <sstream>
#include <d3d11.h>
#include "Vector.h"
#include "Blend.h"
#include "sprite.h"
#include "Texture.h"


//typedef unsigned int D3DCOLOR;
// 文字列描画位置
#define	STR_LEFT (0)
#define	STR_CENTER (1)

#define PI (3.1415926f)

enum {
	No_cube = 0,
	No_cylinder,
};

namespace GameLib
{
	

	const int COLORKEY_NONE = 0;		// 透明色なし
	const int COLORKEY_BLACK = 1;		// 「黒」が透明
	const int COLORKEY_LEFTTOP = 2;		// 「画像の左上と同じ色」が透明（授業では未実装

	namespace system
	{
		const int SCREEN_WIDTH = 1280;  // スクリーンの幅
		const int SCREEN_HEIGHT = 720; // スクリーンの高さ
	}

	//Deviceを取得
	ID3D11Device* GetDevice();
	
	//------------------------------------------------------
	void init(LPCTSTR, int width = (1280), int height = (720),
	bool isFullscreen = (false), double frameRate = (60.0));

	//ライブラリのrender設定
	//画面の消すとか
	void Render();

	//  ライブラリの終わり
	void uninit();

	//ゲームループ
	bool gameLoop(bool isShowFrameRate = (false));
	//------------------------------------------------------
	//  画面フリップ
	//------------------------------------------------------
	//  UINT SyncInterval       1を設定
	//  UINT Flags              0を設定
	//------------------------------------------------------
	HRESULT present(UINT SyncInterval = (1), UINT Flags = (0));


	void setBlendMode(Blend::BLEND_MODE);

	void clear(const VECTOR4&);

	void SetFrame(double frameRate);

	//BOOL OnMouseWheel(UINT nFlags, short zDelta);   //
	
}


//Window部分
namespace GameLib {
	namespace window
	{

		//------------------------------------------------------
		//  ウインドウ初期設定（ウインドウを作成して表示する）
		//------------------------------------------------------
		//  LPCTSTR caption     タイトルバーに表示される見出し
		//  int width           ウインドウの幅
		//  int height          ウインドウの高さ
		//------------------------------------------------------
		//  戻り値：HWND         生成されたウインドウのハンドル
		//------------------------------------------------------
		HWND init(LPCTSTR, int, int);

		//------------------------------------------------------
		//  ウインドウ終了処理
		//------------------------------------------------------
		void uninit();

	}

	void OpenDebugWindow();
	void CloseDebugWindow();
	void printf(const char* format, ...);

}
//DirectX11部分
namespace GameLib {
	namespace DirectX11
	{

		//------------------------------------------------------
		//  DirectX11の初期化
		//------------------------------------------------------
		//  HWND hwnd           ウインドウハンドル
		//  int width           ウインドウの幅
		//  int height          ウインドウの高さ
		//  bool isFullscreen   フルスクリーンにするかどうか
		//------------------------------------------------------
		HRESULT init(HWND, int, int, bool);

		//------------------------------------------------------
		//  DirectX11の終了処理
		//------------------------------------------------------
		void uninit();

	}
}

// キー割り当てデータ用構造体
struct PAD_ASSIGN
{
	int		bit;				// pad_status用ビットデータ
	int		code;				// 対応するキー（ボタン）コード
};
//Input
namespace GameLib {
	// 入力情報クラス
	class Pad
	{
	public:
		int					state;					// キー入力情報
		int					trg;					// トリガー入力
		int					repeat;					// リピート入力
		int					old[8];
		float				volumeX, volumeY;		// アナログ入力
		PAD_ASSIGN			*key_assign;			// キーボード割り当てデータ
		PAD_ASSIGN			*joy_assign;			// ジョイスティック割り当てデータ
	};
	//マウスの座標を取得
	void GetMousePos();
	POINT mScreenToClient();
	POINT GetClientMousePos();
	POINT SetClientToScreen(LONG x, LONG y);
	void KeyFlag(bool& keyflg);
	
}

//読み込んだデータの管理
namespace GameLib {
	


	//2Dデータ管理
	class SPRITE_DATA
	{
	public:
		VECTOR2 TexPos;
		VECTOR2 Texsize;
		float Texangle;
		UINTCOLOR Texcolor;
		
		int texNum;
		SPRITE_DATA(int _texNum = -1, VECTOR2 _TexPos=VECTOR2(0,0), VECTOR2 _Texsize = VECTOR2(0, 0), float _angle = 0, UINTCOLOR _Texcolor= 0xFFFFFFFF) : TexPos(_TexPos),texNum(_texNum),Texsize(_Texsize), Texangle(_angle), Texcolor(_Texcolor){};
		void draw2D(VECTOR2& pos, VECTOR2& size, bool xflip);
	};
	//3Dデータ管理
	//3D描画のベースクラス
	class DATA3D {
	public:
		int step;
		bool bLoad;          //普通はfalse posとか何か変わったらtrue;
		bool tex;            //Billboardではない時テクスチャ付けるか
		bool bWireframe;     //false:塗りつぶし描画 true:線描画
		int Id;				 //建てられる配列用
		int Attribute;   //属性
		DirectX::XMFLOAT2 speed;         //移動速度
		float sx, sy, sw, sh;  //テクスチャ座標
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT3 scale;
		DirectX::XMFLOAT3 angle;
		DirectX::XMFLOAT4 materialColor;  //スキンのcolor

		geometric_primitive *obj;
		DATA3D();    //コンストラクタ
		virtual ~DATA3D();   //ディストラクタ
		 

		//ワールド変換
		DirectX::XMMATRIX WorldChange();
		
		//セット3Dモデル
		void SetPrimitive(geometric_primitive& primitive,bool bWireframe=false);
		void SetPrimitive(int type, bool bWireframe = false, wchar_t *_filename=nullptr);
		
		//MATRIX型代入 
		void XM_CALLCONV Draw(
			DirectX::FXMMATRIX view,
			DirectX::FXMMATRIX projection,
			DirectX::FXMVECTOR light_dir,
			bool _billboard = false, float sx = 0, float sy = 0, float sw = 64,float sh=64);



		//そのまま4x4型を代入
		void DATA3D::Draw(
			const DirectX::XMFLOAT4X4 & 
			,      //ワールド.ビュー.プロジェクション合成行列
			const DirectX::XMFLOAT4X4 & world,   //ワールド変更行列
			const DirectX::XMFLOAT4   & material_color,  //材質色 
			const DirectX::XMFLOAT4   &light_direction,  //ライト進行方向
			bool _billboard
			);
	
	};
}

//描画
namespace GameLib {
	//Font描画
	void drawString(int _x = 0, int _y = 0, char *_buf = nullptr, UINTCOLOR _color = 0xFFFFFFFF, int _format = STR_LEFT, int _sizeX = 32, int _sizeY = 32, float _angle = 0.0f);
	//SpriteDataから描画
	void DrawSprite(VECTOR2& pos, VECTOR2& size, SPRITE_DATA *data, bool xflip);
	//直接Sprite描画
	void DrawSpr(VECTOR2& pos, VECTOR2& size, VECTOR2&Texpos, VECTOR2&Texsize, float &Texangle, UINTCOLOR& Texcolor, sprite *data, bool xflip);
	//テキストの読み込み　その１
	void GameloadTexture(LoadTexture _data[], int textureNO);
	//テキストの読み込み　その 2
	void GameloadTextures(LoadTexture _data[]);
}


#endif //






