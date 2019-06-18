#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib,"winmm.lib")
#include <time.h>
#include <Keyboard.h>
#include "high_resolution_timer.h"
#include "Game_lib.h"
#include "Blend.h"
#include "geometric_primitive.h"
#include "skinned_mesh.h"
#include "delete.h"
#include "font.h"
#include "Texture.h"
#include "NumberName.h"

//二進数色
enum {
	Red = 0xFF0000,
	WHITLE = 0xFFFFFF
};
namespace GameLib
{
	// 内部変数用構造体＆実体宣言
	struct Members
	{
		HWND hwnd;  // ウインドウハンドル
		POINT point;   //マウス座標
					   // DirectX関連
		skinned_mesh* fbx;
		ID3D11Device*           Device;
		ID3D11DeviceContext*    Context;
		IDXGISwapChain*         SwapChain;
		ID3D11RenderTargetView* RenderTargetView;
		ID3D11DepthStencilView* DepthStencilView;
		Blend*       blendState;
		ID3D11Texture2D*        DepthStencil = NULL;
		D3D_FEATURE_LEVEL       Level = D3D_FEATURE_LEVEL_11_0;
		//TEXTURE付いていない
		sprite* sprite;
		geometric_primitive* primitive;
		geometric_cube* cube;
		geometric_cylinder* cylinder;
	};

	static Members m;
	static HighResolutionTimer	hrTimer;


	ID3D11Device* GetDevice() {
		return m.Device;

	}
	//--------------------------------
	//  FPS計算
	//--------------------------------
	//フレームレート計算・タイトルバーに表示
	void calculateFrameStats(HWND hWnd, HighResolutionTimer *timer)
	{
		// Code computes the average frames per second, and also the 
		// average time it takes to render one frame.  These stats 
		// are appended to the window caption bar.
		static int frames = 0;
		static float timeElapsed = 0.0f;

		frames++;

		// Compute averages over one second period.
		if ((timer->timeStamp() - timeElapsed) >= 1.0f)
		{
			float fps = static_cast<float>(frames); // fps = frameCnt / 1
			float mspf = 1000.0f / fps;
			::std::ostringstream outs;
			outs.precision(6);
			outs << "FPS : " << fps << " / " << "Frame Time : " << mspf << " (ms)";
			SetWindowTextA(hWnd, outs.str().c_str());

			// Reset for next average.
			frames = 0;
			timeElapsed += 1.0f;
		}
	}

	/*BOOL OnMouseWheel(UINT nFlags, short zDelta)
	{
	WPARAM upDown;
	if (zDelta < 0) {
	upDown = SB_LINEDOWN;
	}
	else {
	upDown = SB_LINEUP;
	}

	SendMessage(m.hwnd,WM_VSCROLL, upDown, 0);
	return FALSE;
	}*/

	// ライブラリの初期化
	void init(LPCTSTR caption, int width, int height, bool isFullscreen, double frameRate)
	{
		// 乱数系列の設定
		srand((unsigned int)time(NULL));

#ifdef _DEBUG
		// フルスクリーンの設定（念のためデバッグモードでは不可）
		isFullscreen = false;
#endif
		// ウインドウの初期設定
		m.hwnd = window::init(caption, width, height);

		// DirectX11の初期化
		DirectX11::init(m.hwnd, width, height, isFullscreen);

		// フレームレートの設定
		hrTimer.setFrameRate(frameRate);
	}

	void SetFrame(double frameRate) {
		hrTimer.setFrameRate(frameRate);
	}


	//  ゲームループ用
	bool gameLoop(bool isShowFrameRate)
	{
		MSG msg;
		while (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) return false;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		//フレームレート用
		while (!hrTimer.tick());
		if (isShowFrameRate)
		{
			//フレームレート計算・タイトルバーに表示
			calculateFrameStats(m.hwnd, &hrTimer);
		}

		return true;
	}

	//  ライブラリの終わり
	void uninit() {
		// DirectX11の終了処理
		DirectX11::uninit();

		// ウインドウの終了処理
		window::uninit();
	}


