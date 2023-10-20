#include "zzZombieAttack.h"
#include "zzTransform.h"
#include "zzHealthPoint.h"
#include "zzCollider.h"
namespace zz
{
    ZombieAttack::ZombieAttack()
    {
    }
    ZombieAttack::~ZombieAttack()
    {
    }
    void ZombieAttack::Initialize()
    {
        AddComponent<Collider>()->SetScale(5.f, 5.f, 1.0f);
        GameObject::Initialize();
    }
    void ZombieAttack::Update()
    {
        GameObject::Update();
    }
    void ZombieAttack::LateUpdate()
    {
        GameObject::LateUpdate();
    }
    void ZombieAttack::Render()
    {
        GameObject::Render();
    }
    void ZombieAttack::OnCollisionEnter(GameObject* other)
    {
        if (other->GetLayerType() == eLayerType::Player)
        {
            other->GetComponent<HealthPoint>()->Hit(8);
        }
    }
    void ZombieAttack::OnCollisionStay(GameObject* other)
    {
    }
    void ZombieAttack::OnCollisionExit(GameObject* other)
    {
    }
}