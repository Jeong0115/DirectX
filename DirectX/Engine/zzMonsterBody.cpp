#include "zzMonsterBody.h"
#include "zzResourceManager.h"
#include "zzMeshRenderer.h"
#include "zzTexture.h"
#include "zzBox2dCollider.h"
#include "zzTransform.h"
#include "zzTime.h"

namespace zz
{
    MonsterBody::MonsterBody()
        : mImpulse(Vector2::Zero)
        , mBoxScale(Vector3::Zero)
        , mPoint(Vector2::Zero)
        , mTime(0.0f)
    {     
    }
    MonsterBody::~MonsterBody()
    {
    }
    void MonsterBody::Initialize()
    {
        Box2dCollider* box2d = AddComponent<Box2dCollider>();

        Transform* tr = GetComponent<Transform>();
        box2d->Create(tr->GetPosition(), mBoxScale);
        box2d->ApplyLinearImpulse(mImpulse, mPoint);

        GameObject::Initialize();
    }
    void MonsterBody::Update()
    {
        mTime += (float)Time::DeltaTime();

        if (mTime >= 6.5f)
        {
            DeleteObject(this, eLayerType::Effect);
        }
        GameObject::Update();
    }
    void MonsterBody::LateUpdate()
    {
        GameObject::LateUpdate();
    }
    void MonsterBody::Render()
    {
        GameObject::Render();
    }

    void MonsterBody::SetTexture(std::shared_ptr<Texture> tex)
    {
        MeshRenderer* mesh = AddComponent<MeshRenderer>();

        std::shared_ptr<Material> mat = std::make_shared<Material>();

        mat->SetShader(ResourceManager::Find<Shader>(L"SpriteShader")); 
        mat->SetTexture(tex);

        mesh->SetMaterial(mat);
        mesh->SetMesh(ResourceManager::Find<Mesh>(L"RectMesh"));
        
        GetComponent<Transform>()->SetScale(tex->GetImageWidth(), tex->GetImageHeight(), 1.0f);
    }
}