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
		olcPGEX_ResourceManager				rm{};					// Resource Manager
		olcPGEX_SplashScreen				splashScreen{};			// Splash Screen
		olcPGEX_Camera2D					camera{};				// 2D camera
		olcPGEX_ScrollingTile				scrollingTile{};		// Scrolling Tile
		std::vector<olcPGEX_Transition>		transitions{};			// Transitions

	private:
		// Backend Variables
		float fElapsedTime =				0.0f;					// Cached version of main engine eplapsed time
		bool bUseScrollingTile =			false;					// Know whether scrolling tile needs to be updated
		bool bUseTransitions =				true;					// Know whether to initialise the transition PGEX

	public:
		olc::vi2d iScreenSize{};									// Integer version of the screen size
		olc::vf2d fScreenSize{};									// Floating point version of the screen size

		bool bFirstUpdateFrame =			true;					// Is this the first time the update loop has been called?
		bool bSplashScreenOn =				true;					// Tell the game engine whether the run the splash screen at startup
		bool bReturnFalseNextFrame =		false;					// Used to safely terminate the OnUserUpdate loop

		int nGameState{};											// Current game state
		int nNextGameState{};										// Game state to transition to next frame

		// Audio
		#ifdef PGE_GAME_WITH_AUDIO
		olcPGEX_AudioListener				AL{};					// Single instance of an Audio Listener to use throughout the program
		std::vector<olcPGEX_AudioSource>	AS{};					// Vector of Audio Sources to add game sounds to
		#endif

		// Decals to display when the game has been won or lost (possibly remove from this header)
		olc::Decal* decWin =				nullptr;
		olc::Decal* decLose =				nullptr;
		olc::vf2d vecWinPos{};
		olc::vf2d vecLosePos{};
		bool bWon =							false;
		bool bLost =						false;

	public:
		// Functions
		void StartGAME2D(int screenWidth, int screenHeight, bool splashScreenOn = true, const char* defaultMenuAudio = "", bool useTransitions = true);
		void UpdateGAME2D(olc::vf2d cameraPosition, olc::vf2d clampSize = NO_CLAMP);
		void LateUpdateGAME2D(float elapsedTime);

		void AddAudioSource(int ID, const char* fileName);
		bool ForceQuit(bool quit = false);
		void SetScrollingTile(olc::vi2d tileSize, olc::Decal* decal, olc::Decal* altDecal = nullptr);
		void ToggleScrollingTile(bool state, bool swapTile = false);
		void SetWinLoseDecals(olc::Decal* win, olc::Decal* lose, bool centered = true, olc::vf2d leftAlignedPos = { 0.0f, 0.0f });
		void StartScreenTransition(int transitionID, float transitionDirection, float speed = 1.0f);
		void StopScreenTransition(int transitionID = ALL_TRANSITIONS);
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

	void PGE_GAME_2D_BACKEND::LateUpdateGAME2D(float elapsedTime)
	{
		// Process and Display Transitions
		olcPGEX_Transition::ProcessTransitions(transitions, fElapsedTime, fScreenSize);

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
