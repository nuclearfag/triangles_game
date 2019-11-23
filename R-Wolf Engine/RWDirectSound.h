#pragma once
#define method inline auto

#include <ctime>
#include <mmsystem.h>
#include <dsound.h>
#include <dmusics.h>

namespace RWDirectSound 
{
	DECLSPEC_SELECTANY IDirectSound8* dsound;
	DECLSPEC_SELECTANY IDirectSoundBuffer* primaryBuffer;
	DECLSPEC_SELECTANY IDirectSoundBuffer8* secondaryBuffer;
	DECLSPEC_SELECTANY DSBUFFERDESC dsoundbdsc;
	DECLSPEC_SELECTANY WAVEFORMATEX wfex;
	DECLSPEC_SELECTANY char* testPtr;
	DECLSPEC_SELECTANY DWORD testSize;
	enum Errors 
	{
		DSOUND_DSCREATE8_ERROR = 12,
		DSOUND_COOPERATIVELEVEL_ERROR,
		DSOUND_PRIMARYBUFFER_ERROR,
		DSOUND_WAVEFORMAT_ERROR,
		DSOUND_PRIMARY_PLAY_ERROR,
		DSOUND_PRIMARY_STOP_ERROR,
		DSOUND_SECONDBUFFER_ERROR,
		DSOUND_SECONDARY_PLAY_ERROR,
		DSOUND_SECONDARY_STOP_ERROR,
		DSOUND_SECONDARY_LOCK_ERROR,
		DSOUND_SECONDARY_UNLOCK_ERROR
	};
	method play(DWORD reserved1, DWORD priority, DWORD flags)
	{
		HRESULT debug = 0L;
		debug = primaryBuffer->Play(reserved1, priority, flags);
		if (SUCCEEDED(debug))
		{
			return true;
		}
		else 
		{
			auto errorcode = DSOUND_PRIMARY_PLAY_ERROR;
			string definition = "DSOUND_PRIMARY_PLAY_ERROR";
			RW_ERROR err(errorcode, definition, 1);
			delete& err;
			return false;
		}
	}

	method stop() 
	{
		HRESULT debug = 0L;
		debug = primaryBuffer->Stop();
		if (SUCCEEDED(debug)) 
		{
			return true;
		}
		else 
		{
			auto errorcode = DSOUND_PRIMARY_STOP_ERROR;
			string definition = "DSOUND_PRIMARY_STOP_ERROR";
			RW_ERROR err(errorcode, definition, 1);
			delete& err;
			return false;
		}
	}

	method initialize()
	{
		HRESULT debug = 0L;
		debug = DirectSoundCreate8(NULL, &dsound, NULL);
		if (SUCCEEDED(debug)) 
		{
			debug = dsound->SetCooperativeLevel(GetForegroundWindow(), DSSCL_PRIORITY);
			if (SUCCEEDED(debug)) 
			{
				ZeroMemory(&dsoundbdsc, sizeof(DSBUFFERDESC));
				dsoundbdsc.dwSize = sizeof(DSBUFFERDESC);
				dsoundbdsc.dwFlags = DSBCAPS_PRIMARYBUFFER | DSBCAPS_CTRLVOLUME;
				dsoundbdsc.dwBufferBytes = 0;
				dsoundbdsc.lpwfxFormat = NULL;
				debug = dsound->CreateSoundBuffer(&dsoundbdsc, &primaryBuffer, NULL);
				if (SUCCEEDED(debug)) 
				{
					ZeroMemory(&wfex, sizeof(WAVEFORMATEX));
					wfex.wFormatTag = WAVE_FORMAT_PCM;
					wfex.nChannels = 2;
					wfex.nSamplesPerSec = 44100;
					wfex.wBitsPerSample = 16;
					wfex.nBlockAlign = (wfex.wBitsPerSample / 8) * wfex.nChannels;
					wfex.nAvgBytesPerSec = wfex.nSamplesPerSec * wfex.nBlockAlign;
					debug = primaryBuffer->SetFormat(&wfex);
					if (SUCCEEDED(debug)) 
					{
						play(NULL, NULL, DSBPLAY_LOOPING);
						return true;
					}
					else 
					{
						auto errorcode = DSOUND_WAVEFORMAT_ERROR;
						string definition = "DSOUND_WAVEFORMAT_ERROR";
						RW_ERROR err(errorcode, definition, 1);
						delete& err;
						return false;
					}
				}
				else 
				{
					auto errorcode = DSOUND_PRIMARYBUFFER_ERROR;
					string definition = "DSOUND_PRIMARYBUFFER_ERROR";
					RW_ERROR err(errorcode, definition, 1);
					delete& err;
					return false;
				}
			}
			else 
			{
				auto errorcode = DSOUND_COOPERATIVELEVEL_ERROR;
				string definition = "DSOUND_COOPERATIVELEVEL_ERROR";
				RW_ERROR err(errorcode, definition, 1);
				delete& err;
				return false;
			}
		}
		else
		{
			auto errorcode = DSOUND_DSCREATE8_ERROR;
			string definition = "DSOUND_DSCREATE8_ERROR";
			RW_ERROR err(errorcode, definition, 1);
			delete& err;
			return false;
		}
	}
	
