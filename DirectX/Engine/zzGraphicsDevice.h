#pragma once

#include "zzEngine.h"
#include "zzGraphics.h"

#include <d3d11.h>
#include <d3dcompiler.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

namespace zz::graphics
{
	class GraphicsDevice
	{
	public:
		GraphicsDevice();
		~GraphicsDevice();

        void CreatePixelGrid();

        bool CreateSwapChain(const DXGI_SWAP_CHAIN_DESC* desc, HWND hWnd);
        bool CreateTexture(const D3D11_TEXTURE2D_DESC* desc, void* data);
        bool CreateTexture2d(const D3D11_TEXTURE2D_DESC* pDesc, ID3D11Texture2D** ppTexture2D);
        bool CreateInputLayout(const D3D11_INPUT_ELEMENT_DESC* pInputElementDescs, UINT NumElements, ID3DBlob* byteCode, ID3D11InputLayout** ppInputLayout);
        bool CreateBuffer(ID3D11Buffer** buffer, D3D11_BUFFER_DESC* desc, D3D11_SUBRESOURCE_DATA* data);
        bool CreateSamplerState(const D3D11_SAMPLER_DESC* pSamplerDesc, ID3D11SamplerState** ppSamplerState);
        bool CreateRasterizeState(const D3D11_RASTERIZER_DESC* pRasterizerDesc, ID3D11RasterizerState** ppRasterizerState);
        bool CreateDepthStencilState(const D3D11_DEPTH_STENCIL_DESC* pDepthStencilDesc, ID3D11DepthStencilState** ppDepthStencilState);
        bool CreateBlendState(const D3D11_BLEND_DESC* pBlendStateDesc, ID3D11BlendState** ppBlendState);
        bool CreateShaderResourceView(ID3D11Resource* pResource, const D3D11_SHADER_RESOURCE_VIEW_DESC* pDesc, ID3D11ShaderResourceView** ppSRView);
        bool CreateRenderTargetView(ID3D11Resource* pResource, const D3D11_RENDER_TARGET_VIEW_DESC* pDesc, ID3D11RenderTargetView** ppRTView);
        bool CreateUnorderedAccessView(ID3D11Resource* pResource, const D3D11_UNORDERED_ACCESS_VIEW_DESC* pDesc, ID3D11UnorderedAccessView** ppUAView);

        bool CompileFromfile(const std::wstring& fileName, const std::string& funcName, const std::string& version, ID3DBlob** ppCode);
        
        bool CreateVertexShader(const void* pShaderBytecode, SIZE_T BytecodeLength, ID3D11VertexShader** ppVertexShader);
        bool CreateGeometryShader(const void* pShaderBytecode, SIZE_T BytecodeLength, ID3D11GeometryShader** ppGeometryShader);
        bool CreatePixelShader(const void* pShaderBytecode, SIZE_T BytecodeLength, ID3D11PixelShader** ppPixelShader);
        bool CreateComputeShader(const void* pShaderBytecode, SIZE_T BytecodeLength, ID3D11ComputeShader** ppComputeShader);
        
        void BindInputLayout(ID3D11InputLayout* pInputLayout);
        void BindPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY Topology);
        void BindVertexBuffer(UINT StartSlot, ID3D11Buffer* const* ppVertexBuffers, const UINT* pStrides, const UINT* pOffsets);
        void BindIndexBuffer(ID3D11Buffer* pIndexBuffer, DXGI_FORMAT Format, UINT Offset);
        void BindVertexShader(ID3D11VertexShader* pVetexShader);
        void BindHullShader(ID3D11HullShader* pHullShader);
        void BindDomainShader(ID3D11DomainShader* pDomainShader);
        void BindGeometryShader(ID3D11GeometryShader* pGeometryShader);
        void BindPixelShader(ID3D11PixelShader* pPixelShader);
        void BindComputeShader(ID3D11ComputeShader* pComputeShader);
        void Dispatch(UINT ThreadGroupCountX, UINT ThreadGroupCountY, UINT ThreadGroupCountZ);

