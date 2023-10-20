#include "zzApplication.h"
#include "zzSceneManager.h"
#include "zzRenderer.h"
#include "zzTime.h"
#include "zzInput.h"
#include "zzFmod.h"
#include "..\\Editor\\zzEditor.h"
#include "zzPixelWorld.h"
#include "zzUIManager.h"
#include "zzEventManager.h"
#include "zzObjectPoolManager.h"
#include "zzCollisionManger.h"
#include "zzBox2dWorld.h"
#include "zzWrite.h"
#include "zzThreadPool.h"
#include "zzBloomManager.h"

#include "zzOpeningScene.h"
#include "zzBossArenaScene.h"
#include "zzEndScene.h"

#include "zzResourceManager.h"
#include "zzTexture.h"
#include "zztScene.h"
namespace zz
{	
    bool Application::OnDebugMode = false;
    bool Application::LightDisabled = false;
    bool Application::IsDrawBox2d = false;

	Application::Application()
        : mHwnd(NULL)
        , mResolution{}
        , mWidth(-1), mHeight(-1)
        , graphicDevice(nullptr)
        , mPixelHwnd(nullptr)
    {
	}

	Application::~Application()
	{
        //graphicDevice.release();
        
        //ID3D11Debug* debugDevice = nullptr;
        ////graphics::GetDevice()->GetD3D11Device()->QueryInterface(__uuidof(ID3D11Debug), reinterpret_cast<void**>(&debugDevice));

        //if (debugDevice)
        //{
        //    debugDevice->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
        //    debugDevice->Release();
        //}

        //delete graphicDevice;
	}

    void Application::Run()
	{
        Update();
        LateUpdate();
        Render();
	}

	void Application::Initialize()
	{
        ThreadPool thread(1);

        Time::Initialize();
        Input::Initialize();
        Fmod::Initialize();
        renderer::Initialize();

        OpeningScene* opening = new OpeningScene();
        opening->Initialize();

        thread.enqueue([=]() { PixelWorld::Initialize(); });
        thread.enqueue([=]() { WriteManager::Initialize(); });
        thread.enqueue([=]() { UIManager::Initialize(); });
        thread.enqueue([=]() { SceneManager::Initialize(); });
        thread.enqueue([=]() { ObjectPoolManager::Initialize(); });
        thread.enqueue([=]() { Editor::Initialize(); });
        thread.enqueue([=]() { BloomManager::Initialize(); });
        
        bool isBreak = true;

        while (isBreak)
        {
            graphicDevice->ClearRenderTarget();
            graphicDevice->UpdateViewPort();

            isBreak = opening->Run();

            Present();

            if (thread.isEmpty())   
            {
                opening->EndLoading();
            }
        }

        delete opening;
        opening = nullptr;
	}

	void Application::Update()
	{
        Time::Update();
        Input::Update();

        if (Input::GetKey(eKeyCode::CTRL) && Input::GetKeyDown(eKeyCode::F5))
        {
            OnDebugMode = !OnDebugMode;
        }
        if (Input::GetKey(eKeyCode::CTRL) && Input::GetKeyDown(eKeyCode::F6))
        {
            IsDrawBox2d = !IsDrawBox2d;
        }
        if (Input::GetKey(eKeyCode::CTRL) && Input::GetKeyDown(eKeyCode::F8))
        {
            LightDisabled = !LightDisabled;
        }

        UIManager::Update();

        if (UIManager::GetIsOpenInventory()) return;
        PixelWorld::Update();
        SceneManager::Update();
        CollisionManger::Update();
 
	}

	void Application::LateUpdate()
	{
        if (!LightDisabled)
        {
            graphicDevice->SetLightMapRenderTarget();
        }

        UIManager::LateUpdate();
        SceneManager::LateUpdate();
        BloomManager::Update();
	}

	void Application::Render()
	{    
        graphicDevice->ClearRenderTarget();
        graphicDevice->UpdateViewPort();
        
        renderer::Render();

        Camera* mainCamara = renderer::mainCamera;
        Camera::SetGpuViewMatrix(mainCamara->GetViewMatrix());
        Camera::SetGpuProjectionMatrix(mainCamara->GetProjectionMatrix());

        renderer::TransformCB trCB = {};
        trCB.mView = Camera::GetGpuViewMatrix();
        trCB.mProjection = Camera::GetGpuProjectionMatrix();

        ConstantBuffer* cb = renderer::constantBuffer[(UINT)eCBType::Transform];
        cb->SetBufferData(&trCB);
        cb->BindConstantBuffer(eShaderStage::VS);

        if (IsDrawBox2d)
        {
            Box2dWorld::Render();
        }
        if(OnDebugMode)
        {
            Editor::Run();
        }

       
        Present();
        

        EventManager::Update();
	}

    void Application::Release()
    {
        BloomManager::Release();
        SceneManager::Release();
        UIManager::Release();
        ObjectPoolManager::Release();
        Fmod::Release();
    }

    void Application::Present()
    {
        graphicDevice->Present();
    }

	void Application::SetWindow(HWND hwnd, UINT width, UINT height)
	{
        if (graphicDevice == nullptr)
        {
            mHwnd = hwnd;
            mWidth = width;
            mHeight = height;

            mResolution.x = mWidth;
            mResolution.y = mHeight;

            graphicDevice = std::make_unique<graphics::GraphicsDevice>();
            graphics::GetDevice() = graphicDevice.get();
        }

        RECT rt = { 0, 0, (LONG)width , (LONG)height };
        AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, true);
        SetWindowPos(mHwnd, nullptr, 100, 30, rt.right - rt.left, rt.bottom - rt.top, 0);
        ShowWindow(mHwnd, true);
        UpdateWindow(mHwnd);
	}

    void Application::LoadNextScene(const std::wstring& name)
    {
        if (name == L"Excavationsite")
        {
            EventManager::AddOtherEvent([]() { LoadExcavationsite(); });
        }
        else if (name == L"RewardRoom")
        {
            EventManager::AddOtherEvent([]() { LoadRewardRoom(); });
        }
    }

    void Application::LoadExcavationsite()
    {
        ObjectPoolManager::Release();
        BossArenaScene* scene = new BossArenaScene();   
        PixelWorld::CreateBossArena();
        scene->Initialize();
        SceneManager::LoadScene(L"Excavationsite", scene);
        
        scene->MovePlayer();
        ObjectPoolManager::Initialize();
    }
    void Application::LoadRewardRoom()
    {
        ObjectPoolManager::Release();
        EndScene* scene = new EndScene();
        PixelWorld::CreateEndWorld();
        scene->Initialize();
        SceneManager::LoadScene(L"Excavationsite", scene);;

        scene->MovePlayer();
        ObjectPoolManager::Initialize();
    }
}