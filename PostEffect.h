#pragma once
#include "Sprite.h"
class PostEffect :
    public Sprite
{
private:
    template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
private:
    //テクスチャバッファ
    ComPtr<ID3D12Resource>texBuff[2];
    //SRV用でスクリプタヒープ
    ComPtr<ID3D12DescriptorHeap>descHeapSRV;
    //深度バッファ    
    ComPtr<ID3D12Resource>depthBuff;
    //RTV用デスクリプタヒープ
    ComPtr<ID3D12DescriptorHeap>descHeapRTV;
    //DSV用デスクリプタヒープ
    ComPtr<ID3D12DescriptorHeap>descHeapDSV;
    ComPtr<ID3D12PipelineState> pipelineState;
    ComPtr<ID3D12RootSignature> rootSignature;
private:
    static const float clearColor[4];
public:
    PostEffect();
    void Draw(ID3D12GraphicsCommandList* cmdList);
    void Initialize();
    void PreDrawScene(ID3D12GraphicsCommandList* cmdList);
    void PostDrawScene(ID3D12GraphicsCommandList* cmdList);
    void CreateGraphicsPipelineState();
};

