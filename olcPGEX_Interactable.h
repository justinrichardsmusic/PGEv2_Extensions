/*
	olcPGEX_Interactable.h

	+-------------------------------------------------------------+
	|         OneLoneCoder Pixel Game Engine Extension            |
	|                    Interactable v1.1                        |
	+-------------------------------------------------------------+

	What is this?
	~~~~~~~~~~~~~
	This is an extension to the olcPixelGameEngine v2.0 and above.

	Its purpose is to allow interactable objects (such as menu items)
	to be added, organised, manipulated, and interacted with in game
	with consistent results.  Abstracting away all the nuances of
	UI construction and upkeep.


	What it can do:
	~~~~~~~~~~~~~~~
	Construct and organise interactable UI objects for use in game. Its main
	features are as follows:

		Works with decals;
		Can be centered instead of left aligned;
		Hover and Zoom built in;
		Transparency and Tinting;
		Visibility, Enabled Status, and Toggling;

	It also has the ability to perform certain useful operations on groups of
	interactables like:

		Transition between entire groups at once;
		Set status of groups;

	Overall, majorly simplifying the user interface aspect of most simple
	games.


	Limitations:
	~~~~~~~~~~~~
	Doesn't support Sprites, sorry.


	How Does It Work?
	~~~~~~~~~~~~~~~~~
	Add the following define and include underneath your olcPixelGameEngine include

		#define INTERACTABLE_IMPLEMENTATION
		#include "olcPGEX_Interactable.h"


	In your declarations add an interactable (use std::vector for multiples):

		olcPGEX_Interactable testButton;


	In the OnUserCreate function you must now initialise the interactable with
	at least the following options - ID, decal, position, size:

		int nTestButtonID = 1;
		testButton.Construct(nTestButtonID, decalTestButton, vecPos, vecSize));


	Next in OnUserUpdate we can construct a loop to test for interactivity:

		if (testButton.bVisible)										// Visible? we need to draw it
		{
			if (testButton.bEnabled)									// Enabled? we might need to interact with it
			{
				testButton.Update(fElapsedTime, GetMousePos());			// Update the UI internals

				if (testButton.fTransitionDirection == 0.0f)			// Make sure the interactable isn't currently transitioning
				{
					if (GetMouse(0).bReleased && testButton.bHover)		// Did we click whilst hovering over the interactable?
					{
						if (testButton.nID == ntestButtonID)			// This ID test isn't needed in this simple example, but IS needed for vectors of interactables
						{
							// DO SOMETHING COOL
						}
					}
				}

				testButton.Draw();										// Drawing occurs even when disabled (will be drawn faded and darker)
			}
		}



	Those are the basics to be able to use the olcPGEX_Interactable.  Once
	you get the hang of how to manipulate them you can do a lot with them.

	There are a couple of STATIC functions which can be accessed without the
	need to reference an instance of an interactable.  They are:

		olcPGEX_Interactable::GetInteractableFromGroupByID();
		olcPGEX_Interactable::SetGroupVisibility();
		olcPGEX_Interactable::StartTransition();
		olcPGEX_Interactable::ProcessInteractionFromGroup();

	These can only be used on GROUPS of interactables contained insided a
	std::vector.  When interactables are grouped correctly it makes transitioning
	between groups extremely easy and completely removes the need for the
	complex loop you see above.  This is the real advantage of this PGEX as
	it saves a lot of time in the long run once you start to add many menu
	systems into your game...

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

#ifndef OLC_PGEX_INTERACTABLE
#define OLC_PGEX_INTERACTABLE

#pragma once
#include "olcPixelGameEngine.h"

class olcPGEX_Interactable : public olc::PGEX
{
public:
	// Default constructor for declarations ONLY - you must call the Construct function manually in these cases
	olcPGEX_Interactable() {}

	// Construct this way when using std::vectors
	olcPGEX_Interactable(int ID, olc::Decal* decal, olc::vf2d pos, olc::vf2d size, bool visible = true, bool enabled = true, float zoomFactor = 1.2f, bool centered = true)
	{
		Construct(ID, decal, pos, size, visible, enabled, zoomFactor, centered);
	}

	int nID = -1;
	bool bVisible = true;
	bool bEnabled = true;
	bool bToggledOff = false;
	bool bHover = false;
	bool bIncludeInTransition = true;

	olc::vf2d vecPos{};
	olc::vf2d vecSize{};
	
	float fZoomFactor = 1.2f;
	float fZoomSpeed = 1.0f;
	float fTransitionDirection = 0.0f;				// -1.0f for transition out, +1.0f for transition in

	olc::Decal* decInteractable = nullptr;
	olc::Pixel pTint = olc::WHITE;

private:
	bool bCentered = true;

	olc::vf2d vecCenterPos{};
	olc::vf2d vecDrawSize{};
	olc::vf2d vecHalfSize{};

	float fCurrentZoom = 1.0f;
	float fCurrentAlpha = 1.0f;
	float fTransitionSpeed = 2.0f;

public:
	void Construct(int ID, olc::Decal* decal, olc::vf2d pos, olc::vf2d size, bool visible = true, bool enabled = true, float zoomFactor = 1.2f, bool centered = true);
	void Update(float fElapsedTime, olc::vi2d mousePos);
	void Draw();

	static olcPGEX_Interactable*	GetInteractableFromGroupByID(std::vector<olcPGEX_Interactable>& group, int ID);
	static void						SetGroupVisibility(std::vector<olcPGEX_Interactable> &group, bool visible = true);
	static void						StartTransition(std::vector<olcPGEX_Interactable>& group, float transitionDirection, float speed = 2.0f);
	static int						ProcessInteractionFromGroup(std::vector<olcPGEX_Interactable>& group, float fElapsedTime, olc::vi2d mousePos, bool leftMouseButtonReleased);
};


#ifdef INTERACTABLE_IMPLEMENTATION
#undef INTERACTABLE_IMPLEMENTATION

void olcPGEX_Interactable::Construct(int ID, olc::Decal* decal, olc::vf2d pos, olc::vf2d size, bool visible, bool enabled, float zoomFactor, bool centered)
{
	nID = ID;
	vecPos = pos;
	vecSize = size;
	vecHalfSize = vecSize / 2.0f;
	vecCenterPos = vecPos + vecHalfSize;

	vecDrawSize = vecSize;

	fZoomFactor = zoomFactor;
	bCentered = centered;
	bVisible = visible;
	bEnabled = enabled;

	decInteractable = decal;
}

void olcPGEX_Interactable::Update(float fElapsedTime, olc::vi2d mousePos)
{
	if (fTransitionDirection == 0)
	{

		olc::vf2d fMousePos = { (float)mousePos.x, (float)mousePos.y };

		if (fMousePos.x > vecPos.x &&
			fMousePos.y > vecPos.y &&
			fMousePos.x < vecPos.x + vecSize.x &&
			fMousePos.y < vecPos.y + vecSize.y)
		{
			if (fCurrentZoom < fZoomFactor)
				fCurrentZoom += fElapsedTime * fZoomSpeed;

			bHover = true;

		}
		else
		{
			if (fCurrentZoom > 1.0f)
				fCurrentZoom -= fElapsedTime * fZoomSpeed;

			bHover = false;
		}

		vecDrawSize = vecSize * fCurrentZoom;
	}
	else
	{
		fCurrentAlpha += fTransitionDirection * fTransitionSpeed * fElapsedTime;

		if (fCurrentAlpha < 0.0f)
		{
			fTransitionDirection = 0.0f;
			fCurrentAlpha = 0.0f;
			bVisible = false;
		}
		else if (fCurrentAlpha > 1.0f)
		{
			fTransitionDirection = 0.0f;
			fCurrentAlpha = 1.0f;
		}
	}
}

void olcPGEX_Interactable::Draw()
{
	pTint = bToggledOff ? olc::DARK_GREY : olc::WHITE;
	pTint.a = fCurrentAlpha * 255.0f;
	if (!bEnabled) pTint = olc::PixelF(0.25f, 0.25f, 0.25f, 0.25f * fCurrentAlpha);

	pge->DrawDecal(vecCenterPos - vecDrawSize / 2.0f, decInteractable, { fCurrentZoom, fCurrentZoom }, pTint);
}

olcPGEX_Interactable* olcPGEX_Interactable::GetInteractableFromGroupByID(std::vector<olcPGEX_Interactable>& group, int ID)
{
	for (auto& g : group)
		if (g.nID == ID)
			return &g;

	// ID not found
	std::cout << "ID not found in list of Interactables! [GetInteractableByID]";

	return nullptr;
}

void olcPGEX_Interactable::SetGroupVisibility(std::vector<olcPGEX_Interactable> &group, bool visible)
{
	if (group.size() > 0)
	{
		for (auto& g : group)
			g.bVisible = visible;

	}
	else
		std::cout << "Group of Interactables was empty! [SetGroupVisibility]";

}

void olcPGEX_Interactable::StartTransition(std::vector<olcPGEX_Interactable>& group, float transitionDirection, float speed)
{
	if (group.size() > 0)
	{
		for (auto& g : group)
		{
			g.bVisible = g.bIncludeInTransition;
			g.fTransitionDirection = transitionDirection;
			g.fTransitionSpeed = speed;

			if (transitionDirection > 0.0f)
			{
				g.fCurrentAlpha = 0.0f;
				g.fCurrentZoom = 1.0f;
				g.vecDrawSize = g.vecSize;
			}
		}
	}
	else
		std::cout << "Group of Interactables was empty! [StartTransition]";

}

int olcPGEX_Interactable::ProcessInteractionFromGroup(std::vector<olcPGEX_Interactable>& group, float fElapsedTime, olc::vi2d mousePos, bool leftMouseButtonReleased)
{
	int nReturnID = -1;

	for (auto& g : group)
	{
		if (g.bVisible)																	// Requires Drawing
		{
			if (g.bEnabled)																// Requires Updating
			{
				g.Update(fElapsedTime, mousePos);

				if (g.fTransitionDirection == GAME2D::NO_TRANSITION)					// Requires Interaction
				{
					if (leftMouseButtonReleased && g.bHover)
					{
						// Set returnID to corressponding interaction
						nReturnID = g.nID;
					}
				}
			}
			g.Draw();
		}
	}

	return nReturnID; 
}

#endif		// Implementation

#endif


