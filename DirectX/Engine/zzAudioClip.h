#pragma once
#include "zzResource.h"
#include "zzFmod.h"



//#include <Fmod/fmod_studio.hpp>
//#include <Fmod/fmod.hpp>
//#include <Fmod/fmod_common.h>
//#include <Fmod/fmod_codec.h>

//#include "..\External\Include\\DirectXTex\DirectXTex.h"

namespace zz
{
	using namespace zz::math;
	class AudioClip : public Resource
	{
	public:
		AudioClip(FMOD_MODE mode = FMOD_3D, float min = 1.0f, float max = 1000.0f);
		~AudioClip();

		virtual HRESULT Load(const std::wstring& path) override;

		void Play();
		void Stop();
		void Set3DAttributes(const Vector3 pos, const Vector3 vel);
		void SetLoop(bool loop) { mbLoop = loop; }
        void SetVolume(float volume) { mChannel->setVolume(volume); }

	private:
		FMOD::Sound* mSound;
		FMOD::Channel* mChannel;

        FMOD_MODE mMode;

		float mMinDistance;
		float mMaxDistance;
		bool mbLoop;
	};
}

