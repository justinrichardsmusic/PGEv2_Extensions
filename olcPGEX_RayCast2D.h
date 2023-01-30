/*
	olcPGEX_RayCast2D.h

	+-------------------------------------------------------------+
	|         OneLoneCoder Pixel Game Engine Extension            |
	|                   RayCast2D - v1.1                          |
	+-------------------------------------------------------------+

	What is this?
	~~~~~~~~~~~~~
	This is an extension to the olcPixelGameEngine v2.0 and above.
	It is quite simply a ray caster using line intersection.

	It can be used to detect collision / intersection between two
	edges and will return the distance to the intesection from the
	beginning of the ray, and also the pixel location of the 
	intersection.
	
	How to use it?
	~~~~~~~~~~~~~~
	Include the header file in your main class, underneath the
	olcPixelGameEngine header.  Remember to define the implementation
	guard aswell (only do this once)...

		#include "olcPixelGameEngine.h"

		#define OLC_PGEX_RAYCAST2D_IMPLEMENTATION
		#include "olcPGEX_RayCast2D.h"
	
	Then create an instance of the raycaster in your class declarations
	
		private:
			olcPGEX_RayCast2D rayCaster2D;
			
	And last of all, just call the RayCastVsEdge function where needed
	Here is an example that could be looped through to determine which
	edge is the closest to the ray start point (usually the player)
	
	It assumes you are accumulating the fHitDistance and vecHitPosition
	so that you can determine the closest intersection...
	
		bool bOverlap = (rayCaster2D.RayCastVsEdge(vecPlayerRayStart, vecPlayerRayEnd, vecWallEdgeStart, vecWallEdgeEnd, vecHitPosition, fHitDistance))
			
		if (bOverlap)
		{
			if (fClosestHitDistance > fHitDistance)
			{
				fClosestHitDistance = fHitDistance;
				vecClosestHitPosition = vecHitPosition;
			}
			bOverlap = true;
		}
		
	
	This is of course just a simple example, you can use this raycaster
	in many different ways to suit your own program.
	
	I hope this will be useful for others as it has been for me :-)
		


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


#ifndef OLC_PGEX_RAYCAST2D
#define OLC_PGEX_RAYCAST2D

#include "olcPixelGameEngine.h"

class olcPGEX_RayCast2D : public olc::PGEX
{
public:
	bool RayCastVsEdge(const olc::vf2d vecRayStart, const olc::vf2d vecRayEnd, const olc::vf2d vecEdgeStart, const olc::vf2d vecEdgeEnd, olc::vf2d& vecHit, float& fDistance);
};

#ifdef OLC_PGEX_RAYCAST2D_IMPLEMENTATION
#undef OLC_PGEX_RAYCAST2D_IMPLEMENTATION

bool olcPGEX_RayCast2D::RayCastVsEdge(const olc::vf2d vecRayStart, const olc::vf2d vecRayEnd, const olc::vf2d vecEdgeStart, const olc::vf2d vecEdgeEnd, olc::vf2d& vecHit, float& fRayHitDistance)
{
	const olc::vf2d vecRayDistance = vecRayEnd - vecRayStart;
	const olc::vf2d vecEdgeDistance = vecEdgeEnd - vecEdgeStart;

	// Test if the edges could possibly overlap given their start and end positions
	// otherwise, no need to test further
	if (fabs(vecEdgeDistance.x - vecRayDistance.x) > 0.0f && fabs(vecEdgeDistance.y - vecRayDistance.y) > 0.0f)
	{
		// Calculate the distance (normalised) along each ray to where the intersection point is
		const float t2 = (vecRayDistance.x * (vecEdgeStart.y - vecRayStart.y) + (vecRayDistance.y * (vecRayStart.x - vecEdgeStart.x))) / (vecEdgeDistance.x * vecRayDistance.y - vecEdgeDistance.y * vecRayDistance.x);
		const float t1 = (vecEdgeStart.x + vecEdgeDistance.x * t2 - vecRayStart.x) / vecRayDistance.x;
	
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

#endif			// Implementation Guard
#endif			// Header Guard
