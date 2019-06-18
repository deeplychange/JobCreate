#include <DirectXMath.h>
#include <iostream>
#include <d3d11.h>
#include "WICTextureLoader.h"

class ResourceManager {
private:
	static const int RESOURCE_MAX = 64;

	struct ResourceShaderResourceViews {
		wchar_t             path[256];          //ファイルパ                    //
		int iRefNum;                            //データの参照数
		ID3D11ShaderResourceView* SRView;
		ResourceShaderResourceViews() :iRefNum(0), SRView(NULL){ path[0] = '\0'; }

		void Release(bool bForce = false)
		{
			if (iRefNum == 0) return;
			if (--iRefNum <= 0) bForce = true;
			if (bForce)
			{
				if (SRView)
				{
					SRView->Release();
					SRView = NULL;
					iRefNum = 0;
					path[0] = '\0';
				}
			}
		}
	};
	struct ResourceCreateVertexShaders {
		wchar_t             path[256];          //ファイルパ                    //
		int iRefNum;                            //データの参照数
		ID3D11VertexShader*  VertexShader;
		ID3D11InputLayout*    InptLayout;
		ResourceCreateVertexShaders() :iRefNum(0), VertexShader(NULL), InptLayout(NULL){ path[0] = '\0'; }

		void Release(bool bForce=false)
		{
			if (iRefNum == 0) return;
			if (--iRefNum <= 0) bForce = true;
			if (bForce)
			{
				if (VertexShader)VertexShader->Release();
				if (InptLayout)InptLayout->Release();
					VertexShader = NULL;
					InptLayout = NULL;
					iRefNum = 0;
					path[0] = '\0';
			}
		}
	};

	struct ResourceCreatePixelShaders {
		wchar_t           path[256];          //ファイルパ                    //
		int iRefNum;                            //データの参照数
		ID3D11PixelShader*   PixelShader;
		ResourceCreatePixelShaders() :iRefNum(0),PixelShader(NULL){ path[0] = '\0'; }

		void Release(bool bForce = false)
		{
			if (iRefNum == 0) return;
			if (--iRefNum <= 0) bForce = true;
			if (bForce)
			{
				if (PixelShader)PixelShader->Release();	
				PixelShader = NULL;
				iRefNum = 0;
				path[0] = '\0';
			}
		}
	};
	static bool ReadBinaryFile(char* filename, BYTE** data, unsigned int& size)
	{
		FILE*fp = 0;
		if (fopen_s(&fp, filename, "rb"))  return false;
		fseek(fp, 0, SEEK_END);
		size = ftell(fp);
		fseek(fp, 0, SEEK_SET);
		*data = new  unsigned char[size];
		fread(*data, size, 1, fp);

		fclose(fp);
		return true;
	}
	static ResourceShaderResourceViews SRViews[RESOURCE_MAX];
	static ResourceCreateVertexShaders VertexShaders[RESOURCE_MAX];
	static ResourceCreatePixelShaders PixelShaders[RESOURCE_MAX];
	ResourceManager(){}        //private中作ると　外部でnew　禁止
	~ResourceManager() {}
public:
	static void Release();             //当てたっらいらないものです
	static void ReleaseShaderResourceView(ID3D11ShaderResourceView* SRView);
	static bool LoadShaderResourceView(ID3D11Device *Device, const wchar_t* filename, ID3D11ShaderResourceView** SRView,D3D11_TEXTURE2D_DESC* TexDesc);


	static bool LoadCreateVertexShaders(ID3D11Device *Device, char* csoFileName, D3D11_INPUT_ELEMENT_DESC* inputElementDesc, int numElements, ID3D11VertexShader** VertexShader, ID3D11InputLayout** InputLayout);
	static void ReleaseCreateVertexShaders(ID3D11VertexShader*  VertexShader,ID3D11InputLayout* InptLayout);

	static bool LoadCreatePixelShader(ID3D11Device *Device,  ID3D11PixelShader** PixelShader, char* csoFileName);
	static void ReleaseCreatePixelShader(ID3D11PixelShader* PixelShader);
	static HRESULT MakeDummyShaderResourceView(ID3D11Device *Device, ID3D11ShaderResourceView** shaderResourceView);
};

