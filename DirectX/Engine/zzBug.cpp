#include "zzBug.h"
#include "zzAnimator.h"
#include "zzMeshRenderer.h"
#include "zzResourceManager.h"
#include "zzTexture.h"
#include "zzTime.h"
#include "zzTransform.h"
#include "zzRigidBody.h"
#include "zzDetectPlayer.h"
#include "zzLight.h"
#include "zzBugExplosion.h"

namespace zz
{
    Bug::Bug()
    {
    }
    Bug::~Bug()
    {
    }
    void Bug::Initialize()
    {
        GetComponent<Transform>()->SetScale(26.f, 26.f, 1.0f);

        MeshRenderer* mesh = AddComponent<MeshRenderer>();
        mesh->SetMaterial(ResourceManager::Find<Material>(L"m_SpriteAnimation"));
        mesh->SetMesh(ResourceManager::Find<Mesh>(L"RectMesh"));

        std::shared_ptr<Texture> texture = ResourceManager::Load<Texture>(L"bug_blue", L"..\\Resources\\Texture\\Centipede\\bug_blue.png");

        Animator* animator = AddComponent<Animator>();
        animator->Create(L"bug_blue_idle", texture, Vector2(0.0f, 0.0f), Vector2(26.0f, 26.0f), 4, Vector2::Zero, 0.09f);
        animator->PlayAnimation(L"bug_blue_idle", true);

        Light* light = AddComponent<Light>();
        light->SetLightColor(0.1f, 0.2f, 0.5f, 0.5f);
        light->SetLightScale(30.f, 30.f, 30.f);

        AddComponent<DetectPlayer>();

        mLimitTime = 3.0f;

        MonsterAttack::Initialize();

    }
    void Bug::Update()
    {
        Vector3 pos = GetComponent<Transform>()->GetPosition();
        Vector3 pPos = GetComponent<DetectPlayer>()->GetPlayerPos();

        Vector3 dir = pPos - pos;
        dir.Normalize();

        GetComponent<Transform>()->SetPosition(Vector3(pos.x + dir.x * 60.f * (float)Time::DeltaTime(), pos.y + dir.y * 60.f * (float)Time::DeltaTime(), pos.z));

        MonsterAttack::Update();
    }
    void Bug::LateUpdate()
    {
        MonsterAttack::LateUpdate();
    }
    void Bug::Render()
    {
        MonsterAttack::Render();
    }
    void Bug::OnCollisionEnter(GameObject* other)
    {
    }
    void Bug::OnCollisionStay(GameObject* other)
    {
    }
    void Bug::OnCollisionExit(GameObject* other)
    {
    }
    void Bug::dead()
    {
        Vector3 pos = GetComponent<Transform>()->GetPosition();
        CreateObject(new BugExplosion(), eLayerType::MonsterAttack, pos.x, pos.y, pos.z);
        DeleteObject(this, eLayerType::MonsterAttack);
    }
}