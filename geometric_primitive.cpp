#include <direct.h>
#include <DirectXMath.h>
#include "WICTextureLoader.h"
#include "geometric_primitive.h"
#include "ResourceManger.h"

using namespace DirectX;
 
geometric_primitive::geometric_primitive(ID3D11Device* Device)
{
	HRESULT hr = S_OK;

	//VertexShader    InputLayout
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",     0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	ID3DBlob* pPSBlob = NULL;   //�g���Ă��Ȃ�
	UINT numElements = ARRAYSIZE(layout);
	ResourceManager::LoadCreateVertexShaders(Device, "geometric_primitive_vs.cso", layout, numElements, &GVertexShader, &GInptLayout);
	ResourceManager::LoadCreatePixelShader(Device, &GPixelShader, "geometric_primitive_ps.cso");

	//RASTERIZER   ���`��
	D3D11_RASTERIZER_DESC rsDesc;
	ZeroMemory(&rsDesc, sizeof(D3D11_RASTERIZER_DESC));
	rsDesc.FillMode = D3D11_FILL_WIREFRAME;
	rsDesc.CullMode = D3D11_CULL_BACK;
	rsDesc.FrontCounterClockwise = FALSE;   //�����v���
	rsDesc.DepthClipEnable = FALSE;
	hr = Device->CreateRasterizerState(&rsDesc, &RSWireframe);
	if (FAILED(hr)) 
	{
		MessageBox(NULL, L"CreateRasterizerState():Create failed", 0, 0);
		return;
	} 
	

	//RASTERIZER   �h��Ԃ��`��
	ZeroMemory(&rsDesc, sizeof(D3D11_RASTERIZER_DESC));
	rsDesc.FillMode =  D3D11_FILL_SOLID;
	rsDesc.CullMode = D3D11_CULL_BACK;
	rsDesc.FrontCounterClockwise = FALSE;
	rsDesc.DepthClipEnable = TRUE;
	hr = Device->CreateRasterizerState(&rsDesc, &RSSolid);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"CreateRasterizerState():Create failed", 0, 0);
		return;
	}

	//DEPTH_STENCIL �[�x�e�X�g
	D3D11_DEPTH_STENCIL_DESC dsDesc;
	// Depth test parameters
	ZeroMemory(&dsDesc, sizeof(dsDesc));
	dsDesc.DepthEnable = TRUE;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;        // D3D11_DEPTH_WRITE_MASK_ALL
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS;//LESS
											   // Stencil test parameters
	dsDesc.StencilEnable = false;
	dsDesc.StencilReadMask = 0xFF;
	dsDesc.StencilWriteMask = 0xFF;
	hr = Device->CreateDepthStencilState(&dsDesc, &GDepthSS);
	if (FAILED(hr)) {
		MessageBox(NULL, L"geometric_primitive::CreateDepthStencilState():Create failed", 0, 0);
		return;
	}
	
}

geometric_primitive::geometric_primitive(ID3D11Device* Device,wchar_t* _filename)
{
	HRESULT hr = S_OK;

	//VertexShader    InputLayout
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0,   D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",      0, DXGI_FORMAT_R32G32B32A32_FLOAT,0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",     0, DXGI_FORMAT_R32G32B32_FLOAT, 0,   D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",   0, DXGI_FORMAT_R32G32_FLOAT,    0,   D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	//ID3DBlob* pPSBlob = NULL;   //�g���Ă��Ȃ�
	
	UINT numElements = ARRAYSIZE(layout);
	
	if (!ResourceManager::LoadCreateVertexShaders(Device, "billboard_vs.cso", layout, numElements, &GVertexShader, &GInptLayout)) {
		return;
	}
	if (!ResourceManager::LoadCreatePixelShader(Device, &GPixelShader, "billboard_ps.cso")) {
		return;
	}

	//RASTERIZER   �h��Ԃ��`��
	D3D11_RASTERIZER_DESC rsDesc;
	ZeroMemory(&rsDesc, sizeof(D3D11_RASTERIZER_DESC));
	rsDesc.FillMode = D3D11_FILL_WIREFRAME;
	rsDesc.CullMode = D3D11_CULL_NONE;
	rsDesc.FrontCounterClockwise = FALSE;   //�����v���
	rsDesc.DepthClipEnable = FALSE;
	hr = Device->CreateRasterizerState(&rsDesc, &RSWireframe);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"geometric_primitive::CreateRasterizerState():Create failed", 0, 0);
		return;
	}

	//RASTERIZER   ���`��
	ZeroMemory(&rsDesc, sizeof(D3D11_RASTERIZER_DESC));
	rsDesc.FillMode = D3D11_FILL_SOLID;
	rsDesc.CullMode = D3D11_CULL_BACK;
	rsDesc.FrontCounterClockwise = FALSE;
	rsDesc.DepthClipEnable = TRUE;
	hr = Device->CreateRasterizerState(&rsDesc, &RSSolid);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"geometric_primitive::CreateRasterizerState():Create failed", 0, 0);
		return;
	}


	ResourceManager::LoadShaderResourceView(Device, _filename, &SRView, &Desc);


	//DEPTH_STENCIL �[�x�e�X�g
	D3D11_DEPTH_STENCIL_DESC dsDesc;
	// Depth test parameters
	ZeroMemory(&dsDesc, sizeof(dsDesc));
	dsDesc.DepthEnable = TRUE;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;        // D3D11_DEPTH_WRITE_MASK_ALL
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS;//LESS
											 // Stencil test parameters
	dsDesc.StencilEnable = false;
	dsDesc.StencilReadMask = 0xFF;
	dsDesc.StencilWriteMask = 0xFF;
	hr = Device->CreateDepthStencilState(&dsDesc, &GDepthSS);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"geometric_primitive::CreateDepthStencilState():Create failed", 0, 0);
		return;
	}

	//	//5-4
	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof(samplerDesc));
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0;
	samplerDesc.MaxAnisotropy = 16;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	hr = Device->CreateSamplerState(&samplerDesc, &State);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"geometric_primitive::CreateSamplerState():Create failed", 0, 0);
		return;
	}

}

