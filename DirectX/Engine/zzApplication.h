#pragma once

#include "zzEngine.h"
#include "zzGraphicsDevice.h"

namespace zz
{
	class Application
	{
    public:
        SINGLETON(Application) // ; 세미콜론 추가하면 녹색 밑줄

    private:
        Application();
        ~Application();

    public:
        void Run();
     
		void Initialize();
		void Update();
		void LateUpdate();
		void Render();

		void SetWindow(HWND hwnd, UINT width, UINT height);

		UINT GetWidth() { return mWidth; }
		UINT GetHeight() { return mHeight; }
		HWND GetHwnd() { return mHwnd; }
		POINT GetResolution() { return mResolution; }

	private:
		bool mbInitialize = false;
		// 오로지 한개의 객체만 만들수 있는 스마트 포인터
		std::unique_ptr<zz::graphics::GraphicsDevice> graphicDevice;

		// HDC mHdc; -> GPU API
		HWND		mHwnd;
		UINT		mWidth;
		UINT		mHeight;
		POINT	    mResolution;
		
	};

}