	//	Blendの設定
	void setBlendMode(Blend::BLEND_MODE blend)
	{
		m.blendState->Set(m.Context, blend);
	}
}


//Window部分
namespace GameLib
{
	//宣言
	LRESULT CALLBACK fnWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static const LPCWSTR CLASS_NAME = L"EndWork";

	LRESULT CALLBACK fnWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		using DirectX::Keyboard;

		switch (msg)
		{
		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc;
			hdc = BeginPaint(hwnd, &ps);
			EndPaint(hwnd, &ps);
			break;
		}
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		case WM_ACTIVATEAPP:
			Keyboard::ProcessMessage(msg, wParam, lParam);
			break;
		case WM_KEYDOWN:
			if (wParam == VK_ESCAPE)
			{
				PostMessage(hwnd, WM_CLOSE, 0, 0);
				break;
			}
		case WM_MOUSEWHEEL: {      //マウスの中央パタン

		}
		case WM_SYSKEYDOWN:
		case WM_KEYUP:
		case WM_SYSKEYUP:
			Keyboard::ProcessMessage(msg, wParam, lParam);
			break;
		case WM_ENTERSIZEMOVE:
			// WM_EXITSIZEMOVE is sent when the user grabs the resize bars.
			hrTimer.stop();
			break;
		case WM_EXITSIZEMOVE:
			// WM_EXITSIZEMOVE is sent when the user releases the resize bars.
			// Here we reset everything based on the new window dimensions.
			hrTimer.start();
			break;
		default:
			return DefWindowProc(hwnd, msg, wParam, lParam);
		}
		return 0;
	}

	//初期化
	HWND window::init(LPCTSTR caption, int width, int height)
	{
#if defined(DEBUG) | defined(_DEBUG)
		//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

		WNDCLASSEX wcex;
		wcex.cbSize = sizeof(WNDCLASSEX);  //構造体のサイズを sizeof 演算子で自動的に計算してセットします。
										   //CS_DBLCLKS：ダブルクリックが発生したことをウインドウに通知する
										   //CS_HREDRAW：横サイズが変わったとき，ウインドウ全体を再描画する
										   //CS_VREDRAW：縦サイズが変わったとき，ウインドウ全体を再描画する
										   //まだいろいろある
		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = fnWndProc;              //ウインドウのプロシージャ関数名である (プログラマが決めた関数名)
		wcex.cbClsExtra = 0;			           //必要ないので 0 をセットします。
		wcex.cbWndExtra = 0;					   //必要ないので 0 をセットします。
		wcex.hInstance = GetModuleHandle(NULL);    //ウインドウのプロシージャ関数名である (プログラマが決めた関数名)
		wcex.hIcon = NULL;                         //標準の大きいアイコン・ハンドルをセットします
		wcex.hCursor = LoadCursor(NULL, IDC_ARROW);//標準の大きいアイコン・ハンドルをセットします
		wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);////GetStockObject( WHITE_BRUSH )
														//wcex.hbrBackground = (HBRUSH)(CreateSolidBrush(RGB(1,1,1)));
		wcex.lpszMenuName = NULL;					//メニューバーの識別文字列を指定しますが使わないので NULL ポインタをセットします。
		wcex.lpszClassName = CLASS_NAME;			 //クラスの文字列をセットします
		wcex.hIconSm = 0;
		//ウインドウ・クラスを登録
		RegisterClassEx(&wcex);
		//ウインドウのサイズ
		RECT rc = { 0, 0, width, height };
		AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
		//ウインドウの作成
		HWND hwnd = CreateWindow(CLASS_NAME, caption, WS_OVERLAPPEDWINDOW ^ WS_MAXIMIZEBOX ^ WS_THICKFRAME | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, GetModuleHandle(NULL), NULL);
		ShowWindow(hwnd, SW_SHOWDEFAULT);

		return hwnd;
	}

	//終了
	void window::uninit()
	{
		// 終了処理
		UnregisterClass(CLASS_NAME, GetModuleHandle(NULL));
	}

	void GameLib::Render() {
		//Setup the viewport
		D3D11_VIEWPORT vp;
		vp.Width = (FLOAT)system::SCREEN_WIDTH;
		vp.Height = (FLOAT)system::SCREEN_HEIGHT;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;
		m.Context->RSSetViewports(1, &vp);

		//Just clear the backbuffer
		float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f }; //red,green,blue,alpha
		m.Context->ClearRenderTargetView(m.RenderTargetView, ClearColor);

		//Clear the depth buffer to 1.0(max depth)
		m.Context->ClearDepthStencilView(m.DepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

		m.Context->OMSetRenderTargets(1, &m.RenderTargetView, m.DepthStencilView);
	}

	//  画面クリア
	void GameLib::clear(const VECTOR4& color)
	{
		m.Context->ClearRenderTargetView(m.RenderTargetView, (const float *)&color);
		m.Context->ClearDepthStencilView(m.DepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);
	}

	//  画面フリップ
	HRESULT present(UINT SyncInterval, UINT Flags)
	{
		return m.SwapChain->Present(SyncInterval, Flags);
	}
}


