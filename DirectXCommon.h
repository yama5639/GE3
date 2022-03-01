#pragma once


#include<d3d12.h>
#include<d3dx12.h>
#include<dxgi1_6.h>
#include<wrl.h>


#include"WinApp.h"

class DirectXCommon
{
private:
    template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
public:
    void Initialize(WinApp* win);
    void PreDraw();
    void PostDraw();
private:
    void InitializeDevice();
    void InitializeCommand();
    void InitializeSwapchain();
    void InitializeRenderTargetView();
    void InitializeDepthBuffer();
    void InitializeFence();
public:
    ID3D12Device* GetDev() { return dev.Get(); }
    ID3D12GraphicsCommandList* GetCmdList() { return cmdList.Get(); }
private:
    WinApp* win = nullptr;

    ComPtr<ID3D12Device> dev;
    ComPtr<IDXGIFactory6> dxgiFactory;
    ComPtr<IDXGISwapChain4> swapchain;
    ComPtr<ID3D12CommandAllocator> cmdAllocator;
    ComPtr<ID3D12GraphicsCommandList> cmdList;
    ComPtr<ID3D12CommandQueue> cmdQueue;
    ComPtr<ID3D12DescriptorHeap> dsvHeap;
    ComPtr<ID3D12DescriptorHeap> rtvHeaps;
    ComPtr<ID3D12Resource> depthBuffer;
    ComPtr<ID3D12Fence> fence;
    std::vector<ComPtr<ID3D12Resource>> backBuffers;
    UINT64 fenceVal = 0;
};