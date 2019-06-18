//
//
//#include <windows.h>
//#include <tchar.h>
//#include <sstream>
//#include <d3d11.h>
//#include "misc.h"
//#include "high_resolution_timer.h"
//#include "sprite.h"
//#include "ResourceManger.h"
//#include"Blend.h"
//#include "geometric_primitive.h"
//#include "skinned_mesh.h"
//
//class framework
//{
//public:
//
//	////UNIT8
//	//typedef union LARGE_INTEGER
//	//{
//	//	struct
//	//	{
//	//		DWORD LowPart;
//	//		LONG HighPart;
//	//	};
//	//	LONGLONG QuadPart;
//	//}LARGE_INTEGER;
//
//
//	float a = 1.0;
//	static int o;
//	
//	//HWND                   hWnd = NULL;
////	sprite *p;
//	sprite *sprites[1024];
//	sprite* particle;
//	geometric_primitive* cube;
//	skinned_mesh* fbx;
//
//	ID3D11Device*           Device;
//	ID3D11DeviceContext*    Context;
//	IDXGISwapChain*         SwapChain;
//	ID3D11RenderTargetView* RenderTargetView = NULL;
//	ID3D11DepthStencilView* DepthStencilView = NULL;
//	ID3D11Texture2D*        DepthStencil = NULL;
//	D3D_FEATURE_LEVEL       Level = D3D_FEATURE_LEVEL_11_0;
//	ID3D11BlendState* BlendState;
//
//	CONST HWND hwnd;
//	static CONST LONG SCREEN_WIDTH = 1280;
//	static CONST LONG SCREEN_HEIGHT = 720;
//
//	framework(HWND hwnd) : hwnd(hwnd)
//	{
//
//	}
//#define DELETE_IF(x) if(x){delete x;x=NULL;}
//
//#define RELEASE_IF(x) if(x){x->Release();}
//
//	~framework()
//	{
//		for (int i = 0; i < 1024; i++)
//		{
//			DELETE_IF(sprites[i]);
//		}
//		ResourceManager::Release();
//		RELEASE_IF(DepthStencilView);
//		RELEASE_IF(RenderTargetView);
//		RELEASE_IF(SwapChain);
//		RELEASE_IF(Context);
//		RELEASE_IF(Device);
//		DELETE_IF(fbx);
//	}
//	int run()
//	{
//		MSG msg = {};
//
//		if (!initialize()) return 0;
//		while (WM_QUIT != msg.message)
//		{
//			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
//			{
//				TranslateMessage(&msg);
//				DispatchMessage(&msg);
//			}
//			else
//			{
//				timer.tick();
//				calculate_frame_stats();
//				update(timer.time_interval());
//				render(timer.time_interval());
//			}
//		}
//		return static_cast<int>(msg.wParam);
//	}
//
//	LRESULT CALLBACK handle_message(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
//	{
//		switch (msg)
//		{
//		case WM_PAINT:
//		{
//			PAINTSTRUCT ps;
//			HDC hdc;
//			hdc = BeginPaint(hwnd, &ps);
//			EndPaint(hwnd, &ps);
//			break;
//		}
//		case WM_DESTROY:
//			PostQuitMessage(0);
//			break;
//		case WM_CREATE:
//			break;
//		case WM_KEYDOWN:
//			if (wparam == VK_ESCAPE) PostMessage(hwnd, WM_CLOSE, 0, 0);
//			break;
//		case WM_ENTERSIZEMOVE:
//			// WM_EXITSIZEMOVE is sent when the user grabs the resize bars.
//			timer.stop();
//			break;
//		case WM_EXITSIZEMOVE:
//			// WM_EXITSIZEMOVE is sent when the user releases the resize bars.
//			// Here we reset everything based on the new window dimensions.
//			timer.start();
//			break;
//		default:
//			return DefWindowProc(hwnd, msg, wparam, lparam);
//		}
//		return 0;
//	}
//
//private:
//
//	bool initialize();
//	void update(float elapsed_time/*Elapsed seconds from last frame*/);
//	void render(float elapsed_time/*Elapsed seconds from last frame*/);
//
//private:
//	high_resolution_timer timer;
//	void calculate_frame_stats()
//	{
//		// Code computes the average frames per second, and also the 
//		// average time it takes to render one frame.  These stats 
//		// are appended to the window caption bar.
//		static int frames = 0;
//		static float time_tlapsed = 0.0f;
//
//		frames++;
//
//		// Compute averages over one second period.
//		if ((timer.time_stamp() - time_tlapsed) >= 1.0f)
//		{
//	
//			float fps = static_cast<float>(frames); // fps = frameCnt / 1
//			float mspf = 1000.0f / fps;
//			std::ostringstream outs;
//			outs.precision(6);
//			outs << "FPS : " << fps << " / " << "Frame Time : " << mspf << " (ms)";
//			outs << "BlednMode:" << o;
//			outs << "alpha:" << a;
//			SetWindowTextA(hwnd, outs.str().c_str());
//
//			// Reset for next average.
//			frames = 0;
//			time_tlapsed += 1.0f;
//		}
//	}
//};
//
//extern sprite o;