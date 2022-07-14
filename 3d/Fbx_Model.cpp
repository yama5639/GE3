# include "Fbx_Model.h"

Fbx_Model::~Fbx_Model() {
	fbxScene->Destroy();
}

void Fbx_Model::CreateBuffers(ID3D12Device* device)
{
	HRESULT result;
	UINT sizeVB = static_cast<UINT>(sizeof(VertexPosNormalUvSkin) * vertices.size());
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeVB),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));
	VertexPosNormalUvSkin* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	if (SUCCEEDED(result)) {
		std::copy(vertices.begin(), vertices.end(), vertMap);
		vertBuff->Unmap(0, nullptr);
		vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
		vbView.SizeInBytes = sizeVB;
		vbView.StrideInBytes = sizeof(vertices[0]);

		UINT sizeIB = static_cast<UINT>(sizeof(unsigned short) * indices.size());
		
		result = device->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer(sizeIB),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&indexBuff));

		unsigned short* indexMap = nullptr;
		result = indexBuff->Map(0, nullptr, (void**)&indexMap);
		if (SUCCEEDED(result)) {
			std::copy(indices.begin(), indices.end(), indexMap);
			indexBuff->Unmap(0, nullptr);
		}

		ibView.BufferLocation = indexBuff->GetGPUVirtualAddress();
		ibView.Format = DXGI_FORMAT_R16_UINT;
		ibView.SizeInBytes = sizeIB;

		const DirectX::Image* img = scratchImg.GetImage(0, 0, 0);	//���f�[�^���o
		assert(img);

		CD3DX12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
			metadata.format,
			metadata.width,
			(UINT)metadata.height,
			(UINT16)metadata.arraySize,
			(UINT16)metadata.mipLevels);

		//�e�N�X�`���p�o�b�t�@�̐���
		result = device->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0),
			D3D12_HEAP_FLAG_NONE,
			&texresDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&texBuff));

		result = texBuff->WriteToSubresource(
			0,
			nullptr,
			img->pixels,
			(UINT)img->rowPitch,
			(UINT)img->slicePitch
		);
		//SRV�f�X�N���v�^�q�[�v�𐶐�
		D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
		descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;	//�V�F�[�_���猩����悤��
		descHeapDesc.NumDescriptors = 1;	//�e�N�X�`������
		result = device->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&descHeapSRV));

		//�V�F�[�_���\�[�X(SRV)�쐬
		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};	//�ݒ�\����
		D3D12_RESOURCE_DESC resDesc = texBuff->GetDesc();

		srvDesc.Format = resDesc.Format;
		srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;	//2D�e�N�X�`��
		srvDesc.Texture2D.MipLevels = 1;

		device->CreateShaderResourceView(texBuff.Get(),	//�r���[�Ɗ֘A�t����o�b�t�@
			&srvDesc,	//�e�N�X�`���ݒ���
			descHeapSRV->GetCPUDescriptorHandleForHeapStart()	//�q�[�v�̐擪�A�h���X
		);
	}
}

void Fbx_Model::Draw(ID3D12GraphicsCommandList* cmdList)
{
	//���_�o�b�t�@���Z�b�g�iVBV�j
	cmdList->IASetVertexBuffers(0, 1, &vbView);
	//�C���f�b�N�X�o�b�t�@���Z�b�g�iIBV�j
	cmdList->IASetIndexBuffer(&ibView);

	//�f�X�N���v�^�q�[�v�̃Z�b�g
	ID3D12DescriptorHeap* ppHeaps[] = { descHeapSRV.Get() };
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	//�V�F�[�_���\�[�X�r���[���Z�b�g
	cmdList->SetGraphicsRootDescriptorTable(1, descHeapSRV->GetGPUDescriptorHandleForHeapStart());

	//�`��R�}���h
	cmdList->DrawIndexedInstanced((UINT)indices.size(), 1, 0, 0, 0);
}