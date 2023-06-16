#include "zzApplication.h"
#include "zzSceneManger.h"
#include "zzRenderer.h"
#include "zzTime.h"
#include "zzInput.h"

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

        renderer::Initialize();
        SceneManger::GetInst().Initialize();
	}

	void Application::Update()
	{
        Time::Update();
        Input::Update();
        SceneManger::GetInst().Update();
	}

	void Application::LateUpdate()
	{
        SceneManger::GetInst().LateUpdate();
	}

	void Application::Render()
	{
        graphicDevice->Draw();

        graphicDevice->ClearRenderTarget();
        graphicDevice->UpdateViewPort();
        SceneManger::GetInst().Render();
        //graphicDevice->Draw();

        graphicDevice->Present();
	}

	void Application::SetWindow(HWND hwnd, UINT width, UINT height)
	{
        RECT rt = { 0, 0, (LONG)width , (LONG)height };
        AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, false); // true? false?
        SetWindowPos(mHwnd, nullptr, 0, 0, rt.right - rt.left, rt.bottom - rt.top, 0);
        ShowWindow(mHwnd, true);
        UpdateWindow(mHwnd);

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
	}
}