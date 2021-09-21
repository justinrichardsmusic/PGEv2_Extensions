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

	olc::Pixel pTint =						olc::WHITE;
	float fAlpha =							0.0f;
	float fTransitionDirection =			0.0f;					// -1.0f OUT, 0.0f OFF, 1.0f IN
	float fSpeed =							1.0f;					// 1.0f = 1 second, scale accordingly

public:
	int nID =								0;
	bool bActive =							false;

private:
	void			UpdateAndDraw			(float elapsedTime, olc::vf2d screenSize);

public:
	static void		AddTransitionType		(std::vector<olcPGEX_Transition>& transitionGroup, int ID, olc::Decal* decal, olc::Pixel tint);
	static void		SetDefaultTransitions	(std::vector<olcPGEX_Transition>& transitionGroup, olc::Decal* decal);
	static void		ProcessTransitions		(std::vector<olcPGEX_Transition>& transitionGroup, float elapsedTime, olc::vf2d screenSize);
	
	void			StartSingleTransition	(float direction, float speed = 1.0f);
};

#ifdef TRANSITION_IMPLEMENTATION
#undef TRANSITION_IMPLEMENTATION

olcPGEX_Transition::olcPGEX_Transition(int ID, olc::Decal* decal, olc::Pixel tint)
{
	nID =				ID;
	decTransition =		decal;
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
