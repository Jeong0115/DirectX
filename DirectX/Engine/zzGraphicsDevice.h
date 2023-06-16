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

        bool CreateSwapChain(const DXGI_SWAP_CHAIN_DESC* desc, HWND hWnd);
        bool CreateTexture(const D3D11_TEXTURE2D_DESC* desc, void* data);
        bool CreateInputLayout(const D3D11_INPUT_ELEMENT_DESC* pInputElementDescs, UINT NumElements, ID3DBlob* byteCode, ID3D11InputLayout** ppInputLayout);
        bool CreateBuffer(ID3D11Buffer** buffer, D3D11_BUFFER_DESC* desc, D3D11_SUBRESOURCE_DATA* data);
        
        bool CompileFromfile(const std::wstring& fileName, const std::string& funcName, const std::string& version, ID3DBlob** ppCode);
        
        bool CreateVertexShader(const void* pShaderBytecode, SIZE_T BytecodeLength, ID3D11VertexShader** ppVertexShader);
        bool CreatePixelShader(const void* pShaderBytecode, SIZE_T BytecodeLength, ID3D11PixelShader** ppPixelShader);
        
        void BindInputLayout(ID3D11InputLayout* pInputLayout);
        void BindPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY Topology);
        void BindVertexBuffer(UINT StartSlot, ID3D11Buffer* const* ppVertexBuffers, const UINT* pStrides, const UINT* pOffsets);
        void BindIndexBuffer(ID3D11Buffer* pIndexBuffer, DXGI_FORMAT Format, UINT Offset);
        void BindVertexShader(ID3D11VertexShader* pVetexShader);
        void BindPixelShader(ID3D11PixelShader* pPixelShader);

        void SetConstantBuffer(ID3D11Buffer* buffer, void* data, UINT size);
        void BindConstantBuffer(eShaderStage stage, eCBType type, ID3D11Buffer* buffer);
        void BindsConstantBuffer(eShaderStage stage, eCBType type, ID3D11Buffer* buffer);

        void BindViewPort(D3D11_VIEWPORT* viewPort);
        
        void DrawIndexed(UINT IndexCount, UINT StartIndexLocation, INT BaseVertexLocation);
        void ClearRenderTarget();
        void UpdateViewPort();
        void Draw();
        void Present();

        //Microsoft::WRL::ComPtr<ID3D11Device> GetD3D11Device() { return mDevice; }

	private:
		Microsoft::WRL::ComPtr<ID3D11Device> mDevice;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> mContext;
		Microsoft::WRL::ComPtr<ID3D11Texture2D> mRenderTarget;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> mRenderTargetView;
		Microsoft::WRL::ComPtr<ID3D11Texture2D> mDepthStencilBuffer;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> mDepthStencilView;
		Microsoft::WRL::ComPtr<IDXGISwapChain> mSwapChain;

        D3D11_VIEWPORT mViewPort;
	};

    inline GraphicsDevice*& GetDevice()
    {
        static GraphicsDevice* device = nullptr;
        return device;
    }
}
