#include "skinned_mesh.h"
#include "fbxsdk.h"
using namespace fbxsdk;
using namespace DirectX;
#include <vector>
#include <functional>
#include "ResourceManger.h"
#include "misc.h"

//Unit 20
//各ポーンの頂点に対する影響度を確認する
struct bone_influence {
	int index;    //index of bone
	float weight; //weight of bone
};
typedef std::vector<bone_influence> bone_influences_per_control_point;

void fetch_bone_influences(const FbxMesh *fbx_mesh,std::vector<bone_influences_per_control_point> &influences) {
	const int number_of_control_points = fbx_mesh->GetControlPointsCount();
	influences.resize(number_of_control_points);

	const int number_of_deformers = fbx_mesh->GetDeformerCount(FbxDeformer::eSkin);
	for (int index_of_deformer=0; index_of_deformer < number_of_deformers; ++index_of_deformer) {
		FbxSkin* skin = static_cast<FbxSkin*>(fbx_mesh->GetDeformer(index_of_deformer, FbxDeformer::eSkin));

		const int number_of_clusters = skin->GetClusterCount();
		for (int index_of_cluster=0; index_of_cluster < number_of_clusters; ++index_of_cluster) {
			FbxCluster* cluster = skin->GetCluster(index_of_cluster);

			const int number_of_control_point_indices = cluster->GetControlPointIndicesCount();
			const int *array_of_control_point_indices = cluster->GetControlPointIndices();
			const double *array_of_control_point_weights = cluster->GetControlPointWeights();

			for (int i = 0; i < number_of_control_point_indices; ++i) {
				bone_influences_per_control_point &influences_per_control_point = influences.at(array_of_control_point_indices[i]);
				bone_influence influence;
				influence.index = index_of_cluster;
				influence.weight = static_cast<float>(array_of_control_point_weights[i]);
				influences_per_control_point.push_back(influence);
			}
		}
	}
}

