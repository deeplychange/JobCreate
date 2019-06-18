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
	ResourceShaderResourceViews* find = NULL;  //���������f�[�^OR�V�f�[�^
	ID3D11Resource *Resource=NULL;

	////////////////////////////////////////
	//�f�[�^����
	for (int n = 0; n < RESOURCE_MAX; n++) 
	{
		ResourceShaderResourceViews* p = &SRViews[n];
		
		//�f�[�^���Ȃ��Ȃ疳��
		//�A���A�ŏ��Ɍ������H�󂫗̈�Ȃ�@�Z�b�g�p�Ɋm��
		if (p->iRefNum == 0) {
			if (no == -1)  no = n;
			continue;
		}
		//�t�@�C���p�X���Ⴄ�Ȃ疳��
		if (wcscmp(p->path, filename) != 0) continue;


		//�����t�@�C�������݂���
		find = p;                     //����
		p->SRView->GetResource(&Resource);  //Resoure�̏���
		break;
	}


	//�f�[�^��������Ȃ��������V�K�ǂݍ���
	if (!find)
	{
		ResourceShaderResourceViews* p = &SRViews[no];
		HRESULT hr = DirectX::CreateWICTextureFromFile(Device, filename, &Resource, &(p->SRView));
		if (FAILED(hr)) return false;
		find = p;                                    //����
		wcscpy_s(p->path, 256, filename);            //���O�R�s�[
	}

	//�ŏI�����i�Q�Ɠn���Ńf�[�^��Ԃ��j
	ID3D11Texture2D *texture2d;                                       //�e�N�X�`���f�[�^�̏���
	Resource->QueryInterface(&texture2d);                             //


	texture2d->GetDesc(TexDesc);                                      //�f�[�^�R�s�[
	*SRView = find->SRView;                                          //

	find->iRefNum++;                                                //����ǉ�

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
	if (!SRView)return;              //�Ȃ��Ȃ�I��
	for (int n = 0; n < RESOURCE_MAX; n++)
	{
		ResourceShaderResourceViews* p = &SRViews[n];
		//�f�[�^���Ȃ����疳��
		if (p->iRefNum == 0) continue;

		//�f�[�^���Ⴄ���疳��
		if (SRView != p->SRView) continue;

		//�f�[�^�����݂���
		p->Release();      //�f�[�^���
		break;
	}
}



bool ResourceManager::LoadCreatePixelShader (ID3D11Device *Device, ID3D11PixelShader** PixelShader,char* csoFileName) 
{
	BYTE* data;
	unsigned int size;
	int no = -1;
	ResourceCreatePixelShaders*   find = NULL;  //���������f�[�^OR�V�f�[�^
	
	wchar_t filename[256];
	size_t stringSize = 0;
	mbstowcs_s(&stringSize, filename, csoFileName,strlen(csoFileName));

	////////////////////////////////////////
	//�f�[�^����
	for (int n = 0; n < RESOURCE_MAX; n++)
	{
		ResourceCreatePixelShaders* p = &PixelShaders[n];

		//�f�[�^���Ȃ��Ȃ疳��
		//�A���A�ŏ��Ɍ������H�󂫗̈�Ȃ�@�Z�b�g�p�Ɋm��
		if (p->iRefNum == 0) {
			if (no == -1)  no = n;
			continue;
		}
		//�t�@�C���p�X���Ⴄ�Ȃ疳��	
		if (wcscmp(p->path, filename) != 0) continue;
		//�����t�@�C�������݂���
		find = p;                     //����
		break;
	}
	
	//�f�[�^��������Ȃ��������V�K�ǂݍ���
	if (!find)
	{
		ResourceCreatePixelShaders* p = &PixelShaders[no];

		if (!(ResourceManager::ReadBinaryFile(csoFileName, &data, size)))return false;
		HRESULT hr = Device->CreatePixelShader(data, size, nullptr, & (p->PixelShader));
		if (FAILED(hr)) return false;
		find = p;                                    //����
		wcscpy_s(p->path, 256, filename);            //���O�R�s�[

		//mbstowcs_s
	}

	//�ŏI�����i�Q�Ɠn���Ńf�[�^��Ԃ��j
	//ID3D11Texture2D *texture2d;                                       //�e�N�X�`���f�[�^�̏���
                
	*PixelShader = find->PixelShader;                                    //

	find->iRefNum++;                                                //����ǉ�
	return true;
}

void ResourceManager::ReleaseCreatePixelShader(
	ID3D11PixelShader* PixelShader)
{
	if (!PixelShader)return;              //�Ȃ��Ȃ�I��
	for (int n = 0; n < RESOURCE_MAX; n++)
	{
		ResourceCreatePixelShaders* p = &PixelShaders[n];
		//�f�[�^���Ȃ����疳��
		if (p->iRefNum == 0) continue;

		//�f�[�^���Ⴄ���疳��
		if (PixelShader != p->PixelShader) continue;
	
		//�f�[�^�����݂���
		p->Release();      //�f�[�^���
		break;
	}
}



bool ResourceManager::LoadCreateVertexShaders(ID3D11Device *Device,	char* csoFileName,D3D11_INPUT_ELEMENT_DESC* inputElementDesc,int numElements,ID3D11VertexShader** VertexShader, ID3D11InputLayout** InputLayout)
{
	//������
	*VertexShader = NULL;
	*InputLayout = NULL;


	ResourceCreateVertexShaders*   find = NULL;  //���������f�[�^OR�V�f�[�^
	int no = -1;

	wchar_t filename[256];
	size_t stringSize = 0;
	mbstowcs_s(&stringSize, filename, csoFileName, strlen(csoFileName));

	////////////////////////////////////////
	//�f�[�^����
	for (int n = 0; n < RESOURCE_MAX; n++)
	{
		ResourceCreateVertexShaders* p = &VertexShaders[n];

		//�f�[�^���Ȃ��Ȃ疳��
		//�A���A�ŏ��Ɍ������H�󂫗̈�Ȃ�@�Z�b�g�p�Ɋm��
		if (p->iRefNum == 0) {
			if (no == -1)  no = n;
			continue;
		}
		//�t�@�C���p�X���Ⴄ�Ȃ疳��
		if (wcscmp(p->path, filename) != 0) continue;
		//�����t�@�C�������݂���
		find = p;                     //����
		break;
	}

	//�f�[�^��������Ȃ��������V�K�ǂݍ���
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

		find = p;                                    //����
		wcscpy_s(p->path, 256, filename);            //���O�R�s�[
	}

	//�ŏI�����i�Q�Ɠn���Ńf�[�^��Ԃ��j
	//ID3D11Texture2D *texture2d;                                       //�e�N�X�`���f�[�^�̏���

	*VertexShader = find->VertexShader;                                    //
	*InputLayout = find->InptLayout;
	find->iRefNum++;                                                //����ǉ�
	return true;
}


void ResourceManager::ReleaseCreateVertexShaders(
	ID3D11VertexShader*  VertexShader, 
	ID3D11InputLayout* InptLayout)
{
	if (!VertexShader||!InptLayout)return;              //�Ȃ��Ȃ�I��
	for (int n = 0; n < RESOURCE_MAX; n++)
	{
		ResourceCreateVertexShaders* p = &VertexShaders[n];
		//�f�[�^���Ȃ����疳��
		if (p->iRefNum == 0) continue;

		//�f�[�^���Ⴄ���疳��
		if (VertexShader != p->VertexShader||InptLayout != p->InptLayout) continue;
		
		//�f�[�^�����݂���
		p->Release();      //�f�[�^���
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


