/*
	olcPGEX_AudioSource.h

	+-------------------------------------------------------------+
	|         OneLoneCoder Pixel Game Engine Extension            |
	|                     AudioSource v1.0                        |
	+-------------------------------------------------------------+

	What is this?
	~~~~~~~~~~~~~
	This is an extension to the olcPixelGameEngine v2.16 and above.

	It is to be used in conjunction with olcPGEX_AudioListener.h.

	A detailed description and instructions can be found in that
	header file, please refer to it instead :-)
	
	Enjoy!



	License (OLC-3)
	~~~~~~~~~~~~~~~

	Copyright 2018 - 2019 OneLoneCoder.com

	Redistribution and use in source and binary forms, with or without
	modification, are permitted provided that the following conditions
	are met:

	1. Redistributions or derivations of source code must retain the above
	copyright notice, this list of conditions and the following disclaimer.

	2. Redistributions or derivative works in binary form must reproduce
	the above copyright notice. This list of conditions and the following
	disclaimer must be reproduced in the documentation and/or other
	materials provided with the distribution.

	3. Neither the name of the copyright holder nor the names of its
	contributors may be used to endorse or promote products derived
	from this software without specific prior written permission.

	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
	"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
	LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
	A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
	HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
	SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
	LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
	DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
	THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
	(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
	OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

	Author
	~~~~~~
	Justin Richards

*/

#ifndef OLC_PGEX_AUDIO_SOURCE
#define OLC_PGEX_AUDIO_SOURCE

#pragma once
#include "olcPGEX_AudioListener.h"

class olcPGEX_AudioSource : public olc::PGEX
{
public:
	// Pointer to the Audio Listener for this object
	olcPGEX_AudioListener* AL;

	// Handle for this particular copy of the sound
	int handle = 255;

	// Maximum copies of this sound allowed for this audio source
	//const int nMaxSamples = 4;
	//int nSampleHandles[4] = { 255, 255, 255, 255 };

	// Audio Sample ID used to locate which sound to play
	int nID;

	// Convenient BOOL to determine playback status
	bool bIsPlaying = false;

	// Current Playback Speed
	float fPlaySpeed = 1.0f;

	// Volume
	float fVolume = 1.0f;
	float fMinVolume = 0.0f;
	float fMaxVolume = 1.0f;

	// Does the Audio Sample Loop?
	bool bLooping = false;

	// Paused status
	bool bPaused = false;

	// Object position, used for calculating volume
	olc::vf2d pos = { 0.0f, 0.0f };



	// Instruct Audio Listener to load this sound (if not loaded already)
	inline void LoadAudioSample(int ID, const char* fileName);

	// Play the Audio Sample, with given parameters
	inline void Play(float speed = 1.0f, float vol = 1.0f, bool looping = false, bool paused = false);

	// Pause or Un-Pause - maintains the playback position and handle
	inline void Pause(bool pauseState = true);

	// Stop - playback position and handle will be lost
	inline void Stop();

	// Audio Modulation - control the speed of playback
	inline void ModulateAudio(float minPlaySpeed, float maxPlaySpeed, float modulation, bool precise = false, bool deferred = false);

	// Adjust Volume
	inline void SetVolume(float vol, float minVol = 0.0f, float maxVol = 1.0f);

	// Set Default Parameters
	inline void SetDefaults(float speed, float vol, float minVol, float maxVol, bool looping);
};

#ifdef AUDIO_SOURCE_IMPLEMENTATION
#undef AUDIO_SOURCE_IMPLEMENTATION


void olcPGEX_AudioSource::LoadAudioSample(int ID, const char* fileName)
{
	// Link the IDs together
	nID = ID;

	// Call the Audio Listener to load the sample
	AL->LoadAudioSample(ID, fileName);
}

void olcPGEX_AudioSource::Play(float speed, float vol, bool looping, bool paused)
{
	// Set parameters
	fPlaySpeed = speed;
	fVolume = vol;
	bLooping = looping;
	bPaused = paused;

	// Assign a handle to this instance of the sound we are about to play
	handle = AL->soloud.play(*AL->GetAudioSampleByID(nID)->wav, fVolume, 1.0f, bPaused);

	// Set speed and looping 
	AL->soloud.setRelativePlaySpeed(handle, fPlaySpeed);
	AL->soloud.setLooping(handle, looping);

	// Update Play status
	bIsPlaying = true;
}

void olcPGEX_AudioSource::Pause(bool pauseState)
{
	// Use the Audio Listener to pause or un-pause the sound as neccessary
	AL->soloud.setPause(handle, pauseState);

	// Update Play status
	bIsPlaying = !pauseState;
}

void olcPGEX_AudioSource::Stop()
{
	// Use the Audio Listener to stop the sound
	AL->soloud.stop(handle);

	// The current handle will now point to nothing, so we set it to MAX so we can test for validity if need be
	handle = 255;

	// Update Play status
	bIsPlaying = false;
}

void olcPGEX_AudioSource::ModulateAudio(float minPlaySpeed, float maxPlaySpeed, float modulation, bool precise, bool deferred)
{
	// Apply the modulation
	if (precise)
		fPlaySpeed = modulation;
	else
		fPlaySpeed += modulation;

	// Adjust the play speed to keep it within range
	if (fPlaySpeed < minPlaySpeed) fPlaySpeed = minPlaySpeed;
	else if (fPlaySpeed > maxPlaySpeed) fPlaySpeed = maxPlaySpeed;

	// If multiple adjustments to the playback speed are required, then the Audio Listener update itself can be
	// deferred until the very last adjustment is made... A small optimisation
	if (!deferred)
		AL->soloud.setRelativePlaySpeed(handle, fPlaySpeed);
}

void olcPGEX_AudioSource::SetVolume(float vol, float minVol, float maxVol)
{
	// Set volume
	fVolume = vol;

	// Clamp volume withing set bounds
	if (fVolume < minVol)
		fVolume = minVol;
	else if (fVolume > maxVol)
		fVolume = maxVol;

	// Instruct the Audio Listener to apply the volume change
	AL->soloud.setVolume(handle, fVolume);
}

void olcPGEX_AudioSource::SetDefaults(float speed, float vol, float minVol, float maxVol, bool looping)
{
	// Set defaults for this particular Audio Source
	fPlaySpeed = speed;
	fVolume = vol;
	fMinVolume = minVol;
	fMaxVolume = maxVol;
	bLooping = looping;
}

#endif // AUDIO_SOURCE_IMPLEMENTATION
#endif
