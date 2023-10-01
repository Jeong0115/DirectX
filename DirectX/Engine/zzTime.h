#pragma once
#include "zzEngine.h"

#ifdef _DEBUG
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#else
#define DBG_NEW new
#endif

namespace zz
{   
	class Time
	{   
	public:
		static void Initialize();
		static void Update();

		__forceinline static double DeltaTime() { return mDeltaTime; }

	private:
		static LARGE_INTEGER mCpuFreq;
		static LARGE_INTEGER mPrevFreq;
		static LARGE_INTEGER mCurFreq;
		static double mDeltaTime;
		static double mSecond;
	};
}