skinned_mesh::skinned_mesh(ID3D11Device*Device, const char *fbx_filename):geometric_primitive(Device) {
	HRESULT hr = S_OK;
	zahyouFlag = false;
	
	//VertexShader    InputLayout
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0,   D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",     0, DXGI_FORMAT_R32G32B32_FLOAT, 0,   D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",   0, DXGI_FORMAT_R32G32_FLOAT,    0,   D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "WEIGHTS",    0, DXGI_FORMAT_R32G32B32A32_FLOAT,0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BONES",      0, DXGI_FORMAT_R32G32B32A32_SINT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	ID3DBlob* pPSBlob = NULL;
	UINT numElements = ARRAYSIZE(layout);
	ResourceManager::LoadCreateVertexShaders(Device, "skinned_mesh_vs.cso", layout, numElements, &GVertexShader, &GInptLayout);
	ResourceManager::LoadCreatePixelShader(Device, &GPixelShader, "skinned_mesh_ps.cso");


	//RASTERIZER   線描画
	D3D11_RASTERIZER_DESC rsDesc;
	ZeroMemory(&rsDesc, sizeof(D3D11_RASTERIZER_DESC));
	rsDesc.FillMode = D3D11_FILL_WIREFRAME;
	rsDesc.CullMode = D3D11_CULL_BACK;
	rsDesc.FrontCounterClockwise = FALSE;   //仮時計回り
	rsDesc.DepthClipEnable = FALSE;
	hr = Device->CreateRasterizerState(&rsDesc, &RSWireframe);
	if (FAILED(hr)) return;


	//RASTERIZER   塗りつぶし描画
	ZeroMemory(&rsDesc, sizeof(D3D11_RASTERIZER_DESC));
	rsDesc.FillMode = D3D11_FILL_SOLID;
	rsDesc.CullMode = D3D11_CULL_BACK;
	rsDesc.FrontCounterClockwise = TRUE;
	rsDesc.DepthClipEnable = TRUE;
	hr = Device->CreateRasterizerState(&rsDesc, &RSSolid);
	if (FAILED(hr)) return;

	//DEPTH_STENCIL 深度テスト
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
	if (FAILED(hr)) return;
	



	//FBX
	//Create the FBX SDK manager
	FbxManager* manager = FbxManager::Create();

    //Create an IOSettings object. IOSROOT is defined in Fbxiosettingspath.h
	manager->SetIOSettings(FbxIOSettings::Create(manager, IOSROOT));

	//Creater an importer
	FbxImporter* importer = FbxImporter::Create(manager, "");

	//Initialize the importer
	bool import_status = false;
	import_status = importer->Initialize(fbx_filename, -1, manager->GetIOSettings());
	//assert(import_status, importer->GetStatus().GetErrorString());
	//ASSERT_EXPR_A(import_status, importer->GetStatus().GetErrorString());

    //Create a new scene so it can be populated by the imported file
	FbxScene* scene = FbxScene::Create(manager, "");

	//import the contents of the file into the scene
	import_status = importer->Import(scene);
	//assert(import_status, importer->GetStatus().GetErrorString());

	//Convert mesh ,NURBS and patch into triangle mesh
	fbxsdk::FbxGeometryConverter geometry_converter(manager);
	geometry_converter.Triangulate(scene,/*replace*/true);

	//Fetch node attributes and materials under this node recursively. Currently only mesh
	std::vector<FbxNode*> fetched_meshes;
	//class
	std::function<void(FbxNode*)> traverse = [&](FbxNode* node) 
	{
		if (node) {
			FbxNodeAttribute *fbx_node_attribute = node->GetNodeAttribute();
			if (fbx_node_attribute) {
				switch (fbx_node_attribute->GetAttributeType())
				{
				case FbxNodeAttribute::eMesh:
					fetched_meshes.push_back(node);
					break;
				}
			}
			for (int i = 0; i < node->GetChildCount(); i++) {
				traverse(node->GetChild(i));
			}
		}

	};
	traverse(scene->GetRootNode());


	//FbxMesh* fbx_mesh = fetched_meshes.at(0)->GetMesh();  //Currently only one mesh
	//材質の情報を取る
	//Fetch material properties.
	//GetMaterialCount
	//static bool Flag = false;
	//const int number_of_materials = fbx_mesh->GetNode()->GetMaterialCount();
	//if (number_of_materials == 0) {
	//	subsets.resize(1);//Unit18
	//}
	//else subsets.resize(number_of_materials);  //Unit18

	meshes.resize(fetched_meshes.size());
	////Unit19
	for (size_t i = 0; i < fetched_meshes.size(); i++) {
		//Unit19
		FbxMesh*fbx_mesh = fetched_meshes.at(i)->GetMesh();
		mesh &mesh = meshes.at(i);

		//Unit20
		std::vector<bone_influences_per_control_point> bone_influences;
		fetch_bone_influences(fbx_mesh, bone_influences);
		//Unit18
		static bool Flag = false;
		const int number_of_materials = fbx_mesh->GetNode()->GetMaterialCount();
		if (number_of_materials == 0) {
			mesh.subsets.resize(1);
		}
		else mesh.subsets.resize(number_of_materials);  //Unit18

		for (int index_of_material = 0; index_of_material < number_of_materials; ++index_of_material)
		{
			////Unit18
			//subset &subset = subsets.at(index_of_material);
			////Unit19
			subset &subset =mesh.subsets.at(index_of_material);
			////Unit17
			const FbxSurfaceMaterial *surface_material = fbx_mesh->GetNode()->GetMaterial(index_of_material);
			const FbxProperty property = surface_material->FindProperty(FbxSurfaceMaterial::sDiffuse);
			const FbxProperty factor = surface_material->FindProperty(FbxSurfaceMaterial::sDiffuseFactor);
			if (property.IsValid() && factor.IsValid()) {
				FbxDouble3 color = property.Get<FbxDouble3>();
				double f = factor.Get<FbxDouble>();
				subset.diffuse.color.x = static_cast<float>(color[0] * f);
				subset.diffuse.color.y = static_cast<float>(color[1] * f);
				subset.diffuse.color.z = static_cast<float>(color[2] * f);
				subset.diffuse.color.w = 1.0f;
			}
			if (property.IsValid()) {
				//layeredTextureCount 
				const int number_of_textures = property.GetSrcObjectCount<FbxFileTexture>();
				if (number_of_textures)
				{
					const FbxFileTexture *file_texture = property.GetSrcObject<FbxFileTexture>();
					if (file_texture)
					{
						const char *filename = file_texture->GetRelativeFileName();
						//Create"diffuse.shader_resource_view"from"filename"
						wchar_t wfilename[256];
						size_t wsize;
						mbstowcs_s(&wsize, wfilename, filename, 256);
						ResourceManager::LoadShaderResourceView(Device, wfilename, &subset.diffuse.shader_resource_view, &GDesc);
					}
				}
				else Flag = true;
			}
		}

		for (subset& subset : mesh.subsets)
		{
			//textureなかったら偽者を作る
			if (!subset.diffuse.shader_resource_view)
				ResourceManager::MakeDummyShaderResourceView(Device, &subset.diffuse.shader_resource_view);
		}



		//Count the polygon count of each material
		if (number_of_materials > 0) {
			//Count the faces of each material
			const int number_of_polygons = fbx_mesh->GetPolygonCount();
			for (int index_of_polygon = 0; index_of_polygon < number_of_polygons; ++index_of_polygon)
			{
				const u_int material_index = fbx_mesh->GetElementMaterial()->GetIndexArray().GetAt(index_of_polygon);
				mesh.subsets.at(material_index).index_count += 3;
			}

			//Record the offset (how many vertex)
			int offset = 0;
			//for全部のsubsets
			for (subset &s : mesh.subsets) {	
				s.index_start = offset;
				offset += s.index_count;
				//This will be used as counter in the following procedures,reset to zero
				s.index_count = 0;
			}
		}

		
		//Fetch mesh data
		std::vector<vertex> vertices;   //Vertex buffer
		std::vector<u_int>  indices;

		u_int vertex_count = 0;
		//UV座標を取る
		FbxStringList uv_names;
		fbx_mesh->GetUVSetNames(uv_names);
		const FbxVector4 *array_of_control_points = fbx_mesh->GetControlPoints();
		const int number_of_polygons = fbx_mesh->GetPolygonCount();
		indices.resize(number_of_polygons * 3);    //Unit18
		for (int index_of_polygon = 0; index_of_polygon < number_of_polygons; index_of_polygon++)
		{
			//Unit18
			//The material for current face.
			int index_of_material = 0;
			if (number_of_materials > 0) {
				index_of_material = fbx_mesh->GetElementMaterial()->GetIndexArray().GetAt(index_of_polygon);
			}

			//Where should i save the vertex attribute index,according to the material
			subset &subset = mesh.subsets.at(index_of_material);
			const int index_offset = subset.index_start + subset.index_count;

			//Unit17
			for (int index_of_vertex = 0; index_of_vertex < 3; index_of_vertex++) {
				vertex vertex;
				const int index_of_control_point = fbx_mesh->GetPolygonVertex(index_of_polygon, index_of_vertex);
				vertex.position.x = static_cast<float>(array_of_control_points[index_of_control_point][0]);
				vertex.position.y = static_cast<float>(array_of_control_points[index_of_control_point][1]);
				vertex.position.z = static_cast<float>(array_of_control_points[index_of_control_point][2]);
			
				//Unit20
				if (bone_influences.at(index_of_control_point).size() != 0) {
					for (unsigned i = 0; i < bone_influences.at(index_of_control_point).size(); ++i)
					{
						//if (bone_influences.at(index_of_control_point).at(i).index) {
						vertex.bone_indices[i] = bone_influences.at(index_of_control_point).at(i).index;
						//}

						//	if (bone_influences.at(index_of_control_point).at(i).weight) {
						vertex.bone_weights[i] = bone_influences.at(index_of_control_point).at(i).weight;
						//	}

					}
				}
		
				if (fbx_mesh->GetElementNormal() > 0) {
					FbxVector4 normal;
					fbx_mesh->GetPolygonVertexNormal(index_of_polygon, index_of_vertex, normal);
					vertex.normal.x = static_cast<float>(normal[0]);
					vertex.normal.y = static_cast<float>(normal[1]);
					vertex.normal.z = static_cast<float>(normal[2]);
				}

				if (fbx_mesh->GetElementMaterial() > 0) {
					FbxVector2 uv;
					bool unmapped_uv;
					fbx_mesh->GetPolygonVertexUV(index_of_polygon, index_of_vertex, uv_names[0], uv, unmapped_uv);
					vertex.texcoord.x = static_cast<float>(uv[0]);
					vertex.texcoord.y = 1.0f - static_cast<float>(uv[1]);
				}

				vertices.push_back(vertex);
				//indices.push_back(vertex_count);   Unit18はこいつをコメントする
				indices.at(index_offset + index_of_vertex) = static_cast<u_int>(vertex_count);
				vertex_count += 1;


				//Unit19  姿勢行列を取得する
				FbxAMatrix global_transform = fbx_mesh->GetNode()->EvaluateGlobalTransform(0);
				for (int row = 0; row < 4; ++row) {
					for (int column = 0; column < 4; ++column) {
						mesh.global_transform(row, column) = static_cast<float>(global_transform[row][column]);
					}
				}

			}
			subset.index_count += 3;
		}
		BufferSet(Device, vertices.data(), indices.data(), vertices.size(), indices.size(), mesh.vertex_buffer.GetAddressOf() ,mesh.index_buffer.GetAddressOf());
	}
	
	manager->Destroy();

	//サンプラーステート設定
	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof(samplerDesc));
	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0;
	samplerDesc.MaxAnisotropy = 16;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	hr = Device->CreateSamplerState(&samplerDesc, &GState);
	if (FAILED(hr)) return;

}

