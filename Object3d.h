#pragma once

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include<string>
#include"Model.h"

/// 3D�I�u�W�F�N�g
class Object3d
{
private: // �G�C���A�X
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	//���f��
	Model* model = nullptr;
public: // �T�u�N���X
	// �萔�o�b�t�@�p�f�[�^�\����
	struct ConstBufferDataB0
	{
		//XMFLOAT4 color;	// �F (RGBA)
		XMMATRIX mat;	// �R�c�ϊ��s��
	};
private:
	static const int division = 50;
	static const float radius;
	static const float prizmHeight;
	static const int planeCount = division * 2 + division * 2;
	static const int vertexCount = planeCount * 3;
public: // �ÓI�����o�֐�
	static bool StaticInitialize(ID3D12Device* device, int window_width, int window_height);

	/// �`��O����
	static void PreDraw(ID3D12GraphicsCommandList* cmdList);

	/// �`��㏈��
	static void PostDraw();

	/// 3D�I�u�W�F�N�g��
	static Object3d* Create();

	/// ���_���W�̎擾
	static const XMFLOAT3& GetEye() { return eye; }

	/// ���_���W�̐ݒ�
	static void SetEye(XMFLOAT3 eye);

	/// �����_���W�̎擾
	static const XMFLOAT3& GetTarget() { return target; }

	/// �����_���W�̐ݒ�
	static void SetTarget(XMFLOAT3 target);

	/// �x�N�g���ɂ��ړ�
	static void CameraMoveVector(XMFLOAT3 move);

	//
	void SetModel(Model* model) { this->model = model; }
	//static void LoadMaterial(const std::string& directoryPath, const std::string& filename);

private: // �ÓI�����o�ϐ�
	// �f�o�C�X
	static ID3D12Device* device;
	// �R�}���h���X�g
	static ID3D12GraphicsCommandList* cmdList;
	// ���[�g�V�O�l�`��
	static ComPtr<ID3D12RootSignature> rootsignature;
	// �p�C�v���C���X�e�[�g�I�u�W�F�N�g
	static ComPtr<ID3D12PipelineState> pipelinestate;
	// �r���[�s��
	static XMMATRIX matView;
	// �ˉe�s��
	static XMMATRIX matProjection;
	// ���_���W
	static XMFLOAT3 eye;
	// �����_���W
	static XMFLOAT3 target;
	// ������x�N�g��
	static XMFLOAT3 up;


private:// �ÓI�����o�֐�
	/// �J����������
	static void InitializeCamera(int window_width, int window_height);

	/// �O���t�B�b�N�p�C�v���C������
	static bool InitializeGraphicsPipeline();

	/// �e�N�X�`���ǂݍ���
	static bool LoadTexture(const std::string& directoryPath, const std::string& filename);

	static void CreateModel();

	/// �r���[�s����X�V
	static void UpdateViewMatrix();

public: // �����o�֐�
	//������
	bool Initialize();
	/// ���t���[������
	void Update();
	/// �`��
	void Draw();

	/// ���W�̎擾
	const XMFLOAT3& GetPosition() { return position; }

	/// ���W�̐ݒ�
	void SetPosition(XMFLOAT3 position) { this->position = position; }

private: // �����o�ϐ�
	ComPtr<ID3D12Resource> constBuffB0; // �萔�o�b�t�@
	// �F
	XMFLOAT4 color = { 1,1,1,1 };
	// ���[�J���X�P�[��
	XMFLOAT3 scale = { 1.0,1.0,1.0 };
	// X,Y,Z�����̃��[�J����]�p
	XMFLOAT3 rotation = { 0,0,0 };
	// ���[�J�����W
	XMFLOAT3 position = { 0,0,1 };
	// ���[�J�����[���h�ϊ��s��
	XMMATRIX matWorld;
	// �e�I�u�W�F�N�g
	Object3d* parent = nullptr;
};