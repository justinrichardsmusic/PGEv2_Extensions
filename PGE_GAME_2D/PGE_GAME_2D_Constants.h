// A list of constants to be used with the PGE_GAME_2D
// environment and / or with the AudioListener PGEX...
// 
// Also a place to add your own constants on a per
// game basis...
//
// Author:
// Justin Richards


#pragma once
#include "olcPixelGameEngine.h"

namespace GAME2D
{
	// Defined bool values
	const bool PGE_GAME_2D_ENABLED		= true;
	const bool PGE_GAME_2D_DISABLED		= false;

	const bool FULLSCREEN				= true;
	const bool WINDOWED					= false;

	const bool VSYNC_ON					= true;
	const bool VSYNC_OFF				= false;

	const bool COHESION_ON				= true;
	const bool COHESION_OFF				= false;

	const bool ENABLED					= true;
	const bool DISABLED					= false;

	const bool VISIBLE					= true;
	const bool INVISIBLE				= false;

	const bool HASFOCUS					= true;
	const bool HASNOFOCUS				= false;

	const bool CENTERED					= true;
	const bool LEFT_ALIGNED				= false;

	const bool IS_STATIC				= true;
	const bool IS_NOT_STATIC			= false;

	const bool LOOPING					= true;
	const bool NOTLOOPING				= false;

	const bool ON						= true;
	const bool OFF						= false;

	const bool SPLASH_SCREEN_ON			= true;
	const bool SPLASH_SCREEN_OFF		= false;

	const bool SWAP_TILE				= true;
	const bool DONT_SWAP_TILE			= false;

	// Defined float values
	const float NO_TRANSITION			=  0.0f;
	const float TO_TRANSPARENT			= -1.0f;
	const float TO_SOLID				=  1.0f;

	// Defined int values
	const int ALL_TRANSITIONS			= -1;

	// Useful vector values
	const olc::vi2d iVecZero			= { 0, 0 };
	const olc::vi2d iVecOne				= { 1, 1 };
	const olc::vi2d iVecTwo				= { 2, 2 };
	const olc::vi2d iVecThree			= { 3, 3 };
	const olc::vi2d iVecFour			= { 4, 4 };
	const olc::vi2d iVecFive			= { 5, 5 };
	const olc::vi2d iVecSix				= { 6, 6 };
	const olc::vi2d iVecSeven			= { 7, 7 };
	const olc::vi2d iVecEight			= { 8, 8 };
	const olc::vi2d iVecNine			= { 9, 9 };
	const olc::vi2d iVecTen				= { 10, 10 };

	const olc::vf2d fVecZero			= { 0.0f, 0.0f };
	const olc::vf2d fVecOne				= { 1.0f, 1.0f };
	const olc::vf2d fVecTwo				= { 2.0f, 2.0f };
	const olc::vf2d fVecThree			= { 3.0f, 3.0f };
	const olc::vf2d fVecFour			= { 4.0f, 4.0f };
	const olc::vf2d fVecFive			= { 5.0f, 5.0f };
	const olc::vf2d fVecSix				= { 6.0f, 6.0f };
	const olc::vf2d fVecSeven			= { 7.0f, 7.0f };
	const olc::vf2d fVecEight			= { 8.0f, 8.0f };
	const olc::vf2d fVecNine			= { 9.0f, 9.0f };
	const olc::vf2d fVecTen				= { 10.0f, 10.0f };

	const olc::vf2d NO_CLAMP			= { 0.0f, 0.0f };

	// Enums
	enum AUDIO
	{
		// used as the "click" sound in the menus
		DEFAULT_MENU_SND,

		// add constants for your game sounds here (KEEP IN THE SAME ORDER AS YOUR VECTOR INDEXES!)


		// Keep this at the end and do not remove it!
		NULL_SND,
	};

	enum MENU
	{
		MAIN_EXIT,
	};

	enum GAME_STATE
	{
		RESET,
		MAIN_MENU,
	};

	enum TRANSITION
	{
		BLACK,
		WHITE,
		RED,
	};

	enum COLLECTABLES
	{
		DEFAULT_ITEM,
	};

	// Helper Functions
	bool PointInsideRect(const olc::vf2d& point, const olc::vf2d& rectPos, const olc::vf2d& rectSize);
	bool RectVsRect(const olc::vf2d& pos1, const olc::vf2d& size1, const olc::vf2d& pos2, const olc::vf2d& size2);
}
