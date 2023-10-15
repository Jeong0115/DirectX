#include "zzAudioSource.h"
#include "zzAudioClip.h"
#include "zzTransform.h"
#include "zzGameObject.h"
#include "zzResourceManager.h"

namespace zz
{
	AudioSource::AudioSource()
		: Component(eComponentType::AudioSource)
		, mAudioClip(nullptr)
	{

	}

	AudioSource::~AudioSource()
	{

	}

	void AudioSource::Initialize()
	{
	}

	void AudioSource::Update()
	{
	}   

	void AudioSource::LateUpdate()
	{
        if (mAudioClip != nullptr)
        {
            Transform* tr = GetOwner()->GetComponent<Transform>();
            Vector3 pos = tr->GetWorldPosition();
            pos.z = 0.0f;

            //Vector3 foward = tr->Foward();

            Vector3 vel = Vector3::Zero;

            mAudioClip->Set3DAttributes(pos, vel);
        }
	}

	void AudioSource::Render()
	{
	}

	void AudioSource::Play()
	{
		mAudioClip->Play();
	}
	void AudioSource::Stop()
	{
		mAudioClip->Stop();
	}
	void AudioSource::SetLoop(bool loop)
	{
		mAudioClip->SetLoop(loop);
	}

    void AudioSource::SetClip(const std::wstring audio_name)
    {
        mAudioClip = ResourceManager::Find<AudioClip>(audio_name);
    }
}