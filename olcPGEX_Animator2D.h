/*
	olcPGEX_Animator2D.h

	+-------------------------------------------------------------+
	|         OneLoneCoder Pixel Game Engine Extension            |
	|                Animator2D - v1.1			      |
	+-------------------------------------------------------------+

	What is this?
	~~~~~~~~~~~~~
	This is an extension to the olcPixelGameEngine v2.0 and above.
	
	Its purpose is to allow quick and simple animation initialisation
	as well as ease of use with regards to playing, stopping, triggering
	and asignment to game objects.


	What it can do:
	~~~~~~~~~~~~~~~
	Standard animation allowing for rotation around an origin, adjustable
	speed with a user defined duration, and the option to offset the
	animation if need be.

	Static animation which allows the frames to be triggered in game
	rather than animating at a given speed, useful for hud displays (ie
	engine power, or tilt, etc)

	Billboard animation which allows for rotation around an origin whilst
	always displaying the animation 'upright', useful for particle effects
	or flame etc...


	Limitations:
	~~~~~~~~~~~~
	All animations require a single sprite sheet (loaded as a decal only, sorry).

	Each animation can be set to either horizontal (default) or vertical frame
	animation, however each frame must be an equal distance apart.

	Recommend using it with my olcPGEX_ResourceManager extension to ensure no
	duplicate image files get loaded unnecessarily.  It can get messy when
	animating mulitples of the same object in game.


	How Does It Work?
	~~~~~~~~~~~~~~~~~
	This olcPGEX_Animator2D is an Animator Controller.  In other words
	it holds the animations for 'this' object and controls when they play,
	stop, restart, loop, end, etc...

	You can create as many animator controllers as you'd like.  For example,
	you could have an animator on each of your players in the game, one for
	every enemy, one for every pickup, and another in you main game for your
	HUD, etc.

	Once you create your animator controller, you need to assign some animations
	to it using any combination of the 3 available AddAnimation functions.

	Here is a simple example of how to use this extension...

			// Create an animator controller for your object

			olcPGEX_Animator2D animator;


			// Add an animation called "GiveItAName" which plays for a duration of 1/3 of a second, and has 6 frames
			// of animation which are 100 pixels by 100 pixels each, starting at the top left of the image file...

			animator.AddAnimation("GiveItAName", 0.3334f, 6, decSpriteSheet, { 0.0f, 0.0f }, { 100.0f, 100.0f });


			// Play our newly added animation, looping until told to stop

			animator.Play("GiveItAName");


			// Update all of the currently playing animations in our animator controller, passing in fElapsedTime
			// in order to play them at the right speed...

			animator.UpdateAnimations(fElapsedTime)


			// Lastly, we simply need to draw the current animation frame.  Depending on the animation type you
			// may need to specify a rotation as well (in radians)...

			animator.DrawAnimationFrame(playerPos);


			// You can use the Stop and StopAll functions which are self explanatory.  Here you can specify
			// whether to stop after the animation has finished, or immediately...

			animator.Stop("GiveItAName", true);
			animator.StopAll();


	If you need to get a reference to a particular animation in the animation list of your
	animator controller you can use the GetAnim function like so...

			if (animator.GetAnim("GiveItAName")->bIsPlaying) // do some stuff


	Hopefully this extension will be helpful to others wishing to quickly add
	some animation to their project...



	-----------------------
	  v1.1 - NEW FEATURES  
	-----------------------

	Added ability to set an animation to 'Play Next' after current animation
	is told to Stop.

	Added 'PlayAfterSeconds' function which allows the playing of a given
	animation to be delayed by a set amount of time.



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

#ifndef OLC_PGEX_ANIMATOR
#define OLC_PGEX_ANIMATOR

#include "olcPixelGameEngine.h"

class olcPGEX_Animator2D : public olc::PGEX
{
public:
	struct Animation
	{
		std::string strName = "";
		std::string strPlayNext = ""; // v1.1

		bool bIsPlaying = false;
		bool bWasPlaying = false;
		bool bStopAfterComplete = false;
		bool bStopNextAfterComplete = false; // v1.1
		bool bBillboardAnimation = false;

		bool JustFinished() { return !bIsPlaying && bWasPlaying; }

		float fDuration = -1.0f;
		int nNumberOfFrames;
		float fFrameLength = 0.0f;
		int nCurrentFrame = 0;
		float fFrameTick = 0.0f;
		float fPlayAfterSeconds = 0.0f; // v1.1

		olc::vf2d vecFramePos;
		olc::vf2d vecFrameSize;
		olc::vf2d vecNextFrameOffset;
		olc::vf2d vecFrameDisplayOffset = { 0.0f, 0.0f };
		olc::vf2d vecOrigin = { 0.0f, 0.0f };
		olc::vf2d vecNoRotationPos = { 0.0f, 0.0f };

		olc::Decal* decAnimDecal = nullptr;
	};

private:
	std::vector<Animation> anims;

public:

	std::string errorMessage = ""; // you can access the last recorded error message from your parent classes in order to troubleshoot animation errors

	// Add a standard animation that can rotate around an origin (default)
	inline void AddAnimation(std::string animName, float duration, int numFrames, olc::Decal* decal, olc::vf2d firstFramePos, olc::vf2d frameSize, olc::vf2d origin = { 0.0f, 0.0f }, olc::vf2d frameDisplayOffset = { 0.0f, 0.0f }, bool horizontalSprite = true);


	// Add an animation that does not play by default, used for manually switching between frames on demand (ie switching between items on a HUD...)
	inline void AddStaticAnimation(std::string animName, int numFrames, olc::Decal* decal, olc::vf2d firstFramePos, olc::vf2d frameSize, olc::vf2d nextFrameOffset, bool horizontalSprite = true);


	// Add an animation that can rotate around an origin, but will always display upright (ie particle effects, flame, etc...)
	inline void AddBillboardAnimation(std::string animName, float duration, int numFrames, olc::Decal* decal, olc::vf2d firstFramePos, olc::vf2d frameSize, olc::vf2d frameDisplayOffset = { 0.0f, 0.0f }, bool horizontalSprite = true);
	

	inline void SetNextAnimation(std::string animName, std::string nextAnimName, bool bPlayOnce = false); // v1.1
	inline Animation* GetAnim(std::string name);

	inline void Play(std::string name, bool bPlayOnce = false, int startFrame = 0);
	inline void PlayAfterSeconds(std::string name, float seconds, bool bPlayOnce = false, int startFrame = 0); // v1.1
	inline void Stop(std::string name, bool bAfterCompletion = false);
	inline void StopAll();
	inline void UpdateAnimations(float fElapsedTime);

	inline void DrawAnimationFrame(olc::vf2d pos, float angle, olc::vf2d scale);

private:
	inline bool DuplicateAnimationExists(std::string name);
};

void olcPGEX_Animator2D::AddAnimation(std::string animName, float duration, int numFrames, olc::Decal* decal, olc::vf2d firstFramePos, olc::vf2d frameSize, olc::vf2d origin, olc::vf2d frameDisplayOffset, bool horizontalSprite)
{
	// Prevent multiple animations with the same name
	if (!DuplicateAnimationExists(animName)) return;

	// Add new animation to list of animations
	Animation newAnim;

	newAnim.strName = animName;
	newAnim.fDuration = duration;
	newAnim.nNumberOfFrames = numFrames;
	newAnim.fFrameLength = duration / numFrames;

	newAnim.vecFramePos = firstFramePos;
	newAnim.vecFrameSize = frameSize;

	if (horizontalSprite)
		newAnim.vecNextFrameOffset = { frameSize.x, 0.0f };
	else
		newAnim.vecNextFrameOffset = { 0.0f, frameSize.y };

	newAnim.vecFrameDisplayOffset = frameDisplayOffset;
	newAnim.vecOrigin = origin;
	newAnim.decAnimDecal = decal;

	anims.push_back(newAnim);
}

void olcPGEX_Animator2D::AddStaticAnimation(std::string animName, int numFrames, olc::Decal* decal, olc::vf2d firstFramePos, olc::vf2d frameSize, olc::vf2d nextFrameOffset, bool horizontalSprite)
{
	// Prevent multiple animations with the same name
	if (!DuplicateAnimationExists(animName)) return;

	// Add new animation to list of animations
	Animation newAnim;

	newAnim.strName = animName;
	newAnim.nNumberOfFrames = numFrames;

	newAnim.vecFramePos = firstFramePos;
	newAnim.vecFrameSize = frameSize;

	if (horizontalSprite)
		newAnim.vecNextFrameOffset = { frameSize.x, 0.0f };
	else
		newAnim.vecNextFrameOffset = { 0.0f, frameSize.y };

	newAnim.vecNextFrameOffset = nextFrameOffset;
	newAnim.decAnimDecal = decal;

	anims.push_back(newAnim);
}

void olcPGEX_Animator2D::AddBillboardAnimation(std::string animName, float duration, int numFrames, olc::Decal* decal, olc::vf2d firstFramePos, olc::vf2d frameSize, olc::vf2d frameDisplayOffset, bool horizontalSprite)
{
	// Prevent multiple animations with the same name
	if (!DuplicateAnimationExists(animName)) return;

	// Add new animation to list of animations
	Animation newAnim;

	newAnim.strName = animName;
	newAnim.fDuration = duration;
	newAnim.nNumberOfFrames = numFrames;
	newAnim.fFrameLength = duration / numFrames;

	newAnim.vecFramePos = firstFramePos;
	newAnim.vecFrameSize = frameSize;

	if (horizontalSprite)
		newAnim.vecNextFrameOffset = { frameSize.x, 0.0f };
	else
		newAnim.vecNextFrameOffset = { 0.0f, frameSize.y };

	newAnim.vecFrameDisplayOffset = frameDisplayOffset;
	newAnim.decAnimDecal = decal;

	newAnim.bBillboardAnimation = true;

	anims.push_back(newAnim);
}

void olcPGEX_Animator2D::SetNextAnimation(std::string animName, std::string nextAnimName, bool bPlayOnce)
{
	for (auto& a : anims)
		if (a.strName == animName)
		{
			a.strPlayNext = nextAnimName;
			a.bStopNextAfterComplete = bPlayOnce;
		}

	return;

	errorMessage = "Animation (" + animName + ") - not a valid animation name... [SetNextAnimation]";
}

olcPGEX_Animator2D::Animation* olcPGEX_Animator2D::GetAnim(std::string name)
{
	for (auto& a : anims)
		if (a.strName == name) return &a;

	errorMessage = "Unable to get animation (" + name + ") - not a valid animation name... [GetAnim]";
	return nullptr;
}

void olcPGEX_Animator2D::Play(std::string name, bool bPlayOnce, int startFrame)
{
	for (auto& a : anims)
		if (a.strName == name)
		{
			a.bIsPlaying = true;

			// Prevent starting on an invalid frame
			if (a.fPlayAfterSeconds == -1.0f)
			{
				if (a.nCurrentFrame > a.nNumberOfFrames) a.nCurrentFrame = 0;
				a.fPlayAfterSeconds = 0.0f;
			}
			else
			{
				a.nCurrentFrame = startFrame < a.nNumberOfFrames ? startFrame : 0;
				a.bStopAfterComplete = bPlayOnce;
			}

			a.fFrameTick = 0.0f;

			return;
		}

	errorMessage = "Unable to play animation (" + name + ") - not a valid animation name... [Play]";
}

void olcPGEX_Animator2D::PlayAfterSeconds(std::string name, float seconds, bool bPlayOnce, int startFrame)
{
	for (auto& a : anims)
		if (a.strName == name)
		{
			a.fPlayAfterSeconds = seconds;
			a.nCurrentFrame = startFrame < a.nNumberOfFrames ? startFrame : 0;
			a.bStopAfterComplete = bPlayOnce;
		}

	return;

	errorMessage = "Unable to play animation (" + name + ") - not a valid animation name... [PlayAfterSeconds]";
}

void olcPGEX_Animator2D::Stop(std::string name, bool bAfterCompletion)
{
	for (auto& a : anims)
		if (a.strName == name)
		{
			if (bAfterCompletion)
			{
				a.bStopAfterComplete = true;
				return;
			}
			else
			{
				a.bIsPlaying = false;
				if (a.strPlayNext != "")
					Play(a.strPlayNext, a.bStopNextAfterComplete);

				return;
			}
		}

	errorMessage = "Unable to stop animation (" + name + ") - not a valid animation name... [Stop]";
}

void olcPGEX_Animator2D::StopAll()
{
	for (auto& a : anims)
		Stop(a.strName);
}

void olcPGEX_Animator2D::UpdateAnimations(float fElapsedTime)
{
	for (auto& a : anims)
	{
		a.bWasPlaying = a.bIsPlaying;

		if (a.fPlayAfterSeconds > 0.0f)
		{
			a.fPlayAfterSeconds -= fElapsedTime;
			if (a.fPlayAfterSeconds <= 0.0f)
			{
				a.fPlayAfterSeconds = -1.0f; // Tell the play function to use existing Play presets from previous call...
				Play(a.strName);
			}
		}

		if (a.bIsPlaying && a.fDuration >= 0.0f)
		{
			a.fFrameTick += fElapsedTime;
			if (a.fFrameTick > a.fFrameLength)
			{
				a.fFrameTick -= a.fFrameTick;
				a.nCurrentFrame++;
				if (a.nCurrentFrame == a.nNumberOfFrames)
				{
					a.nCurrentFrame = 0;
					if (a.bStopAfterComplete)
					{
						a.bIsPlaying = false;
						if (a.strPlayNext != "")
							Play(a.strPlayNext, a.bStopNextAfterComplete);
					}
				}
			}
		}
	}
}

void olcPGEX_Animator2D::DrawAnimationFrame(olc::vf2d pos, float angle, olc::vf2d scale = {1.0f, 1.0f})
{
	for (auto& a : anims)
		if (a.bIsPlaying)
		{
			if (a.nCurrentFrame > a.nNumberOfFrames - 1) a.nCurrentFrame = a.nNumberOfFrames - 1;

			if (a.bBillboardAnimation)
			{
				// translate pos based on rotation around origin
				float s = sinf(angle);
				float c = cosf(angle);

				olc::vf2d vecBillboardPos;

				vecBillboardPos.x = c * a.vecFrameDisplayOffset.x - s * a.vecFrameDisplayOffset.y + a.vecOrigin.x;
				vecBillboardPos.y = s * a.vecFrameDisplayOffset.x + c * a.vecFrameDisplayOffset.y + a.vecOrigin.y;

				// offset to account for frame size
				vecBillboardPos.x -= a.vecFrameSize.x * 0.5f;
				vecBillboardPos.y -= a.vecFrameSize.y;

				pge->DrawPartialDecal(pos + vecBillboardPos, a.decAnimDecal, a.vecFramePos + (a.vecNextFrameOffset * a.nCurrentFrame), a.vecFrameSize);
			}
			else
				pge->DrawPartialRotatedDecal(pos, a.decAnimDecal, angle, a.vecOrigin - a.vecFrameDisplayOffset, a.vecFramePos + (a.vecNextFrameOffset * a.nCurrentFrame), a.vecFrameSize, scale);
		}
}

bool olcPGEX_Animator2D::DuplicateAnimationExists(std::string name)
{
	for (auto& a : anims)
		if (a.strName == name)
		{
			errorMessage = "Tried to create multiple animations with the same name... [AddAnimation_XX]";
			return false;
		}

	return true;
}

#endif
