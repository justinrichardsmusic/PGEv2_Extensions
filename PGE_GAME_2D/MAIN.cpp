// An example main.cpp file that inherits from the PGE_GAME_2D_BACKEND
// and implements the basic framework needed for the backend to function.
//
// Audio is disabled as it requires the inclusion of a default menu sound
// file, however it can be easily enabled by uncommenting the define at
// the top and swapping out the StartGAME2D function call in OnUserCreate.
// You will need to provide the path to a sound file of your own of course.
//
// The audio system uses the SoLoud library as a frontend and can be set
// to use multiple backend implementations. There are detailed instructions
// on seting up SoLoud for use with my Audio Exentions in the PixelGameEngine
// in the AUDIO folder in my GitHub. Therefore the audio will not work "out
// of the box" so to speak, but is well worth the effort to get working in
// my opinion...


#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

#define PGE_GAME_2D
//#define PGE_GAME_WITH_AUDIO
#include "PGE_GAME_2D.h"


using namespace GAME2D;
class GAME_MAIN : public olc::PixelGameEngine, public GAME2D::PGE_GAME_2D_BACKEND
{
public:
	GAME_MAIN()	{ sAppName = "PGE GAME 2D EXAMPLE"; }

private:
	// Declarations

public:
	bool OnUserCreate() override
	{
		// Start the PGE_GAME_2D engine
		StartGAME2D(ScreenWidth(), ScreenHeight(), SPLASH_SCREEN_ON);
		//StartGAME2D(ScreenWidth(), ScreenHeight(), SPLASH_SCREEN_ON, "./assets/DEFAULT_MENU_SOUND.wav");
		
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		// Exit
		if (bReturnFalseNextFrame)
			return false;

		// User Initiated Exit
		if (GetKey(olc::Key::ESCAPE).bPressed)
			bReturnFalseNextFrame = true;

		// First Frame Only
		if (bFirstUpdateFrame)
		{

		}

		// Splash Screen Mode
		if (bSplashScreenOn)
		{
			UpdateGAME2D(fVecZero);
			return true;
		}

		// Update the game engine
		UpdateGAME2D(fVecZero);

		// GAME CODE
		//
		//

		// Perform late update after primary game code is executed
		LateUpdateGAME2D();

		return true;
	}
};

using namespace GAME2D;
int main()
{
	GAME_MAIN game;
	if (game.Construct(1280, 640, 1, 1, WINDOWED, VSYNC_ON))
		game.Start();

	return 0;
}
