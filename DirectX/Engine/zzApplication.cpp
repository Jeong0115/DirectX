#include "zzApplication.h"
#include "zzSceneManager.h"
#include "zzRenderer.h"
#include "zzTime.h"
#include "zzInput.h"
#include "..\\Editor\\zzEditor.h"
#include "zzPixelWorld.h"
#include "zzUIManager.h"
#include "zzEventManager.h"
#include "zzObjectPoolManager.h"
#include "zzCollisionManger.h"
#include "zzBox2dWorld.h"
#include "zzWrite.h"

namespace zz
{	
    bool Application::OnDebugMode = false;
    bool Application::LightDisabled = false;

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
        Time::Initialize();
        Input::Initialize();
        
        PixelWorld::Initialize();

        renderer::Initialize();
        WriteManager::Initialize();
        UIManager::Initialize();
        SceneManager::Initialize();
        ObjectPoolManager::Initialize();

	}

	void Application::Update()
	{
        Time::Update();
        Input::Update();

        if (Input::GetKey(eKeyCode::CTRL) && Input::GetKeyDown(eKeyCode::F5))
        {
            OnDebugMode = !OnDebugMode;
        }

        if (Input::GetKey(eKeyCode::CTRL) && Input::GetKeyDown(eKeyCode::F8))
        {
            LightDisabled = !LightDisabled;
        }

        UIManager::Update();
        PixelWorld::Update();
        SceneManager::Update();
        CollisionManger::Update();
	}

	void Application::LateUpdate()
	{
        //Camera* mainCamara = renderer::mainCamera;
        //Camera::SetGpuViewMatrix(mainCamara->GetViewMatrix());
        //Camera::SetGpuProjectionMatrix(mainCamara->GetProjectionMatrix());
        //renderer::TransformCB trCB = {};
        //trCB.mView = Camera::GetGpuViewMatrix();
        //trCB.mProjection = Camera::GetGpuProjectionMatrix();

        if (!LightDisabled)
        {
            graphicDevice->SetLightMapRenderTarget();
        }

        UIManager::LateUpdate();
        SceneManager::LateUpdate();
	}

	void Application::Render()
	{    
        graphicDevice->ClearRenderTarget();
        graphicDevice->UpdateViewPort();
        
        renderer::Render();

        Camera* mainCamara = renderer::mainCamera;
        Camera::SetGpuViewMatrix(mainCamara->GetViewMatrix());
        Camera::SetGpuProjectionMatrix(mainCamara->GetProjectionMatrix());

        if(OnDebugMode)
        {
            Box2dWorld::Render();
            Editor::Run();
        }
        Present();

        EventManager::Update();
	}

    void Application::Release()
    {
        SceneManager::Release();
        UIManager::Release();
        ObjectPoolManager::Release();
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
        AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, true); // true? false?
        SetWindowPos(mHwnd, nullptr, 30, -1000, rt.right - rt.left, rt.bottom - rt.top, 0);
        ShowWindow(mHwnd, true);
        UpdateWindow(mHwnd);
	}
}