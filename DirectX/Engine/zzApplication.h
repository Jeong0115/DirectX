#pragma once

#include "zzEngine.h"
#include "zzGraphicsDevice.h"

namespace zz
{
	class Application
	{
    public:
        SINGLETON(Application) // ; �����ݷ� �߰��ϸ� ��� ����

    private:
        Application();
        ~Application();

    public:
        void Run();
     
		void Initialize();
		void Update();
		void LateUpdate();
		void Render();
        void Release();

		void SetWindow(HWND hwnd, UINT width, UINT height);
        void SetPixelHwnd(HWND hwnd) { mPixelHwnd = hwnd; }

		UINT GetWidth() { return mWidth; }
		UINT GetHeight() { return mHeight; }
		HWND GetHwnd() { return mHwnd; }
		POINT GetResolution() { return mResolution; }

	private:
		bool mbInitialize = false;
		std::unique_ptr<zz::graphics::GraphicsDevice> graphicDevice;

		HWND		mHwnd;
		UINT		mWidth;
		UINT		mHeight;
		POINT	    mResolution;

        HWND		mPixelHwnd;
		
	};

}