void skinned_mesh::BufferSet(ID3D11Device* Device,vertex *vertices, u_int *indices, int VerticesNumber, int indicesNumber, ID3D11Buffer **vertex_buffer, ID3D11Buffer **index_buffer) {
	HRESULT hr = S_OK;
	//Buffer  
	//頂点バッファオブジェクト生成
	//VertexBuffer
	D3D11_BUFFER_DESC bd;
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&bd, sizeof(bd));
	ZeroMemory(&InitData, sizeof(InitData));

	//bd.ByteWidth = sizeof(vertices);
	bd.ByteWidth = VerticesNumber*sizeof(vertex);
	bd.Usage = D3D11_USAGE_IMMUTABLE;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	bd.StructureByteStride = 0;

	InitData.pSysMem = vertices;
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;
	hr = Device->CreateBuffer(&bd, &InitData, vertex_buffer);
	if (FAILED(hr)) {
		return;
	}

	//インデックスバッファ
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
	hr = Device->CreateBuffer(&bd, &InitData, index_buffer);
	if (FAILED(hr)) {
		return;
	}
	
	//定数バッファ
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
	if (FAILED(hr)) {
		return;
	}
}

skinned_mesh::~skinned_mesh() {
}

void skinned_mesh::update() {
	
}


void skinned_mesh::render(ID3D11DeviceContext*Context,
	const DirectX::XMFLOAT4X4 & world_view_projection,
	const DirectX::XMFLOAT4X4 & world,
	const DirectX::XMFLOAT4   & material_color,
	const DirectX::XMFLOAT4 &light_direction,bool line)

