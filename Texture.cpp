#include "Texture.h"
#include "Game_lib.h"

     LoadTexture *texture[TEXTURE_MAX] = { nullptr };



	 //2Dテクスチャを読み込み
	void TextureManager::loadTexture(LoadTexture _data[], int a_textureNO)
	{
		GameLib::GameloadTexture(_data,a_textureNO);
	}

	void TextureManager::loadTextures(LoadTexture _data[])
	{
		GameLib::GameloadTextures(_data);
	}


	void TextureManager::releaseTexture()
	{
		int i;
		for (i = 0; i < TEXTURE_MAX; i++)
		{
			if (texture[i])
			{
				delete texture[i]->img;
				ZeroMemory(texture[i], sizeof(*texture[i]));

			}
		}
		ZeroMemory(texture, sizeof(LoadTexture)*i);
	}

