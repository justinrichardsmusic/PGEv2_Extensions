#include "olcPGEX_Menu.h"

olcPGEX_Menu::olcPGEX_Menu() {}

olcPGEX_MenuItem* olcPGEX_Menu::GetMenuItemByID(const int ID)
{
	for (auto& m : menu)
		if (m->nID == ID)
			return m;

	std::cout << "ID not found in menu list [GetMenuItemByID]";

	return nullptr;
}

void olcPGEX_Menu::AddMenuItem(const int ID, olc::Decal* decal, const olc::vf2d pos, const olc::vf2d size, const olc::vf2d sourcePos, const bool visible, const bool enabled, const float zoomFactor, const bool centered, const bool isStatic)
{
	menu.push_back(new olcPGEX_MenuItem( ID, decal, pos, size, sourcePos, visible, enabled, zoomFactor, centered, isStatic ));
}

void olcPGEX_Menu::SetMenuVisibility(const bool visible)
{
	if (menu.size() > 0)
	{
		for (auto& m : menu)
			m->bVisible = visible;

	}
	else
		std::cout << "Menu was empty! [SetMenuVisibility]";

}

void olcPGEX_Menu::StartTransition(const float transitionDirection, const float speed)
{
	if (menu.size() > 0)
	{
		for (auto& m : menu)
		{
			m->bVisible = m->bIncludeInTransition;
			m->fTransitionDirection = transitionDirection;
			m->fTransitionSpeed = speed;

			if (transitionDirection > 0.0f)
			{
				m->fCurrentAlpha = 0.0f;
				m->fCurrentZoom = 1.0f;
				m->vecDrawSize = m->vecSize;
			}
		}
	}
	else
		std::cout << "Menu was empty! [StartTransition]";

}

int olcPGEX_Menu::ProcessMenuInteractions(const float fElapsedTime, const olc::vi2d mousePos, const bool leftMouseButtonReleased)
{
	int nReturnID = -1;

	for (auto& m : menu)
	{
		if (m->bVisible)												// Requires Drawing
		{
			if (m->bEnabled)											// Requires Updating
			{
				m->Update(fElapsedTime, mousePos);
				if (m->fTransitionDirection == NO_TRANSITION)			// Requires Interaction
				{
					if (leftMouseButtonReleased && m->bHover)
					{
						nReturnID = m->nID;								// Set returnID to corressponding interaction
					}
				}
			}
			m->Draw();
		}
	}

	return nReturnID;
}
