#pragma once
#include <d3d11.h>
#include "string"

//#define MODE_MAX 8;

namespace GameLib {
	class Blend {
	public:
		enum BLEND_MODE
		{
			NONE = 0,
			ALPHA,
			ADD,
			SUB,
			REPLACE,
			MULTIPLY,
			LIGHTEN,
			DARKEN,
			SCREEN,
			MODE_MAX
		};


	private:
		static ID3D11BlendState* BlendState[MODE_MAX];   //ブレンド設定配列
		static bool  bLoad;                              //true:設定配列作成済み
		static BLEND_MODE  enumMode;                     //現在使用しているブレンドモード
		Blend() {};
		~Blend() { Release();  }
	public:
		static bool Initialize(ID3D11Device* Device);     //ブレンド設定配列初期化


		static void Release()
		{
			for (BLEND_MODE mode = NONE; mode < MODE_MAX; mode = (BLEND_MODE)(mode + 1))
			{
				BlendState[mode]->Release();
			}
		}

		static void Set(ID3D11DeviceContext* Context, BLEND_MODE mode)
		{
			if (!bLoad)                          return;
			if (mode < 0 || mode >= MODE_MAX)    return;
			if (mode == enumMode)                return;

			Context->OMSetBlendState(BlendState[mode], nullptr, 0xffffffff);
			enumMode = mode;
		}

	};
}

