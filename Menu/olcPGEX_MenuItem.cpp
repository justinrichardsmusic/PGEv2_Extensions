#include "olcPGEX_MenuItem.h"

olcPGEX_MenuItem::olcPGEX_MenuItem() {}

olcPGEX_MenuItem::olcPGEX_MenuItem(const int ID, olc::Decal* decal, const olc::vf2d pos, const olc::vf2d size, const olc::vf2d sourcePos, const bool visible, const bool enabled, const float zoomFactor, const bool centered, const bool isStatic)
{
	Construct(ID, decal, pos, size, sourcePos, visible, enabled, zoomFactor, centered, isStatic);
}

bool olcPGEX_MenuItem::i_FPointInsideRect(olc::vf2d point, olc::vf2d rPos, olc::vf2d rSize)
{
	return (point.x >= rPos.x &&
			point.y >= rPos.y &&
			point.x < rPos.x + rSize.x &&
			point.y < rPos.y + rSize.y);
}


void olcPGEX_MenuItem::Construct(const int ID, olc::Decal* decal, const olc::vf2d pos, const olc::vf2d size, const olc::vf2d sourcePos, const bool visible, const bool enabled, const float zoomFactor, const bool centered, const bool isStatic)
{
	nID = ID;

	bCentered = centered;
	Reposition(pos, size, centered);
	vecSourcePos = sourcePos;

	fZoomFactor = zoomFactor;
	bVisible = visible;
	bEnabled = enabled;
	bIsStatic = isStatic;

	decMenu = decal;
}

void olcPGEX_MenuItem::Update(const float fElapsedTime, const olc::vi2d mousePos)
{
	if (fTransitionDirection == 0)
	{
		if (!bIsStatic)
		{
			olc::vf2d fMousePos = { (float)mousePos.x, (float)mousePos.y };

			if (i_FPointInsideRect(fMousePos, vecPos, vecSize) && !bToggledOff)
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

void olcPGEX_MenuItem::Draw()
{
	pTint = bToggledOff ? olc::DARK_GREY : olc::WHITE;
	pTint.a = (uint8_t)(fCurrentAlpha * 255.0f);
	if (!bEnabled) pTint = olc::PixelF(0.25f, 0.25f, 0.25f, 0.25f * fCurrentAlpha);

	pge->DrawPartialDecal(vecCenterPos - vecDrawSize / 2.0f, decMenu, vecSourcePos, vecSize, { fCurrentZoom, fCurrentZoom }, pTint);
}

void olcPGEX_MenuItem::Reposition(const olc::vf2d newPos, const olc::vf2d newSize, const bool centered)
{
	if (newSize != olc::vf2d{ 0.0f, 0.0f })
		vecSize = newSize;

	vecHalfSize = vecSize / 2.0f;

	vecPos = (centered ? newPos - vecHalfSize : newPos);
	vecCenterPos = vecPos + vecHalfSize;
	vecDrawSize = vecSize;
}

