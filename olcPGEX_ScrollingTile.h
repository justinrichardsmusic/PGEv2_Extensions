/*
	olcPGEX_ScrollingTile.h

	+-------------------------------------------------------------+
	|         OneLoneCoder Pixel Game Engine Extension            |
	|                  Scrolling Tile - v1.0                      |
	+-------------------------------------------------------------+

	What is this?
	~~~~~~~~~~~~~
	This is an extension to the olcPixelGameEngine v2.0 and above.
	It's purpose is to allow seamless tiling across an infinite
	landscape with appropriate culling to minimise draw costs.

	Its use is simple, set your screen size, tile size and image
	once somewhere in your initialisation code...

		olcPGEX_ScrollingTile tileBackground;
		tileBackground.SetTileValues(camera.vecCamViewSize, { 256, 256 }, decalBackgroundTile));

	Then just move your camera position around and Draw the tiles to the screen...

		tileBackground.DrawAllTiles(camera.vecCamPos);


	That's it... Enjoy.


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

#ifndef OLC_PGEX_SCROLLINGTILE
#define OLC_PGEX_SCROLLINGTILE

#pragma once
#include "olcPixelGameEngine.h"

class olcPGEX_ScrollingTile : public olc::PGEX
{
private:
	olc::vi2d vecScreenSize{};
	olc::vi2d vecTileSize{};
	olc::vi2d vecTilesToDraw{};

	olc::vi2d vecCurrentOffset{};

	olc::Decal* decTile = nullptr;

	inline void DrawSingleTile(olc::vi2d screenPos);

public:
	inline void SetTileValues(olc::vi2d screenSize, olc::vi2d tileSize, olc::Decal* decal);
	inline void DrawAllTiles(olc::vf2d camPos);

};


void olcPGEX_ScrollingTile::SetTileValues(olc::vi2d screenSize, olc::vi2d tileSize, olc::Decal* decal)
{
	vecScreenSize = screenSize;
	vecTileSize = tileSize;
	decTile = decal;

	vecTilesToDraw = (vecScreenSize / vecTileSize) + olc::vi2d(1, 1);
}

void olcPGEX_ScrollingTile::DrawSingleTile(olc::vi2d screenPos)
{
	pge->DrawDecal(screenPos, decTile);
}

void olcPGEX_ScrollingTile::DrawAllTiles(olc::vf2d camPos)
{
	vecCurrentOffset.x = (int)camPos.x % vecTileSize.x;
	vecCurrentOffset.y = (int)camPos.y % vecTileSize.y;

	for (int y = -1; y < vecTilesToDraw.y + 1; y++)
	{
		for (int x = -1; x < vecTilesToDraw.x + 1; x++)
		{
			olc::vi2d screenPos{};
			screenPos.x = x * vecTileSize.x - vecCurrentOffset.x;
			screenPos.y = y * vecTileSize.y - vecCurrentOffset.y;

			DrawSingleTile(screenPos);
		}
	}
}

#endif
