#include "zzApplication.h"
#include "zzSceneManager.h"
#include "zzRenderer.h"
#include "zzTime.h"
#include "zzInput.h"
#include "..\\Editor\\zzEditor.h"
#include "zzPixelWorld.h"
#include "zzInventoryManager.h"
#include "zzEventManager.h"
#include "zzObjectPoolManager.h"

namespace zz
{	
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

       //PixelGrid::GetInst().Initialize();
        PixelWorld::Initialize();
        renderer::Initialize();
        InventoryManager::Initialize();
        SceneManager::Initialize();
        ObjectPoolManager::Initialize();
	}

	void Application::Update()
	{
        Time::Update();
        Input::Update();
        InventoryManager::Update();
        PixelWorld::Update();
        SceneManager::Update();
	}

	void Application::LateUpdate()
	{
        InventoryManager::LateUpdate();
        SceneManager::LateUpdate();
	}

	void Application::Render()
	{
        //graphicDevice->Draw();
       
        graphicDevice->ClearRenderTarget();
        graphicDevice->UpdateViewPort();
        
       // SceneManager::GetInst().Render();
        renderer::Render();
        Editor::Run();
        Present();
        //graphicDevice->Draw();

        EventManager::Update();
	}

    void Application::Release()
    {
        ObjectPoolManager::Release();

        SceneManager::Release();
        InventoryManager::Release();
        //graphicDevice.release();
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