#include "DirectXCommon.h"

#include <cassert>


#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
void DirectXCommon::Initialize(WinApp* win) {

    assert(win);
    this->win = win;

    InitializeDevice();
    InitializeCommand();
    InitializeSwapchain();
    InitializeRenderTargetView();
    InitializeDepthBuffer();
    InitializeFence();

}

void DirectXCommon::PreDraw() {
    // �o�b�N�o�b�t�@�̔ԍ����擾�i2�Ȃ̂�0�Ԃ�1�ԁj
    UINT bbIndex = swapchain->GetCurrentBackBufferIndex();
    // �P�D���\�[�X�o���A�ŏ������݉\�ɕύX
    cmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(backBuffers[bbIndex].Get(),
        D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));
    // �Q�D�`���w��
    // �����_�[�^�[�Q�b�g�r���[�p�f�B�X�N���v�^�q�[�v�̃n���h�����擾
    CD3DX12_CPU_DESCRIPTOR_HANDLE rtvH = CD3DX12_CPU_DESCRIPTOR_HANDLE(rtvHeaps->GetCPUDescriptorHandleForHeapStart(), bbIndex, dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV));
    // �[�x�X�e���V���r���[�p�f�X�N���v�^�q�[�v�̃n���h�����擾
    CD3DX12_CPU_DESCRIPTOR_HANDLE dsvH = CD3DX12_CPU_DESCRIPTOR_HANDLE(dsvHeap->GetCPUDescriptorHandleForHeapStart());
    cmdList->OMSetRenderTargets(1, &rtvH, false, &dsvH);
    // �R�D��ʃN���A           R     G     B    A
    float clearColor[] = { 0.1f,0.25f, 0.5f,0.0f }; // ���ۂ��F
    cmdList->ClearRenderTargetView(rtvH, clearColor, 0, nullptr);
    cmdList->ClearDepthStencilView(dsvH, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

    // �r���[�|�[�g�̈�̐ݒ�
    cmdList->RSSetViewports(1, &CD3DX12_VIEWPORT(0.0f, 0.0f, win->window_width, win->window_height));
    // �V�U�[��`�̐ݒ�
    cmdList->RSSetScissorRects(1, &CD3DX12_RECT(0, 0, win->window_width, win->window_height));

}

void DirectXCommon::PostDraw() {
    UINT bbIndex = swapchain->GetCurrentBackBufferIndex();
    cmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(backBuffers[bbIndex].Get(),
        D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));
    // ���߂̃N���[�Y
    cmdList->Close();
    // �R�}���h���X�g�̎��s
    ID3D12CommandList* cmdLists[] = { cmdList.Get() }; // �R�}���h���X�g�̔z��
    cmdQueue->ExecuteCommandLists(1, cmdLists);
    // �R�}���h���X�g�̎��s������҂�
    cmdQueue->Signal(fence.Get(), ++fenceVal);
    if (fence->GetCompletedValue() != fenceVal) {
        HANDLE event = CreateEvent(nullptr, false, false, nullptr);
        fence->SetEventOnCompletion(fenceVal, event);
        WaitForSingleObject(event, INFINITE);
        CloseHandle(event);
    }

    cmdAllocator->Reset(); // �L���[���N���A
    cmdList->Reset(cmdAllocator.Get(), nullptr);  // �ĂуR�}���h���X�g�𒙂߂鏀��
#pragma endregion �O���t�B�b�N�X�R�}���h
        // �o�b�t�@���t���b�v�i���\�̓��ւ��j
    swapchain->Present(1, 0);
}



void DirectXCommon::InitializeDevice() {
#ifdef _DEBUG
    //�f�o�b�O���C���[���I����
    ComPtr<ID3D12Debug> debugController;
    if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)))) {
        debugController->EnableDebugLayer();
    }
#endif

    HRESULT result;
    result = CreateDXGIFactory1(IID_PPV_ARGS(&dxgiFactory));
    // �A�_�v�^�[�̗񋓗p
    std::vector<ComPtr<IDXGIAdapter1>> adapters;
    // �����ɓ���̖��O�����A�_�v�^�[�I�u�W�F�N�g������
    ComPtr<IDXGIAdapter1> tmpAdapter = nullptr;
    for (int i = 0;
        dxgiFactory->EnumAdapters1(i, &tmpAdapter) != DXGI_ERROR_NOT_FOUND;
        i++)
    {
        adapters.push_back(tmpAdapter); // ���I�z��ɒǉ�����
    }
    for (int i = 0; i < adapters.size(); i++) {
        DXGI_ADAPTER_DESC1 adesc;
        adapters[i]->GetDesc1(&adesc);  // �A�_�v�^�[�̏����擾

        // �\�t�g�E�F�A�f�o�C�X�����
        if (adesc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) {
            continue;
        }

        std::wstring strDesc = adesc.Description;   // �A�_�v�^�[��
        // Intel UHD Graphics�i�I���{�[�h�O���t�B�b�N�j�����
        if (strDesc.find(L"Intel") == std::wstring::npos)
        {
            tmpAdapter = adapters[i];   // �̗p
            break;
        }
    }

    // �Ή����x���̔z��
    D3D_FEATURE_LEVEL levels[] = {
        D3D_FEATURE_LEVEL_12_1,
        D3D_FEATURE_LEVEL_12_0,
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
    };

    D3D_FEATURE_LEVEL featureLevel;
    for (int i = 0; i < _countof(levels); i++) {
        // �̗p�����A�_�v�^�[�Ńf�o�C�X�𐶐�
        result = D3D12CreateDevice(tmpAdapter.Get(), levels[i], IID_PPV_ARGS(&dev));
        if (result == S_OK) {
            // �f�o�C�X�𐶐��ł������_�Ń��[�v�𔲂���
            featureLevel = levels[i];
            break;
        }
    }

}

