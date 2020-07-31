#ifndef OLC_PGEX_RAYCAST2D
#define OLC_PGEX_RAYCAST2D

class olcPGEX_RayCast2D : public olc::PGEX
{
public:
	inline bool RayCastVsEdge(olc::vf2d vecRayStart, olc::vf2d vecRayEnd, olc::vf2d vecEdgeStart, olc::vf2d vecEdgeEnd, olc::vf2d& vecHit, float& fDistance);
};

bool olcPGEX_RayCast2D::RayCastVsEdge(olc::vf2d vecRayStart, olc::vf2d vecRayEnd, olc::vf2d vecEdgeStart, olc::vf2d vecEdgeEnd, olc::vf2d& vecHit, float& fRayHitDistance)
{
	olc::vf2d vecRayDistance = vecRayEnd - vecRayStart;
	olc::vf2d vecEdgeDistance = vecEdgeEnd - vecEdgeStart;

	// Test if the edges could possibly overlap given their start and end positions
	// otherwise, no need to test further
	if (fabs(vecEdgeDistance.x - vecRayDistance.x) > 0.0f && fabs(vecEdgeDistance.y - vecRayDistance.y) > 0.0f)
	{
		// Calculate the distance (normalised) along each ray to where the intersection point is
		float t2 = (vecRayDistance.x * (vecEdgeStart.y - vecRayStart.y) + (vecRayDistance.y * (vecRayStart.x - vecEdgeStart.x))) / (vecEdgeDistance.x * vecRayDistance.y - vecEdgeDistance.y * vecRayDistance.x);
		float t1 = (vecEdgeStart.x + vecEdgeDistance.x * t2 - vecRayStart.x) / vecRayDistance.x;
	
		// If there is an intersection we need to record it
		if (t1 > 0.0f && t1 < 1.0f && t2 > 0.0f && t2 <= 1.0f)
		{
			// Set user provided hit location vector to our hit location
			vecHit = vecRayStart + vecRayDistance * t1;

			// Set user provided hit distance (normalised) for calculating minimum hit distance in parent class
			fRayHitDistance = t1;

			// Return true if hit was detected
			return true;
		}
	}

	// Return false as there was no hit detected
	return false;
}

#endif