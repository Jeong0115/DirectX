#include "zzLuminous_drill.h"
#include "zzMeshRenderer.h"
#include "zzAnimator.h"
#include "zzResourceManager.h"
#include "zzLight.h"
#include "zzCollider.h"
#include "zzTransform.h"
#include "zzPixelWorld.h"
#include "zzAudioSource.h"
#include "zzAudioClip.h"
#include "zzObjectPoolManager.h"

namespace zz
{
    Luminous_drill::Luminous_drill()
        : mbFirstFrame(false)
    {
        mSpeed = 0.0f;
        mCastDelay = -0.58f;
        mDamage = 10.0f;
        mManaDrain = 1.f;
        mRechargeTime = -2.17f;

        MeshRenderer* mesh = AddComponent<MeshRenderer>();
        mesh->SetMaterial(ResourceManager::Find<Material>(L"m_SpriteAnimation"));
        mesh->SetMesh(ResourceManager::Find<Mesh>(L"RectMesh"));

        std::shared_ptr<Texture> texture = ResourceManager::Find<Texture>(L"spark_green");

        Animator* ani = AddComponent<Animator>();
        ani->Create(L"spark_green_Idle", texture, Vector2(0.0f, 0.0f), Vector2(20.0f, 20.0f), 1, Vector2::Zero, 1.0f);
        ani->PlayAnimation(L"spark_green_Idle", true);

        mAudio = AddComponent<AudioSource>();
        mAudio->SetClip(ResourceManager::LoadAudioClip(L"freezing_gaze_add_01", L"..\\Resources\\Audio\\Projectiles\\freezing_gaze_add_01.wav"));
        mAudio->SetLoop(false);

        GetComponent<Transform>()->SetScale(35.f, 1.f, 1.0f);
        AddComponent<Collider>()->SetScale(35.f, 3.f, 1.0f);

        Light* light = AddComponent<Light>();

        light->SetLightType(1);
        light->SetLightScale(50.f, 20.f, 1.0f);
        light->SetLightColor(155.f / 255.f, 250.f / 255.f, 70.f / 255.f, 1.f);
    }
    Luminous_drill::~Luminous_drill()
    {
    }

    void Luminous_drill::Initialize()
    {   
        mTime = 0.f;
        mSpeed = 0.0f;
        mCastDelay = -0.58f;
        mDamage = 10.0f;
        mManaDrain = 1.f;
        mRechargeTime = -2.17f;

        mTrailElement.clear();
        mbFirstFrame = true;

        ProjectileSpell::Initialize();
    }

    void Luminous_drill::Update()
    {
        if (mbFirstFrame)
        {
            deletePixel();
            mbFirstFrame = false;
         
        }
		else
		{
			Dead();
		}

        ProjectileSpell::Update();
    }

    void Luminous_drill::LateUpdate()
    {
        ProjectileSpell::LateUpdate();
    }
    void Luminous_drill::Render()
    {
        ProjectileSpell::Render();
    }
    void Luminous_drill::Dead()
    {
        ObjectPoolManager::ReturnObjectToPool(this);
    }

    ProjectileSpell* Luminous_drill::Clone()
    {
        return ObjectPoolManager::GetObjectInPool<Luminous_drill>();
    }

    void Luminous_drill::InitialSetting()
    {
        mAudio->Play();

        Transform* tr = GetComponent<Transform>();

        Vector3 pos = tr->GetPosition();
        float angle = tr->GetRotation().z;

        Vector3 offset = Vector3::Zero;
        offset.x = 18.f * cos(angle);
        offset.y = 18.f * sin(angle);

        tr->SetPosition(pos + offset);
    }

    void Luminous_drill::deletePixel()
    {
        Vector3 pos = GetComponent<Transform>()->GetPosition();
        float angle = GetComponent<Transform>()->GetRotation().z;

        for (int i = -17; i <= 17; i++)
        {
            Vector3 offset = Vector3::Zero;

            offset.x = i * cos(angle);
            offset.y = i * sin(angle);

            offset += pos;

            PixelWorld::InsertElement(offset.x, -offset.y, EMPTY);
        }

    }
}