void DirectXCommon::InitializeCommand() {
    HRESULT result;
    // �R�}���h�A���P�[�^�𐶐�
    result = dev->CreateCommandAllocator(
        D3D12_COMMAND_LIST_TYPE_DIRECT,
        IID_PPV_ARGS(&cmdAllocator));
    // �R�}���h���X�g�𐶐�
    result = dev->CreateCommandList(0,
        D3D12_COMMAND_LIST_TYPE_DIRECT,
        cmdAllocator.Get(), nullptr,
        IID_PPV_ARGS(&cmdList));
    // �W���ݒ�ŃR�}���h�L���[�𐶐�
    D3D12_COMMAND_QUEUE_DESC cmdQueueDesc{};
    dev->CreateCommandQueue(&cmdQueueDesc, IID_PPV_ARGS(&cmdQueue));
}

void DirectXCommon::InitializeSwapchain() {
    HRESULT result;

    // �e��ݒ�����ăX���b�v�`�F�[���𐶐�
    DXGI_SWAP_CHAIN_DESC1 swapchainDesc{};
    swapchainDesc.Width = 1280;
    swapchainDesc.Height = 720;
    swapchainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;  // �F���̏���
    swapchainDesc.SampleDesc.Count = 1; // �}���`�T���v�����Ȃ�
    swapchainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER; // �o�b�N�o�b�t�@�p
    swapchainDesc.BufferCount = 2;  // �o�b�t�@�����Q�ɐݒ�
    swapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD; // �t���b�v��͔j��
    swapchainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

    ComPtr<IDXGISwapChain1> swapchain1;

    // �X���b�v�`�F�[���̐���
    dxgiFactory->CreateSwapChainForHwnd(
        cmdQueue.Get(),
        win->GetHwnd(),
        &swapchainDesc,
        nullptr,
        nullptr,
        &swapchain1);

    // ��������IDXGISwapChain1�̃I�u�W�F�N�g��IDXGISwapChain4�ɕϊ�����
    swapchain1.As(&swapchain);
}

void DirectXCommon::InitializeRenderTargetView() {
    HRESULT result;

    // �e��ݒ�����ăf�X�N���v�^�q�[�v�𐶐�
    D3D12_DESCRIPTOR_HEAP_DESC heapDesc{};
    heapDesc.Type =
        D3D12_DESCRIPTOR_HEAP_TYPE_RTV; // �����_�[�^�[�Q�b�g�r���[
    heapDesc.NumDescriptors = 2;    // ���\�̂Q��
    dev->CreateDescriptorHeap(&heapDesc,
        IID_PPV_ARGS(&rtvHeaps));
    backBuffers.resize(2);

    for (int i = 0; i < 2; i++) {
        // �X���b�v�`�F�[������o�b�t�@���擾
        result = swapchain->GetBuffer(i, IID_PPV_ARGS(&backBuffers[i]));

        // �����_�[�^�[�Q�b�g�r���[�̐���
        dev->CreateRenderTargetView(
            backBuffers[i].Get(),
            nullptr,
            CD3DX12_CPU_DESCRIPTOR_HANDLE(
                rtvHeaps->GetCPUDescriptorHandleForHeapStart(),
                i,
                dev->GetDescriptorHandleIncrementSize(heapDesc.Type)
            )
        );
    }
}

void DirectXCommon::InitializeDepthBuffer() {
    HRESULT result;

    // �[�x�o�b�t�@���\�[�X�ݒ�
    CD3DX12_RESOURCE_DESC depthResDesc = CD3DX12_RESOURCE_DESC::Tex2D(
        DXGI_FORMAT_D32_FLOAT,
        WinApp::window_width,
        WinApp::window_height,
        1, 0,
        1, 0,
        D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL);
    // �[�x�o�b�t�@�̐���
    result = dev->CreateCommittedResource(
        &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
        D3D12_HEAP_FLAG_NONE,
        &depthResDesc,
        D3D12_RESOURCE_STATE_DEPTH_WRITE, // �[�x�l�������݂Ɏg�p
        &CD3DX12_CLEAR_VALUE(DXGI_FORMAT_D32_FLOAT, 1.0f, 0),
        IID_PPV_ARGS(&depthBuffer));

    // �[�x�r���[�p�f�X�N���v�^�q�[�v�쐬
    D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc{};
    dsvHeapDesc.NumDescriptors = 1; // �[�x�r���[��1��
    dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV; // �f�v�X�X�e���V���r���[
    result = dev->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&dsvHeap));

    // �[�x�r���[�쐬
    D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
    dsvDesc.Format = DXGI_FORMAT_D32_FLOAT; // �[�x�l�t�H�[�}�b�g
    dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
    dev->CreateDepthStencilView(
        depthBuffer.Get(),
        &dsvDesc,
        dsvHeap->GetCPUDescriptorHandleForHeapStart());

}

void DirectXCommon::InitializeFence() {
    HRESULT result;


    result = dev->CreateFence(fenceVal, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));
}
