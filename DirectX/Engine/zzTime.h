#pragma once
#include "zzEngine.h"

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

