#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

#define PGE_GAME_2D
#define PGE_GAME_WITH_AUDIO
#include "PGE_GAME_2D.h"


using namespace GAME2D;
class GAME_MAIN : public olc::PixelGameEngine, public GAME2D::PGE_GAME_2D_BACKEND
{
public:
	GAME_MAIN()	{ sAppName = "PGE GAME 2D (001)"; }

private:


public:
	bool OnUserCreate() override
	{
		// Start the PGE_GAME_2D engine
		StartGAME2D(ScreenWidth(), ScreenHeight(), SPLASH_SCREEN_ON, "./assets/snd/shotgun.wav");

		// Modify the default SplashScreen options
		splashScreen.SetOptions(6, 2, 3.0f, 0.0f, olc::BLACK, olc::RED, olc::DARK_RED, olc::DARK_GREY, PGE_GAME_2D_ENABLED);

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		// Exit
		if (bReturnFalseNextFrame)
			return false;

		// User Initiated Exit
		if (ForceQuit(GetKey(olc::Key::ESCAPE).bPressed))
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
		LateUpdateGAME2D(fElapsedTime);

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