//DirectX11部分
namespace GameLib
{

	//DirectX11初期化
	HRESULT DirectX11::init(HWND hwnd, int width, int height, bool isFullscreen)
	{
		HRESULT hr = S_OK;
		RECT rc;
		UINT createDeviceFlags = 0;
		GetClientRect(hwnd, &rc);
		//UINT width = rc.right - rc.left;
		//UINT height = rc.bottom - rc.top;
		// マウス座標の取得
		GetMousePos();

		//Unit08

#ifdef _DEBUG
		//debug check
		//createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif


		D3D_DRIVER_TYPE         g_driverType = D3D_DRIVER_TYPE_NULL;


		D3D_DRIVER_TYPE driverTypes[] =
		{
			D3D_DRIVER_TYPE_HARDWARE,
			D3D_DRIVER_TYPE_WARP,
			D3D_DRIVER_TYPE_REFERENCE,
		};
		UINT numDriverTypes = ARRAYSIZE(driverTypes);


		D3D_FEATURE_LEVEL featureLevels[] =
		{
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0,
		};
		UINT numFeatureLevels = ARRAYSIZE(featureLevels);

		DXGI_SWAP_CHAIN_DESC sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.BufferCount = 1;
		sd.BufferDesc.Width = width;
		sd.BufferDesc.Height = height;
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.BufferDesc.RefreshRate.Numerator = 60;
		sd.BufferDesc.RefreshRate.Denominator = 1;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.OutputWindow = hwnd;
		/*UINT mass_quality_level;
		Device->CheckMultisampleQualityLevels(sd_BufferDesc.Format,4,&mass_quality_level);*/
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
		sd.Windowed = TRUE;
		sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		sd.Flags = 0;



		for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
		{
			D3D_DRIVER_TYPE driverType = driverTypes[driverTypeIndex];
			D3D_FEATURE_LEVEL featureLevel;
			hr = D3D11CreateDeviceAndSwapChain(NULL, driverType, NULL, createDeviceFlags, featureLevels, numFeatureLevels,
				D3D11_SDK_VERSION, &sd, &m.SwapChain, &m.Device, &featureLevel, &m.Context);
			if (SUCCEEDED(hr))   break;
		}
		//	_ASSERT_EXPR_A(SUCCEEDED(hr), hr_trace(hr));
		if (FAILED(hr))  return false;

		//Create a render target view
		ID3D11Texture2D* pBackBuffer = NULL;
		hr = m.SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
		if (FAILED(hr)) return false;

		hr = m.Device->CreateRenderTargetView(pBackBuffer, NULL, &m.RenderTargetView);
		pBackBuffer->Release();
		if (FAILED(hr))  return false;

		// Tutorial 05

		//Create depth stencil texture 
		ID3D11Texture2D*   DepthStencil;
		D3D11_TEXTURE2D_DESC descDepth;
		ZeroMemory(&descDepth, sizeof(descDepth));
		descDepth.Width = width;
		descDepth.Height = height;
		descDepth.MipLevels = 1;
		descDepth.ArraySize = 1;
		descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		descDepth.SampleDesc.Count = 1;
		descDepth.SampleDesc.Quality = 0;
		descDepth.Usage = D3D11_USAGE_DEFAULT;
		descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		descDepth.CPUAccessFlags = 0;
		descDepth.MiscFlags = 0;
		hr = m.Device->CreateTexture2D(&descDepth, NULL, &DepthStencil);
		if (FAILED(hr))return false;

		//Create the depth stencil view
		D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
		ZeroMemory(&descDSV, sizeof(descDSV));
		descDSV.Format = descDepth.Format;
		descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		descDSV.Texture2D.MipSlice = 0;
		hr = m.Device->CreateDepthStencilView(DepthStencil, &descDSV, &m.DepthStencilView);
		DepthStencil->Release();

		//blendについて
		m.blendState->Initialize(m.Device);

		m.primitive = new geometric_primitive(m.Device);
		m.cube = new geometric_cube(m.Device);
		m.cylinder = new geometric_cylinder(m.Device);
		Font::initSprString(m.Device);
		// InputManagerの初期化
		//InputManager::getInstance()->init();

		// DXTKAudio
		//audio::DXTKAudio::getInstance();   // インスタンスを生成しておく

		return hr;
	}

