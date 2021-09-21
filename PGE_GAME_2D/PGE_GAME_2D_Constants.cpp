#include "PGE_GAME_2D_Constants.h"

bool GAME2D::PointInsideRect(const olc::vf2d &point, const olc::vf2d &rectPos, const olc::vf2d &rectSize)
{
	return	point.x > rectPos.x &&
			point.y > rectPos.y &&
			point.x < rectPos.x + rectSize.x &&
			point.y < rectPos.y + rectSize.y;
}

bool GAME2D::RectVsRect(const olc::vf2d &pos1, const olc::vf2d &size1, const olc::vf2d &pos2, const olc::vf2d &size2)
{
	return	PointInsideRect(pos1, pos2, size2) ||
			PointInsideRect(pos1 + size1, pos2, size2) ||
			PointInsideRect({ pos1.x, pos1.y + size1.y }, pos2, size2) ||
			PointInsideRect({ pos1.x + size1.x, pos1.y }, pos2, size2);
}
