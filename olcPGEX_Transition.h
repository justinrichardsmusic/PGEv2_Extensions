/*
	olcPGEX_Transition.h

	+-------------------------------------------------------------+
	|         OneLoneCoder Pixel Game Engine Extension            |
	|                Screen Transition - v1.0                     |
	+-------------------------------------------------------------+

	What is this?
	~~~~~~~~~~~~~
	This is an extension to the olcPixelGameEngine v2.0 and above.
	It allows for smoothly fading screen transitions in and out of
	game scenes.
	
	
	How to use it?
	~~~~~~~~~~~~~~
	Simply include the header file somewhere under the pixel game
	engine include and remember to also define the implementation as
	well like shown:
	
		#define TRANSITION_IMPLEMENTATION
		#include "olcPGEX_Transition.h"
	
	Then create a std::vector of transitions to add to in your
	declarations:
	
		std::vector<olcPGEX_Transition> transitions{};
		
	Then in OnUserCreate you can either add the default transitions
	or some of your own (or both).  You will need to provide a decal
	which for best results should simply be a 1x1 white pixel:
	
		olcPGEX_Transition::SetDefaultTransitions(transitions, decalWHITE1x1);
		
	Somewhere in you OnUserUpdate function you need to process the
	transitions.  This should be done after ALL drawing that is effected
	by the transition has taken place.  You need to pass in fElapsedTime
	as well as the ScreenWidth and ScreenHeight as an olc::vf2d:
	
		olcPGEX_Transition::ProcessTransitions(transitions, fElapsedTime, olc::vf2d{ ScreenWidth(), ScreenHeight() });
	
	The only thing left to now is to start a transition.  You can use
	integers as IDs or create an Enum to manage them with more useful
	names.  Note, the direction can either be 1.0f (transition TO solid),
	or -1.0f (transition TO transparent).  The speed value is set to
	1.0f by default (1 second):
	
		transitions[0].StartSingleTransition(1.0f);
	
	
	All done :-)
	

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

#ifndef OLC_PGEX_TRANSITION
#define OLC_PGEX_TRANSITION

#pragma once
#include "olcPixelGameEngine.h"

class olcPGEX_Transition : public olc::PGEX
{
public:
	olcPGEX_Transition(int ID, olc::Decal* decal, olc::Pixel tint);

private:
	olc::Decal* decTransition =				nullptr;

	olc::Pixel pTint =					olc::WHITE;
	float fAlpha =						0.0f;
	float fTransitionDirection =				0.0f;					// -1.0f OUT, 0.0f OFF, 1.0f IN
	float fSpeed =						1.0f;					// 1.0f = 1 second, scale accordingly

public:
	int nID =						0;
	bool bActive =						false;

private:
	void			UpdateAndDraw			(float elapsedTime, olc::vf2d screenSize);

public:
	static void		AddTransitionType		(std::vector<olcPGEX_Transition>& transitionGroup, int ID, olc::Decal* decal, olc::Pixel tint);
	static void		SetDefaultTransitions		(std::vector<olcPGEX_Transition>& transitionGroup, olc::Decal* decal);
	static void		ProcessTransitions		(std::vector<olcPGEX_Transition>& transitionGroup, float elapsedTime, olc::vf2d screenSize);
	
	void			StartSingleTransition		(float direction, float speed = 1.0f);
};

#ifdef TRANSITION_IMPLEMENTATION
#undef TRANSITION_IMPLEMENTATION

olcPGEX_Transition::olcPGEX_Transition(int ID, olc::Decal* decal, olc::Pixel tint)
{
	nID =				ID;
	decTransition =			decal;
	pTint =				tint;
	fAlpha =			0.0f;
}

void olcPGEX_Transition::AddTransitionType(std::vector<olcPGEX_Transition>& transitionGroup, int ID, olc::Decal* decal, olc::Pixel tint)
{ transitionGroup.push_back(olcPGEX_Transition{ ID, decal, tint }); }

void olcPGEX_Transition::SetDefaultTransitions(std::vector<olcPGEX_Transition>& transitionGroup, olc::Decal* decal)
{
	AddTransitionType(transitionGroup, 0,	decal,		olc::BLACK);
	AddTransitionType(transitionGroup, 1,	decal,		olc::WHITE);
	AddTransitionType(transitionGroup, 2,	decal,		olc::RED);
}

void olcPGEX_Transition::StartSingleTransition(float direction, float speed)
{
	fTransitionDirection = direction;
	fSpeed = speed;

	fAlpha = fTransitionDirection < 0.0f ? 1.0f : 0.0f;
	bActive = true;
}

void olcPGEX_Transition::ProcessTransitions(std::vector<olcPGEX_Transition>& transitionGroup, float elapsedTime, olc::vf2d screenSize)
{
	for (auto& t : transitionGroup)
		t.UpdateAndDraw(elapsedTime, screenSize);

}

void olcPGEX_Transition::UpdateAndDraw(float elapsedTime, olc::vf2d screenSize)
{
	if (bActive)
	{
		if (fTransitionDirection < 0.0f)
			fAlpha -= elapsedTime * fSpeed;
		else if (fTransitionDirection > 0.0f)
			fAlpha += elapsedTime * fSpeed;

		if (fAlpha < 0.0f)
		{
			fAlpha = 0.0f;
			fTransitionDirection = 0.0f;
			bActive = false;
		}
		else if (fAlpha > 1.0f)
		{
			fAlpha = 1.0f;
			fTransitionDirection = 0.0f;
		}

		pTint.a = fAlpha * 255.0f;
		if (pTint.a > 255)
			pTint.a = 255;

		pge->DrawPartialDecal({ 0.0f, 0.0f }, screenSize, decTransition, { 0.0f, 0.0f }, { 1.0f, 1.0f }, pTint);
	}
}

#endif
#endif