void geometric_primitive::BufferSet(ID3D11Device* Device, vertex *vertices, u_int *indices, int VerticesNumber,int indicesNumber) {
	HRESULT hr = S_OK;
	//Buffer  
	//���_�o�b�t�@�I�u�W�F�N�g����
	//VertexBuffer
	D3D11_BUFFER_DESC bd;
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&bd, sizeof(bd));
	ZeroMemory(&InitData, sizeof(InitData));

	bd.ByteWidth = VerticesNumber*sizeof(vertex);
	bd.Usage=D3D11_USAGE_DYNAMIC;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bd.MiscFlags = 0;
	bd.StructureByteStride = 0;

	InitData.pSysMem = vertices;
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;
	hr = Device->CreateBuffer(&bd, &InitData, &VBuffer);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"BufferSet::CreateBuffer():Create failed", 0, 0);
		return;
	}

	//�C���f�b�N�X�o�b�t�@
	//Index Buffer
	ZeroMemory(&bd, sizeof(bd));
	ZeroMemory(&InitData, sizeof(InitData));

	bd.ByteWidth = indicesNumber*sizeof(u_int);
	bd.Usage = D3D11_USAGE_IMMUTABLE;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	bd.StructureByteStride = 0;

	InitData.pSysMem = indices;
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;
	numIndices = indicesNumber;
	hr = Device->CreateBuffer(&bd, &InitData, &IBuffer);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"BufferSet::CreateBuffer():Create failed", 0, 0);
		return;
	}
	//�萔�o�b�t�@
	//Constant Buffer
	ZeroMemory(&bd, sizeof(bd));
	ZeroMemory(&InitData, sizeof(InitData));

	bd.ByteWidth = sizeof(cbuffer);
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	bd.StructureByteStride = 0;

	
	hr = Device->CreateBuffer(&bd, nullptr, &CBuffer);
	int test = 0;
	#if DEBUG_FG
	if (FAILED(hr)) {
		MessageBox(NULL, L"BufferSet::BufferSet():Create Buffer failed", 0, 0);
		return;
	} 
	#endif

}

//primitive�`��
void geometric_primitive::render
(
	ID3D11DeviceContext*Context, const DirectX::XMFLOAT4X4 & world_view_projection, const DirectX::XMFLOAT4X4 & world, 
	const DirectX::XMFLOAT4   & material_color,const DirectX::XMFLOAT4 &light_direction,bool line )

{
	cbuffer cb;
	cb.world_view_projection = world_view_projection;
	cb.world = world;
	cb.material_color = material_color;
	cb.light_direction = light_direction;

	//set �萔�o�b�t�@�@3D
	//3D�̂�
	Context->UpdateSubresource(CBuffer, 0, NULL, &cb, 0, 0);
	Context->VSSetConstantBuffers(0, 1, &CBuffer);

	//Set vertex buffer
	UINT stride = sizeof(vertex);
	UINT offset = 0;
	//set index buffer
	Context->IASetVertexBuffers(0, 1, &VBuffer, &stride, &offset);
	Context->IASetIndexBuffer(IBuffer,DXGI_FORMAT_R32_UINT, offset);
	Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	Context->IASetInputLayout(GInptLayout);
	//State�̐ݒ�
	if (line)Context->RSSetState(RSWireframe);
	else Context->RSSetState(RSSolid);
	//Render a triangle
	Context->VSSetShader(GVertexShader, NULL, 0);
	Context->PSSetShader(GPixelShader, NULL, 0);
	//�[�x�e�X�g��ݒ肷��@
	Context->OMSetDepthStencilState(GDepthSS, 1);
	
	//index�t���`��
	Context->DrawIndexed(numIndices, 0, 0);

}


//�Ŕ`��
void geometric_primitive::renderBillboard
(
	ID3D11DeviceContext*Context,const DirectX::XMFLOAT4X4 & world_view_projection,const DirectX::XMFLOAT4X4 & world,
	const DirectX::XMFLOAT4   & material_color,const DirectX::XMFLOAT4 &light_direction,bool line,float sx, float sy, float sw, float sh)
{
	cbuffer cb;
	cb.world_view_projection = world_view_projection;
	cb.world = world;
	cb.material_color = material_color;
	cb.light_direction = light_direction;

	
	vertex verticesNDC[4];
	for (int i = 0; i < 4; ++i) {
		verticesNDC[i].color = VectexMemory[i].color;
		verticesNDC[i].position = VectexMemory[i].position;
		verticesNDC[i].normal = VectexMemory[i].normal;
	}
	 
	verticesNDC[0].tecoord.x = sx;
	verticesNDC[0].tecoord.y = sy;
	verticesNDC[1].tecoord.x = sx + sw;
	verticesNDC[1].tecoord.y = sy;
	verticesNDC[2].tecoord.x = sx;
	verticesNDC[2].tecoord.y = sy + sh;
	verticesNDC[3].tecoord.x = sx + sw;
	verticesNDC[3].tecoord.y = sy + sh;
	

	for (int i = 0; i < 4; ++i) {
		verticesNDC[i].tecoord.x /= Desc.Width;
		verticesNDC[i].tecoord.y /= Desc.Height;
	}
	D3D11_MAPPED_SUBRESOURCE sub0;
	Context->Map(VBuffer,0, D3D11_MAP_WRITE_DISCARD, 0, &sub0);
	memcpy(sub0.pData,verticesNDC,sizeof(verticesNDC));
	Context->Unmap(VBuffer, 0);
	
	//set �萔�o�b�t�@�@3D
	//3D�̂�
	Context->UpdateSubresource(CBuffer, 0, NULL, &cb, 0, 0);
	Context->VSSetConstantBuffers(0, 1, &CBuffer);


	//Set vertex buffer
	UINT stride = sizeof(vertex);
	UINT offset = 0;

	Context->IASetVertexBuffers(0, 1, &VBuffer, &stride, &offset);
	//set index buffer
	//3D�̂�
	Context->IASetIndexBuffer(IBuffer, DXGI_FORMAT_R32_UINT, offset);

	//Set primitive topology
	Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//Set the input layout
	Context->IASetInputLayout(GInptLayout);
	//State�̐ݒ�
	if (line)Context->RSSetState(RSWireframe);
	else Context->RSSetState(RSSolid);
	//Render a triangle
	Context->VSSetShader(GVertexShader, NULL, 0);
	Context->PSSetShader(GPixelShader, NULL, 0);
	
	//2D�̂�
	Context->PSSetShaderResources(0, 1, &SRView);

	//2D�̂�
	Context->PSSetSamplers(0, 1, &State);

	//�[�x�e�X�g��ݒ肷��@
	Context->OMSetDepthStencilState(GDepthSS, 0);

	//index�t���`��
	Context->DrawIndexed(numIndices, 0, 0);
}

