#pragma once

namespace ks
{
	struct AudioState;
	struct AudioClip;

	AudioState* InitAudio();
	AudioClip* CreateAudioClip(const wchar_t* path, const AudioState& state, bool loop);
	void PlayAudioClip(const AudioClip& clip);
}