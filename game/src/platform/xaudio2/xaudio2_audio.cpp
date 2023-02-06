#include "../audio.h"

#include <xaudio2.h>
// TODO: Replace with custom assert
#include <cassert>

namespace ks
{
	struct AudioState
	{
		IXAudio2* pXAudio;
		IXAudio2MasteringVoice* pMasterVoice;
	};

	struct AudioClip
	{
		IXAudio2SourceVoice* pSourceVoice;
	};

	// NOTE: RIFFs needed to find chunks
	constexpr DWORD RIFF = 'FFIR';
	constexpr DWORD DATA = 'atad';
	constexpr DWORD FMT = ' tmf';
	constexpr DWORD WAVE = 'EVAW';

	INTERNAL void XAudio2FindChunk(HANDLE file, DWORD fourcc, DWORD& chunkSize, DWORD& chunkDataPosition)
	{
		if(SetFilePointer(file, 0, nullptr, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
		{
			// TODO: Diagnostics ...
			assert(false);
		}

		DWORD chunkType = 0;
		DWORD chunkDataSize = 0;
		DWORD riffDataSize = 0;
		DWORD fileType = 0;
		DWORD bytesRead = 0;
		DWORD offset = 0;

		HRESULT result = S_OK;
		while(result == S_OK)
		{
			DWORD read;
			if(!ReadFile(file, &chunkType, sizeof(DWORD), &read, nullptr))
			{
				// TODO: Diagnostics ...
				assert(false);
			}

			if(!ReadFile(file, &chunkDataSize, sizeof(DWORD), &read, nullptr))
			{
				// TODO: Diagnostics ...
				assert(false);
			}

			if(chunkType == RIFF)
			{
				riffDataSize = chunkDataSize;
				chunkDataSize = 4;
				if(!ReadFile(file, &fileType, sizeof(DWORD), &read, nullptr))
				{
					// TODO: Diagnostics ...
					assert(false);
				}
			}
			else
			{
				if(SetFilePointer(file, chunkDataSize, nullptr, FILE_CURRENT) == INVALID_SET_FILE_POINTER)
				{
					// TODO: Diagnostics ...
					assert(false);
				}
			}

			offset += 2 * sizeof(DWORD);

			if(chunkType == fourcc)
			{
				chunkSize = chunkDataSize;
				chunkDataPosition = offset;
				return;
			}

			offset += chunkDataSize;

			if(bytesRead >= riffDataSize)
			{
				return;
			}
		}
	}

	INTERNAL void XAudio2ReadChunkData(HANDLE file, void* buffer, DWORD buffersize, DWORD bufferoffset)
	{
		if(INVALID_SET_FILE_POINTER == SetFilePointer(file, bufferoffset, NULL, FILE_BEGIN))
		{
			// TODO: Diagnostics ...
			assert(false);
		}

		DWORD read;
		if(!ReadFile(file, buffer, buffersize, &read, NULL))
		{
			// TODO: Diagnostics ...
			assert(false);
		}
	}

	AudioState* InitAudio()
	{
		CoInitialize(nullptr);
		AudioState* state = new AudioState();

		HRESULT result = XAudio2Create(&state->pXAudio, 0U, XAUDIO2_DEFAULT_PROCESSOR);
		if(FAILED(result))
		{
			// TODO: Diagnostics ...
			assert(false);
		}

		result = state->pXAudio->CreateMasteringVoice(&state->pMasterVoice);
		if(FAILED(result))
		{
			// TODO: Diagnostics ...
			assert(false);
		}

		return state;
	}

	AudioClip* CreateAudioClip(const wchar_t* path, const AudioState& state, bool loop)
	{
		WAVEFORMATEXTENSIBLE wf = {};
		XAUDIO2_BUFFER buffer = {};

		HANDLE file = CreateFile(path, GENERIC_READ, FILE_SHARE_READ, nullptr,
			OPEN_EXISTING, 0, nullptr);

		if(INVALID_HANDLE_VALUE == file)
		{
			// TODO: Diagnostics ...
			assert(false);
		}

		if(SetFilePointer(file, 0, nullptr, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
		{
			// TODO: Diagnostics ...
			assert(false);
		}

		DWORD chunkSize;
		DWORD chunkPosition;
		XAudio2FindChunk(file, RIFF, chunkSize, chunkPosition);
		DWORD filetype;
		XAudio2ReadChunkData(file, &filetype, sizeof(DWORD), chunkPosition);
		if(filetype != WAVE)
		{
			// TODO: Diagnostics ...
			assert(false);
		}


		XAudio2FindChunk(file, FMT, chunkSize, chunkPosition);
		XAudio2ReadChunkData(file, &wf, chunkSize, chunkPosition);

		XAudio2FindChunk(file, DATA, chunkSize, chunkPosition);
		BYTE * pDataBuffer = new BYTE[chunkSize];
		XAudio2ReadChunkData(file, pDataBuffer, chunkSize, chunkPosition);

		buffer.AudioBytes = chunkSize;
		buffer.pAudioData = pDataBuffer;
		buffer.Flags = XAUDIO2_END_OF_STREAM;
		if (loop)
		{
			//int bytesPerSample = wf.Format.wBitsPerSample / 8;
			//int sampleCount = buffer.AudioBytes / bytesPerSample;
			//buffer.LoopBegin = 0;
			//buffer.LoopLength = sampleCount;
			buffer.LoopCount = XAUDIO2_LOOP_INFINITE;
		}

		AudioClip* pClip = new AudioClip();
		HRESULT result = state.pXAudio->CreateSourceVoice(&pClip->pSourceVoice, (WAVEFORMATEX*)&wf);
		if(FAILED(result))
		{
			// TODO: Diagnostics ...
			assert(false);
		}

		result = pClip->pSourceVoice->SubmitSourceBuffer(&buffer);
		if(FAILED(result))
		{
			// TODO: Diagnostics ...
			assert(false);
		}

		return pClip;
	}

	void PlayAudioClip(const AudioClip& clip)
	{
		HRESULT result = clip.pSourceVoice->Start(0);
		if(FAILED(result))
		{
			// TODO: Diagnostics ...
			assert(false);
		}
	}
}