//�Ŕ`��
void geometric_primitive::renderTexture
(
	ID3D11DeviceContext*Context,const DirectX::XMFLOAT4X4 & world_view_projection,const DirectX::XMFLOAT4X4 & world,
	const DirectX::XMFLOAT4   & material_color,const DirectX::XMFLOAT4 &light_direction,bool line, float sx, float sy, float sw, float sh)
{
	cbuffer cb;
	cb.world_view_projection = world_view_projection;
	cb.world = world;
	cb.material_color = material_color;
	cb.light_direction = light_direction;


	vertex verticesNDC[24];
	for (int i = 0; i < 24; ++i) {
		verticesNDC[i].color = VectexMemory[i].color;
		verticesNDC[i].position = VectexMemory[i].position;
		verticesNDC[i].normal = VectexMemory[i].normal;
	}
		verticesNDC[0].tecoord.x = sx;
		verticesNDC[0].tecoord.y = sy;
		verticesNDC[1].tecoord.x = sx + sw;
		verticesNDC[1].tecoord.y = sy;
		verticesNDC[2].tecoord.x = sx;
		verticesNDC[2].tecoord.y = sy + sh;
		verticesNDC[3].tecoord.x = sx + sw;
		verticesNDC[3].tecoord.y = sy + sh;
	

	for (int i = 0; i < 4; ++i) {
		verticesNDC[i].tecoord.x /= Desc.Width;
		verticesNDC[i].tecoord.y /= Desc.Height;
	}
	D3D11_MAPPED_SUBRESOURCE sub0;
	Context->Map(VBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &sub0);
	memcpy(sub0.pData, verticesNDC, sizeof(verticesNDC));
	Context->Unmap(VBuffer, 0);

	//set �萔�o�b�t�@�@3D
	//3D�̂�
	Context->UpdateSubresource(CBuffer, 0, NULL, &cb, 0, 0);
	Context->VSSetConstantBuffers(0, 1, &CBuffer);


	//Set vertex buffer
	UINT stride = sizeof(vertex);
	UINT offset = 0;

	Context->IASetVertexBuffers(0, 1, &VBuffer, &stride, &offset);
	//set index buffer
	//3D�̂�
	Context->IASetIndexBuffer(IBuffer, DXGI_FORMAT_R32_UINT, offset);

	//Set primitive topology
	Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//Set the input layout
	Context->IASetInputLayout(GInptLayout);
	//State�̐ݒ�
	if (line)Context->RSSetState(RSWireframe);
	else Context->RSSetState(RSSolid);
	//Render a triangle
	Context->VSSetShader(GVertexShader, NULL, 0);
	Context->PSSetShader(GPixelShader, NULL, 0);

	//2D�̂�
	Context->PSSetShaderResources(0, 1, &SRView);

	//2D�̂�
	Context->PSSetSamplers(0, 1, &State);

	//�[�x�e�X�g��ݒ肷��@
	Context->OMSetDepthStencilState(GDepthSS, 0);

	//index�t���`��
	Context->DrawIndexed(numIndices, 0, 0);
}


#define RELEASE_IF(x) if(x){x->Release();x=NULL;}
geometric_primitive::~geometric_primitive() {

	RELEASE_IF(GDepthSS);
	RELEASE_IF(RSSolid);
	RELEASE_IF(RSWireframe);
	RELEASE_IF(CBuffer);
	RELEASE_IF(IBuffer);
	RELEASE_IF(VBuffer);
	RELEASE_IF(State); 
	if (VectexMemory) {
		delete VectexMemory;
		VectexMemory = nullptr;
	}
	ResourceManager::ReleaseCreatePixelShader(GPixelShader);
	ResourceManager::ReleaseCreateVertexShaders(GVertexShader, GInptLayout);
}


