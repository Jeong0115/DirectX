#include "zzApplication.h"
#include "zzSceneManger.h"
#include "zzResourceManager.h"
#include "zzRenderer.h"
#include "zzTime.h"
#include "zzInput.h"
#include "zzPixelGrid.h"

namespace zz
{	
	Application::Application()
        : mHwnd(NULL)
        , mResolution{}
        , mWidth(-1), mHeight(-1)
        , graphicDevice(nullptr)
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
        renderer::Initialize();
        SceneManger::GetInst().Initialize();
	}

	void Application::Update()
	{
        Time::Update();
        Input::Update();
        SceneManger::GetInst().Update();
        PixelGrid::GetInst().Update();
	}

	void Application::LateUpdate()
	{
        SceneManger::GetInst().LateUpdate();
	}

	void Application::Render()
	{
        //graphicDevice->Draw();

        graphicDevice->ClearRenderTarget();
        graphicDevice->UpdateViewPort();
        SceneManger::GetInst().Render();
        //graphicDevice->Draw();

        graphicDevice->Present();
        PixelGrid::GetInst().Render();
	}

    void Application::Release()
    {
        SceneManger::GetInst().Release();
        //graphicDevice.release();
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
        SetWindowPos(mHwnd, nullptr, 30, 30, rt.right - rt.left, rt.bottom - rt.top, 0);
        ShowWindow(mHwnd, true);
        UpdateWindow(mHwnd);
	}
}