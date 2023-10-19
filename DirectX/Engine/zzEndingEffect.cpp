#include "zzEndingEffect.h"
#include "zzParticleSystem.h"
#include "zzTransform.h"
#include "zzResourceManager.h"
#include "zzRenderer.h"
#include "zzTime.h"
#include "zzPixelWorld.h"
#include "zzLight.h"
#include "zzEndingView.h"

namespace zz
{
    EndingEffect::EndingEffect()
        : mParticleTime(0.0f)
        , mMin(0.f)
        , mMax(0.f)
        , mLight(nullptr)
        , mbChange(true)
    {
    }
    EndingEffect::~EndingEffect()
    {
    }
    void EndingEffect::Initialize()
    {
        mParticle = AddComponent<ParticleSystem>();
        mParticle->SetMaterial(ResourceManager::Find<Material>(L"m_Particle"));
        mParticle->SetMesh(ResourceManager::Find<Mesh>(L"PointMesh"));
        mParticle->SetParticleShader(ResourceManager::Find<ParticleShader>(L"ParticleImageCS"));

        auto mParticleBuffer = std::make_shared<Particle[]>(116960);

        mParticle->CreateStructedBuffer(sizeof(Particle), 116960, eViewType::UAV, mParticleBuffer.get(), true, 0, 14, 0);
        mParticle->CreateStructedBuffer(sizeof(ParticleImageShared), 1, eViewType::UAV, nullptr, true, 7, 14, 1);
        mParticle->SetGroupCount(11, 11, 1);

        mLight = AddComponent<Light>();
        mLight->SetLightType(5);
        mLight->SetLightColor(Vector4(0.3f, 0.3f, 0.3f, 1.0f));
        mLight->SetLightScale(Vector3(0.0f, 0.0f, 1.0f));

        MeshRenderer* particleLight = new MeshRenderer();
        particleLight->SetMaterial(ResourceManager::Find<Material>(L"m_particle_glow_particleLight"));
        particleLight->SetMesh(ResourceManager::Find<Mesh>(L"PointMesh"));
        mParticle->SetParticleLight(particleLight);

        mSharedData.imageSize = Vector2(340.f, 344.f);
        mSharedData.randLifeTime = Vector2(0.5f, 2.3f);
        mSharedData.randVelocityMin = Vector2(-2.0f, -2.0f);
        mSharedData.randVelocityMax = Vector2(2.0f, 2.0f);
        mSharedData.scale = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
        mSharedData.lightScale = Vector4(5.0f, 5.0f, 1.0f, 1.0f);
        mSharedData.angle = 0.0f;
        mSharedData.color_min = mSharedData.color_max = Vector4(0.8f, 0.2f, 0.2f, 0.1f);
        mSharedData.curPosition = (GetComponent<Transform>()->GetPosition()) + 0.0f;
        mView = new EndingView();
        mView->GetComponent<Transform>()->SetPosition(GetComponent<Transform>()->GetPosition());
        CreateObject(mView, eLayerType::Effect);
        GameObject::Initialize();
    }

    void EndingEffect::Update()
    {
        Vector3 pos = GetComponent<Transform>()->GetPosition();

        if(mbChange)
        {
            mMax += (float)Time::DeltaTime() * 100.f;

            for (float r = mMin; r < mMax; r += 1)
            {
                for (float theta = 0; theta < 2 * PI; theta += 1 / r)
                {
                    float x = r * cos(theta);
                    float y = r * sin(theta);

                    int roundedX = round(pos.x + x);
                    int roundedY = round(pos.y + y);

                    PixelWorld::InsertEndingMat(roundedX, -roundedY);
                }
            }
            mMin = mMax;
            mView->GetComponent<Transform>()->SetScale(Vector3(mMax * 2.f * 1.9 * 1.3f, mMax * 2.f * 1.9 * 1.3f, 1.0f));

            if (mMax >= 256.f)
            {
                mbChange = false;
            }
        }
        
        GameObject::Update();
    }

    void EndingEffect::LateUpdate()
    {
        // 이니셜 라이즈에서 구조화 버퍼 설정하면 에러난다
        renderer::ColorRangeCB colorCB = {};

        colorCB.colorRange.x = mParticleTime;
        mParticleTime += (float)Time::DeltaTime() * 0.5f;
        colorCB.colorRange.y = mParticleTime;

        mParticle->SetStructedBufferData(&mSharedData, 1, 1);

        //if (mParticleTime >= 5.0f)
        //{
        //    DeleteObject(this, eLayerType::Effect);
        //}
        ConstantBuffer* cb = renderer::constantBuffer[(UINT)eCBType::ColorRange];
        cb->SetBufferData(&colorCB);
        cb->BindConstantBuffer(eShaderStage::CS);

        std::shared_ptr<Texture> particleImage = ResourceManager::Find<Texture>(L"animated_emitter_large");
        particleImage->BindShader(eShaderStage::CS, 10);
        GameObject::LateUpdate();

        colorCB = {};
        cb->SetBufferData(&colorCB);
        cb->BindConstantBuffer(eShaderStage::CS);
    }
    void EndingEffect::Render()
    {
        GameObject::Render();
    }
}