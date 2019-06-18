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
		static ID3D11BlendState* BlendState[MODE_MAX];   //�u�����h�ݒ�z��
		static bool  bLoad;                              //true:�ݒ�z��쐬�ς�
		static BLEND_MODE  enumMode;                     //���ݎg�p���Ă���u�����h���[�h
		Blend() {};
		~Blend() { Release();  }
	public:
		static bool Initialize(ID3D11Device* Device);     //�u�����h�ݒ�z�񏉊���


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