//������
geometric_cube::geometric_cube(ID3D11Device* Device):
	geometric_primitive(Device)
{
	vertex vertices[24];     //�ʁ��S(���_�l��)
	u_int  indices[36] ;   //�ʁ��U�i�O�p�`��j

	//top-side
	//0-------1
	//|       |
	//|  +Y   |
	//|       |
	//2-------3
	face = 0;
	vertices[face * 4 + 0].position = DirectX::XMFLOAT3((-0.5f),(+1.0f),( +0.5f));
	vertices[face * 4 + 1].position = DirectX::XMFLOAT3((+0.5f),(+1.0f),( +0.5f));
	vertices[face * 4 + 2].position = DirectX::XMFLOAT3((-0.5f),(+1.0f),( -0.5f));
	vertices[face * 4 + 3].position = DirectX::XMFLOAT3((+0.5f),(+1.0f),( -0.5f));
	vertices[face * 4 + 0].normal = DirectX::XMFLOAT3(+0.0f, +1.0f, +0.0f);
	vertices[face * 4 + 1].normal = DirectX::XMFLOAT3(+0.0f, +1.0f, +0.0f);
	vertices[face * 4 + 2].normal = DirectX::XMFLOAT3(+0.0f, +1.0f, +0.0f);
	vertices[face * 4 + 3].normal = DirectX::XMFLOAT3(+0.0f, +1.0f, +0.0f);
	indices[face * 6 + 0] = face * 4 + 0;
	indices[face * 6 + 1] = face * 4 + 1;
	indices[face * 6 + 2] = face * 4 + 2;
	indices[face * 6 + 3] = face * 4 + 1;
	indices[face * 6 + 4] = face * 4 + 3;
	indices[face * 6 + 5] = face * 4 + 2;
	/*bottom-side
	0-------1
	|       |
	|  -Y   |
	|       |
	2-------3*/
	face += 1;
	vertices[face * 4 + 0].position = DirectX::XMFLOAT3(-0.5f, 0.0f, +0.5f);
	vertices[face * 4 + 1].position = DirectX::XMFLOAT3(+0.5f, 0.0f, +0.5f);
	vertices[face * 4 + 2].position = DirectX::XMFLOAT3(-0.5f, 0.0f, -0.5f);
	vertices[face * 4 + 3].position = DirectX::XMFLOAT3(+0.5f, 0.0f, -0.5f);
	vertices[face * 4 + 0].normal = DirectX::XMFLOAT3(+0.0f, -1.0f, +0.0f);
	vertices[face * 4 + 1].normal = DirectX::XMFLOAT3(+0.0f, -1.0f, +0.0f);
	vertices[face * 4 + 2].normal = DirectX::XMFLOAT3(+0.0f, -1.0f, +0.0f);
	vertices[face * 4 + 3].normal = DirectX::XMFLOAT3(+0.0f, -1.0f, +0.0f);
	indices[face * 6 + 0] = face * 4 + 0;
	indices[face * 6 + 1] = face * 4 + 2;
	indices[face * 6 + 2] = face * 4 + 1;
	indices[face * 6 + 3] = face * 4 + 1;
	indices[face * 6 + 4] = face * 4 + 2;
	indices[face * 6 + 5] = face * 4 + 3;
	////front-side
	////0-------1
	////|       |
	////|  +Z   |
	////|       |
	////2-------3
	face += 1;
	vertices[face * 4 + 0].position = DirectX::XMFLOAT3(-0.5f, 1.0f, -0.5f);
	vertices[face * 4 + 1].position = DirectX::XMFLOAT3(+0.5f, 1.0f, -0.5f);
	vertices[face * 4 + 2].position = DirectX::XMFLOAT3(-0.5f, 0.0f, -0.5f);
	vertices[face * 4 + 3].position = DirectX::XMFLOAT3(+0.5f, 0.0f, -0.5f);
	vertices[face * 4 + 0].normal = DirectX::XMFLOAT3(0, 0, -1);
	vertices[face * 4 + 1].normal = DirectX::XMFLOAT3(0, 0, -1);
	vertices[face * 4 + 2].normal = DirectX::XMFLOAT3(0, 0, -1);
	vertices[face * 4 + 3].normal = DirectX::XMFLOAT3(0, 0, -1);
	indices[face * 6 + 0] = face * 4 + 0;
	indices[face * 6 + 1] = face * 4 + 1;
	indices[face * 6 + 2] = face * 4 + 2;
	indices[face * 6 + 3] = face * 4 + 1;
	indices[face * 6 + 4] = face * 4 + 3;
	indices[face * 6 + 5] = face * 4 + 2;
	////behind-side
	////0-------1
	////|       |
	////|  +Z   |
	////|       |
	////2-------3
	face += 1;
	vertices[face * 4 + 0].position = DirectX::XMFLOAT3(-0.5f, 1.0f, +0.5f);
	vertices[face * 4 + 1].position = DirectX::XMFLOAT3(+0.5f, 1.0f, +0.5f);
	vertices[face * 4 + 2].position = DirectX::XMFLOAT3(-0.5f, 0.0f, +0.5f);
	vertices[face * 4 + 3].position = DirectX::XMFLOAT3(+0.5f, 0.0f, +0.5f);
	vertices[face * 4 + 0].normal = DirectX::XMFLOAT3(0, 0, +1);
	vertices[face * 4 + 1].normal = DirectX::XMFLOAT3(0, 0, +1);
	vertices[face * 4 + 2].normal = DirectX::XMFLOAT3(0, 0, +1);
	vertices[face * 4 + 3].normal = DirectX::XMFLOAT3(0, 0, +1);
	indices[face * 6 + 0] = face * 4 + 0;
	indices[face * 6 + 1] = face * 4 + 2;
	indices[face * 6 + 2] = face * 4 + 1;
	indices[face * 6 + 3] = face * 4 + 2;
	indices[face * 6 + 4] = face * 4 + 3;
	indices[face * 6 + 5] = face * 4 + 1;

	////left-side
	////0-------1
	////|       |
	////|  -X   |
	////|       |
	////2-------3
	face += 1;
	vertices[face * 4 + 0].position = DirectX::XMFLOAT3(-0.5f, 0.0f, +0.5f);
	vertices[face * 4 + 1].position = DirectX::XMFLOAT3(-0.5f, 1.0f, +0.5f);
	vertices[face * 4 + 2].position = DirectX::XMFLOAT3(-0.5f, 0.0f, -0.5f);
	vertices[face * 4 + 3].position = DirectX::XMFLOAT3(-0.5f, 1.0f, -0.5f);
	vertices[face * 4 + 0].normal = DirectX::XMFLOAT3(-1, 0, 0);
	vertices[face * 4 + 1].normal = DirectX::XMFLOAT3(-1, 0, 0);
	vertices[face * 4 + 2].normal = DirectX::XMFLOAT3(-1, 0, 0);
	vertices[face * 4 + 3].normal = DirectX::XMFLOAT3(-1, 0, 0);
	indices[face * 6 + 0] = face * 4 + 0;
	indices[face * 6 + 1] = face * 4 + 1;
	indices[face * 6 + 2] = face * 4 + 2;
	indices[face * 6 + 3] = face * 4 + 1;
	indices[face * 6 + 4] = face * 4 + 3;
	indices[face * 6 + 5] = face * 4 + 2;
	////right-side
	////0-------1
	////|       |
	////|  +X   |
	////|       |
	////2-------3
	face += 1;
	vertices[face * 4 + 0].position = DirectX::XMFLOAT3(+0.5f, 0.0f, +0.5f);
	vertices[face * 4 + 1].position = DirectX::XMFLOAT3(+0.5f, 1.0f, +0.5f);
	vertices[face * 4 + 2].position = DirectX::XMFLOAT3(+0.5f, 0.0f, -0.5f);
	vertices[face * 4 + 3].position = DirectX::XMFLOAT3(+0.5f, 1.0f, -0.5f);
	vertices[face * 4 + 0].normal = DirectX::XMFLOAT3(+1, 0, 0);
	vertices[face * 4 + 1].normal = DirectX::XMFLOAT3(+1, 0, 0);
	vertices[face * 4 + 2].normal = DirectX::XMFLOAT3(+1, 0, 0);
	vertices[face * 4 + 3].normal = DirectX::XMFLOAT3(+1, 0, 0);
	indices[face * 6 + 0] = face * 4 + 0;
	indices[face * 6 + 1] = face * 4 + 2;
	indices[face * 6 + 2] = face * 4 + 1;
	indices[face * 6 + 3] = face * 4 + 1;
	indices[face * 6 + 4] = face * 4 + 2;
	indices[face * 6 + 5] = face * 4 + 3;
	vertices;
	VerticesNumber = 24;
	indicesNumber = 36;
	VectexMemory=(vertex*) malloc(VerticesNumber*sizeof(vertex));
	memcpy(VectexMemory,vertices, VerticesNumber*sizeof(vertex));
	geometric_primitive::BufferSet(Device, vertices,indices,24,36);
	//delete VectexMemory;
}