        void SetConstantBuffer(ID3D11Buffer* buffer, void* data, UINT size);
        void BindConstantBuffer(eShaderStage stage, eCBType type, ID3D11Buffer* buffer);
        void BindsConstantBuffer(eShaderStage stage, eCBType type, ID3D11Buffer* buffer);
        void BindBuffer(ID3D11Buffer* buffer, void* data, UINT size);
        void ReadBuffer(ID3D11Buffer* buffer, void* data, UINT size);
        void BindStagingBuffer(ID3D11Buffer* buffer, void* data, UINT size);
        void BindSamplerState(eShaderStage stage, UINT StartSlot, ID3D11SamplerState** ppSamplers);
        void BindShaderResource(eShaderStage stage, UINT startSlot, ID3D11ShaderResourceView** ppSRV);
        void BindUnorderedAccess(UINT slot, ID3D11UnorderedAccessView** ppUnorderedAccessViews, const UINT* pUAVInitialCounts);

        void BindViewPort(D3D11_VIEWPORT* viewPort);       
        void BindRasterizeState(ID3D11RasterizerState* pRasterizerState);
        void BindDepthStencilState(ID3D11DepthStencilState* pDepthStencilState);
        void BindBlendState(ID3D11BlendState* pBlendState);
        void CopyResource(ID3D11Resource* pDstResource, ID3D11Resource* pSrcResource);

        void CreateLightMap();
        void CreateBloomRenderTarget();

        void DrawIndexed(UINT IndexCount, UINT StartIndexLocation, INT BaseVertexLocation);
        void DrawIndexedInstanced(UINT IndexCountPerInstance, UINT InstanceCount, UINT StartIndexLocation, INT BaseVertexLocation, UINT StartInstanceLocation);
        void SetLightMapRenderTarget();
        void SetBloomRenderTarget();
        void ClearRenderTarget();
        void UpdateViewPort();
        void Draw();
        void Present();

        void UpdateSubresource(ID3D11Resource* pDstResource, const void* pSrcData) { mContext->UpdateSubresource(pDstResource, 0, NULL, pSrcData, 1536 * 4, 0); }

        ID3D11Device* GetID3D11Device() { return mDevice.Get(); }
        ID3D11DeviceContext* GetID3D11DeviceContext() { return mContext.Get(); }
        ID3D11ShaderResourceView* GetLightMapResource() { return mLightSRV.Get(); }

        ID3D11ShaderResourceView* GetBloomResource() { return mBloomSRV.Get(); }
        ID3D11Texture2D* GetBloomTexture() { return mBloomRenderTarget.Get(); }
	private:
		Microsoft::WRL::ComPtr<ID3D11Device> mDevice;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> mContext;
		Microsoft::WRL::ComPtr<ID3D11Texture2D> mRenderTarget;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> mRenderTargetView;
		Microsoft::WRL::ComPtr<ID3D11Texture2D> mDepthStencilBuffer;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> mDepthStencilView;
		Microsoft::WRL::ComPtr<IDXGISwapChain> mSwapChain;

        Microsoft::WRL::ComPtr<ID3D11Texture2D> mLightRenderTarget;
        Microsoft::WRL::ComPtr<ID3D11RenderTargetView> mLightRenderTargetView;
        Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> mLightSRV;

        Microsoft::WRL::ComPtr<ID3D11Texture2D> mBloomRenderTarget;
        Microsoft::WRL::ComPtr<ID3D11RenderTargetView> mBloomRenderTargetView;
        Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> mBloomSRV;

        D3D11_VIEWPORT mViewPort;
        D3D11_VIEWPORT mBloomViewPort;
	};

    inline GraphicsDevice*& GetDevice()
    {
        static GraphicsDevice* device = nullptr;
        return device;
    }
}
