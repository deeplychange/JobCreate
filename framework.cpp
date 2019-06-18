////
////#include <windows.h>
////#include "framework.h"
////#include "sprite.h"
////#include "Blend.h"
////
////int framework::o = 0;
////
////bool framework::initialize()
////{
////	HRESULT hr = S_OK;
////	RECT rc;
////	UINT createDeviceFlags = 0;
////	GetClientRect(hwnd, &rc);
////	UINT width = rc.right - rc.left;
////	UINT height = rc.bottom - rc.top;
////	//Unit08
////	
////#ifdef _DEBUG
////	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
////#endif
////	
////
////	D3D_DRIVER_TYPE         g_driverType = D3D_DRIVER_TYPE_NULL;
////
////
////	D3D_DRIVER_TYPE driverTypes[] =
////	{
////		D3D_DRIVER_TYPE_HARDWARE,
////		D3D_DRIVER_TYPE_WARP,
////		D3D_DRIVER_TYPE_REFERENCE,
////	};
////	UINT numDriverTypes = ARRAYSIZE(driverTypes);
////
////
////	D3D_FEATURE_LEVEL featureLevels[] =
////	{
////		D3D_FEATURE_LEVEL_11_0,
////		D3D_FEATURE_LEVEL_10_1,
////		D3D_FEATURE_LEVEL_10_0,
////	};
////	UINT numFeatureLevels = ARRAYSIZE(featureLevels);
////
////	DXGI_SWAP_CHAIN_DESC sd;
////	ZeroMemory(&sd, sizeof(sd));
////	sd.BufferCount = 1;
////	sd.BufferDesc.Width = width;
////	sd.BufferDesc.Height = height;
////	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
////	sd.BufferDesc.RefreshRate.Numerator = 60;
////	sd.BufferDesc.RefreshRate.Denominator = 1;
////	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
////	sd.OutputWindow = hwnd;   
////	/*UINT mass_quality_level;
////	Device->CheckMultisampleQualityLevels(sd_BufferDesc.Format,4,&mass_quality_level);*/
////	sd.SampleDesc.Count = 1;
////	sd.SampleDesc.Quality = 0;
////	sd.Windowed = TRUE;
////	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
////	sd.Flags = 0;
////
////
////	
////	for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
////	{
////		D3D_DRIVER_TYPE driverType = driverTypes[driverTypeIndex];
////		D3D_FEATURE_LEVEL featureLevel;
////		hr = D3D11CreateDeviceAndSwapChain(NULL, driverType, NULL, createDeviceFlags, featureLevels, numFeatureLevels,
////			D3D11_SDK_VERSION, &sd, &SwapChain, &Device, &featureLevel, &Context);
////		if (SUCCEEDED(hr))   break;	
////	}
//////	_ASSERT_EXPR_A(SUCCEEDED(hr), hr_trace(hr));
////	if (FAILED(hr))  return false; 
////		
////	//Create a render target view
////	ID3D11Texture2D* pBackBuffer = NULL;
////	hr = SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
////	if (FAILED(hr)) return false;
////
////	hr = Device->CreateRenderTargetView(pBackBuffer, NULL, &RenderTargetView);
////	pBackBuffer->Release();
////	if (FAILED(hr))  return false;
////
////	// Tutorial 05
////
////	//Create depth stencil texture 
////	ID3D11Texture2D*   DepthStencil;
////	D3D11_TEXTURE2D_DESC descDepth;
////	ZeroMemory(&descDepth, sizeof(descDepth));
////	descDepth.Width = width;
////	descDepth.Height = height;
////	descDepth.MipLevels = 1;
////	descDepth.ArraySize = 1;
////	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
////	descDepth.SampleDesc.Count = 1;
////	descDepth.SampleDesc.Quality = 0;
////	descDepth.Usage = D3D11_USAGE_DEFAULT;
////	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
////	descDepth.CPUAccessFlags = 0;
////	descDepth.MiscFlags = 0;
////	hr = Device->CreateTexture2D(&descDepth, NULL, &DepthStencil);
////	if (FAILED(hr))return false;
////	
////	//Create the depth stencil view
////	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
////	ZeroMemory(&descDSV, sizeof(descDSV));
////	descDSV.Format = descDepth.Format;
////	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
////	descDSV.Texture2D.MipSlice = 0;
////	hr = Device->CreateDepthStencilView(DepthStencil, &descDSV, &DepthStencilView);
////	DepthStencil->Release();
////
////	//Blend::Initialize(Device);
////
////	//sprites[0] = new sprite(Device, L"logos.jpg");
////	//sprites[1] = new sprite(Device, L"n64.png");
////	//particle   = new sprite(Device, L"particle-smoke.png");
////	
//////	cube = new geometric_cube(Device);        //立方体
//////	cube = new geometric_cylinder(Device,4,0.5,0.5); //多面体＆円柱
////
////	fbx = new skinned_mesh(Device,"004_cube.fbx");
////	return true;
////}
////
////
////
////void framework::update(float elapsed_time/*Elapsed seconds from last frame*/)
////{
////	static int i = 0; 
////	static int step = 1;
////	{
////		switch (step) {
////		case 0:
////			switch (i) {
////			case 0:
////				delete fbx;
////				fbx = new skinned_mesh(Device, "000_cube.fbx");
////				step++;
////				break;
////			case 1:
////				delete fbx;
////				fbx = new skinned_mesh(Device, "001_cube.fbx");
////				step++;
////				break;
////			case 2:
////				delete fbx;
////				fbx = new skinned_mesh(Device, "002_cube.fbx");
////				step++;
////				break;
////			case 3:
////				delete fbx;
////				fbx = new skinned_mesh(Device, "003_cube.fbx");
////				step++;
////				break;
////			case 4:
////				delete fbx;
////				fbx = new skinned_mesh(Device, "004_cube.fbx");
////				step++;
////				break;
////			case 5:
////				delete fbx;
////				fbx = new skinned_mesh(Device, "005_cube.fbx");
////				step++;
////				break;
////			}
////		case 1:
////			/*class OBJ3D {
////				skinned_mesh data;
////				void draw(Vec3 posX) {
////					data.render();
////				}
////			};
////			class Player :public OBJ3D {
////				Vec3 posX;
////			};
////			Player player;
////			player.draw*/
////			fbx->update();
////			if (GetAsyncKeyState('0') & 1) {
////				step = 0;
////				i = 0;
////			}
////			if (GetAsyncKeyState('1') & 1) {
////				step = 0;
////				i = 1;
////			}
////			if (GetAsyncKeyState('2') & 1) {
////				step = 0;
////				i = 2;
////			}
////			if (GetAsyncKeyState('3') & 1) {
////				step = 0;
////				i = 3;
////			}
////			if (GetAsyncKeyState('4') & 1) {
////				step = 0;
////				i = 4;
////			}
////			if (GetAsyncKeyState('5') & 1) {
////				step = 0;
////				i = 5;
////			}
////		
////			break;
////		}
////	}
////	
////}
//void framework::render(float elapsed_time/*Elapsed seconds from last frame*/)
//{
//
//	
//
//
//	//Unit10
//
//	static DirectX::XMFLOAT3 pos(0, 0, 0);
//	pos = DirectX::XMFLOAT3(fbx->pos.x,fbx->pos.y,fbx->pos.z);
//	static DirectX::XMFLOAT3 angle(0, 0, 0);
//	static DirectX::XMFLOAT3 scale(1, 1, 1);
//
//	static DirectX::XMFLOAT4 cpos(10,5,-25,0);
//	static DirectX::XMFLOAT4 ctarget(0,0,0,0);
//
//	static float fovY = DirectX::XMConvertToRadians(30.0f);	//視野角３０度
//
//	//guangyuan sheding
//	static	DirectX::XMFLOAT4 light_dir(-1,0,0, 0);
//	//スキンのcolor
//	static	DirectX::XMFLOAT4 material_col(1.0f,1.0f,1.0f, 1);
//	
//	//angle.x += DirectX::XMConvertToRadians(30.0f)*elapsed_time;
//	angle.y += DirectX::XMConvertToRadians(30.0f)*elapsed_time;
//	//angle.z += DirectX::XMConvertToRadians(30.0f)*elapsed_time;
//
//	
//
//	//ワールド変換
//	DirectX::XMMATRIX S, Rx, Ry, Rz, R, T, W;
////	S=DirectX::XMMatrixRotationQuaternion()
//	S  = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
//	Rx = DirectX::XMMatrixRotationX(angle.x);
//	Ry = DirectX::XMMatrixRotationY(angle.y);
//	Rz = DirectX::XMMatrixRotationZ(angle.z);
//	R = Rz*Rx*Ry;
//	T = DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z);
//	W = R*S*T;
//
//
//
//	//ビュー変換              // 視図変換
//	DirectX::XMMATRIX V;
//	const DirectX::XMFLOAT4 up1(0,1,0, 0);    //カメラーを設定
//	
//	DirectX::XMVECTOR cp ,ct, up;
//	cp = DirectX::XMLoadFloat4(&cpos);
//	ct = DirectX::XMLoadFloat4(&ctarget);
//	up = DirectX::XMLoadFloat4(&up1);
//	V  = DirectX::XMMatrixLookAtLH(cp, ct,up);
//
//
//
//	//プロジェクション変換       //投影変換
//	DirectX::XMMATRIX P;
//	static bool bPers =true;
//	if (GetAsyncKeyState(VK_SPACE) & 1) {
//		bPers = !bPers;
//	}
//	const float aspect = SCREEN_WIDTH / (float)SCREEN_HEIGHT;
//
//	//1.2位は画面の調整  3位はNearクリップ値　4位はFarクリップ値
//	//平行
//	if (!bPers)P = DirectX::XMMatrixOrthographicLH(2 * aspect, 2, 0.1f, 200.0f); 
//	//透視
//	else P = DirectX::XMMatrixPerspectiveFovLH(fovY, aspect, 0.1f, 200.0f);
//
//
//
//	//描画
//
//	static bool bWareframe = false;
//	if (GetAsyncKeyState(VK_RETURN) & 1) {
//		bWareframe = !bWareframe;
//	}
//	//FLoat4X4型に変換する
//	DirectX::XMFLOAT4X4 w, wvp;
//	DirectX::XMStoreFloat4x4(&w, W);
//	DirectX::XMStoreFloat4x4(&wvp, W*V*P);
//
//	//描画
//	if(cube)cube->render(Context,wvp, w,  material_col, light_dir, bWareframe);
//	if (fbx)fbx->render(Context, wvp, w, material_col, light_dir, bWareframe);
//	SwapChain->Present(0, 0);
//}
//
//
