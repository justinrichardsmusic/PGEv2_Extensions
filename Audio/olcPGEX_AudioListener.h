/*
	olcPGEX_AudioListener.h

	+-------------------------------------------------------------+
	|         OneLoneCoder Pixel Game Engine Extension            |
	|                    AudioListener v1.0                       |
	+-------------------------------------------------------------+

	What is this?
	~~~~~~~~~~~~~
	This is an extension to the olcPixelGameEngine v2.16 and above.

	Its purpose is to allow audio integration into PGE which is
	compatible with Emscripten for Web applications also.

	Unlike my other extensions, this one requires some external
	libraries and additional setup (refer to github for instructions).

	In addition to the libraries the following files are required:

	olcPGEX_AudioListener.h      (this file)
	olcPGEX_AudioSource.h
	PGE_GAME_2D_Constants.h

	These can be found in the github repo as well...


	What it can do:
	~~~~~~~~~~~~~~~
	Play Audio!

	Using SDL2 as a backend and SoLoud as a frontend, this extension allows you
	to easily load and manipulate sound in a way that is cross platform and also
	has web support (using emscripten for PGE).

	In addition to the standard PLAY, PAUSE, and STOP controls you can also adjust
	VOLUME settings and MODULATION (playback speed).

	WAV and OGG are supported on most backends, however MP3 is also supported by
	SoLoud and SDL2 (even inside the web browser!).


	Limitations:
	~~~~~~~~~~~~
	Requires SDL2, SDL2_Mixer, and SoLoud libraries be installed and linked
	successfully (detailed instructions on the github repo).


	How Does It Work?
	~~~~~~~~~~~~~~~~~
	Once you have followed the setup instructions on the github repo and
	successfully compiled the test program you are ready to follow these
	instructions...

	Add the following includes underneath your olcPixelGameEngine include

		#include "PGE_GAME_2D_Constants.h"
		#include "olcPGEX_AudioListener.h"
		#include "olcPGEX_AudioSource.h"

	(Order matters here, they must be included exactly as above!)

	In your declarations add exactly (1) AudioListener

		olcPGEX_AudioListener AL{};

	
	(Note: Currently, only 1 instance of an AudioListener is permitted)

	Now add at least (1) AudioSource (you will eventually add many of these)

		olcPGEX_AudioSource AS_Test{};


	In the OnUserCreate function you must now initialise the AudioListener

		AL.AudioSystemInit();


	Next we can assign our AudioListener to our AudioSource and load an audio file

		AS_Test.AL = &game.AL;
		AS_Test.LoadAudioSample(GAME2D::AUDIO::TEST_SND, "./assets/mus/Test.mp3");


	This assumes you have an MP3 file called "Test.mp3" in the listed folder in your
	project directory.  Also note the "GAME2D::AUDIO::TEST_SND" label in this example.
	This is part of an Enum in the PGE_GAME_2D_CONSTANS.h file.  If you haven't included
	this file then you will get an error here.  This file is included for convenience
	to assign labels to your sounds in order to keep track of them more easily...

	(Note: all that is required in the first argument it an integer value, however it is
	 recommended that you assign names to the integer values (IDs) in order to be able
	 to keep track of the audio files as it is likely that there will be many of them)
	

	Now all that is left to do is play the sound...

	In OnUserUpdate we can play / stop the sound using the SPACEBAR like so

		if (GetKey(olc::Key::SPACE).bPressed)
		{
			if (AS_Test.bIsPlaying)
				AS_Test.Stop();
			else
				AS_Test.Play();

		}


	Those are the basics... other features can be accessed in much the same way.

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

#pragma once

#ifndef OLC_PGEX_AUDIO_LISTENER
#define OLC_PGEX_AUDIO_LISTENER

#include "soloud.h"
#include "soloud_wav.h"

class olcPGEX_AudioListener : public olc::PGEX
{
public:
	// Struct to keep the Audio Sample data together
	struct sAudioSample
	{
		sAudioSample(int ID, SoLoud::Wav* wavPtr)
		{
			nSampleID = ID;
			wav = wavPtr;
		}

		int nSampleID = GAME2D::AUDIO::NULL_SND; // requires PGE_GAME_2D_Constants.h to be included (or just modify this to suit your own needs...)
		SoLoud::Wav* wav;
	};

	// SoLoud Audio Engine Object
	SoLoud::Soloud soloud;

	// Position used for volume calculations
	olc::vf2d vecPos = { 0.0f, 0.0f };

	// Global volume settings
	float fMusicVolume = 0.2f;
	float fEngineVolume = 0.1f;
	float fSoundFXVolume = 0.4f;

	bool bMusicOn = true;
	bool bSoundOn = true;

	// Vector of Audio Samples
	std::vector<sAudioSample> audioSamples;
	std::list<SoLoud::Wav> wavs;



	// Initialise the Audio Engine, and Destroy it when done
	inline void AudioSystemInit();
	inline void AudioSystemDestroy();

	// Load a file and store it in the list of wavs
	inline void LoadAudioSample(int ID, const char* fileName);

	// Identify a particular Audio Sample based on its ID
	inline sAudioSample* GetAudioSampleByID(int ID);

	// Update the spacial position of the Audio Listener
	inline void UpdatePosition(olc::vf2d pos);

	// Calculate distance between listener and source
	inline float GetDistance(olc::vf2d sourcePos, bool returnRoot = true);
};

void olcPGEX_AudioListener::AudioSystemInit()
{
	// Initialise the SoLoud backend
	soloud.init();
}

void olcPGEX_AudioListener::AudioSystemDestroy()
{
	// Clean up the SoLoud engine
	soloud.deinit();
}

void olcPGEX_AudioListener::LoadAudioSample(int ID, const char* fileName)
{
	// Search for any matching IDs
	for (auto& a : audioSamples)
		if (a.nSampleID == ID) return; // Audio Sample is already loaded, no need to load the same file twice!

	// Add a new wav to the list of wavs and get a pointer to it
	wavs.push_back(SoLoud::Wav());
	SoLoud::Wav* wavPtr = &wavs.back();

	// Use the pointer to load the file into the back of the wav list
	wavPtr->load(fileName);

	// Create a new Audio sample object in the vector of samples that links both the ID and wav file itself, for convenience
	audioSamples.emplace_back(sAudioSample(ID, wavPtr));
}

olcPGEX_AudioListener::sAudioSample* olcPGEX_AudioListener::GetAudioSampleByID(int ID)
{
	// Look for matching IDs
	for (auto& a : audioSamples)
		if (ID == a.nSampleID) return &a; // Match found! Return it...

	// No match found, need to return a reference so we return the first sample in the list
	return &audioSamples[0];
}

void olcPGEX_AudioListener::UpdatePosition(olc::vf2d pos)
{
	// Position
	vecPos = pos;
}

float olcPGEX_AudioListener::GetDistance(olc::vf2d sourcePos, bool returnRoot)
{
	// Return the distance via square root if needed, or the squared version when optimisation is possible
	if (returnRoot)
		return sqrtf(abs(sourcePos.x * sourcePos.x - vecPos.x * vecPos.x) + abs(sourcePos.y * sourcePos.y - vecPos.y * vecPos.y));
	else
		return abs(sourcePos.x * sourcePos.x - vecPos.x * vecPos.x) + abs(sourcePos.y * sourcePos.y - vecPos.y * vecPos.y);

}

#endif