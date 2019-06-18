#include "ResourceManger.h"

ResourceManager::ResourceShaderResourceViews 
ResourceManager::SRViews[ResourceManager::RESOURCE_MAX];

ResourceManager::ResourceCreateVertexShaders
ResourceManager::VertexShaders[ResourceManager::RESOURCE_MAX];

ResourceManager::ResourceCreatePixelShaders
ResourceManager::PixelShaders[ResourceManager::RESOURCE_MAX];

bool ResourceManager::LoadShaderResourceView(ID3D11Device *Device, const wchar_t* filename, ID3D11ShaderResourceView** SRView, D3D11_TEXTURE2D_DESC* TexDesc) 
{
	
	int no = -1;
	ResourceShaderResourceViews* find = NULL;  //見つかったデータOR新データ
	ID3D11Resource *Resource=NULL;

	////////////////////////////////////////
	//データ検索
	for (int n = 0; n < RESOURCE_MAX; n++) 
	{
		ResourceShaderResourceViews* p = &SRViews[n];
		
		//データがないなら無視
		//但し、最初に見つけた？空き領域なら　セット用に確保
		if (p->iRefNum == 0) {
			if (no == -1)  no = n;
			continue;
		}
		//ファイルパスが違うなら無視
		if (wcscmp(p->path, filename) != 0) continue;


		//同名ファイルが存在した
		find = p;                     //発見
		p->SRView->GetResource(&Resource);  //Resoureの準備
		break;
	}


	//データが見つからなかった＝新規読み込み
	if (!find)
	{
		ResourceShaderResourceViews* p = &SRViews[no];
		HRESULT hr = DirectX::CreateWICTextureFromFile(Device, filename, &Resource, &(p->SRView));
		if (FAILED(hr)) return false;
		find = p;                                    //発見
		wcscpy_s(p->path, 256, filename);            //名前コピー
	}

	//最終処理（参照渡しでデータを返す）
	ID3D11Texture2D *texture2d;                                       //テクスチャデータの準備
	Resource->QueryInterface(&texture2d);                             //


	texture2d->GetDesc(TexDesc);                                      //データコピー
	*SRView = find->SRView;                                          //

	find->iRefNum++;                                                //数を追加

	texture2d->Release();
	Resource->Release();
	
	return true;
}

HRESULT ResourceManager::MakeDummyShaderResourceView(ID3D11Device *Device, ID3D11ShaderResourceView** shaderResourceView)
{
	HRESULT hr = S_OK;

	D3D11_TEXTURE2D_DESC texture2d_desc = {};
	texture2d_desc.Width = 1;
	texture2d_desc.Height = 1;
	texture2d_desc.MipLevels = 1;
	texture2d_desc.ArraySize = 1;
	texture2d_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	texture2d_desc.SampleDesc.Count = 1;
	texture2d_desc.SampleDesc.Quality = 0;
	texture2d_desc.Usage = D3D11_USAGE_DEFAULT;
	texture2d_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	texture2d_desc.CPUAccessFlags = 0;
	texture2d_desc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA subresource_data = {};
	u_int color = 0xFFFFFFFF;
	subresource_data.pSysMem = &color;
	subresource_data.SysMemPitch = 4;
	subresource_data.SysMemSlicePitch = 4;

	ID3D11Texture2D *texture2d;
	hr = Device->CreateTexture2D(&texture2d_desc, &subresource_data, &texture2d);
	if (hr)		return hr;

	D3D11_SHADER_RESOURCE_VIEW_DESC shader_resource_view_desc = {};
	shader_resource_view_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	shader_resource_view_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shader_resource_view_desc.Texture2D.MipLevels = 1;

	hr = Device->CreateShaderResourceView(texture2d, &shader_resource_view_desc, shaderResourceView);
	if (hr)		return hr;

	texture2d->Release();

	return hr;
}

void ResourceManager::ReleaseShaderResourceView(ID3D11ShaderResourceView* SRView) {
	if (!SRView)return;              //ないなら終了
	for (int n = 0; n < RESOURCE_MAX; n++)
	{
		ResourceShaderResourceViews* p = &SRViews[n];
		//データがないから無視
		if (p->iRefNum == 0) continue;

		//データが違うから無視
		if (SRView != p->SRView) continue;

		//データが存在した
		p->Release();      //データ解放
		break;
	}
}