	//--------------------------------
	//  DirectX11の終了処理
	//--------------------------------
#define DELETE_PTR(x) if(x){delete x; x=nullptr;}
#define RELEASE_PTR(x) if(x){x->Release();x = nullptr; }
	void DirectX11::uninit()
	{
		if (m.SwapChain)m.SwapChain->SetFullscreenState(false, nullptr);
		DELETE_PTR(m.primitive);
		DELETE_PTR(m.cube);
		DELETE_PTR(m.cylinder);
		if (m.blendState)m.blendState->Release();
		RELEASE_PTR(m.blendState);
		RELEASE_PTR(m.DepthStencilView);
		RELEASE_PTR(m.RenderTargetView);
		RELEASE_PTR(m.SwapChain);
		RELEASE_PTR(m.Context);
		RELEASE_PTR(m.Device)

	}

	//--------------------------------------------------------------------------
}

//Input
namespace GameLib {

	// 入力情報の更新
	//mouse座標の取得
	void GetMousePos() {
		GetCursorPos(&m.point);
	}

	POINT SetClientToScreen(LONG x, LONG y) {
		POINT point;
		point.x = x;
		point.y = y;
		ClientToScreen(m.hwnd, &point);
		return point;
	}

	POINT SetClientToScreen(POINT point) {
		ClientToScreen(m.hwnd, &point);
		return point;
	}

	POINT mScreenToClient() {
		GetCursorPos(&m.point);
		ScreenToClient(m.hwnd, &m.point);
		return m.point;
	};


	POINT GetClientMousePos() { return m.point; }

	void KeyFlag(bool& keyflg) {
		if (GetAsyncKeyState(VK_LBUTTON) & 0x1) {
			if (!keyflg) {
				//キーを押した瞬間の処理
				keyflg = true;
			}
			//キーを押している間の処理
			else {
				//キーを離している間の処理
				keyflg = false;
			}
		}
	}
}


//font
namespace GameLib {

	typedef UINT UINTCOLOR;
	void drawString(int _x, int _y, char *_buf, UINTCOLOR color, int _format, int _sizeX, int _sizeY, float _angle)
	{
		Font::drawSprString(m.Context, _x, _y, _buf, _format, _sizeX, _sizeY, _angle, color);
	}
}

//OBJ2D
namespace GameLib {

	//LOAD_TEXTURE *texture[TEXTURE_MAX] = { nullptr };

	void GameloadTexture(LoadTexture _data[], int textureNO) {
		if (_data[textureNO].texNum != -1 || _data[textureNO].fileName != NULL)
		{
			//charからwchar_tへの変換
			wchar_t wfilename[256];
			size_t wsize;
			mbstowcs_s(&wsize, wfilename, _data[textureNO].fileName, 256);
			_data[textureNO].img = new sprite(m.Device, wfilename);
			texture[textureNO] = &_data[textureNO];
		}
	}
	void GameloadTextures(LoadTexture _data[]) {
		for (int i = _data[0].texNum; _data[i].texNum != -1 || _data[i].fileName != NULL; i++)
		{
			//charからwchar_tへの変換
			wchar_t wfilename[256];
			size_t wsize;
			mbstowcs_s(&wsize, wfilename, _data[i].fileName, 256);
			_data[i].img = new sprite(m.Device, wfilename);
			texture[i] = &_data[i];
		}
	}



