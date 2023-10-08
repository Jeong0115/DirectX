#include "zzOpeningScene.h"
#include "zzResourceManager.h"
#include "zzMeshRenderer.h"
#include "zzTime.h"
#include "zzAnimator.h"
#include "zzTransform.h"
#include "zzRenderer.h"
#include "zzMainCamera.h"


namespace zz
{
    OpeningScene::OpeningScene()
        : trans(nullptr)
        , mesh(nullptr)
        , animator(nullptr)
        , camera(nullptr)
        , sign(1.f)
        , time(0.f)
        , fadeTime(1.4f)
        , duration(0.2f)
        , mbEnd(false)
    {
    }
    OpeningScene::~OpeningScene()
    {
        delete trans;
        delete mesh;
        delete animator;
        delete camera;
    }

    void OpeningScene::Initialize()
    {
        std::shared_ptr<Texture> texture = ResourceManager::Load<Texture>(L"noita_logo_anim_01", L"..\\Resources\\Texture\\Opening\\noita_logo_anim_01.png");
        std::shared_ptr<Texture> texture2 = ResourceManager::Load<Texture>(L"nollagames_logo", L"..\\Resources\\Texture\\Opening\\nollagames_logo.png");

        trans = new Transform();
        mesh = new MeshRenderer();
        animator = new Animator;

        mesh->SetMaterial(ResourceManager::Find<Material>(L"m_FadeAnimation"));
        mesh->SetMesh(ResourceManager::Find<Mesh>(L"RectMesh"));

        animator->Create(L"noita_logo_anim_01_play", texture, Vector2(0.0f, 0.0f), Vector2(96.0f, 59.0f), 24, Vector2::Zero, 0.03f);
        animator->Create(L"nollagames_logo_play", texture2, Vector2(0.0f, 0.0f), Vector2(1440.f, 1202.f), 1, Vector2::Zero, 3.0f);

        animator->EndEvent(L"nollagames_logo_play") = std::bind(&OpeningScene::playNextAnimation, this);
        animator->PlayAnimation(L"nollagames_logo_play", false);

        trans->SetScale(1440.f * 0.7f, 1202.f * 0.7f, 1.f);
        trans->SetPosition(0.f, 0.f, 0.f);

        camera = new GameObject();
        MainCamera* comp = camera->AddComponent<MainCamera>();
        camera->GetComponent<Transform>()->SetPosition(Vector3(0.0f, 0.0f, -10.0f));
        camera->GetComponent<Transform>()->LateUpdate();

        comp->CreateViewMatrix();
        comp->CreateProjectionMatrix(Camera::eProjectionType::OrthoGraphic);

        comp->SetGpuViewMatrix(comp->GetViewMatrix());
        comp->SetGpuProjectionMatrix(comp->GetProjectionMatrix());

    }
    void OpeningScene::Update()
    {
        animator->Update();
    }
    void OpeningScene::LateUpdate()
    {
        trans->LateUpdate();
    }
    void OpeningScene::Render()
    {
        time += (float)Time::DeltaTime() * sign;

        renderer::ColorCB color = {};
        color.color = Vector4(0.f, 0.f, 0.f, time / fadeTime);

        ConstantBuffer* cb = renderer::constantBuffer[(UINT)eCBType::Color];
        cb->SetBufferData(&color);
        cb->BindConstantBuffer(eShaderStage::PS);

        if (time >= fadeTime + duration)
        {
            time = fadeTime;
            sign *= -1.f;
        }

        trans->Render();

        mesh->GetMesh()->BindBuffer();
        mesh->GetMaterial()->Binds();

        animator->Binds();

        mesh->GetMesh()->Render();
        mesh->GetMaterial()->Clear();

    }

    void OpeningScene::EndLoading()
    {
        mbEnd = true;

        if (duration != 0.f)
        {
            duration = 0.f;
            time = fadeTime = 1.f;
        }
    }

    bool OpeningScene::Run()
    {
        Time::Update();
        Update();
        LateUpdate();
        Render();

        if (mbEnd)
        {
            if (time < 0.f)
            {
                return false;
            }
        }

        return true;
    }

    void OpeningScene::playNextAnimation()
    {
        sign = 1.f;
        time = 0.f;
        fadeTime = 1.f;
        duration = 50.f;
        trans->SetScale(1600.f * 0.5f, 900.f * 0.5f, 1.f);
        animator->PlayAnimation(L"noita_logo_anim_01_play", true);
    }
}