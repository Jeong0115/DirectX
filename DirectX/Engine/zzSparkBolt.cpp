#include "zzSparkBolt.h"
#include "zzAnimator.h"
#include "zzTransform.h"
#include "zzMeshRenderer.h"
#include "zzTexture.h"
#include "zzMaterial.h"
#include "zzResourceManager.h"
#include "zzTime.h"
#include "zzParticle.h"
#include "zzSceneManager.h"

namespace zz
{
    SparkBolt::SparkBolt()
    {
    }

    SparkBolt::~SparkBolt()
    {
    }

    void SparkBolt::Initialize()
    {
        MeshRenderer* mesh = AddComponent<MeshRenderer>();
        mesh->SetMaterial(ResourceManager::Find<Material>(L"m_SpriteAnimation"));
        mesh->SetMesh(ResourceManager::Find<Mesh>(L"RectMesh"));

        std::shared_ptr<Texture> texture = ResourceManager::Load<Texture>(L"SparkBolt");

        Animator* ani = AddComponent<Animator>();
        ani->Create(L"SparkBolt_Idle", texture, Vector2(0.0f, 1.0f), Vector2(10.0f, 10.0f), 2, Vector2::Zero, 0.2f);
        ani->PlayAnimation(L"SparkBolt_Idle", true);

        GetComponent<Transform>()->SetScale(10.f, 10.f, 1.0f);
            
        GameObject::Initialize();
    }

    

    void SparkBolt::Update()
    {
        mT += Time::DeltaTime();

        Transform* tr = GetComponent<Transform>();
        Vector3 prevPos = tr->GetPosition();
        Vector3 curPos;
        float angle = tr->GetRotation().z;

        curPos = prevPos + mDirection * 1000.f * (float)Time::DeltaTime();

        tr->SetPosition(curPos);

        float moveX = curPos.x - prevPos.x;
        float moveY = curPos.y - prevPos.y;

        float count = max(fabs(moveX), fabs(moveY));

        for (int i = 0; i < count; i++)
        {
            Vector3 vel = { random() , sin(c += 0.1f) / 1.7f, 0.f};

            Vector3 calVel;
            calVel.x = vel.x * cos(angle) - vel.y * sin(angle);
            calVel.y = vel.x * sin(angle) + vel.y * cos(angle);

            TempParticle* particle = new TempParticle();
            particle->SetVelocity(calVel);
            particle->SetLifeTime(random() / 10.f);
            particle->GetComponent<Transform>()->SetPosition(prevPos);

            CreateObject(particle, eLayerType::Particle);

            prevPos.x += moveX / count;
            prevPos.y += moveY / count;
        }
        
        if(random() >= 0.3f)
        {
            Vector3 vel = { random() / 10.f , random() * (random() >= 0.5f ? 1 : -1), 0.f };
            TempParticle* particle = new TempParticle();
            particle->SetVelocity(vel);
            particle->SetLifeTime(random());
            particle->GetComponent<Transform>()->SetPosition(curPos);

            CreateObject(particle, eLayerType::Particle);
        }

        if (mT >= 2.0f)
        {
            DeleteObject(this, eLayerType::Object);
        }
        GameObject::Update();
    }

    void SparkBolt::LateUpdate()
    {
        GameObject::LateUpdate();
    }

    void SparkBolt::Render()
    {
        GameObject::Render();
    }
}