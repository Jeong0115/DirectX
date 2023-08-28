#include "zzTime.h"
#include "zzApplication.h"

namespace zz
{
	double Time::mDeltaTime = 0.f;
	double Time::mSecond = 0.f;
	LARGE_INTEGER Time::mCpuFreq = {};
	LARGE_INTEGER Time::mPrevFreq = {};
	LARGE_INTEGER Time::mCurFreq = {};

	void Time::Initialize()
	{
		QueryPerformanceFrequency(&mCpuFreq);
		QueryPerformanceCounter(&mPrevFreq);
	}

	void Time::Update()
	{
		QueryPerformanceCounter(&mCurFreq);

		mDeltaTime = (double)(mCurFreq.QuadPart - mPrevFreq.QuadPart) / (double)mCpuFreq.QuadPart;

		mPrevFreq = mCurFreq;

		mSecond += mDeltaTime;

		if (mSecond >= 1.f)
		{
			HWND hWnd = Application::GetInst().GetHwnd();

			wchar_t szFPS[100] = {};
			float FPS = 1.f / (float)mDeltaTime;
			swprintf_s(szFPS, 100, L"FPS : %d,    DT : %f", (UINT)FPS, mDeltaTime);
			SetWindowText(hWnd, szFPS);

			mSecond = 0.f;
		}

#ifdef _DEBUG
		if (mDeltaTime > (1. / 30.))
			mDeltaTime = (1. / 30.);
#endif

	}

}