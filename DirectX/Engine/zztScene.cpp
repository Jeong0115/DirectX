#include "zztScene.h"
#include "zzTransform.h"
#include "zzMeshRenderer.h"
#include "zzResourceManager.h"

zz::tScene::tScene()
{
}

zz::tScene::~tScene()
{
}

void zz::tScene::Initialize()
{
    GameObject* object = new GameObject();
    AddGameObject(object, eLayerType::PixelWorld);
    object->GetComponent<Transform>()->SetPosition(Vector3(768, - 1024, PIXEL_WORLD_Z));
    object->GetComponent<Transform>()->SetScale(Vector3(1536.f, 2048.f, 1.0f));
    MeshRenderer* mesh = object->AddComponent<MeshRenderer>();
    mesh->SetMaterial(ResourceManager::Find<Material>(L"m_PixelTexture"));
    mesh->SetMesh(ResourceManager::Find<Mesh>(L"RectMesh"));

    Scene::Initialize();
}

void zz::tScene::Update()
{
    Scene::Update();
}

void zz::tScene::LateUpdate()
{
    Scene::LateUpdate();
}

void zz::tScene::Render()
{
    Scene::Render();
}
