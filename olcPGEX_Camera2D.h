/*
	olcPGEX_Camera2D.h

	+-------------------------------------------------------------+
	|         OneLoneCoder Pixel Game Engine Extension            |
	|                Camera2D - v1.0				          |
	+-------------------------------------------------------------+

	What is this?
	~~~~~~~~~~~~~
	This is an extension to the olcPixelGameEngine v2.0 and above.
	It is quite simply, a 2D camera with smooth scrolling (lerping)
	capabilities.

	It needs to be initialised with a position and view size (usually
	set to the application screen width and height).

			olcPGEX_Camera2D camera;
			camera.InitialiseCamera(playerPos, screenRes);

	If you want smooth movement and boundary clipping you can lerp the
	camera like shown below.  This example assumes you are moving towards
	the players current position and centering the camera on the player.

			camera.vecCamPos = camera.LerpCamera(camera.ClampVector({ 0, 0 }, vecMapSize, (player.vecPos - camera.vecCamViewSize / 2.0f)), 15.0f);

	... and should be followed by a final clamp.

			camera.ClampCamera({ 0, 0 }, vecMapSize);


	This allows for decent camera tracking with relatively little
	setup and upkeep...


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
	olc::vf2d vecCamPos;
	olc::vf2d vecDesiredPos;
	olc::vf2d vecCamViewSize;

	inline void InitialiseCamera(olc::vf2d camPos, olc::vf2d viewSize);
	inline olc::vf2d LerpCamera(olc::vf2d desiredPos, float fSpeed);

	inline void ClampCamera(olc::vf2d vecTopLeft, olc::vf2d vecBottomRight);
	inline olc::vf2d ClampVector(olc::vf2d vecTopLeft, olc::vf2d vecBottomRight, olc::vf2d vecOriginalVector);
};

void olcPGEX_Camera2D::InitialiseCamera(olc::vf2d camPos, olc::vf2d camViewSize)
{
	vecCamPos = camPos;
	vecCamViewSize = camViewSize;
}

olc::vf2d olcPGEX_Camera2D::LerpCamera(olc::vf2d desiredPos, float fSpeed)
{
	return { vecCamPos + (desiredPos - vecCamPos) / fSpeed };
}

void olcPGEX_Camera2D::ClampCamera(olc::vf2d vecTopLeft, olc::vf2d vecBottomRight)
{
	if (vecCamPos.x < vecTopLeft.x) vecCamPos.x = vecTopLeft.x;
	if (vecCamPos.y < vecTopLeft.y) vecCamPos.y = vecTopLeft.y;
	if (vecCamPos.x + vecCamViewSize.x > vecBottomRight.x) vecCamPos.x = vecBottomRight.x - vecCamViewSize.x;
	if (vecCamPos.y + vecCamViewSize.y > vecBottomRight.y) vecCamPos.y = vecBottomRight.y - vecCamViewSize.y;
}

olc::vf2d olcPGEX_Camera2D::ClampVector(olc::vf2d vecTopLeft, olc::vf2d vecBottomRight, olc::vf2d vecOriginalVector)
{
	olc::vf2d vecAdjustedVector = vecOriginalVector;
	if (vecOriginalVector.x < vecTopLeft.x) vecAdjustedVector.x = vecTopLeft.x;
	if (vecOriginalVector.y < vecTopLeft.y) vecAdjustedVector.y = vecTopLeft.y;
	if (vecOriginalVector.x + vecCamViewSize.x > vecBottomRight.x) vecAdjustedVector.x = vecBottomRight.x - vecCamViewSize.x;
	if (vecOriginalVector.y + vecCamViewSize.y > vecBottomRight.y) vecAdjustedVector.y = vecBottomRight.y - vecCamViewSize.y;

	return vecAdjustedVector;
}

#endif

