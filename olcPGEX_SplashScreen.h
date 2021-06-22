/*
	olcPGEX_SplashScreen.h

	+-------------------------------------------------------------+
	|         OneLoneCoder Pixel Game Engine Extension            |
	|                SplashScreen - v1.1			      |
	+-------------------------------------------------------------+

	What is this?
	~~~~~~~~~~~~~
	This is an extension to the olcPixelGameEngine v1.0 and above.
	It will allow you to display a short splash screen animation
	at the beginning of your program to help fulfill licensing
	requirements should you choose to release your project into
	the world...

	Its use is very simple. 
	
	1) Include the header in your main class (after the
	   olcPixelGameEngine is included)

	2) Declare an instance of the extension at the top of your
	   main class

	3) Call the AnimateSplashScreen function from OnUserUpdate using
	   the method shown below:

	   if (splashScreen.AnimateSplashScreen(fElapsedTime)) return true;

	   ...assuming you called your instance of the extension
	   "splashScreen" that is...

	   You put this at the very top of your OnUserUpdate() function
	   and it will continue to return true whilst the splash screen
	   animation is playing, which will skip the rest of your game
	   code whilst this is happening... Simple.

	   OPTIONAL: You can adjust the parameters of the Splash Screen
	   using the SetOptions function in OnUserCreate like so...

	   splashScreen.SetOptions(2, 1, 6.0f, 2.0f, olc::WHITE, olc::BLUE, olc::DARK_GREY, olc::BLACK);

	   (the above example will set the colours, duration, delay, etc to
	    values that are different from the standard implementation)


	I needed a splash screen for my games and I assume others will
	eventually need one to, so I wrapped it up in an extension for
	others to make use of as well :-)


	---------------------
	  v1.1 NEW FEATURES
	---------------------

	Ability to set the parameters of the Splash Screen using an
	initialisation function, instead of having to modify the
	header file directly...



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

#ifndef OLC_PGEX_SPLASHSCREEN
#define OLC_PGEX_SPLASHSCREEN

class olcPGEX_SplashScreen : public olc::PGEX
{
private:
	enum class SS_State
	{
		SS_INIT,
		SS_PRE_FADE_IN,
		SS_FADE_IN,
		SS_DISPLAY,
		SS_FADE_OUT,
		SS_COMPLETE
	} 
	nSplashScreenState = SS_State::SS_INIT, nNextSplashScreenState = SS_State::SS_INIT;

	int nScale = 4;
	int nPartialScale = 2;
	olc::Pixel pBackGroundColour = olc::Pixel(0, 0, 0, 255);	// default olc::Pixel(0, 0, 0, 255)
	olc::Pixel pPGEColour = olc::Pixel(255, 0, 0, 255);			// default olc::Pixel(255, 191, 0, 255)
	olc::Pixel pNameSpaceColour = olc::Pixel(127, 0, 0, 255);	// default olc::Pixel(95, 0, 191, 255)
	olc::Pixel pDefaultColour = olc::WHITE;						// default olc::WHITE

	float fAlpha = 0.0f;
	float fTotalDuration = 2.0f;
	float fFadeDuration = 1.0f;
	float fAnimationCounter = 0.0f;
	float fTextOffset = 0.0f;
	float fInitialDelay = 1.5f;  // v1.1

	std::string strMadeWith = "made with";
	std::string strPGE =       "olc  PixelGameEngine";
	std::string strNameSpace = "   ::";
	std::string strOneLoneCoder = "OneLoneCoder.com";

	olc::vf2d vecMadeWithPos;
	olc::vf2d vecPGEPos;
	olc::vf2d vecOneLoneCoderPos;

	olc::vf2d vecMiddleScreenPos;

public:
	// Optional function to modify the standard splash screen parameters - v1.1
	inline void SetOptions(int scale, int partialScale, float totalDuration, float initialDelay, olc::Pixel backGroundColour, olc::Pixel PGEColour, olc::Pixel nameSpaceColour, olc::Pixel defaultColour);
	
	// The main function that will display the splash screen
	inline bool AnimateSplashScreen(float fElapsedTime);

};

void olcPGEX_SplashScreen::SetOptions(int scale, int partialScale, float totalDuration, float initialDelay, olc::Pixel backGroundColour, olc::Pixel PGEColour, olc::Pixel nameSpaceColour, olc::Pixel defaultColour)
{
	nScale = scale;
	nPartialScale = partialScale;
	fTotalDuration = totalDuration;
	fInitialDelay = initialDelay;

	pBackGroundColour = backGroundColour;
	pPGEColour = PGEColour;
	pNameSpaceColour = nameSpaceColour;
	pDefaultColour = defaultColour;
}

bool olcPGEX_SplashScreen::AnimateSplashScreen(float fElapsedTime)
{
	// Don't run any logic if the splash screen is complete
	if (nSplashScreenState == SS_State::SS_COMPLETE) return false;

	// Clear screen to desired background colour and increment the counter
	pge->Clear(pBackGroundColour);
	fAnimationCounter += fElapsedTime;
	fTextOffset = fAnimationCounter * (float)nScale * 4.0f;

	switch (nSplashScreenState)
	{
	case SS_State::SS_INIT:
	{
		// Prevent invalid scale size
		if (nScale < 1) nScale = 1;
		if (nScale * 160 > pge->ScreenWidth()) nScale = pge->ScreenWidth() / 160;

		// Calculate the scale of the smaller text
		nPartialScale = nScale > 1 ? nScale / 2 : 1;

		// Set initial position of text
		vecMiddleScreenPos = { (float)pge->ScreenWidth() / 2, (float)pge->ScreenHeight() / 2 };
		vecPGEPos = { vecMiddleScreenPos.x - (8 * nScale * 10), vecMiddleScreenPos.y - (4 * nScale) };
		vecMadeWithPos = { vecPGEPos.x - (8 * nPartialScale * 4), vecPGEPos.y - (8 * nPartialScale) - (nPartialScale / 2) };
		vecOneLoneCoderPos = { vecPGEPos.x + (8 * nScale * 20) - (8 * nPartialScale * 12), vecPGEPos.y + (8 * nScale) + (8 * (nPartialScale / 2)) };

		// Calculated the fade duration based on the total duration
		fFadeDuration = fTotalDuration / 4.0f;

		nNextSplashScreenState = SS_State::SS_PRE_FADE_IN;

	} break;

	case SS_State::SS_PRE_FADE_IN:
	{
		// Pause for a small period before starting to fade in...  // useful if program lags at start up (ie. switch to full screen mode, etc)
		if (fAnimationCounter >= fInitialDelay)
		{
			fAnimationCounter = 0.0f;
			nNextSplashScreenState = SS_State::SS_FADE_IN;
		}

	} break;

	case SS_State::SS_FADE_IN:
	{
		// Lerp the alpha values and display the text
		fAlpha += (255.0f / fFadeDuration) * fElapsedTime;
		if (fAlpha > 255.0f) fAlpha = 255.0f;

		pNameSpaceColour.a = (uint8_t)fAlpha;
		pPGEColour.a = (uint8_t)fAlpha;
		pDefaultColour.a = (uint8_t)fAlpha;

		pge->DrawString(vecPGEPos, strNameSpace, pNameSpaceColour, nScale);
		pge->DrawString(vecPGEPos, strPGE, pPGEColour, nScale);
		pge->DrawString(olc::vf2d{ vecMadeWithPos.x + fTextOffset, vecMadeWithPos.y }, strMadeWith, pDefaultColour, nPartialScale);
		pge->DrawString(olc::vf2d{ vecOneLoneCoderPos.x - fTextOffset, vecOneLoneCoderPos.y }, strOneLoneCoder, pDefaultColour, nPartialScale);

		if (fAnimationCounter >= fFadeDuration)
			nNextSplashScreenState = SS_State::SS_DISPLAY;

	} break;

	case SS_State::SS_DISPLAY:
	{
		// Display the text with no transparency
		pge->DrawString(vecPGEPos, strNameSpace, pNameSpaceColour, nScale);
		pge->DrawString(vecPGEPos, strPGE, pPGEColour, nScale);
		pge->DrawString(olc::vf2d{ vecMadeWithPos.x + fTextOffset, vecMadeWithPos.y }, strMadeWith, pDefaultColour, nPartialScale);
		pge->DrawString(olc::vf2d{ vecOneLoneCoderPos.x - fTextOffset, vecOneLoneCoderPos.y }, strOneLoneCoder, pDefaultColour, nPartialScale);

		if (fAnimationCounter >= fTotalDuration - fFadeDuration)
			nNextSplashScreenState = SS_State::SS_FADE_OUT;

	} break;

	case SS_State::SS_FADE_OUT:
	{
		// Lerp the alpha back down to clear and display the text
		fAlpha -= (255.0f / fFadeDuration) * fElapsedTime;
		if (fAlpha < 0.0f) fAlpha = 0.0f;

		pNameSpaceColour.a = (uint8_t)fAlpha;
		pPGEColour.a = (uint8_t)fAlpha;
		pDefaultColour.a = (uint8_t)fAlpha;

		pge->DrawString(vecPGEPos, strNameSpace, pNameSpaceColour, nScale);
		pge->DrawString(vecPGEPos, strPGE, pPGEColour, nScale);
		pge->DrawString(olc::vf2d{ vecMadeWithPos.x + fTextOffset, vecMadeWithPos.y }, strMadeWith, pDefaultColour, nPartialScale);
		pge->DrawString(olc::vf2d{ vecOneLoneCoderPos.x - fTextOffset, vecOneLoneCoderPos.y }, strOneLoneCoder, pDefaultColour, nPartialScale);

		if (fAnimationCounter >= fTotalDuration)
			nNextSplashScreenState = SS_State::SS_COMPLETE;

	} break;

	default:
	{

		return false;
	} break;

	}

	nSplashScreenState = nNextSplashScreenState;
	return true;
}

#endif