//�~��
geometric_cylinder::geometric_cylinder(ID3D11Device* Device, int x, float r, float h) :
	geometric_primitive(Device),NUM(x)
{
	type = CYLINDER;
	 vertex *vPtr = new vertex[(NUM +1)*2+ NUM *4];
	 u_int *iPtr = new u_int[NUM * 3 * 2 + NUM * 6];
//	vertex vertices[(CYLINDER_FACE + 1) * 2+ CYLINDER_FACE*4];    //�O�p�`�͂U�ʂ�����
//	u_int  indices [CYLINDER_FACE * 3 * 2 + CYLINDER_FACE * 6];      //���_��3*�ʐ� 

						  //bottom-side
						  // 6------1
						//  /        >
	                   // 5<     0    >2
	                  //    <        >
	                   //    4------3
	
	face = 0;
	//���S���W�Ɩ@��
	const float DANGLE = XMConvertToRadians(360.0f / NUM);
	//(vPtr+0)->position= XMFLOAT3(+0.0f, (0 - h / 2), 0.0f);
	(vPtr + 0)->position = XMFLOAT3(+0.0f,0, 0.0f);
	(vPtr+0)->normal=  XMFLOAT3((0),-1.0f, (0));
	//CYLINDER_FACE�ɂ���Ē��_���W�Ɩ@�����v�Z����
	for (int n = 1; n <= NUM; n++)
	{
		float x = r*sin(DANGLE*(n-1));
		float z = r*cos(DANGLE*(n-1));
		//(vPtr+n)->position= XMFLOAT3(x, (0 - h / 2), z);
		(vPtr + n)->position = XMFLOAT3(x, 0, z);
		(vPtr+n)->normal= XMFLOAT3(+0.0f, -1.0f, 0.0f);
	}
	//indices��Ή�����
	for (int n = 0; n < NUM; n++)
	{
		if (n == (NUM - 1))
		{
			iPtr[0 + n * 3] = 0;
			iPtr[1 + n * 3] = n + 1;
			iPtr[2 + n * 3] = 1;
		}
		else
		{
			iPtr[0 + n * 3] = 0;
			iPtr[1 + n * 3] = n + 1;
			iPtr[2 + n * 3] = n+2;

		}
	}

	//up-side
	    //  6------1
       //  /         >
   	  // 5<     0    >2
	 //    <        >
  	//      4------3
	face += 1;    //face = 1
	 //���S���W�Ɩ@��

	//(vPtr + face*NUM + 1 + 0)->position = XMFLOAT3(+0.0f, (0 + h / 2), 0.0f);
	(vPtr + face*NUM + 1 + 0)->position = XMFLOAT3(+0.0f, h, 0.0f);
	(vPtr + face*NUM + 1 + 0)->normal = XMFLOAT3((0), +1.0f, (0));
	//CYLINDER_FACE�ɂ���Ē��_���W�Ɩ@�����v�Z����

	for (int n = 1; n <= NUM; n++)
	{
		float x = r*sin(DANGLE*(n - 1));
		float z = r*cos(DANGLE*(n - 1));
		//(vPtr + (face*NUM + 1) + n)->position = XMFLOAT3(x, (0 +h / 2), z);
		(vPtr + (face*NUM + 1) + n)->position = XMFLOAT3(x, h, z);
		(vPtr + (face*NUM + 1) + n)->normal = XMFLOAT3(+0.0f, +1.0f, +0.0f);
	}
	//indices��Ή�����

	for (int n = 0; n < NUM; n++)
	{
		if (n == (NUM - 1))
		{
			iPtr[(face*NUM) * 3 + (0 + n * 3)]=  (face*NUM + 1) + 0;
			iPtr[(face*NUM) * 3 + (1 + n * 3)] = (face*NUM + 1) + 1;
			iPtr[(face*NUM) * 3 + (2 + n * 3)] = (face*NUM + 1) + n+1;
		}
		else
		{
			iPtr[(face*NUM) * 3 + (0 + n * 3)] = (face*NUM + 1) + 0;
			iPtr[(face*NUM) * 3 + (1 + n * 3)] = (face*NUM + 1) + n + 2;
			iPtr[(face*NUM) * 3 + (2 + n * 3)] = (face*NUM + 1) + n + 1;
		}
	}

	//����
	//���W�Ɩ@��
	face += 1;          //face=2
	//�����̂̎l���_
	for (int n = 0; n < NUM; n++)
	{
		//����
		float x1 = r*sin(DANGLE*(n ));
		float z1 = r*cos(DANGLE*(n ));
		float normalX1 = r*sin(DANGLE*(n + 0)) ;
		float normalZ1 = r*cos(DANGLE*(n + 0)) ;
		//(vPtr+(face*(NUM + 1) + n*4))->position = XMFLOAT3(x1, 0 - h / 2, z1);
		(vPtr + (face*(NUM + 1) + n * 4))->position = XMFLOAT3(x1, 0, z1);
		(vPtr+(face*(NUM + 1) + n*4))->normal = XMFLOAT3(normalX1,0, normalZ1);
		//�E��
		float x2 = r*sin(DANGLE*(n + 1));
		float z2 = r*cos(DANGLE*(n + 1));
		float normalX2 = r*sin(DANGLE*(n + 1)) ;
		float normalZ2 = r*cos(DANGLE*(n + 1)) ;
		//(vPtr+(face*(NUM + 1) + n*4+1))->position = XMFLOAT3(x2, 0 - h / 2, z2);
		(vPtr + (face*(NUM + 1) + n * 4 + 1))->position = XMFLOAT3(x2, 0, z2);
		(vPtr+(face*(NUM + 1) + n*4+1))->normal = XMFLOAT3(normalX2,0, normalZ2);
		//����
		float x3 = r*sin(DANGLE*(n ));
		float z3 = r*cos(DANGLE*(n ));
		float normalX3 = r*sin(DANGLE*(n +0));
		float normalZ3 = r*cos(DANGLE*(n +0));
		//(vPtr+(face*(NUM + 1) + n*4+2))->position = XMFLOAT3(x3, 0 + h / 2, z3);
		(vPtr + (face*(NUM + 1) + n * 4 + 2))->position = XMFLOAT3(x3,h, z3);
		(vPtr+(face*(NUM + 1) + n*4+2))->normal = XMFLOAT3(normalX3, 0, normalZ3);
		//�E��
		float x4 = r*sin(DANGLE*(n + 1));
		float z4 = r*cos(DANGLE*(n + 1));
		float normalX4 = r*sin(DANGLE*(n + 1));
		float normalZ4 = r*cos(DANGLE*(n + 1));
		//(vPtr+(face*(NUM + 1) + n * 4 + 3))->position = XMFLOAT3(x4, 0 + h / 2, z4);
		(vPtr + (face*(NUM + 1) + n * 4 + 3))->position = XMFLOAT3(x4, h, z4);
		(vPtr+(face*(NUM + 1) + n * 4 + 3))->normal = XMFLOAT3(normalX4, 0, normalZ4);

	}
	//indices��Ή�����
	//  8-----9    9-----10      13-----8
	//  |     |	   |     |		  |     |
	//  |     |	   |     |		  |     |
	//  1-----2    2-----3		  6-----1
	for (int n = 0; n < NUM; n++) {
		iPtr[NUM * 3*2 + 6*n+0] = (NUM + 1) * 2 +4* n + 2;    //����@�@�Q��
		iPtr[NUM * 3*2 + 6*n+1] = (NUM + 1) * 2 +4* n + 3;	 //�E��@�@�R��	
		iPtr[NUM * 3*2 + 6*n+2] = (NUM + 1) * 2 +4* n + 0;    //�����@�@�O��
		iPtr[NUM * 3*2 + 6*n+3] = (NUM + 1) * 2 +4* n + 3;	 //�E��@�@�R��
		iPtr[NUM * 3*2 + 6*n+4] = (NUM + 1) * 2 +4* n + 1;	 //�����@�@�O��
		iPtr[NUM * 3*2 + 6*n+5] = (NUM + 1) * 2 +4* n + 0;	 //�E���@�@�P��
	}
	VerticesNumber = (NUM + 1) * 2 + NUM * 4;
	indicesNumber = NUM * 3 * 2 + NUM * 6;
	VectexMemory = (vertex*)malloc(VerticesNumber*sizeof(vertex));
	memcpy(VectexMemory, vPtr, VerticesNumber*sizeof(vertex));
	BufferSet(Device, vPtr,iPtr, ((NUM + 1) * 2 + NUM * 4), (NUM * 3 * 2 + NUM * 6));	
}


