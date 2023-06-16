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

		void SetWindow(HWND hwnd, UINT width, UINT height);

		UINT GetWidth() { return mWidth; }
		UINT GetHeight() { return mHeight; }
		HWND GetHwnd() { return mHwnd; }
		POINT GetResolution() { return mResolution; }

	private:
		bool mbInitialize = false;
		// ������ �Ѱ��� ��ü�� ����� �ִ� ����Ʈ ������
		std::unique_ptr<zz::graphics::GraphicsDevice> graphicDevice;

		// HDC mHdc; -> GPU API
		HWND		mHwnd;
		UINT		mWidth;
		UINT		mHeight;
		POINT	    mResolution;
		
	};

}

