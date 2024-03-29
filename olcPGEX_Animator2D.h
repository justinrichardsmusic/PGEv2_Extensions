/*
	olcPGEX_Animator2D.h

	+-------------------------------------------------------------+
	|         OneLoneCoder Pixel Game Engine Extension            |
	|                Animator2D - v1.7			      |
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

	Ability to flip animation sequences if required, as well as applying
	a "ping pong" mode in order to cycle backwards and forwards through a
	single animation if needed...


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

			// First of all, be sure to include the PGEX and DEFINE as necessary
			#define ANIMATOR_IMPLEMENTATION
			#include "olcPGEX_Animator2D.h"


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



	-----------------------
	  v1.2 - NEW FEATURES
	-----------------------

	Added Ping Pong animations (traverse frames forwards, then backwards)
	Added ability to play flipped animations



	-----------------------
	  v1.3 - NEW FEATURES
	-----------------------

	Renamed "Flipped" animations to "PlayInReverse" as they do just that and
	don't actually flip the image in any way... still useful to play animations
	backwards...

	Added MirroredImage as an option (proper flipping along either (or both) axis)...



	-----------------------
	  v1.4 - NEW FEATURES
	-----------------------

	Added two new functions ScaleAnimation and TintAnimation.  You can now scale
	and tint any individual animation in the list of animations on each animator
	controller.  The scaling will work with rotation around the origin set for the
	animation which means you don't have to adjust any of these in order to scale
	a rotated animation :-)



	-----------------------
	  v1.5 - NEW FEATURES
	-----------------------

	Added the ability to Pause and Resume an animation using the Pause() Function.
	It will pause by default, if you want to resume the animation simply pass 'false'
	in after the animation name.



	-----------------------
	  v1.6 - NEW FEATURES  
	-----------------------

	General code cleanup as well as added the ability to check a trigger state called
	bHasStopped which will be set to TRUE on the frame that the animation stops, whether
	it is told to stop manually, or stops automatically after its animation frames are
	over.

	It can be accessed using GetAnim("animation_name")->bHasStopped and will be reset to
	FALSE each time the UpdateAnimations function is called.



	-----------------------
	  v1.7 - NEW FEATURES
	-----------------------

	Added the ability to dynamically adjust an animation's duration, whether it be playing,
	paused, or stopped.

	Just call the AdjustAnimationDuration function with a new duration setting and the
	animator will re-calculate all the required settings for you :-)

	Simple.

	[IMPORTANT NOTE]
	The INLINE functions have been removed and IMPLEMENTATION GUARDS have been added
	instead.

	This now means you will need to add a #define above the location that you first
	include the Animator2D PGEX like so...

			#define ANIMATOR_IMPLEMENTATION
			#include "olcPGEX_Animator2D.h"

	This is due to the way the animator is sometimes used which causes the inline functions
	to error out.




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

class olcPGEX_Animator2D : public olc::PGEX
{
public:
	struct Animation
	{
		std::string		strName =				"";
		std::string		strPlayNext =				"";			// v1.1

		bool			bIsPlaying =				false;
		bool			bIsPaused =				false;			// v1.5
		bool			bHasStopped =				false;			// v1.6
		bool			bStopAfterComplete =			false;
		bool			bStopNextAfterComplete =		false;			// v1.1
		bool			bBillboardAnimation =			false;
		bool			bPlayInReverse =			false;			// v1.2 (updated v1.3)
		bool			bPingPong =				false;			// v1.2

		float			fDuration =				-1.0f;
		int			nNumberOfFrames =			-1;
		float			fFrameLength =				0.0f;
		int			nCurrentFrame =				0;
		int			nFrameIncrement =			1;
		float			fFrameTick =				0.0f;
		float			fPlayAfterSeconds =			0.0f;			// v1.1

		olc::vf2d		vecFramePos				{};
		olc::vf2d		vecFrameSize				{};
		olc::vf2d		vecNextFrameOffset			{};
		olc::vf2d		vecFrameDisplayOffset =			{};
		olc::vf2d		vecOrigin =				{};
		olc::vf2d		vecNoRotationPos =			{};
		olc::vf2d		vecMirrorImage =			{};			// v1.3 - set either axis or both to -1.0f to mirror the sprite image
		olc::vf2d		vecScale =				{ 1.0f, 1.0f }; 	// v1.4
		olc::Pixel		pTint =					olc::WHITE;		// v1.4

		olc::Decal*		decAnimDecal =				nullptr;
	};

private:
	std::vector<Animation> anims;

public:
	std::string		errorMessage = "";			// you can access the last recorded error message from your parent classes in order to troubleshoot animation errors

									// Add a standard animation that can rotate around an origin (default)
	const void		AddAnimation				(const std::string& animName, const float duration, const int numFrames, olc::Decal* decal, const olc::vf2d firstFramePos, const olc::vf2d frameSize, const olc::vf2d origin = { 0.0f, 0.0f }, const olc::vf2d frameDisplayOffset = { 0.0f, 0.0f }, const bool horizontalSprite = true, const bool playInReverse = false, const bool pingpong = false, const olc::vf2d mirrorImage = { 0.0f, 0.0f });


									// Add an animation that does not play by default, used for manually switching between frames on demand (ie switching between items on a HUD...)
	const void		AddStaticAnimation			(const std::string& animName, const int numFrames, olc::Decal* decal, const olc::vf2d firstFramePos, const olc::vf2d frameSize, const olc::vf2d nextFrameOffset, const bool horizontalSprite = true, const bool playInReverse = false, const bool pingpong = false, const olc::vf2d mirrorImage = { 0.0f, 0.0f });


									// Add an animation that can rotate around an origin, but will always display upright (ie particle effects, flame, etc...)
	const void		AddBillboardAnimation			(const std::string& animName, const float duration, const int numFrames, olc::Decal* decal, const olc::vf2d firstFramePos, const olc::vf2d frameSize, const olc::vf2d frameDisplayOffset = { 0.0f, 0.0f }, const bool horizontalSprite = true, const bool playInReverse = false, const bool pingpong = false, const olc::vf2d mirrorImage = { 0.0f, 0.0f });


	const void		SetNextAnimation			(const std::string& animName, const std::string& nextAnimName, const bool bPlayOnce = false); // v1.1
	Animation*		GetAnim					(const std::string& name);

	const void		Play					(const std::string& name, const bool bPlayOnce = false, const int startFrame = 0);
	const void		PlayAfterSeconds			(const std::string& name, const float seconds, const bool bPlayOnce = false, const int startFrame = 0); // v1.1
	const bool		IsAnyAnimationPlaying			();
	const void		Stop					(const std::string& name, const bool bAfterCompletion = false);
	const void		StopAll					();
	const void		Pause					(const std::string& name, const bool bPaused = true); // v1.5
	const void		UpdateAnimations			(const float fElapsedTime);

	const void		DrawAnimationFrame			(const olc::vf2d pos, const float angle = 0.0f);

	const void		ScaleAnimation				(const std::string& animToScale, const olc::vf2d scale); // v1.6
	const void		TintAnimation				(const std::string& animToTint, const olc::Pixel tint); // v1.6
	const void		AdjustAnimationDuration			(const std::string& animToAdjust, const float newDuration); // v1.7

private:
	const bool		DuplicateAnimationExists		(const std::string& name);
};



#ifdef ANIMATOR_IMPLEMENTATION
#undef ANIMATOR_IMPLEMENTATION

const void olcPGEX_Animator2D::AddAnimation(const std::string& animName, const float duration, const int numFrames, olc::Decal* decal, const olc::vf2d firstFramePos, const olc::vf2d frameSize, const olc::vf2d origin, const olc::vf2d frameDisplayOffset, const bool horizontalSprite, const bool playInReverse, const bool pingpong, const olc::vf2d mirrorImage)
{
	// Prevent multiple animations with the same name
	if (!DuplicateAnimationExists(animName))
		return;

	// Add new animation to list of animations
	Animation newAnim;

	newAnim.strName =				animName;
	newAnim.fDuration =				duration;
	newAnim.nNumberOfFrames =			numFrames;
	newAnim.fFrameLength =				duration / numFrames;

	newAnim.vecFramePos =				firstFramePos;
	newAnim.vecFrameSize =				frameSize;
	newAnim.bPlayInReverse =			playInReverse;

	if (horizontalSprite)
		newAnim.vecNextFrameOffset =	newAnim.bPlayInReverse ? olc::vf2d(-frameSize.x, 0.0f) : olc::vf2d(frameSize.x, 0.0f);
	else
		newAnim.vecNextFrameOffset =	newAnim.bPlayInReverse ? olc::vf2d(0.0f, -frameSize.y) : olc::vf2d(0.0f, frameSize.y);

	newAnim.vecFrameDisplayOffset =			frameDisplayOffset;
	newAnim.vecOrigin =				origin;
	newAnim.decAnimDecal =				decal;
	newAnim.bPingPong =				pingpong;
	newAnim.vecMirrorImage =			mirrorImage;

	anims.push_back(newAnim);
}

const void olcPGEX_Animator2D::AddStaticAnimation(const std::string& animName, const int numFrames, olc::Decal* decal, const olc::vf2d firstFramePos, const olc::vf2d frameSize, const olc::vf2d nextFrameOffset, const bool horizontalSprite, const bool playInReverse, const bool pingpong, const olc::vf2d mirrorImage)
{
	// Prevent multiple animations with the same name
	if (!DuplicateAnimationExists(animName))
		return;

	// Add new animation to list of animations
	Animation newAnim;

	newAnim.strName =				animName;
	newAnim.nNumberOfFrames =			numFrames;

	newAnim.vecFramePos =				firstFramePos;
	newAnim.vecFrameSize =				frameSize;
	newAnim.bPlayInReverse =			playInReverse;

	if (horizontalSprite)
		newAnim.vecNextFrameOffset =	newAnim.bPlayInReverse ? olc::vf2d(-frameSize.x, 0.0f) : olc::vf2d(frameSize.x, 0.0f);
	else
		newAnim.vecNextFrameOffset =	newAnim.bPlayInReverse ? olc::vf2d(0.0f, -frameSize.y) : olc::vf2d(0.0f, frameSize.y);

	newAnim.vecNextFrameOffset =			nextFrameOffset;
	newAnim.decAnimDecal =				decal;
	newAnim.bPingPong =				pingpong;
	newAnim.vecMirrorImage =			mirrorImage;

	anims.push_back(newAnim);
}

const void olcPGEX_Animator2D::AddBillboardAnimation(const std::string& animName, const float duration, const int numFrames, olc::Decal* decal, const olc::vf2d firstFramePos, const olc::vf2d frameSize, const olc::vf2d frameDisplayOffset, const bool horizontalSprite, const bool playInReverse, const bool pingpong, const olc::vf2d mirrorImage)
{
	// Prevent multiple animations with the same name
	if (!DuplicateAnimationExists(animName)) 
		return;

	// Add new animation to list of animations
	Animation newAnim;

	newAnim.strName =				animName;
	newAnim.fDuration =				duration;
	newAnim.nNumberOfFrames =			numFrames;
	newAnim.fFrameLength =				duration / numFrames;

	newAnim.vecFramePos =				firstFramePos;
	newAnim.vecFrameSize =				frameSize;
	newAnim.bPlayInReverse =			playInReverse;

	if (horizontalSprite)
		newAnim.vecNextFrameOffset =	newAnim.bPlayInReverse ? olc::vf2d(-frameSize.x, 0.0f) : olc::vf2d(frameSize.x, 0.0f);
	else
		newAnim.vecNextFrameOffset =	newAnim.bPlayInReverse ? olc::vf2d(0.0f, -frameSize.y) : olc::vf2d(0.0f, frameSize.y);

	newAnim.vecFrameDisplayOffset =			frameDisplayOffset;
	newAnim.decAnimDecal =				decal;
	newAnim.bBillboardAnimation =			true;
	newAnim.bPingPong =				pingpong;
	newAnim.vecMirrorImage =			mirrorImage;

	anims.push_back(newAnim);
}

const void olcPGEX_Animator2D::SetNextAnimation(const std::string& animName, const std::string& nextAnimName, const bool bPlayOnce)
{
	for (auto& a : anims)
		if (a.strName == animName)
		{
			a.strPlayNext =			nextAnimName;
			a.bStopNextAfterComplete =	bPlayOnce;
		}

	return;

	errorMessage = "Animation (" + animName + ") - not a valid animation name... [SetNextAnimation]";
}

olcPGEX_Animator2D::Animation* olcPGEX_Animator2D::GetAnim(const std::string& name)
{
	for (auto& a : anims)
		if (a.strName == name)
			return &a;

	errorMessage = "Unable to get animation (" + name + ") - not a valid animation name... [GetAnim]";
	return nullptr;
}

const void olcPGEX_Animator2D::Play(const std::string& name, const bool bPlayOnce, const int startFrame)
{
	for (auto& a : anims)
		if (a.strName == name)
		{
			a.bIsPlaying =			true;
			a.bIsPaused =			false;

			// Prevent starting on an invalid frame
			if (a.fPlayAfterSeconds == -1.0f)
			{
				if (a.nCurrentFrame > a.nNumberOfFrames) a.nCurrentFrame = 0;
				a.fPlayAfterSeconds =	0.0f;
			}
			else
			{
				a.nCurrentFrame = startFrame < a.nNumberOfFrames ? startFrame : 0;
				a.bStopAfterComplete =	bPlayOnce;
			}

			a.fFrameTick =			0.0f;
			a.nFrameIncrement =		1;

			return;
		}

	errorMessage = "Unable to play animation (" + name + ") - not a valid animation name... [Play]";
}

const void olcPGEX_Animator2D::PlayAfterSeconds(const std::string& name, const float seconds, const bool bPlayOnce, const int startFrame)
{
	for (auto& a : anims)
		if (a.strName == name)
		{
			a.fPlayAfterSeconds =		seconds;
			a.nCurrentFrame =		startFrame < a.nNumberOfFrames ? startFrame : 0;
			a.bStopAfterComplete =		bPlayOnce;
		}

	return;

	errorMessage = "Unable to play animation (" + name + ") - not a valid animation name... [PlayAfterSeconds]";
}

const bool olcPGEX_Animator2D::IsAnyAnimationPlaying()
{
	for (auto& a : anims)
		if (a.bIsPlaying)
			return true;

	return false;
}

const void olcPGEX_Animator2D::Stop(const std::string& name, const bool bAfterCompletion)
{
	for (auto& a : anims)
		if (a.strName == name)
		{
			if (bAfterCompletion)
			{
				a.bStopAfterComplete =	true;
				return;
			}
			else
			{
				a.bIsPlaying =		false;
				a.bIsPaused =		false;
				a.bHasStopped =		true;

				if (a.strPlayNext != "")
					Play(a.strPlayNext, a.bStopNextAfterComplete);

				return;
			}
		}

	errorMessage = "Unable to stop animation (" + name + ") - not a valid animation name... [Stop]";
}

const void olcPGEX_Animator2D::StopAll()
{
	for (auto& a : anims)
		Stop(a.strName);
}

const void olcPGEX_Animator2D::Pause(const std::string& name, const bool bPaused)
{
	for (auto& a : anims)
		if (a.strName == name)
			if (a.bIsPlaying)
			{
				if (a.bIsPaused)
					a.bIsPaused =	false;
				else
					a.bIsPaused =	true;

				return;
			}

	errorMessage = "Unable to pause/resume animation (" + name + ") - not a valid animation name... [Pause]";
}

const void olcPGEX_Animator2D::UpdateAnimations(const float fElapsedTime)
{
	for (auto& a : anims)
	{
		a.bHasStopped =				false;			// Reset the HasStopped value each update frame (as this is a trigger)

		if (a.fPlayAfterSeconds > 0.0f)
		{
			a.fPlayAfterSeconds -=		fElapsedTime;
			if (a.fPlayAfterSeconds <= 0.0f)
			{
				a.fPlayAfterSeconds =	-1.0f;			// Tell the play function to use existing Play presets from previous call...
				Play(a.strName);
			}
		}

		if (a.bIsPlaying && a.fDuration >= 0.0f)
		{
			if (!a.bIsPaused) a.fFrameTick += fElapsedTime;
			if (a.fFrameTick > a.fFrameLength)
			{
				a.fFrameTick -= a.fFrameTick;
				a.nCurrentFrame += a.nFrameIncrement;
				if (a.nCurrentFrame == a.nNumberOfFrames)
				{
					if (a.bPingPong)
					{
						a.nCurrentFrame--;
						a.nFrameIncrement = -1;
					}
					else
					{
						a.nCurrentFrame = 0;
						if (a.bStopAfterComplete)
						{
							a.bIsPlaying =		false;
							a.bIsPaused =		false;
							a.bHasStopped =		true;

							if (a.strPlayNext != "")
								Play(a.strPlayNext, a.bStopNextAfterComplete);

						}
					}
				}

				if (a.bPingPong && a.nCurrentFrame == 0)
				{
					if (a.bStopAfterComplete)
					{
						a.bIsPlaying =			false;
						a.bIsPaused =			false;
						a.bHasStopped =			true;

						if (a.strPlayNext != "")
							Play(a.strPlayNext, a.bStopNextAfterComplete);

					}
					else
					{
						a.nCurrentFrame++;
						a.nFrameIncrement = 1;
					}
				}
			}
		}
	}
}

const void olcPGEX_Animator2D::DrawAnimationFrame(const olc::vf2d pos, const float angle)
{
	for (auto& a : anims)
		if (a.bIsPlaying)
		{
			if (a.nCurrentFrame > a.nNumberOfFrames - 1) a.nCurrentFrame = a.nNumberOfFrames - 1;

			if (a.bBillboardAnimation)
			{
				// translate pos based on rotation around origin
				float s = angle == 0.0f ? 0.0f : sinf(angle);
				float c = angle == 0.0f ? 1.0f : cosf(angle);

				olc::vf2d vecBillboardPos;

				vecBillboardPos.x = c * a.vecFrameDisplayOffset.x - s * a.vecFrameDisplayOffset.y + a.vecOrigin.x;
				vecBillboardPos.y = s * a.vecFrameDisplayOffset.x + c * a.vecFrameDisplayOffset.y + a.vecOrigin.y;

				// offset to account for frame size
				vecBillboardPos.x -= a.vecFrameSize.x * 0.5f * a.vecScale.x;
				vecBillboardPos.y -= a.vecFrameSize.y * a.vecScale.y;

				pge->DrawPartialDecal(pos + vecBillboardPos + (-a.vecMirrorImage * a.vecFrameSize), a.decAnimDecal, a.vecFramePos + (a.vecNextFrameOffset * (float)a.nCurrentFrame), a.vecFrameSize, { a.vecMirrorImage.x < 0.0f ? -1.0f * a.vecScale.x : 1.0f * a.vecScale.x, a.vecMirrorImage.y < 0.0f ? -1.0f * a.vecScale.y : 1.0f * a.vecScale.y }, a.pTint);
			}
			else
				pge->DrawPartialRotatedDecal(pos, a.decAnimDecal, angle, a.vecOrigin - a.vecFrameDisplayOffset * a.vecScale, a.vecFramePos + (a.vecNextFrameOffset * (float)a.nCurrentFrame), a.vecFrameSize, { a.vecMirrorImage.x < 0.0f ? -1.0f * a.vecScale.x : 1.0f * a.vecScale.x, a.vecMirrorImage.y < 0.0f ? -1.0f * a.vecScale.y : 1.0f * a.vecScale.y }, a.pTint);
		}
}

const void olcPGEX_Animator2D::ScaleAnimation(const std::string& animToScale, const olc::vf2d scale)
{
	auto& a = GetAnim(animToScale)->vecScale =	scale;
}

const void olcPGEX_Animator2D::TintAnimation(const std::string& animToTint , const olc::Pixel tint)
{
	auto& a = GetAnim(animToTint)->pTint =		tint;
}

const void olcPGEX_Animator2D::AdjustAnimationDuration(const std::string& animToAdjust, const float newDuration)
{
	olcPGEX_Animator2D::Animation* a = GetAnim(animToAdjust);

	// Return early with an error if the specified animation is not found
	if (a == nullptr)
	{
		errorMessage = "Animation does not exist... [AdjustAnimationDuration]";
		return;
	}

	// Set the new duration and calculate the new frame length
	a->fDuration = newDuration;
	a->fFrameLength = a->fDuration / a->nNumberOfFrames;

	// If the animation is currently playing (or paused) then we need to adjust
	// the frameTick by the correct proportion based on the new frameLength
	if (a->bIsPlaying || a->bIsPaused)
	{
		const float fFractionOfCurrentFrame = a->fFrameTick / a->fFrameLength;
		a->fFrameTick = a->fFrameLength * fFractionOfCurrentFrame;
	}
}

const bool olcPGEX_Animator2D::DuplicateAnimationExists(const std::string& name)
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
#endif
