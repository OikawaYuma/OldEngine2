#include "Skybox.h"
#include "Mesh.h"
#include "DirectXCommon.h"
#include <VertexData.h>
#include <Skeleton.h>
#include <PSOModel.h>
#include <SRVManager.h>


void Skybox::Init(const Material& material)
{
	
	std::vector<Vector4> positions; // 位置
	std::vector<Vector3> normals; // 法線
	std::vector<Vector2> texcoords; // テクスチャ座標
	
	// 右面。描画インデックスは[0,1,2][2,1,3]で内側を向く
	modelData_[0].position = {1.0f, 1.0f, 1.0f, 1.0f};
	modelData_[1].position = {1.0f, 1.0f, -1.0f, 1.0f};
	modelData_[2].position = {1.0f, -1.0f, 1.0f, 1.0f};
	modelData_[3].position = {1.0f, -1.0f, -1.0f, 1.0f};
			 
	modelData_[4].position = { -1.0f, 1.0f, -1.0f, 1.0f };
	modelData_[5].position = { -1.0f, 1.0f, 1.0f, 1.0f };
	modelData_[6].position = { -1.0f, -1.0f, -1.0f, 1.0f };
	modelData_[7].position = { -1.0f, -1.0f, 1.0f, 1.0f };
			 
	modelData_[8].position = { -1.0f, 1.0f, 1.0f, 1.0f };
	modelData_[9].position = { 1.0f, 1.0f, 1.0f, 1.0f };
	modelData_[10].position = { -1.0f, -1.0f, 1.0f, 1.0f };
	modelData_[11].position = { 1.0f, -1.0f, 1.0f, 1.0f };
	
	for (int i = 0; i < 12;++i) {
		modelData_[i].normal = {};
		modelData_[i].texcorrd = {};
	}
	
	// 頂点リソースを作る
	vertexResource_ = Mesh::CreateBufferResource(DirectXCommon::GetInstance()->GetDevice(), sizeof(VertexData) * 12);

	// 頂点バッファビューを作成する
	vertexBufferView_.BufferLocation = vertexResource_->GetGPUVirtualAddress(); // リソースの先頭のアドレスから使う
	vertexBufferView_.SizeInBytes = UINT(sizeof(VertexData) * 12); // 使用するリソースのサイズは頂点のサイズ
	vertexBufferView_.StrideInBytes = sizeof(VertexData); // 1頂点当たりのサイズ

	// 頂点リソースにデータを書き込む
	vertexData_ = modelData_;
	vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));

	//std::memcpy(vertexData_, modelData.vertices.data(), sizeof(VertexData) * modelData.vertices.size());
	vertexData_ = modelData_;
	// 実際に頂点リソースを作る
	materialResource = Mesh::CreateBufferResource(DirectXCommon::GetInstance()->GetDevice(), sizeof(Material));

	/*materialBufferView = CreateBufferView();;*/
	// 頂点リソースにデータを書き込む
	materialData_ = nullptr;
	// 書き込むためのアドレスを取得
	materialResource->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));
	// 色のデータを変数から読み込み
	materialData_->color = material.color;
	materialData_->enableLighting = material.enableLighting;
	materialData_->uvTransform = MakeIdentity4x4();
	materialData_->shininess = material.shininess;

	transformUv = {
		{1.0f,1.0f,1.0f},
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,0.0f}
	};



	directionalLightData = nullptr;
	directionalLightResource = Mesh::CreateBufferResource(DirectXCommon::GetInstance()->GetDevice(), sizeof(DirectionalLight));
	// 書き込むためのアドレスを取得
	directionalLightResource->Map(0, nullptr, reinterpret_cast<void**>(&directionalLightData));

	// デフォルト値はとりあえず以下のようにしておく
	directionalLightData->color = { 1.0f,1.0f,1.0f,1.0f };
	directionalLightData->direction = { 0.0f,-1.0f,0.0f };
	directionalLightData->intensity = 1.0f;

	indexResource_ = Mesh::CreateBufferResource(DirectXCommon::GetInstance()->GetDevice().Get(), sizeof(uint32_t) * 12);
	indexBufferView_.BufferLocation = indexResource_->GetGPUVirtualAddress();
	indexBufferView_.SizeInBytes = sizeof(uint32_t) * static_cast<uint32_t>(12);
	indexBufferView_.Format = DXGI_FORMAT_R32_UINT;

	uint32_t* indexDataSprite = nullptr;
	indexResource_->Map(0, nullptr, reinterpret_cast<void**>(&indexDataSprite));

	std::memcpy(indexDataSprite, modelData_, sizeof(uint32_t) * 12);
	//worldTransform_.Initialize();
}

void Skybox::Draw(uint32_t texture, const Material& material, const DirectionalLight& dire)
{

	PSO* pso_ = PSO::GatInstance();
	

	TextureManager* textureManager_ = TextureManager::GetInstance();
	// 色のデータを変数から読み込み
	materialData_->color = material.color;
	materialData_->enableLighting = material.enableLighting;
	materialData_->shininess = 0.5f;
	directionalLightData->direction = dire.direction;
	//directionalLightData->direction =  Normalize(directionalLightData->direction);
	//directXCommon_->GetCommandList()->SetGraphicsRootSignature(pso_->GetProperty().rootSignature.Get());
	//directXCommon_->GetCommandList()->SetPipelineState(pso_->GetProperty().graphicsPipelineState.Get());    //PSOを設定
	//directXCommon_->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView_);    //VBVを設定
	DirectXCommon::GetInstance()->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView_);    //VBVを設定
	DirectXCommon::GetInstance()->GetCommandList()->IASetIndexBuffer(&indexBufferView_);    //VBVを設定
	//形状を設定。PSOに設定しているものとはまた別。同じものを設定すると考えておけば良い
	//directXCommon_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	// マテリアルCBufferの場所を設定
	DirectXCommon::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource->GetGPUVirtualAddress());

	// SRV のDescriptorTableの先頭を設定。2はrootParameter[2]である。
	DirectXCommon::GetInstance()->GetCommandList()->SetGraphicsRootDescriptorTable(2, SRVManager::GetGPUDescriptorHandle(texture));

	DirectXCommon::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(3, directionalLightResource->GetGPUVirtualAddress());
	//directXCommon_->GetCommandList()->SetGraphicsRootDescriptorTable(5, skinCluster_.paletteSrvHandle.second);
	DirectXCommon::GetInstance()->GetCommandList()->DrawIndexedInstanced(12, 1, 0, 0, 0);
	//directXCommon_->GetCommandList()->DrawInstanced(UINT(modelData_.vertices.size()), 1, 0, 0);
}