{
	//Set vertex buffer
	UINT stride = sizeof(vertex);
	UINT offset = 0;
	


	//Set primitive topology
	Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//Set the input layout
	Context->IASetInputLayout(GInptLayout);
	//Stateの設定
	if (line)Context->RSSetState(RSWireframe);
	else Context->RSSetState(RSSolid);
	//Render a triangle
	Context->VSSetShader(GVertexShader, NULL, 0);
	Context->PSSetShader(GPixelShader, NULL, 0);


	//深度テストを設定する
	Context->OMSetDepthStencilState(GDepthSS, 0);
	//UNIT17
	
	Context->PSSetSamplers(0, 1, &GState);
	
	for (mesh &mesh : meshes) {
		cbuffer cb;
		Context->IASetVertexBuffers(0, 1, mesh.vertex_buffer.GetAddressOf(), &stride, &offset);
		//
		Context->IASetIndexBuffer(mesh.index_buffer.Get(), DXGI_FORMAT_R32_UINT, offset);

		//boneの回転
		static float angle = 0;
		XMStoreFloat4x4(&cb.bone_transforms[0], XMMatrixIdentity());
		XMStoreFloat4x4(&cb.bone_transforms[1], XMMatrixRotationRollPitchYaw(0, 0, angle*0.01745f));
		XMStoreFloat4x4(&cb.bone_transforms[2], XMMatrixIdentity());
		angle += 0.1f;
		//座標系の変換
		if (zahyouFlag) {
			XMStoreFloat4x4(&cb.world_view_projection, XMLoadFloat4x4(&mesh.global_transform)*XMLoadFloat4x4(&coordinate_conversion)*XMLoadFloat4x4(&world_view_projection));
			XMStoreFloat4x4(&cb.world, XMLoadFloat4x4(&mesh.global_transform)*XMLoadFloat4x4(&coordinate_conversion)*XMLoadFloat4x4(&world));
		}

		//Unit19
		else {
			XMStoreFloat4x4(&cb.world_view_projection, XMLoadFloat4x4(&mesh.global_transform)*XMLoadFloat4x4(&world_view_projection));
			XMStoreFloat4x4(&cb.world, XMLoadFloat4x4(&mesh.global_transform)*XMLoadFloat4x4(&world));
		}

		for (subset &s : mesh.subsets) {
			//cb.world_view_projection = world_view_projection;
			//cb.world = world;
			//cb.material_color = material_color;
			//Unit 21
			
		
			cb.material_color.x = material_color.x*s.diffuse.color.x;
			cb.material_color.y = material_color.y*s.diffuse.color.y;
			cb.material_color.z = material_color.z*s.diffuse.color.z;
			cb.material_color.w = material_color.w*s.diffuse.color.w;
			cb.light_direction = light_direction;
			Context->PSSetShaderResources(0, 1, &s.diffuse.shader_resource_view);
			Context->UpdateSubresource(CBuffer, 0, NULL, &cb, 0, 0);
			Context->VSSetConstantBuffers(0, 1, &CBuffer);

			//index付き描画
			Context->DrawIndexed(s.index_count, s.index_start, 0);
		}
	}
	
	//Context->UpdateSubresource(CBuffer, 0, NULL, &cb, 0, 0);
	//Context->VSSetConstantBuffers(0, 1, &CBuffer);
}
