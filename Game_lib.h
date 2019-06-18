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
// ������`��ʒu
#define	STR_LEFT (0)
#define	STR_CENTER (1)

#define PI (3.1415926f)

enum {
	No_cube = 0,
	No_cylinder,
};

namespace GameLib
{
	

	const int COLORKEY_NONE = 0;		// �����F�Ȃ�
	const int COLORKEY_BLACK = 1;		// �u���v������
	const int COLORKEY_LEFTTOP = 2;		// �u�摜�̍���Ɠ����F�v�������i���Ƃł͖�����

	namespace system
	{
		const int SCREEN_WIDTH = 1280;  // �X�N���[���̕�
		const int SCREEN_HEIGHT = 720; // �X�N���[���̍���
	}

	//Device���擾
	ID3D11Device* GetDevice();
	
	//------------------------------------------------------
	void init(LPCTSTR, int width = (1280), int height = (720),
	bool isFullscreen = (false), double frameRate = (60.0));

	//���C�u������render�ݒ�
	//��ʂ̏����Ƃ�
	void Render();

	//  ���C�u�����̏I���
	void uninit();

	//�Q�[�����[�v
	bool gameLoop(bool isShowFrameRate = (false));
	//------------------------------------------------------
	//  ��ʃt���b�v
	//------------------------------------------------------
	//  UINT SyncInterval       1��ݒ�
	//  UINT Flags              0��ݒ�
	//------------------------------------------------------
	HRESULT present(UINT SyncInterval = (1), UINT Flags = (0));


	void setBlendMode(Blend::BLEND_MODE);

	void clear(const VECTOR4&);

	void SetFrame(double frameRate);

	//BOOL OnMouseWheel(UINT nFlags, short zDelta);   //
	
}


//Window����
namespace GameLib {
	namespace window
	{

		//------------------------------------------------------
		//  �E�C���h�E�����ݒ�i�E�C���h�E���쐬���ĕ\������j
		//------------------------------------------------------
		//  LPCTSTR caption     �^�C�g���o�[�ɕ\������錩�o��
		//  int width           �E�C���h�E�̕�
		//  int height          �E�C���h�E�̍���
		//------------------------------------------------------
		//  �߂�l�FHWND         �������ꂽ�E�C���h�E�̃n���h��
		//------------------------------------------------------
		HWND init(LPCTSTR, int, int);

		//------------------------------------------------------
		//  �E�C���h�E�I������
		//------------------------------------------------------
		void uninit();

	}

	void OpenDebugWindow();
	void CloseDebugWindow();
	void printf(const char* format, ...);

}
//DirectX11����
namespace GameLib {
	namespace DirectX11
	{

		//------------------------------------------------------
		//  DirectX11�̏�����
		//------------------------------------------------------
		//  HWND hwnd           �E�C���h�E�n���h��
		//  int width           �E�C���h�E�̕�
		//  int height          �E�C���h�E�̍���
		//  bool isFullscreen   �t���X�N���[���ɂ��邩�ǂ���
		//------------------------------------------------------
		HRESULT init(HWND, int, int, bool);

		//------------------------------------------------------
		//  DirectX11�̏I������
		//------------------------------------------------------
		void uninit();

	}
}

// �L�[���蓖�ăf�[�^�p�\����
struct PAD_ASSIGN
{
	int		bit;				// pad_status�p�r�b�g�f�[�^
	int		code;				// �Ή�����L�[�i�{�^���j�R�[�h
};
//Input
namespace GameLib {
	// ���͏��N���X
	class Pad
	{
	public:
		int					state;					// �L�[���͏��
		int					trg;					// �g���K�[����
		int					repeat;					// ���s�[�g����
		int					old[8];
		float				volumeX, volumeY;		// �A�i���O����
		PAD_ASSIGN			*key_assign;			// �L�[�{�[�h���蓖�ăf�[�^
		PAD_ASSIGN			*joy_assign;			// �W���C�X�e�B�b�N���蓖�ăf�[�^
	};
	//�}�E�X�̍��W���擾
	void GetMousePos();
	POINT mScreenToClient();
	POINT GetClientMousePos();
	POINT SetClientToScreen(LONG x, LONG y);
	void KeyFlag(bool& keyflg);
	
}

//�ǂݍ��񂾃f�[�^�̊Ǘ�
namespace GameLib {
	


	//2D�f�[�^�Ǘ�
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
	//3D�f�[�^�Ǘ�
	//3D�`��̃x�[�X�N���X
	class DATA3D {
	public:
		int step;
		bool bLoad;          //���ʂ�false pos�Ƃ������ς������true;
		bool tex;            //Billboard�ł͂Ȃ����e�N�X�`���t���邩
		bool bWireframe;     //false:�h��Ԃ��`�� true:���`��
		int Id;				 //���Ă���z��p
		int Attribute;   //����
		DirectX::XMFLOAT2 speed;         //�ړ����x
		float sx, sy, sw, sh;  //�e�N�X�`�����W
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT3 scale;
		DirectX::XMFLOAT3 angle;
		DirectX::XMFLOAT4 materialColor;  //�X�L����color

		geometric_primitive *obj;
		DATA3D();    //�R���X�g���N�^
		virtual ~DATA3D();   //�f�B�X�g���N�^
		 

		//���[���h�ϊ�
		DirectX::XMMATRIX WorldChange();
		
		//�Z�b�g3D���f��
		void SetPrimitive(geometric_primitive& primitive,bool bWireframe=false);
		void SetPrimitive(int type, bool bWireframe = false, wchar_t *_filename=nullptr);
		
		//MATRIX�^��� 
		void XM_CALLCONV Draw(
			DirectX::FXMMATRIX view,
			DirectX::FXMMATRIX projection,
			DirectX::FXMVECTOR light_dir,
			bool _billboard = false, float sx = 0, float sy = 0, float sw = 64,float sh=64);



		//���̂܂�4x4�^����
		void DATA3D::Draw(
			const DirectX::XMFLOAT4X4 & 
			,      //���[���h.�r���[.�v���W�F�N�V���������s��
			const DirectX::XMFLOAT4X4 & world,   //���[���h�ύX�s��
			const DirectX::XMFLOAT4   & material_color,  //�ގ��F 
			const DirectX::XMFLOAT4   &light_direction,  //���C�g�i�s����
			bool _billboard
			);
	
	};
}

//�`��
namespace GameLib {
	//Font�`��
	void drawString(int _x = 0, int _y = 0, char *_buf = nullptr, UINTCOLOR _color = 0xFFFFFFFF, int _format = STR_LEFT, int _sizeX = 32, int _sizeY = 32, float _angle = 0.0f);
	//SpriteData����`��
	void DrawSprite(VECTOR2& pos, VECTOR2& size, SPRITE_DATA *data, bool xflip);
	//����Sprite�`��
	void DrawSpr(VECTOR2& pos, VECTOR2& size, VECTOR2&Texpos, VECTOR2&Texsize, float &Texangle, UINTCOLOR& Texcolor, sprite *data, bool xflip);
	//�e�L�X�g�̓ǂݍ��݁@���̂P
	void GameloadTexture(LoadTexture _data[], int textureNO);
	//�e�L�X�g�̓ǂݍ��݁@���� 2
	void GameloadTextures(LoadTexture _data[]);
}


#endif //






