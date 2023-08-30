#include "zzEditor.h"

#include "..\\Engine\\zzMesh.h"
#include "..\\Engine\\zzResourceManager.h"
#include "..\\Engine\\zzTransform.h"
#include "..\\Engine\\zzMeshRenderer.h"
#include "..\\Engine\\zzMaterial.h"
#include "..\\Engine\\zzRenderer.h"

namespace zz
{
    std::vector<DebugObject*> Editor::mDebugObjects = {};

    void Editor::Initialize()
    {
        {
            std::shared_ptr<Mesh> mesh = ResourceManager::Find<Mesh>(L"DebugMesh");
            std::shared_ptr<Material> material = ResourceManager::Find<Material>(L"DebugMaterial");

            mDebugObjects.push_back(new DebugObject());

            MeshRenderer* meshRenderer = mDebugObjects[0]->AddComponent<MeshRenderer>();
            meshRenderer->SetMaterial(material);
            meshRenderer->SetMesh(mesh);
        }

        std::shared_ptr<Mesh> mesh = ResourceManager::Find<Mesh>(L"RedDebugMesh");
        std::shared_ptr<Material> material = ResourceManager::Find<Material>(L"DebugMaterial");

        mDebugObjects.push_back(new DebugObject());

        MeshRenderer* meshRenderer = mDebugObjects[1]->AddComponent<MeshRenderer>();
        meshRenderer->SetMaterial(material);
        meshRenderer->SetMesh(mesh);
        
    }
    void Editor::Run()
    {
       Update();
       LateUpdate();
       Render();
    }
    void Editor::Update()
    {
    }
    void Editor::LateUpdate()
    {
    }
    void Editor::Render()
    {
        for (const DebugMesh& mesh : renderer::debugMeshs)
        {
            DebugRender(mesh);
        }       
    }

    void Editor::Release()
    {
        for (auto debugObj : mDebugObjects)
        {
            delete debugObj;
            debugObj = nullptr;
        }
    }

    void Editor::DebugRender(const DebugMesh& mesh)
    {
        DebugObject* object;
        if (mesh.temp == 0)
            object = mDebugObjects[0];
        else 
            object = mDebugObjects[1];

        Transform* tr = object->GetComponent<Transform>();

        Vector3 pos = mesh.position;
        //pos.z -= 0.01f;

        tr->SetPosition(pos);
        tr->SetScale(mesh.scale);
        tr->SetRotation(mesh.rotation);

        tr->LateUpdate();

        object->Render();
    }
}