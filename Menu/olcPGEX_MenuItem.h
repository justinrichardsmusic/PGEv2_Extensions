/*
	olcPGEX_MenuItem.h

	+-------------------------------------------------------------+
	|          OneLoneCoder Pixel Game Engine Extension           |
	|                        Menu - v1.0                          |
	+-------------------------------------------------------------+

	What is this?
	~~~~~~~~~~~~~
	This is an extension for the olcPixelGameEngine version 2.0 and above.
	It has formally replaced my olcPGEX_Interactable extension.

	It allows for image based menus to be added to PGE projects with very
	little fuss. All you need is an image file (or files) to use as your
	menu items (single image and separate image options are both supported).

	This is a little different from some of my other PGEXs as it is not a
	single file solution. You will require all of these files;

		olcPGEX_Menu.h
		olcPGEX_Menu.cpp
		olcPGEX_MenuItem.h (this file)
		olcPGEX_MenuItem.cpp

	There is an example.cpp file in my github here:
	https://github.com/justinrichardsmusic/PGEv2_Extensions/menu

	This will explain (and demonstrate) how to use this menu PGEX in your
	PGE projects. I've also include some basic menu graphics which I created
	myself (completely free license, use as you see fit).

	Enjoy.


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

class olcPGEX_MenuItem : public olc::PGEX
{
public:
	olcPGEX_MenuItem();
	olcPGEX_MenuItem(const int ID, olc::Decal* decal, const olc::vf2d pos, const olc::vf2d size, const olc::vf2d sourcePos = { 0.0f, 0.0f }, const bool visible = true, const bool enabled = true, const float zoomFactor = 1.2f, const bool centered = true, const bool isStatic = false);

	int			nID =				-1;
	bool			bVisible =			true;
	bool			bEnabled =			true;
	bool			bToggledOff =			false;
	bool			bIsStatic =			false;
	bool			bHover =			false;
	bool			bIncludeInTransition =	        true;

	olc::vf2d		vecPos				{};
	olc::vf2d		vecSize				{};
	olc::vf2d		vecSourcePos			{};

	float			fZoomFactor =			1.2f;
	float			fZoomSpeed =			1.0f;
	float			fTransitionDirection =	        0.0f;		// -1.0f transition out, +1.0f transition in, 0.0f no transition

	olc::Pixel		pTint =				olc::WHITE;

	olc::vf2d		vecDrawSize			{};
	float			fCurrentZoom =			1.0f;
	float			fCurrentAlpha =			1.0f;
	float			fTransitionSpeed =		2.0f;

private:
	olc::Decal*		decMenu =			nullptr;

	bool			bCentered =			true;

	olc::vf2d		vecCenterPos			{};
	olc::vf2d		vecHalfSize			{};

	bool			i_FPointInsideRect(olc::vf2d point, olc::vf2d rPos, olc::vf2d rSize);

public:
	void			Construct(const int ID, olc::Decal* decal, const olc::vf2d pos, const olc::vf2d size, const olc::vf2d sourcePos = { 0.0f, 0.0f }, const bool visible = true, const bool enabled = true, const float zoomFactor = 1.2f, const bool centered = true, const bool isStatic = false);
	void			Update(const float fElapsedTime, const olc::vi2d mousePos);
	void			Draw();
	void			Reposition(const olc::vf2d newPos, const olc::vf2d newSize = { 0.0f, 0.0f }, const bool centered = true);
};
