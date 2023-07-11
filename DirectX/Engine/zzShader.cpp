#include "zzShader.h"
#include "zzRenderer.h"

using namespace zz::graphics;
namespace zz
{   
    Shader::Shader()
        : Resource(eResourceType::Shader)
        , mInputLayout(nullptr)
        , mTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
        , mRSType(eRSType::SolidBack)
        , mDSType(eDSType::Less)
        , mBSType(eBSType::AlphaBlend)
    {
    }

    Shader::~Shader()
    {
        mInputLayout->Release();
        //mInputLayout->Release();
    }

    HRESULT Shader::Load(const std::wstring& path)
    {
        return E_NOTIMPL;
    }

    bool Shader::CreateShader(const eShaderStage stage, const std::wstring& fileName, const std::string& funcName)
    {
        std::filesystem::path shaderPath = std::filesystem::current_path().parent_path();
        shaderPath += L"\\Shader\\";

        std::filesystem::path fullPath(shaderPath.c_str());
        fullPath += fileName;

        ID3DBlob* errorBlob = nullptr;

        if (stage == eShaderStage::VS)
        {
            GetDevice()->CompileFromfile(fullPath, funcName, "vs_5_0", mVSBlob.GetAddressOf());
            GetDevice()->CreateVertexShader(mVSBlob->GetBufferPointer(), mVSBlob->GetBufferSize(), mVS.GetAddressOf());
        }
        else if (stage == eShaderStage::PS)
        {
            GetDevice()->CompileFromfile(fullPath, funcName, "ps_5_0", mPSBlob.GetAddressOf());
            GetDevice()->CreatePixelShader(mPSBlob->GetBufferPointer(), mPSBlob->GetBufferSize(), mPS.GetAddressOf());
        }

        return true;
    }

    void Shader::BindShaders()
    {
        GetDevice()->BindInputLayout(mInputLayout);
        GetDevice()->BindPrimitiveTopology(mTopology); // 위치 고민해보자

        GetDevice()->BindVertexShader(mVS.Get());
        GetDevice()->BindPixelShader(mPS.Get());

        // 그냥 포인터 들고 있을지 생각
        Microsoft::WRL::ComPtr<ID3D11RasterizerState> rsState = zz::renderer::rasterizerStates[(UINT)mRSType];
        Microsoft::WRL::ComPtr<ID3D11DepthStencilState> dsState = zz::renderer::depthStencilStates[(UINT)mDSType];
        Microsoft::WRL::ComPtr<ID3D11BlendState> bsState = zz::renderer::blendStates[(UINT)mBSType];

        GetDevice()->BindRasterizeState(rsState.Get());
        GetDevice()->BindDepthStencilState(dsState.Get());
        GetDevice()->BindBlendState(bsState.Get());
    }
}