	method initSecondBuffer(float seconds) 
	{
		HRESULT debug = 0L;
		IDirectSoundBuffer* temp;
		WAVEFORMATEX wf;
		ZeroMemory(&wf, sizeof(WAVEFORMATEX));
		wf.wFormatTag = WAVE_FORMAT_PCM;
		wf.nChannels = 2;
		wf.nSamplesPerSec = 44100;
		wf.wBitsPerSample = 16;
		wf.nBlockAlign = (wf.wBitsPerSample / 8) * wf.nChannels;
		wf.nAvgBytesPerSec = wf.nSamplesPerSec * wf.nBlockAlign;
		
		DSBUFFERDESC ds;
		ZeroMemory(&ds, sizeof(DSBUFFERDESC));
		ds.dwSize = sizeof(DSBUFFERDESC);
		ds.dwFlags = DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLPAN;
		ds.dwBufferBytes = wfex.nAvgBytesPerSec * seconds;
		ds.lpwfxFormat = &wf;

		debug = dsound->CreateSoundBuffer(&ds, &temp, NULL);
		if (SUCCEEDED(debug)) 
		{
			debug = temp->QueryInterface(IID_IDirectSoundBuffer8, (void**) &secondaryBuffer);
			if (SUCCEEDED(debug)) 
			{
				temp->Release();
				return true;
			}
			else 
			{
				temp->Release();
				auto errorcode = DSOUND_SECONDBUFFER_ERROR;
				string definition = "DSOUND_SECONDBUFFER_ERROR";
				RW_ERROR err(errorcode, definition, 1);
				delete& err;
				return false;
			}
		}
		else {
			auto errorcode = DSOUND_SECONDBUFFER_ERROR;
			string definition = "DSOUND_SECONDBUFFER_ERROR";
			RW_ERROR err(errorcode, definition, 1);
			delete& err;
			return false;
		}
	}

	method lock(void* auPtr1, DWORD auBytes1, void* auPtr2, DWORD auBytes2)
	{
		HRESULT debug = 0L;
		debug = secondaryBuffer->Lock(0, 0, (void**) &auPtr1, (DWORD*) &auBytes1, (void**) &auPtr2, (DWORD*) &auBytes2, NULL);
		if (SUCCEEDED(debug)) 
		{
			return true;
		}
		else 
		{
			auto errorcode = DSOUND_SECONDARY_LOCK_ERROR;
			string definition = "DSOUND_SECONDARY_LOCK_ERROR";
			RW_ERROR err(errorcode, definition, 1);
			delete& err;
			return false;
		}
	}

	method unlock(void* auPtr1, DWORD auBytes1, void* auPtr2, DWORD auBytes2) 
	{
		HRESULT debug = 0L;
		debug = secondaryBuffer->Unlock(auPtr1, auBytes1, auPtr2, auBytes2);
		if (SUCCEEDED(debug)) 
		{
			return true;
		}
		else 
		{
			auto errorcode = DSOUND_SECONDARY_LOCK_ERROR;
			string definition = "DSOUND_SECONDARY_LOCK_ERROR";
			RW_ERROR err(errorcode, definition, 1);
			delete& err;
			return false;
		}
	}

	method test()
	{
		lock(testPtr, testSize, NULL, 0);
		for (long iter = 0; iter < testSize; iter++) 
		{
			testPtr[iter] = 65535;
		}
		unlock(testPtr, testSize, NULL, 0);
		secondaryBuffer->SetCurrentPosition(0);
		secondaryBuffer->SetVolume(0);
		secondaryBuffer->Play(NULL, NULL, DSBPLAY_LOOPING);
		secondaryBuffer->Stop();
	}
}