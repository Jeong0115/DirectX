#include "zzComputeShader.h"
#include "zzGraphicsDevice.h"

namespace zz
{
    ComputeShader::ComputeShader(int x, int y, int z)
        : Resource(eResourceType::ComputeShader)
        , mThreadGroupCountX(x)
        , mThreadGroupCountY(y)
        , mThreadGroupCountZ(z)
    {
    }
        
    ComputeShader::~ComputeShader()
    {
    }

    bool ComputeShader::Create(const std::wstring& name, const std::string& methodName)
    {
        std::filesystem::path shaderPath = std::filesystem::current_path().parent_path();
        shaderPath += L"\\Shader\\";

        std::filesystem::path fullPath(shaderPath.c_str());
        fullPath += name;

        ID3DBlob* errorBlob = nullptr;
        graphics::GetDevice()->CompileFromfile(fullPath, methodName, "cs_5_0", mCSBlob.GetAddressOf());
        graphics::GetDevice()->CreateComputeShader(mCSBlob->GetBufferPointer(), mCSBlob->GetBufferSize(), mCS.GetAddressOf());

        return true;
    }

    void ComputeShader::OnExcute()
    {
        Binds();

        graphics::GetDevice()->BindComputeShader(mCS.Get());
        graphics::GetDevice()->Dispatch(mGroupX, mGroupY, mGroupZ);

        Clear();
    }

    void ComputeShader::Binds()
    {
    }

    void ComputeShader::Clear()
    {
    }
}