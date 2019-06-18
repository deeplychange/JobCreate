#ifndef TETURE_H_
#define TETURE_H_

#include "sprite.h"
#include "geometric_primitive.h"
#define TEXTURE_MAX (128)


	struct LoadTexture
	{
		int		texNum;			// �e�N�X�`���ԍ�
		char	*fileName;		// �t�@�C����
		sprite* img;
		//�R���X�g���N�^
		LoadTexture(int a_texNum, char *_FileName) :texNum(a_texNum), fileName(_FileName) {};
	};



	class TextureManager
	{

	public:
		TextureManager() {};
		~TextureManager() {
			releaseTexture();
		};

		void loadTextures(LoadTexture _data[]);
		void loadTexture(LoadTexture _data[], int a_textureNO);
		void releaseTexture();

		static TextureManager* getInstance() {
			static TextureManager instance;
			return &instance;
		};

	};

#define pTextureManager (TextureManager::getInstance())


extern LoadTexture *texture[TEXTURE_MAX];

#endif //