//��ʌ���
geometric_polygon::geometric_polygon(ID3D11Device* Device) :
	geometric_primitive(Device)
{
	vertex vertices[4];     //�ʁ��S(���_�l��)
	u_int  indices[6];   //�ʁ��U�i�O�p�`��j
	face = 0;
	vertices[face * 4 + 0].position = DirectX::XMFLOAT3(-0.5f, +0.5f, -0.5f);
	vertices[face * 4 + 1].position = DirectX::XMFLOAT3(+0.5f, +0.5f, -0.5f);
	vertices[face * 4 + 2].position = DirectX::XMFLOAT3(-0.5f, -0.5f, -0.5f);
	vertices[face * 4 + 3].position = DirectX::XMFLOAT3(+0.5f, -0.5f, -0.5f);
	vertices[face * 4 + 0].normal = DirectX::XMFLOAT3(0, 0, -1);
	vertices[face * 4 + 1].normal = DirectX::XMFLOAT3(0, 0, -1);
	vertices[face * 4 + 2].normal = DirectX::XMFLOAT3(0, 0, -1);
	vertices[face * 4 + 3].normal = DirectX::XMFLOAT3(0, 0, -1);
	indices[face * 6 + 0] = face * 4 + 0;
	indices[face * 6 + 1] = face * 4 + 1;
	indices[face * 6 + 2] = face * 4 + 2;
	indices[face * 6 + 3] = face * 4 + 1;
	indices[face * 6 + 4] = face * 4 + 3;
	indices[face * 6 + 5] = face * 4 + 2;
	VerticesNumber = 4;
	indicesNumber = 6;
	VectexMemory = (vertex*)malloc(VerticesNumber*sizeof(vertex));
	memcpy(VectexMemory, vertices, VerticesNumber*sizeof(vertex));
	geometric_primitive::BufferSet(Device, vertices, indices, 4,6);
	
}

//�����
geometric_upside::geometric_upside(ID3D11Device* Device) :
	geometric_primitive(Device)
{
	
	vertex vertices[4];     //�ʁ��S(���_�l��)
	u_int  indices[6];   //�ʁ��U�i�O�p�`��j
	vertices[0].position = DirectX::XMFLOAT3((-0.5f), (+1.0f), (+0.5f));
	vertices[1].position = DirectX::XMFLOAT3((+0.5f), (+1.0f), (+0.5f));
	vertices[2].position = DirectX::XMFLOAT3((-0.5f), (+1.0f), (-0.5f));
	vertices[3].position = DirectX::XMFLOAT3((+0.5f), (+1.0f), (-0.5f));
	vertices[0].normal = DirectX::XMFLOAT3(+0.0f, +1.0f, +0.0f);
	vertices[1].normal = DirectX::XMFLOAT3(+0.0f, +1.0f, +0.0f);
	vertices[2].normal = DirectX::XMFLOAT3(+0.0f, +1.0f, +0.0f);
	vertices[3].normal = DirectX::XMFLOAT3(+0.0f, +1.0f, +0.0f);
	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;
	indices[3] = 1;
	indices[4] = 3;
	indices[5] = 2;
	VerticesNumber = 4;
	indicesNumber = 6;
	VectexMemory = new vertex[VerticesNumber];
	memcpy(VectexMemory, vertices, VerticesNumber*sizeof(vertex));
	geometric_primitive::BufferSet(Device, vertices, indices, VerticesNumber, indicesNumber);
	
}

