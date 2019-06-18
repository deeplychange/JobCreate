#include <iostream>
#include "sprite.h"
#include <string.h> 
#include <string> 
using namespace std;
namespace Font {
	// •¶Žš—ñ•`‰æˆÊ’u
#define	STR_LEFT (0)
#define	STR_CENTER (1)
	typedef UINT UINTCOLOR;
	class SpriteString
	{
	private:
		SpriteString() {};
		~SpriteString() {};
		static sprite *s_pSprString;

	public:
		friend sprite* initSprString(ID3D11Device* a_pDevice, const wchar_t*a_pFilename = nullptr);
		friend void drawSprString(ID3D11DeviceContext* a_pDeviceConetxt, int a_drawX = 0, int a_drawY = 0, char *a_pIn = nullptr, int a_format = STR_LEFT, int a_sizeX = 32, int a_sizeY = 32, float a_rotateAngle = .0f, UINTCOLOR a_blendColor = 0xFFFFFFFF, sprite *a_pSprStringFont = nullptr);

		friend void releaseSprString();
	};

}
