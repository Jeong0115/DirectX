#include "zzAudioClip.h"
#include "zzTransform.h"

namespace zz
{
	AudioClip::AudioClip(FMOD_MODE mode, float min, float max)
		: Resource(eResourceType::AudioClip)
		, mSound(nullptr)
		, mChannel(nullptr)
        , mMode(mode)
		, mMinDistance(min)
		, mMaxDistance(max)
		, mbLoop(false)
	{

	}

	AudioClip::~AudioClip()
	{
		//mSound->release();
		//mSound = nullptr;
	}

	HRESULT AudioClip::Load(const std::wstring& path)
	{
		std::string cPath(path.begin(), path.end());
        if (!Fmod::CreateSound(cPath, mMode, &mSound))
        {
            assert(false);
        }

		mSound->set3DMinMaxDistance(mMinDistance, mMaxDistance);

		return S_OK;
	}

	void AudioClip::Play()
	{
		if (mbLoop)
			mSound->setMode(FMOD_LOOP_NORMAL);
		else
			mSound->setMode(FMOD_LOOP_OFF);

		Fmod::SoundPlay(mSound, &mChannel);
	}

	void AudioClip::Stop()
	{
		mChannel->stop();
	}

	void AudioClip::Set3DAttributes(const Vector3 pos, const Vector3 vel)
	{
		FMOD_VECTOR fmodPos(pos.x, pos.y, pos.z);
		FMOD_VECTOR fmodVel(vel.x, vel.y, vel.z);

		mChannel->set3DAttributes(&fmodPos, &fmodVel);
	}

	
}
