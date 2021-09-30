/*
	PGE_GAME_2D.h

	+-------------------------------------------------------------+
	|    OneLoneCoder Pixel Game Engine - Framework Extension     |
	|                    PGE_GAME_2D v1.0                         |
	+-------------------------------------------------------------+

	What is this?
	~~~~~~~~~~~~~
	This is a framework extension to the olcPixelGameEngine v2.16
	and above.
	
	It is, put simply, a way to implement ALL of my pixel game engine
	extensions into your project and have the default behaviours
	implemented for you.
	
	That said, you will of course need to have a copy of my latest
	PGEXs versions downloadable from my github here:
	
		https://github.com/justinrichardsmusic/PGEv2_Extensions
		
	The current list of extensions needed are as follows:
	
		olcPGEX_ResourceManager.h
		olcPGEX_SplashScreen.h
		olcPGEX_Camera2D.h
		olcPGEX_Animator2D.h
		olcPGEX_RayCast2D.h"
		olcPGEX_ScrollingTile.h
		olcPGEX_Interactable.h
		olcPGEX_Transition.h
		
		olcPGEX_AudioListener.h
		olcPGEX_AudioSource.h
		
	(NOTE) The AudioListener and AudioSource are only required if you
	intend to use audio in your project, otherwise they may cause
	compilation errors in some environments.
	
	(ADDITIONAL NOTE) You do not need to include ANY of these files
	in your main project, the PGE_GAME_2D_BACKEND will take care of
	all the necessary includes.
	
	
	What does it do?
	~~~~~~~~~~~~~~~~
	Simplifies the setup required to perform common game engine tasks.
	
	For example, games usually need a menu, a camera, animations, a
	splash screen, audio, screen transitions, raycasting, state machines,
	etc... All of these things are accessible via the GAME2D namespace
	making the organisation and use of them extremely quick and easy.


	How do I use it?
	~~~~~~~~~~~~~~~~
	Create an empty c++ project and add the following files (all found
	in this GitHub repository, except for the olcPixelGameEngine of
	course...)
	
		olcPixelGameEngine.h
		PGE_GAME_2D.h
		PGE_GAME_2D_Constants.h
		
		MAIN.cpp
		PGE_GAME_2D_Constants.cpp
	
	(NOTE) If you are using AUDIO you need to follow the linker
	instructions found in the Readme file in that repository.
	
	(ADDITIONAL NOTE) If you intend to compile for emscripten
	you should change your compiler settings to C++17 standard.
	
	Now just compile and run (x86 confirmed working).  The example
	program only displays a splash screen and nothing more, however
	the PGE_GAME_2D_BACKEND is running and you are now ready to 
	add your own code and get cracking on making a game!
	
	

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

#include "olcPixelGameEngine.h"
#include "PGE_GAME_2D_Constants.h"

#include "olcPGEX_ResourceManager.h"
#include "olcPGEX_SplashScreen.h"
#include "olcPGEX_Camera2D.h"
#include "olcPGEX_Animator2D.h"
#include "olcPGEX_RayCast2D.h"
#include "olcPGEX_ScrollingTile.h"
#define INTERACTABLE_IMPLEMENTATION
#include "olcPGEX_Interactable.h"
#define TRANSITION_IMPLEMENTATION
#include "olcPGEX_Transition.h"

#ifdef PGE_GAME_WITH_AUDIO

#define AUDIO_LISTENER_IMPLEMENTATION
#include "olcPGEX_AudioListener.h"
#define AUDIO_SOURCE_IMPLEMENTATION
#include "olcPGEX_AudioSource.h"

#endif



////////////////////////////////////
//          DECLARATIONS          //
////////////////////////////////////

namespace GAME2D
{
	class PGE_GAME_2D_BACKEND : public olc::PGEX
	{
	public:
		PGE_GAME_2D_BACKEND() {}
		~PGE_GAME_2D_BACKEND();

		// PGEX
		olcPGEX_ResourceManager				rm{};				// Resource Manager
		olcPGEX_SplashScreen				splashScreen{};			// Splash Screen
		olcPGEX_Camera2D				camera{};			// 2D camera
		olcPGEX_ScrollingTile				scrollingTile{};		// Scrolling Tile
		std::vector<olcPGEX_Transition>			transitions{};			// Transitions

	private:
		// Backend Variables
		float fElapsedTime =				0.0f;				// Cached version of main engine eplapsed time
		bool bUseScrollingTile =			false;				// Know whether scrolling tile needs to be updated
		bool bUseTransitions =				true;				// Know whether to initialise the transition PGEX

	public:
		olc::vi2d iScreenSize{};							// Integer version of the screen size
		olc::vf2d fScreenSize{};							// Floating point version of the screen size

		bool bFirstUpdateFrame =			true;				// Is this the first time the update loop has been called?
		bool bSplashScreenOn =				true;				// Tell the game engine whether to run the splash screen at startup
		bool bReturnFalseNextFrame =			false;				// Used to safely terminate the OnUserUpdate loop

		int nGameState{};								// Current game state
		int nNextGameState{};								// Game state to transition to next frame
		int nTransitionGameState =			NO_TRANSITION_STATE;		// Game state to transition to once a particular transition has finished

		// Audio
		#ifdef PGE_GAME_WITH_AUDIO
		olcPGEX_AudioListener				AL{};				// Single instance of an Audio Listener to use throughout the program
		std::vector<olcPGEX_AudioSource>		AS{};				// Vector of Audio Sources to add game sounds to
		#endif

		// Decals to display when the game has been won or lost (possibly remove from this header)
		olc::Decal* decWin =				nullptr;
		olc::Decal* decLose =				nullptr;
		olc::vf2d vecWinPos{};
		olc::vf2d vecLosePos{};
		bool bWon =					false;
		bool bLost =					false;

	public:
		// Functions
		void StartGAME2D(int screenWidth, int screenHeight, bool splashScreenOn = true, const char* defaultMenuAudio = "", bool useTransitions = true);
		void UpdateGAME2D(olc::vf2d cameraPosition, olc::vf2d clampSize = NO_CLAMP);
		void LateUpdateGAME2D();

		void AddAudioSource(int ID, const char* fileName);
		bool ForceQuit(bool quit = false);
		void SetScrollingTile(olc::vi2d tileSize, olc::Decal* decal, olc::Decal* altDecal = nullptr);
		void ToggleScrollingTile(bool state, bool swapTile = false);
		void SetWinLoseDecals(olc::Decal* win, olc::Decal* lose, bool centered = true, olc::vf2d leftAlignedPos = { 0.0f, 0.0f });
		void StartScreenTransition(int transitionID, float transitionDirection, float speed = 1.0f);
		void StopScreenTransition(int transitionID = ALL_TRANSITIONS);
		bool AnyScreenTransitionHasFinished();
	};




	////////////////////////////////////
	//         IMPLEMENTATION         //
	////////////////////////////////////

#ifdef PGE_GAME_2D
#undef PGE_GAME_2D

	void PGE_GAME_2D_BACKEND::StartGAME2D(int screenWidth, int screenHeight, bool splashScreenOn, const char* defaultMenuAudio, bool useTransitions)
	{
		std::cout << "PGE_GAME_2D backend started...\n";

		// Cache screen size values
		iScreenSize = { screenWidth, screenHeight };
		fScreenSize = { (float)iScreenSize.x, (float)iScreenSize.y };

		// Set whether to run the splash screen
		bSplashScreenOn = splashScreenOn;
		if (bSplashScreenOn)
		{
			splashScreen.SetOptions(3, 1, 3.0f, 0.0f, olc::BLACK, olc::GREEN, olc::DARK_GREEN, olc::CYAN, PGE_GAME_2D_ENABLED);
			std::cout << "Splash screen ON\n";
		}

		// Set camera to the middle of the screen by default
		camera.InitialiseCamera(fScreenSize / 2.0f, fScreenSize);
		std::cout << "Camera initialised...\n";

		// Set default game state
		nGameState = MAIN_MENU;
		nNextGameState = MAIN_MENU;

		// Audio
		#ifdef PGE_GAME_WITH_AUDIO
		AL.AudioSystemInit();
		std::cout << "SDL2 Audio initialised...\n";
		std::cout << "Default Menu Audio Sample ";

		if (defaultMenuAudio == "")
		{
			std::cout << "[NOT LOADED!]\n";
		}
		else
		{
			AddAudioSource(DEFAULT_MENU_SND, defaultMenuAudio);
			std::cout << "[loaded]\n";
		}
		#endif

		// Transitions
		bUseTransitions = useTransitions;
		if (bUseTransitions)
		{
			olcPGEX_Transition::SetDefaultTransitions(transitions, rm.RM_Sprite("./assets/white.png"));
			std::cout << "Transitions [ON] \n";
		}

		std::cout << "PGE_GAME_2D [running] - Use CTRL + SHIFT + X to Terminate\n";
	}

	void PGE_GAME_2D_BACKEND::UpdateGAME2D(olc::vf2d cameraPosition, olc::vf2d clampSize)
	{
		// Force Quit if need be
		if (ForceQuit())
			bReturnFalseNextFrame = true;

		// Set fElapsedTime here for convenience
		fElapsedTime = pge->GetElapsedTime();

		// Run splash screen and return prior to further code execution
		if (bSplashScreenOn)
		{
			bFirstUpdateFrame = false;

			if (splashScreen.AnimateSplashScreen(fElapsedTime))
				return;

			bSplashScreenOn = false;
		}

		// Update camera
		if (clampSize > NO_CLAMP)
			camera.vecCamPos = camera.LerpCamera(camera.ClampVector({ 0, 0 }, clampSize, (cameraPosition - fScreenSize / 2.0f)), camera.fSpeed);
		else
			camera.vecCamPos = camera.LerpCamera(cameraPosition - fScreenSize / 2.0f, camera.fSpeed);

		// Update Scrolling Tile
		if (bUseScrollingTile)
			scrollingTile.DrawAllTiles(camera.vecCamPos);

		bFirstUpdateFrame = false;
	}

	void PGE_GAME_2D_BACKEND::LateUpdateGAME2D()
	{
		// Process and Display Transitions
		olcPGEX_Transition::ProcessTransitions(transitions, fElapsedTime, fScreenSize);

		// Automate Game State changes when set transitions have finished
		if (AnyScreenTransitionHasFinished() && nTransitionGameState != NO_TRANSITION_STATE)
		{
			nNextGameState = nTransitionGameState;
			nTransitionGameState = NO_TRANSITION_STATE;
		}

		// Update state machine
		nGameState = nNextGameState;

		// Prepare for Exit
		if (bReturnFalseNextFrame)
			pge->Clear(olc::BLACK);

	}

	void PGE_GAME_2D_BACKEND::AddAudioSource(int ID, const char* fileName)
	{
		#ifdef PGE_GAME_WITH_AUDIO
		olcPGEX_AudioSource audioToAdd{};
		audioToAdd.nID = ID;
		audioToAdd.AL = &AL;
		audioToAdd.LoadAudioSample(ID, fileName);

		AS.push_back(audioToAdd);
		#endif
	}

	bool PGE_GAME_2D_BACKEND::ForceQuit(bool quit)
	{
		// If the user is holding down CTRL + SHIFT + X then quit PGE
		if (pge->GetKey(olc::Key::CTRL).bHeld)
		{
			if (pge->GetKey(olc::Key::SHIFT).bHeld && pge->GetKey(olc::Key::X).bPressed)
				return true;

		}

		// Checks if user supplied key has been pressed and returns appropriately
		return quit;
	}

	void PGE_GAME_2D_BACKEND::SetScrollingTile(olc::vi2d tileSize, olc::Decal* decal, olc::Decal* altDecal)
	{
		// Turn the scolling tile option on and set its values
		bUseScrollingTile = true;
		if (altDecal != nullptr)
			scrollingTile.SetTileValues(iScreenSize, tileSize, decal, altDecal);
		else
			scrollingTile.SetTileValues(iScreenSize, tileSize, decal);

		std::cout << "Scrolling Tile mode ON...\n";
	}

	void PGE_GAME_2D_BACKEND::ToggleScrollingTile(bool state, bool swapTile)
	{
		if (bUseScrollingTile != state)
		{
			bUseScrollingTile = state;

			std::cout << "Scrolling Tile mode ";
			if (bUseScrollingTile)
				std::cout << "ON...\n";
			else
				std::cout << "OFF...\n";

		}

		if (swapTile)
		{
			scrollingTile.bShowAltTile = !scrollingTile.bShowAltTile;
		
			std::cout << "Scrolling Tile [SWAPPED] \n";
		}
	}

	void PGE_GAME_2D_BACKEND::SetWinLoseDecals(olc::Decal* win, olc::Decal* lose, bool centeredHigh, olc::vf2d leftAlignedPos)
	{
		decWin = win;
		decLose = lose;

		if (centeredHigh)
		{
			vecWinPos.x = fScreenSize.x / 2.0f - decWin->sprite->width / 2.0f;
			vecWinPos.y = fScreenSize.y / 3.0f - decWin->sprite->height / 2.0f;
			vecLosePos.x = fScreenSize.x / 2.0f - decLose->sprite->width / 2.0f;
			vecLosePos.y = fScreenSize.y / 3.0f - decLose->sprite->height / 2.0f;
		}
		else
		{
			vecWinPos = leftAlignedPos;
			vecLosePos = leftAlignedPos;
		}
	}

	void PGE_GAME_2D_BACKEND::StartScreenTransition(int transitionID, float transitionDirection, float speed)
	{
		transitions[transitionID].StartSingleTransition(transitionDirection, speed);
	}

	void PGE_GAME_2D_BACKEND::StopScreenTransition(int transitionID)
	{
		if (transitionID == ALL_TRANSITIONS)
		{
			for (auto& t : transitions)
				t.bActive = false;

		}
		else
			transitions[transitionID].bActive = false;

	}

	bool PGE_GAME_2D_BACKEND::AnyScreenTransitionHasFinished()
	{
		for (auto& t : transitions)
			if (t.bTransitionFinished)
				return true;

		return false;
	}


	PGE_GAME_2D_BACKEND::~PGE_GAME_2D_BACKEND()
	{
		// Audio
		#ifdef PGE_GAME_WITH_AUDIO
		AL.AudioSystemDestroy();
		std::cout << "SDL2 Audio [destroyed]\n";
		#endif

		std::cout << "PGE_GAME_2D [destroyed]\n";
	}
}
#endif