//��ʂɃe�N�X�`���\���Ă���l�p�`
geometric_polygon::geometric_polygon(ID3D11Device* Device,int _type, wchar_t *_filename) :
	geometric_primitive(Device,_filename)
{
	RELEASE_IF(GDepthSS);
	if (_type == FRONT) {
		vertex vertices[4];   
		u_int  indices[6];   
		//DEPTH_STENCIL �[�x�e�X�g
		D3D11_DEPTH_STENCIL_DESC dsDesc;
		// Depth test parameters
		ZeroMemory(&dsDesc, sizeof(dsDesc));
		dsDesc.DepthEnable = FALSE;
		dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;        // D3D11_DEPTH_WRITE_MASK_ALL
		dsDesc.DepthFunc = D3D11_COMPARISON_LESS;//LESS
												 // Stencil test parameters
		dsDesc.StencilEnable = false;
		dsDesc.StencilReadMask = 0xFF;
		dsDesc.StencilWriteMask = 0xFF;
		Device->CreateDepthStencilState(&dsDesc, &GDepthSS);

		vertices[0].position = DirectX::XMFLOAT3(-0.5f, +1.0f, 0);
		vertices[1].position = DirectX::XMFLOAT3(+0.5f, +1.0f, 0);
		vertices[2].position = DirectX::XMFLOAT3(-0.5f, 0.0f, 0);
		vertices[3].position = DirectX::XMFLOAT3(+0.5f, 0.0f, 0);
		vertices[0].normal = DirectX::XMFLOAT3(0, 0, -1);
		vertices[1].normal = DirectX::XMFLOAT3(0, 0, -1);
		vertices[2].normal = DirectX::XMFLOAT3(0, 0, -1);
		vertices[3].normal = DirectX::XMFLOAT3(0, 0, -1);
		indices[0] = 0;
		indices[1] = 1;
		indices[2] = 2;
		indices[3] = 1;
		indices[4] = 3;
		indices[5] = 2;

		for (int i = 0; i < 4; i++) {
			vertices[i].color = DirectX::XMFLOAT4(1, 1, 1, 1);
		}
		VerticesNumber = 4;
		indicesNumber = 6;
		VectexMemory = new vertex[VerticesNumber];
		memcpy(VectexMemory, vertices, VerticesNumber*sizeof(vertex));
		geometric_primitive::BufferSet(Device, vertices, indices, 4, 6);
		
	}
	if (_type == UP) {
		//DEPTH_STENCIL �[�x�e�X�g
		D3D11_DEPTH_STENCIL_DESC dsDesc;
		// Depth test parameters
		ZeroMemory(&dsDesc, sizeof(dsDesc));
		dsDesc.DepthEnable = TRUE;
		dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;        // D3D11_DEPTH_WRITE_MASK_ALL
		dsDesc.DepthFunc = D3D11_COMPARISON_LESS;//LESS
												 // Stencil test parameters
		dsDesc.StencilEnable = false;
		dsDesc.StencilReadMask = 0xFF;
		dsDesc.StencilWriteMask = 0xFF;
		Device->CreateDepthStencilState(&dsDesc, &GDepthSS);
		vertex vertices[24];    
		u_int  indices[36];   
		face = 0;
		vertices[face * 4 + 0].position = DirectX::XMFLOAT3((-0.5f), (+1.0f), (+0.5f));
		vertices[face * 4 + 1].position = DirectX::XMFLOAT3((+0.5f), (+1.0f), (+0.5f));
		vertices[face * 4 + 2].position = DirectX::XMFLOAT3((-0.5f), (+1.0f), (-0.5f));
		vertices[face * 4 + 3].position = DirectX::XMFLOAT3((+0.5f), (+1.0f), (-0.5f));
		vertices[face * 4 + 0].normal = DirectX::XMFLOAT3(+0.0f, +1.0f, +0.0f);
		vertices[face * 4 + 1].normal = DirectX::XMFLOAT3(+0.0f, +1.0f, +0.0f);
		vertices[face * 4 + 2].normal = DirectX::XMFLOAT3(+0.0f, +1.0f, +0.0f);
		vertices[face * 4 + 3].normal = DirectX::XMFLOAT3(+0.0f, +1.0f, +0.0f);
		indices[face * 6 + 0] = face * 4 + 0;
		indices[face * 6 + 1] = face * 4 + 1;
		indices[face * 6 + 2] = face * 4 + 2;
		indices[face * 6 + 3] = face * 4 + 1;
		indices[face * 6 + 4] = face * 4 + 3;
		indices[face * 6 + 5] = face * 4 + 2;

		face += 1;
		vertices[face * 4 + 0].position = DirectX::XMFLOAT3(-0.5f, 0.0f, +0.5f);
		vertices[face * 4 + 1].position = DirectX::XMFLOAT3(+0.5f, 0.0f, +0.5f);
		vertices[face * 4 + 2].position = DirectX::XMFLOAT3(-0.5f, 0.0f, -0.5f);
		vertices[face * 4 + 3].position = DirectX::XMFLOAT3(+0.5f, 0.0f, -0.5f);
		vertices[face * 4 + 0].normal = DirectX::XMFLOAT3(+0.0f, -1.0f, +0.0f);
		vertices[face * 4 + 1].normal = DirectX::XMFLOAT3(+0.0f, -1.0f, +0.0f);
		vertices[face * 4 + 2].normal = DirectX::XMFLOAT3(+0.0f, -1.0f, +0.0f);
		vertices[face * 4 + 3].normal = DirectX::XMFLOAT3(+0.0f, -1.0f, +0.0f);
		indices[face * 6 + 0] = face * 4 + 0;
		indices[face * 6 + 1] = face * 4 + 2;
		indices[face * 6 + 2] = face * 4 + 1;
		indices[face * 6 + 3] = face * 4 + 1;
		indices[face * 6 + 4] = face * 4 + 2;
		indices[face * 6 + 5] = face * 4 + 3;
		////front-side
		////0-------1
		////|       |
		////|  +Z   |
		////|       |
		////2-------3
		face += 1;
		vertices[face * 4 + 0].position = DirectX::XMFLOAT3(-0.5f, 1.0f, -0.5f);
		vertices[face * 4 + 1].position = DirectX::XMFLOAT3(+0.5f, 1.0f, -0.5f);
		vertices[face * 4 + 2].position = DirectX::XMFLOAT3(-0.5f, 0.0f, -0.5f);
		vertices[face * 4 + 3].position = DirectX::XMFLOAT3(+0.5f, 0.0f, -0.5f);
		vertices[face * 4 + 0].normal = DirectX::XMFLOAT3(0, 0, -1);
		vertices[face * 4 + 1].normal = DirectX::XMFLOAT3(0, 0, -1);
		vertices[face * 4 + 2].normal = DirectX::XMFLOAT3(0, 0, -1);
		vertices[face * 4 + 3].normal = DirectX::XMFLOAT3(0, 0, -1);
		indices[face * 6 + 0] = face * 4 + 0;
		indices[face * 6 + 1] = face * 4 + 1;
		indices[face * 6 + 2] = face * 4 + 2;
		indices[face * 6 + 3] = face * 4 + 1;
		indices[face * 6 + 4] = face * 4 + 3;
		indices[face * 6 + 5] = face * 4 + 2;
		////behind-side
		////0-------1
		////|       |
		////|  +Z   |
		////|       |
		////2-------3
		face += 1;
		vertices[face * 4 + 0].position = DirectX::XMFLOAT3(-0.5f, 1.0f, +0.5f);
		vertices[face * 4 + 1].position = DirectX::XMFLOAT3(+0.5f, 1.0f, +0.5f);
		vertices[face * 4 + 2].position = DirectX::XMFLOAT3(-0.5f, 0.0f, +0.5f);
		vertices[face * 4 + 3].position = DirectX::XMFLOAT3(+0.5f, 0.0f, +0.5f);
		vertices[face * 4 + 0].normal = DirectX::XMFLOAT3(0, 0, +1);
		vertices[face * 4 + 1].normal = DirectX::XMFLOAT3(0, 0, +1);
		vertices[face * 4 + 2].normal = DirectX::XMFLOAT3(0, 0, +1);
		vertices[face * 4 + 3].normal = DirectX::XMFLOAT3(0, 0, +1);
		indices[face * 6 + 0] = face * 4 + 0;
		indices[face * 6 + 1] = face * 4 + 2;
		indices[face * 6 + 2] = face * 4 + 1;
		indices[face * 6 + 3] = face * 4 + 2;
		indices[face * 6 + 4] = face * 4 + 3;
		indices[face * 6 + 5] = face * 4 + 1;

		////left-side
		////0-------1
		////|       |
		////|  -X   |
		////|       |
		////2-------3
		face += 1;
		vertices[face * 4 + 0].position = DirectX::XMFLOAT3(-0.5f, 0.0f, +0.5f);
		vertices[face * 4 + 1].position = DirectX::XMFLOAT3(-0.5f, 1.0f, +0.5f);
		vertices[face * 4 + 2].position = DirectX::XMFLOAT3(-0.5f, 0.0f, -0.5f);
		vertices[face * 4 + 3].position = DirectX::XMFLOAT3(-0.5f, 1.0f, -0.5f);
		vertices[face * 4 + 0].normal = DirectX::XMFLOAT3(-1, 0, 0);
		vertices[face * 4 + 1].normal = DirectX::XMFLOAT3(-1, 0, 0);
		vertices[face * 4 + 2].normal = DirectX::XMFLOAT3(-1, 0, 0);
		vertices[face * 4 + 3].normal = DirectX::XMFLOAT3(-1, 0, 0);
		indices[face * 6 + 0] = face * 4 + 0;
		indices[face * 6 + 1] = face * 4 + 1;
		indices[face * 6 + 2] = face * 4 + 2;
		indices[face * 6 + 3] = face * 4 + 1;
		indices[face * 6 + 4] = face * 4 + 3;
		indices[face * 6 + 5] = face * 4 + 2;
		////right-side
		////0-------1
		////|       |
		////|  +X   |
		////|       |
		////2-------3
		face += 1;
		vertices[face * 4 + 0].position = DirectX::XMFLOAT3(+0.5f, 0.0f, +0.5f);
		vertices[face * 4 + 1].position = DirectX::XMFLOAT3(+0.5f, 1.0f, +0.5f);
		vertices[face * 4 + 2].position = DirectX::XMFLOAT3(+0.5f, 0.0f, -0.5f);
		vertices[face * 4 + 3].position = DirectX::XMFLOAT3(+0.5f, 1.0f, -0.5f);
		vertices[face * 4 + 0].normal = DirectX::XMFLOAT3(+1, 0, 0);
		vertices[face * 4 + 1].normal = DirectX::XMFLOAT3(+1, 0, 0);
		vertices[face * 4 + 2].normal = DirectX::XMFLOAT3(+1, 0, 0);
		vertices[face * 4 + 3].normal = DirectX::XMFLOAT3(+1, 0, 0);
		indices[face * 6 + 0] = face * 4 + 0;
		indices[face * 6 + 1] = face * 4 + 2;
		indices[face * 6 + 2] = face * 4 + 1;
		indices[face * 6 + 3] = face * 4 + 1;
		indices[face * 6 + 4] = face * 4 + 2;
		indices[face * 6 + 5] = face * 4 + 3;
		
		for (int i = 0; i < 4; i++) {
			vertices[i].color = DirectX::XMFLOAT4(1, 1, 1, 1);
		}

		VerticesNumber = 24;
		indicesNumber = 36;
		VectexMemory = new vertex[VerticesNumber];
		memcpy(VectexMemory, vertices, VerticesNumber*sizeof(vertex));
		geometric_primitive::BufferSet(Device, vertices, indices, 24, 36);
		
	}
	
}



DirectX::XMFLOAT3 geometric_primitive::GetVertexPos(int Num) {
	
	return VectexMemory[Num].position;
}

void geometric_primitive::SetVectexPos(vertex *vectexs) {
	VectexMemory = vectexs;
}