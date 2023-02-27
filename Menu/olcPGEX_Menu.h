/*
	olcPGEX_Menu.h

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

		olcPGEX_Menu.h (this file)
		olcPGEX_Menu.cpp
		olcPGEX_MenuItem.h
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
#include "olcPGEX_MenuItem.h"

class olcPGEX_Menu : public olcPGEX_MenuItem
{
public:
	olcPGEX_Menu();
	olcPGEX_MenuItem*		GetMenuItemByID(const int ID);
	void					      AddMenuItem(const int ID, olc::Decal* decal, const olc::vf2d pos, const olc::vf2d size, const olc::vf2d sourcePos = { 0.0f, 0.0f }, const bool visible = true, const bool enabled = true, const float zoomFactor = 1.2f, const bool centered = true, const bool isStatic = false);
	void					      SetMenuVisibility(const bool visible = true);
	void					      StartTransition(const float transitionDirection, const float speed = 2.0f);
	int						      ProcessMenuInteractions(const float fElapsedTime, const olc::vi2d mousePos, const bool leftMouseButtonReleased);

private:
	const float NO_TRANSITION = 0.0f;

public:
	std::vector<olcPGEX_MenuItem*> menu{};
};