	void SPRITE_DATA::draw2D(VECTOR2& pos, VECTOR2& size, bool xflip = true) {
		//正読み込み
		if (xflip) {
			texture[texNum]->img->render(m.Context, pos.x, pos.y, size.x, size.y, TexPos.x, TexPos.y, Texsize.x, Texsize.y, Texangle, Texcolor);
		}
	}

	//Loadtexture描画
	void DrawSprite(VECTOR2& pos, VECTOR2& size, SPRITE_DATA *data, bool xflip) {
		if (data) data->draw2D(pos, size, xflip);
	}
	//直接Spriteから描画
	void DrawSpr(VECTOR2& pos, VECTOR2& size, VECTOR2&Texpos, VECTOR2&Texsize, float &Texangle, UINTCOLOR& Texcolor, sprite *data, bool xflip) {
		if (data)data->render(m.Context, pos.x, pos.y, size.x, size.y, Texpos.x, Texpos.y, Texsize.x, Texsize.y, Texangle, Texcolor);
	}

}

//DATA_3D
namespace GameLib {
	//コンストラクタ
	DATA3D::DATA3D() {
		step = 0;
		pos = DirectX::XMFLOAT3(0, 0, 0);
		scale = DirectX::XMFLOAT3(1, 1, 1);
		angle = DirectX::XMFLOAT3(0, 0, 0);
		materialColor = DirectX::XMFLOAT4(1, 1, 1, 1);
		bLoad = false;
		bWireframe = false;
		speed = DirectX::XMFLOAT2(0.0f, 0.0f);
		tex = false;
		Attribute = NONE;
	}

	enum
	{
		CUBE = 0,
		CYLINDER,
		POLYGON,
	};

	//セット3Dモデル 
	//primitiveのnew,テクスチャ貼り付け
	void DATA3D::SetPrimitive(geometric_primitive& primitive, bool _bWireframe)
	{
		if (primitive.type == CUBE) {
			//こピーコンストラクタ 型変換
			if (obj==nullptr) {
				obj = new geometric_cube(*((geometric_cube*)&primitive));
			}
		}
		else if (primitive.type == CYLINDER) {
			if (obj == nullptr) {
				obj = new geometric_cylinder(*(geometric_cylinder*)&primitive);
			}
		}
		else if (primitive.type == POLYGON) {
			if (obj == nullptr) {
				obj = new geometric_polygon(*(geometric_polygon*)&primitive);
			}
		}
		else
		{
			return;
		}
		bLoad = true;
		bWireframe = _bWireframe;
	}

	void DATA3D::SetPrimitive(int type, bool _bWireframe, wchar_t *_filename) {
		
		if (type == playerDraw) {
			if (obj == nullptr) {
				obj = new geometric_polygon(m.Device,FRONT, _filename);
			}
		}
		else if (type == roadDraw) {
			if (obj == nullptr) {
				obj = new geometric_upside(m.Device);
			}
		}
		else if (type == wallDraw) {
			if (obj == nullptr) {
				obj = new geometric_cube(m.Device);
			}
		}
		else if (type == buildingDraw) {
			if (obj == nullptr) {
				obj = new geometric_polygon(m.Device, UP, _filename);
			}
		}
		bLoad = true;
		bWireframe = _bWireframe;
	}

	//削除
	//#define RELEASE_IF(x) if(x){x->Release();x=NULL;}
	DATA3D::~DATA3D()
	{
		if (obj)
		{
			delete	obj;
			obj = nullptr;
		}
		
		bLoad = false;
	}

