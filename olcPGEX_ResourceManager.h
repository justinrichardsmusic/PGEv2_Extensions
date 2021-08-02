/*
	olcPGEX_ResourceManager.h

	+-------------------------------------------------------------+
	|         OneLoneCoder Pixel Game Engine Extension            |
	|                ResourceManager - v1.1				          |
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



	-----------------------
	  v1.1 - NEW FEATURES
	-----------------------

	Added ability to set Sprite IDs when loading images so that string
	comparision is no longer necessary when checking if a sprite already
	exists in the list - optional optimisation, the resource manager will
	still function as expected if simply using strings without IDs

	Added an ERROR string which can be accessed to debug each function

	Added the ability to FREE sprites if the sprite is no longer needed.
	This can drastically decrease memory usage (GPU usage will not be effected).



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
		olc::Sprite* spr = nullptr;
		olc::Decal* dec = nullptr;
		std::string fileName = "";
		int ID = -1;
	};

	std::vector<spriteResource> resSprites;

public:
	std::string strError = "";

	inline olc::Decal* RM_Sprite(std::string spriteFileName);
	inline olc::Decal* RM_Sprite(int fileNameID, std::string spriteFileName = "");

	inline void RM_FreeSpriteData(std::string spriteFileName);
	inline void RM_FreeSpriteData(int fileNameID);
};

olc::Decal* olcPGEX_ResourceManager::RM_Sprite(std::string spriteFileName)
{
	strError = "";

	spriteResource resCurrentSprite;

	for (auto& s : resSprites)
		if (s.fileName == spriteFileName)
			return s.dec;

	resCurrentSprite.spr = new olc::Sprite(spriteFileName);
	resCurrentSprite.dec = new olc::Decal(resCurrentSprite.spr);
	resCurrentSprite.fileName = spriteFileName;
	resCurrentSprite.ID = resSprites.size() > 0 ? (resSprites.back().ID) + 1 : 0;

	resSprites.push_back(resCurrentSprite);

	return resCurrentSprite.dec;
}

// Optimised for faster comparison checks - uses ID (eg Enum Class) instead of string comparison
olc::Decal* olcPGEX_ResourceManager::RM_Sprite(int fileNameID, std::string spriteFileName)
{
	strError = "";

	spriteResource resCurrentSprite;

	// Optimised comparison check
	for (auto& s : resSprites)
		if (s.ID == fileNameID)
			return s.dec;

	// Allow for error checking if duplicate fileNameID exists
	if (resSprites.size() > 0)
		strError = fileNameID <= resSprites.back().ID ? "ERROR: RM_Sprite - fileNameID duplicate" : "";


	resCurrentSprite.spr = new olc::Sprite(spriteFileName);
	resCurrentSprite.dec = new olc::Decal(resCurrentSprite.spr);
	resCurrentSprite.fileName = spriteFileName;
	resCurrentSprite.ID = fileNameID;

	resSprites.push_back(resCurrentSprite);

	return resCurrentSprite.dec;
}

void olcPGEX_ResourceManager::RM_FreeSpriteData(std::string spriteFileName)
{
	strError = "";

	for (auto& s : resSprites)
		if (s.fileName == spriteFileName)
		{
			if (s.spr == nullptr)
				strError = "ERROR: RM_FreeSpriteData - Sprite Data Not Found";
			else
				s.spr->~Sprite();

			return;
		}

	strError = "ERROR: RM_FreeSpriteData - Sprite File Name Not Found";
}

void olcPGEX_ResourceManager::RM_FreeSpriteData(int fileNameID)
{
	strError = "";

	for (auto& s : resSprites)
		if (s.ID == fileNameID)
		{
			if (s.spr == nullptr)
				strError = "ERROR: RM_FreeSpriteData - Sprite Data Not Found";
			else
				s.spr->~Sprite();

			return;
		}

	strError = "ERROR: RM_FreeSpriteData - Sprite ID Not Found";
}


#endif


