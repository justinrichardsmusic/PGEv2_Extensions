/*
	olcPGEX_Camera2D.h

	+-------------------------------------------------------------+
	|          OneLoneCoder Pixel Game Engine Extension           |
	|                    Camera2D - v1.1                          |
	+-------------------------------------------------------------+

	What is this?
	~~~~~~~~~~~~~
	This is an extension to the olcPixelGameEngine v2.0 and above.
	It is quite simply, a 2D camera with smooth scrolling (lerping)
	capabilities.

	Firstly the header must be included and implementation guard defined
	You must define the implementation guard only once the first time
	you include the camera extension.

			#define OLC_PGEX_CAMERA2D_IMPLEMENTATION
			#include "olcPGEX_Camera2D.h"


	It then needs to be initialised with a position and view size (usually
	set to the application screen width and height).

			olcPGEX_Camera2D camera;
			camera.InitialiseCamera(playerPos, screenRes);


	Or alternatively it can be intialised when constructed if the required
	parameters are known.

			olcPGEX_Camera2D camera = olcPGEX_Camera2D(playerPos, screenRes);


	If you want smooth movement and boundary clipping you can lerp the
	camera like shown below.  This example assumes you are moving towards
	the player's current position and centering the camera on the player.

			camera.vecCamPos = camera.LerpCamera(camera.ClampVector({ 0, 0 }, vecMapSize, (player.vecPos - camera.vecCamViewSize / 2.0f)), 15.0f);

	... and should be followed by a final clamp.

			camera.ClampCamera({ 0, 0 }, vecMapSize);


	This allows for decent camera tracking with relatively little
	setup and upkeep...



	-----------------------
	  v1.1 - NEW FEATURES
	-----------------------

	Removed INLINE functions, header guards are now required (see example
	above).

	Added default constructor as well as a secondary constructor that
	also intialises the camera.  Use the option that suits you.

	Fixed a bug in the DrawDebugInfo function.  The DrawPropDecal function
	was supplied an int for textScale instead of an olc::vf2d.

	Separated the code to calculate the size, position, margin, etc of the
	debug info from the drawing routines.  They are now only calculated if
	the text scale supplied is different from the current scale.  A decent
	optimisation.

	Also prevented invalid textScales below 1 or above 10.

	General code tidy up as well.



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

#ifndef OLC_PGEX_CAMERA2D 
#define OLC_PGEX_CAMERA2D

class olcPGEX_Camera2D : public olc::PGEX
{
public:
	olcPGEX_Camera2D() {}
	olcPGEX_Camera2D(const olc::vf2d camPos, const olc::vf2d camViewSize) { InitialiseCamera(camPos, camViewSize); }

	olc::vf2d vecCamPos{};
	olc::vf2d vecDesiredPos{};
	olc::vf2d vecCamViewSize{};

private:
	olc::vf2d vfTextScale{ 1.0f, 1.0f };
	olc::vi2d viDisplaySize{ 160, 52 };
	olc::vi2d viMargin{ 4, 4 };

	int nYIterator = 12;

	const olc::Pixel pDebugRect = olc::PixelF( 0.5f, 0.75f, 1.0f, 0.25f );

	void UpdateDebugProperties(const int textScale);

public:
	void InitialiseCamera(const olc::vf2d camPos, const olc::vf2d viewSize);
	olc::vf2d LerpCamera(const olc::vf2d desiredPos, const float fSpeed);

	void ClampCamera(const olc::vf2d vecTopLeft, const olc::vf2d vecBottomRight);
	olc::vf2d ClampVector(const olc::vf2d vecTopLeft, const olc::vf2d vecBottomRight, const olc::vf2d vecOriginalVector);

	void DrawDebugInfo(olc::vi2d screenPos, int textScale = 1);
};

#ifdef OLC_PGEX_CAMERA2D_IMPLEMENTATION
#undef OLC_PGEX_CAMERA2D_IMPLEMENTATION

void olcPGEX_Camera2D::InitialiseCamera(const olc::vf2d camPos, const olc::vf2d camViewSize)
{
	vecCamPos = camPos;
	vecCamViewSize = camViewSize;
}

olc::vf2d olcPGEX_Camera2D::LerpCamera(const olc::vf2d desiredPos, const float fSpeed)
{
	return { vecCamPos + (desiredPos - vecCamPos) / fSpeed };
}

void olcPGEX_Camera2D::ClampCamera(const olc::vf2d vecTopLeft, const olc::vf2d vecBottomRight)
{
	if (vecCamPos.x < vecTopLeft.x) vecCamPos.x = vecTopLeft.x;
	if (vecCamPos.y < vecTopLeft.y) vecCamPos.y = vecTopLeft.y;
	if (vecCamPos.x + vecCamViewSize.x > vecBottomRight.x) vecCamPos.x = vecBottomRight.x - vecCamViewSize.x;
	if (vecCamPos.y + vecCamViewSize.y > vecBottomRight.y) vecCamPos.y = vecBottomRight.y - vecCamViewSize.y;
}

olc::vf2d olcPGEX_Camera2D::ClampVector(const olc::vf2d vecTopLeft, const olc::vf2d vecBottomRight, const olc::vf2d vecOriginalVector)
{
	olc::vf2d vecAdjustedVector = vecOriginalVector;
	if (vecOriginalVector.x < vecTopLeft.x) vecAdjustedVector.x = vecTopLeft.x;
	if (vecOriginalVector.y < vecTopLeft.y) vecAdjustedVector.y = vecTopLeft.y;
	if (vecOriginalVector.x + vecCamViewSize.x > vecBottomRight.x) vecAdjustedVector.x = vecBottomRight.x - vecCamViewSize.x;
	if (vecOriginalVector.y + vecCamViewSize.y > vecBottomRight.y) vecAdjustedVector.y = vecBottomRight.y - vecCamViewSize.y;

	return vecAdjustedVector;
}

void olcPGEX_Camera2D::DrawDebugInfo(olc::vi2d screenPos, int textScale)
{
	UpdateDebugProperties(textScale);

	pge->FillRectDecal(screenPos - viMargin, viDisplaySize, pDebugRect);

	pge->DrawStringPropDecal(screenPos, "CAMERA", olc::CYAN, vfTextScale);
	screenPos.y += nYIterator;

	pge->DrawStringPropDecal(screenPos, "Position: " + std::to_string((int)vecCamPos.x) + ", " + std::to_string((int)vecCamPos.y), olc::CYAN, vfTextScale);
	screenPos.y += nYIterator;

	pge->DrawStringPropDecal(screenPos, "Desired Pos: " + std::to_string((int)vecDesiredPos.x) + ", " + std::to_string((int)vecDesiredPos.y), olc::CYAN, vfTextScale);
	screenPos.y += nYIterator;

	pge->DrawStringPropDecal(screenPos, "Cam Size: " + std::to_string((int)vecCamViewSize.x) + ", " + std::to_string((int)vecCamViewSize.y), olc::CYAN, vfTextScale);
}

void olcPGEX_Camera2D::UpdateDebugProperties(const int textScale)
{
	if (textScale == (int)vfTextScale.y)
		return;

	if (textScale > 0 && textScale < 11)
	{
		vfTextScale = { (float)textScale, (float)textScale };
		viDisplaySize = { 160 * textScale, 52 * textScale };
		viMargin = { 4 * textScale, 4 * textScale };

		nYIterator = 12 * textScale;
	}
}

#endif	// implementation guard
#endif	// header guard
