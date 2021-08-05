#pragma once
#include "olcPixelGameEngine.h"			// Include the game engine so we can make use of it's internals
#include "olcPGEX_Animator2D.h"			// Include the animator class
#include "olcPGEX_ResourceManager.h"	// We need to pass in the resource manager, so include it here

class cCharacter
{
public:
	olc::vi2d vecPos = { 0, 0 };								// Character position (located at his feet)
	olc::vf2d vecVel = { 0.0f, 0.0f };							// Character velocity (Y-axis used for jumping)
	
	float fAngle = 0.0f;										// Rotational angle of character (in radians)
	olc::vf2d vecPosTranslated = { 0.0f, 0.0f };				// Translated rotated pos

	bool bCrouching = false;									// Is he crouching or not?
	bool bFacingRight = false;									// Character direction
	bool bGrounded = true;										// Is the player on the ground?

	olcPGEX_Animator2D animator;								// The animator controller to add animations to

	void InitialiseCharacter(olcPGEX_ResourceManager &rm);		// Add the animations to this character
	
	olc::vf2d TranslateCurrentPosition(olc::vf2d origin);		// Function to transform the player position based on current position and angle
};

