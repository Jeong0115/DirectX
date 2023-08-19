#include "zzExplosion_128.h"
#include "zzResourceManager.h"
#include "zzTexture.h"
#include "zzAnimator.h"
#include "zzMeshRenderer.h"
#include "zzMesh.h"
#include "zzMaterial.h"
#include "zzTransform.h"
#include "zzPixelWorld.h"

namespace zz
{
    Explosion_128::Explosion_128()
        : mbFirstFrame(true)
    {
    }

    Explosion_128::~Explosion_128()
    {
    }

    void Explosion_128::Initialize()
    {
        std::shared_ptr<Texture> texture = ResourceManager::Find<Texture>(L"explosion_128");

        MeshRenderer* meshRender = AddComponent<MeshRenderer>();
        meshRender->SetMaterial(ResourceManager::Find<Material>(L"m_SpriteAnimation"));
        meshRender->SetMesh(ResourceManager::Find<Mesh>(L"RectMesh"));

        Animator* animator = AddComponent<Animator>();
        animator->Create(L"Explosion_128", texture, Vector2(0.0f, 0.0f), Vector2(128.8f, 129.f), 5, Vector2::Zero, 0.03f);
        animator->PlayAnimation(L"Explosion_128", false);
        animator->EndEvent(L"Explosion_128") = std::bind(&Explosion_128::EndAnimation, this);

        GetComponent<Transform>()->SetScale(129.f, 129.f, 1.0f);

        GameObject::Initialize();
    }

    void Explosion_128::Update()
    {
        if (mbFirstFrame)
        {
            Vector3 pos = GetComponent<Transform>()->GetPosition();

            for (int x = pos.x - 60; x <= pos.x + 60; x++)
            {
                int y = static_cast<int>(std::round(std::sqrt(60 * 60 - (x - pos.x) * (x - pos.x))));
                for (int j = pos.y - y; j <= pos.y + y; j++)
                {
                    PixelWorld::DeleteElement(x, -j);
                }
            }
            mbFirstFrame = false;
        }
        GameObject::Update();
    }

    void Explosion_128::LateUpdate()
    {
        GameObject::LateUpdate();
    }

    void Explosion_128::Render()
    {
        GameObject::Render();
    }

    void Explosion_128::EndAnimation()
    {
        if (!IsDead())
        {
            DeleteObject(this, eLayerType::Effect);
        }
    }
}