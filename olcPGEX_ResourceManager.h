/*
	olcPGEX_ResourceManager.h

	+-------------------------------------------------------------+
	|         OneLoneCoder Pixel Game Engine Extension            |
	|                ResourceManager - v1.0				          |
	+-------------------------------------------------------------+

	What is this?
	~~~~~~~~~~~~~
	This is an extension to the olcPixelGameEngine v2.0 and above.
	Its features are simple... You supply a filename to load, and 
	the resource manager will only load the file if it has not
	already been loaded.

	It was designed to be used with decals in PGE_v2xx, however the
	Sprite info can be accessed via the decal->sprite method already
	avaialable in PGE...

	You generally create one instance of the resource manager in your
	main class and then pass it by reference into any class other than
	your main class that may need to load sprites or decals.

	It has only one function - RM_Sprite() which serves to both load
	the sprite file, or simply return a reference to it when needed.

	It has no "garbage collection" so be wary of using it in loops.
	Any sprites or decals loaded with this extention under normal usage
	will be destroyed upon application exit in most operating systems.

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

#ifndef OLC_PGEX_RESOURCE_MANAGER
#define OLC_PGEX_RESOURCE_MANAGER

class olcPGEX_ResourceManager : public olc::PGEX
{
private:
	struct spriteResource
	{
		olc::Sprite* spr;
		olc::Decal* dec;
		std::string fileName;
	};

	std::list<spriteResource> resSprites;

public:
	inline olc::Decal* RM_Sprite(std::string spriteFileName);
};

olc::Decal* olcPGEX_ResourceManager::RM_Sprite(std::string spriteFileName)
{
	spriteResource resCurrentSprite;

	for (auto& s : resSprites)
		if (s.fileName == spriteFileName)
			return s.dec;

	resCurrentSprite.spr = new olc::Sprite(spriteFileName);
	resCurrentSprite.dec = new olc::Decal(resCurrentSprite.spr);
	resCurrentSprite.fileName = spriteFileName;

	resSprites.push_back(resCurrentSprite);

	return resCurrentSprite.dec;
}

#endif


