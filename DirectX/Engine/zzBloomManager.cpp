#include "zzBloomManager.h"
#include "zzResourceManager.h"
#include "zzGraphics.h"
#include "zzMesh.h"
#include "zzShader.h"
#include "zzGraphicsDevice.h"
#include "zzRenderer.h"

using namespace zz::graphics;
namespace zz
{
    std::shared_ptr<ComputeShader> BloomManager::mExtracionCS = {};
    std::shared_ptr<ComputeShader> BloomManager::mHorizonCS = {};
    std::shared_ptr<ComputeShader> BloomManager::mVerticalCS = {};

    Texture2D* BloomManager::mBloomPingTex = nullptr;
    Texture2D* BloomManager::mBloomPongTex = nullptr;

    BloomManager::BloomManager()
    {
    }   
    BloomManager::~BloomManager()
    {
        //delete mBloomBuffer;
    }

    void BloomManager::Initialize()
    {
        mExtracionCS = ResourceManager::Find<ComputeShader>(L"BloomExtractionCS");
        mHorizonCS = ResourceManager::Find<ComputeShader>(L"BloomHorizonCS");
        mVerticalCS = ResourceManager::Find<ComputeShader>(L"BloomVerticalCS");

        mExtracionCS->SetGroupCount(13, 8, 1);
        mHorizonCS->SetGroupCount(13, 8, 1);
        mVerticalCS->SetGroupCount(13, 8, 1);

        mBloomPingTex = new Texture2D();
        mBloomPingTex->Create(400, 225);
        mBloomPingTex->SetUAVSlot(1);
        mBloomPingTex->SetSRVSlot(2);

        mBloomPongTex = new Texture2D();
        mBloomPongTex->Create(400, 225);
        mBloomPongTex->SetUAVSlot(5);
        //mBloomPongTex->SetSRVSlot(2);
    }

    void BloomManager::Update()
    {
        GraphicsDevice*& device = GetDevice();
        device->SetBloomRenderTarget();
       
        ResourceManager::Find<Mesh>(L"LightMesh")->BindBuffer();
        ResourceManager::Find<Shader>(L"LightMapShader")->BindShaders();
        ID3D11ShaderResourceView* lightSrv = device->GetLightMapResource();
        ID3D11ShaderResourceView* null = nullptr;
       
        device->BindShaderResource(eShaderStage::PS, 0, &lightSrv);
        device->DrawIndexed(6, 0, 0);
        device->BindShaderResource(eShaderStage::PS, 0, &null);

        device->ClearRenderTarget();

        ID3D11ShaderResourceView* bloomSrv = device->GetBloomResource();
        device->BindShaderResource(eShaderStage::CS, 1, &bloomSrv);

        mBloomPingTex->BindUAV();
        mBloomPongTex->BindUAV();

        mExtracionCS->OnExcute();
        mVerticalCS->OnExcute();
        mHorizonCS->OnExcute();
        mVerticalCS->OnExcute();
        mHorizonCS->OnExcute();

        mBloomPingTex->Clear();
        mBloomPongTex->Clear();
    }

    void BloomManager::Render()
    {
        mBloomPingTex->BindSRV(eShaderStage::VS);
        mBloomPingTex->BindSRV(eShaderStage::GS);
        mBloomPingTex->BindSRV(eShaderStage::PS);

        GetDevice()->DrawIndexed(6, 0, 0);

        mBloomPingTex->Clear();

        //ID3D11ShaderResourceView* lightSrv = GetDevice()->GetLightMapResource();
        //ID3D11ShaderResourceView* null = nullptr;

        //GetDevice()->BindShaderResource(eShaderStage::PS, 2, &lightSrv);
        //GetDevice()->DrawIndexed(6, 0, 0);
        //GetDevice()->BindShaderResource(eShaderStage::PS, 2, &null);
    }

    void BloomManager::SetComputeShader(std::shared_ptr<ComputeShader> shader, int type)
    {
        switch (type)
        {
        case 0: mExtracionCS    = shader; break;
        case 1: mHorizonCS      = shader; break;
        case 2: mVerticalCS     = shader; break;
        default: break;
        }
    }
}


