#pragma once

#include "zzEngine.h"
#include "zzGraphicsDevice.h"

#ifdef _DEBUG
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#else
#define DBG_NEW new
#endif

namespace zz
{
	class Application
	{
    public:
        SINGLETON(Application);

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
        void Present();

		void SetWindow(HWND hwnd, UINT width, UINT height);
        void SetPixelHwnd(HWND hwnd) { mPixelHwnd = hwnd; }

        static void LoadNextScene(const std::wstring& name);
        static void LoadExcavationsite();

		UINT GetWidth() { return mWidth; }
		UINT GetHeight() { return mHeight; }
		HWND GetHwnd() { return mHwnd; }
		POINT GetResolution() { return mResolution; }

        static bool OnDebugMode;
        static bool LightDisabled;
        static bool IsDrawBox2d;

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