	DirectX::XMMATRIX DATA3D::WorldChange()
	{
		//ワールド変換
		DirectX::XMMATRIX S, Rx, Ry, Rz, R, T, W;
		S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
		DirectX::XMFLOAT3 _angle;
		_angle.x = angle.x * (PI / 180);
		_angle.y = angle.y * (PI / 180);
		_angle.z = angle.z * (PI / 180);
		Rx = DirectX::XMMatrixRotationX(_angle.x);
		Ry = DirectX::XMMatrixRotationY(_angle.y);
		Rz = DirectX::XMMatrixRotationZ(_angle.z);
		R = Rz*Rx*Ry;
		T = DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z);
		W = S*R*T;
		return W;
	}

	//描画
	void XM_CALLCONV  DATA3D::Draw(
		DirectX::FXMMATRIX view,
		DirectX::FXMMATRIX projection,
		DirectX::FXMVECTOR light_dir,
		bool _billboard, float sx, float sy, float sw, float sh
		)
	{
		//	モデルがなければ描画しない
		if (!obj)return;
		//	ワールド変換行列取得
		DirectX::XMMATRIX world = WorldChange();


		//逆行列を取得
		if (_billboard) {
			DirectX::XMFLOAT4X4 workView, work;
			DirectX::XMStoreFloat4x4(&workView, view);
			work._11 = workView._11*scale.x;	work._12 = workView._21*scale.x;	work._13 = workView._31*scale.x;	work._14 = 0.0f;
			work._21 = workView._12*scale.y;	work._22 = workView._22*scale.y;	work._23 = workView._32*scale.y;	work._24 = 0.0f;
			work._31 = workView._13*scale.z;	work._32 = workView._23*scale.z;	work._33 = workView._33*scale.z;	work._34 = 0.0f;
			work._41 = pos.x;			work._42 = pos.y;			work._43 = pos.z;			work._44 = 1.0f;
			world = DirectX::XMLoadFloat4x4(&work);
		}

		// 次にオフセットさせます。
		//	//FLoat4X4型に変換する
		DirectX::XMFLOAT4X4 w, wvp;
		DirectX::XMStoreFloat4x4(&wvp, (world*view*projection));
		DirectX::XMStoreFloat4x4(&w, world);
		DirectX::XMFLOAT4 lightColor;
		XMStoreFloat4(&lightColor, light_dir);
		//テクスチャ貼ってない描画
		if (tex)         //billboard座標変更しないしテクスチャないし
		{
			obj->renderTexture(m.Context, wvp, w, materialColor, lightColor, bWireframe, sx, sy, sw, sh);
		}
		//BillBoard描画あるいは３Dにテクスチャを貼り付ける
		else if(_billboard)
		{
			obj->renderBillboard(m.Context, wvp, w, materialColor, lightColor, bWireframe, sx, sy, sw, sh);
		}
		else obj->render(m.Context, wvp, w, materialColor, lightColor, bWireframe);

	}


	//そのまま4x4型を代入
	//使っていない
	void DATA3D::Draw(const DirectX::XMFLOAT4X4 & world_view_projection,      //ワールド.ビュー.プロジェクション合成行列
		const DirectX::XMFLOAT4X4 & world,   //ワールド変更行列
		const DirectX::XMFLOAT4   & material_color,  //材質色 
		const DirectX::XMFLOAT4   &light_direction,  //ライト進行方向
		bool _billboard)
	{
		//	モデルが無ければ描画処理を一切行わない
		if (!obj)		return;

		if (!_billboard)obj->render(m.Context, world_view_projection, world, material_color, light_direction, bWireframe);
	}
}



//Debug
namespace GameLib {

	static FILE* DebugFP = NULL;
	void OpenDebugWindow()
	{
#ifdef _DEBUG
		AllocConsole();
		freopen_s(&DebugFP, "CON", "w", stdout);
#endif
	}

	void CloseDebugWindow()
	{
#ifdef _DEBUG
		if (DebugFP)
		{
			fclose(DebugFP);
			FreeConsole();
		}
#endif
	}
	void printf(const char* format, ...)
	{
#ifdef _DEBUG
		va_list arg;
		va_start(arg, format);

		vprintf(format, arg);

		va_end(arg);
#endif
	}

}