bool ResourceManager::LoadCreatePixelShader (ID3D11Device *Device, ID3D11PixelShader** PixelShader,char* csoFileName) 
{
	BYTE* data;
	unsigned int size;
	int no = -1;
	ResourceCreatePixelShaders*   find = NULL;  //見つかったデータOR新データ
	
	wchar_t filename[256];
	size_t stringSize = 0;
	mbstowcs_s(&stringSize, filename, csoFileName,strlen(csoFileName));

	////////////////////////////////////////
	//データ検索
	for (int n = 0; n < RESOURCE_MAX; n++)
	{
		ResourceCreatePixelShaders* p = &PixelShaders[n];

		//データがないなら無視
		//但し、最初に見つけた？空き領域なら　セット用に確保
		if (p->iRefNum == 0) {
			if (no == -1)  no = n;
			continue;
		}
		//ファイルパスが違うなら無視	
		if (wcscmp(p->path, filename) != 0) continue;
		//同名ファイルが存在した
		find = p;                     //発見
		break;
	}
	
	//データが見つからなかった＝新規読み込み
	if (!find)
	{
		ResourceCreatePixelShaders* p = &PixelShaders[no];

		if (!(ResourceManager::ReadBinaryFile(csoFileName, &data, size)))return false;
		HRESULT hr = Device->CreatePixelShader(data, size, nullptr, & (p->PixelShader));
		if (FAILED(hr)) return false;
		find = p;                                    //発見
		wcscpy_s(p->path, 256, filename);            //名前コピー

		//mbstowcs_s
	}

	//最終処理（参照渡しでデータを返す）
	//ID3D11Texture2D *texture2d;                                       //テクスチャデータの準備
                
	*PixelShader = find->PixelShader;                                    //

	find->iRefNum++;                                                //数を追加
	return true;
}

void ResourceManager::ReleaseCreatePixelShader(
	ID3D11PixelShader* PixelShader)
{
	if (!PixelShader)return;              //ないなら終了
	for (int n = 0; n < RESOURCE_MAX; n++)
	{
		ResourceCreatePixelShaders* p = &PixelShaders[n];
		//データがないから無視
		if (p->iRefNum == 0) continue;

		//データが違うから無視
		if (PixelShader != p->PixelShader) continue;
	
		//データが存在した
		p->Release();      //データ解放
		break;
	}
}



bool ResourceManager::LoadCreateVertexShaders(ID3D11Device *Device,	char* csoFileName,D3D11_INPUT_ELEMENT_DESC* inputElementDesc,int numElements,ID3D11VertexShader** VertexShader, ID3D11InputLayout** InputLayout)
{
	//初期化
	*VertexShader = NULL;
	*InputLayout = NULL;


	ResourceCreateVertexShaders*   find = NULL;  //見つかったデータOR新データ
	int no = -1;

	wchar_t filename[256];
	size_t stringSize = 0;
	mbstowcs_s(&stringSize, filename, csoFileName, strlen(csoFileName));

	////////////////////////////////////////
	//データ検索
	for (int n = 0; n < RESOURCE_MAX; n++)
	{
		ResourceCreateVertexShaders* p = &VertexShaders[n];

		//データがないなら無視
		//但し、最初に見つけた？空き領域なら　セット用に確保
		if (p->iRefNum == 0) {
			if (no == -1)  no = n;
			continue;
		}
		//ファイルパスが違うなら無視
		if (wcscmp(p->path, filename) != 0) continue;
		//同名ファイルが存在した
		find = p;                     //発見
		break;
	}

	//データが見つからなかった＝新規読み込み
	if (!find)
	{
		ResourceCreateVertexShaders* p = &VertexShaders[no];
		BYTE* data;
		unsigned int size;
		if (!(ResourceManager::ReadBinaryFile(csoFileName, &data, size)))return false;
		HRESULT	hr = Device->CreateVertexShader(data, size, nullptr, &(p->VertexShader));
		
		if (FAILED(hr)) { delete[] data; return false ; }

	
		//4-4-2
		hr=Device->CreateInputLayout(inputElementDesc, numElements, data, size,&( p->InptLayout));
		delete[] data;
		if (FAILED(hr)) { return false; }

		find = p;                                    //発見
		wcscpy_s(p->path, 256, filename);            //名前コピー
	}

	//最終処理（参照渡しでデータを返す）
	//ID3D11Texture2D *texture2d;                                       //テクスチャデータの準備

	*VertexShader = find->VertexShader;                                    //
	*InputLayout = find->InptLayout;
	find->iRefNum++;                                                //数を追加
	return true;
}


void ResourceManager::ReleaseCreateVertexShaders(
	ID3D11VertexShader*  VertexShader, 
	ID3D11InputLayout* InptLayout)
{
	if (!VertexShader||!InptLayout)return;              //ないなら終了
	for (int n = 0; n < RESOURCE_MAX; n++)
	{
		ResourceCreateVertexShaders* p = &VertexShaders[n];
		//データがないから無視
		if (p->iRefNum == 0) continue;

		//データが違うから無視
		if (VertexShader != p->VertexShader||InptLayout != p->InptLayout) continue;
		
		//データが存在した
		p->Release();      //データ解放
		break;
	}
	
}


void ResourceManager::Release()
{
	for (int i = 0; i < RESOURCE_MAX; i++) 
	{
		SRViews[i].Release(true);
		VertexShaders[i].Release(true);
		PixelShaders[i].Release(true);
	}
		
}


