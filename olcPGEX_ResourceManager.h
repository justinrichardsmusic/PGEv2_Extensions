/*
	olcPGEX_ResourceManager.h

	+-------------------------------------------------------------+
	|         OneLoneCoder Pixel Game Engine Extension            |
	|                ResourceManager - v1.2				          |
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

	It has one main function - RM_Sprite() which serves to both load
	the sprite file and return its Decal, or simply return the Decal
	of the sprite should it already be loaded.

	It also has a secondary function - RM_FreeSpriteData() which can
	be used to delete the sprite data from memory leaving only the
	loaded instance of the decal available.  This can be useful when
	dealing with large sprites that never change state.  However, it is
	to be used with absolute discretion as trying to access the newly
	invalidated sprite will crash your application.

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



	-----------------------
	  v1.2 - NEW FEATURES
	-----------------------

	Added more error handling and polished the code in general.  Added two
	new 'helper' functions to handle the creation of new sprite resources and
	unloading of sprite data to reduce duplicate code.

	Removed inlined functions in favour of implementation guards.  This will
	break compatibility with previous versions only slightly.  Inclusion of
	the implementation guard define will now be required before the first include
	of the Resource Manager.



	-----------------------
	     HOW TO USE IT
	-----------------------

	Wherever you first include the resource manager (usually your main file)
	you need to define the implementation guard before including the PGEX.
	Do this only once, subsequent includes do not require additional defines.

			#define OLC_PGEX_RESOURCE_MANAGER_IMPLEMENTATION
			#include "olcPGEX_ResourceManager.h"


	Create an instance of the Resource Manager that will be passed around
	between files, once again your main file is an ideal place for this.

			olcPGEX_ResourceManager rm;


	Create a new sprite resource (or access an existing one) like this. A
	pointer to a Decal will be used for convenience.

			olc::Decal* decImage = rm.RM_Sprite("image.png");


	Or alternatively, use Image_ID's instead of strings.  This example will
	draw an already loaded sprite.  If you need to load a sprite, the file
	name must also be passed as a parameter into the RM_Sprite function.

			enum class IMG
			{
				BLANK,
				GRASS,
				TREE,
			}

			DrawDecal({ 0.0f, 0.0f }, rm.RM_Sprite(GRASS));



	Hopefully this quick explanation and example code should be sufficient
	for most users to get this PGEX working without too much effort.



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
#pragma once
#include "olcPixelGameEngine.h"

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
	bool i_NewSpriteResource(spriteResource& sprRes, const std::string& sprFileName);
	bool i_UnloadSpriteData(spriteResource& sprRes);

public:
	std::string strError = "";																// Should always be "", if not - you have an error (check the console log)

	olc::Decal* RM_Sprite(const std::string& spriteFileName);								// Use a File Name to create a new Sprite Resource and return its Decal, or if it exists - return that Decal Instance
	olc::Decal* RM_Sprite(const int fileNameID, const std::string& spriteFileName = "");	// Use an ID to create a new Sprite Resource and return its Decal, or if it exists - return that Decal Instance

	void RM_FreeSpriteData(const std::string& spriteFileName); 								// Locate a Sprite Resource by File Name and delete its Sprite Data (Will invalidate existing Sprite References, use with caution)
	void RM_FreeSpriteData(const int fileNameID);											// Locate a Sprite Resource by ID and delete its Sprite Data (Will invalidate existing Sprite References, use with caution)
};

#ifdef OLC_PGEX_RESOURCE_MANAGER_IMPLEMENTATION
#undef OLC_PGEX_RESOURCE_MANAGER_IMPLEMENTATION

bool olcPGEX_ResourceManager::i_NewSpriteResource(spriteResource& sprRes, const std::string& sprFileName)
{
	sprRes.spr = new olc::Sprite(sprFileName);
	sprRes.dec = new olc::Decal(sprRes.spr);
	sprRes.fileName = sprFileName;
	if (sprRes.ID == -1)
		sprRes.ID = resSprites.size() > 0 ? (resSprites.back().ID) + 1 : 0;

	resSprites.push_back(sprRes);

	if (sprRes.spr->pColData.size() > 0)
		return true;

	return false;
}

bool olcPGEX_ResourceManager::i_UnloadSpriteData(spriteResource& sprRes)
{
	if (!sprRes.spr->pColData.empty())
	{
		sprRes.spr->pColData.clear();
		sprRes.spr->pColData = std::vector<olc::Pixel>{};

		return true;
	}

	return false;
}

olc::Decal* olcPGEX_ResourceManager::RM_Sprite(const std::string& spriteFileName)
{
	strError = "";

	for (auto& s : resSprites)
		if (s.fileName == spriteFileName)
			return s.dec;

	spriteResource resCurrentSprite{};
	if (i_NewSpriteResource(resCurrentSprite, spriteFileName))
		return resCurrentSprite.dec;

	strError = "ERROR: RM_Sprite - Sprite data was empty...";
	return resCurrentSprite.dec;
}

olc::Decal* olcPGEX_ResourceManager::RM_Sprite(const int fileNameID, const std::string& spriteFileName)
{
	strError = "";

	if (spriteFileName != "")
	{
		for (auto& s : resSprites)
			if (s.fileName == spriteFileName)
			{
				strError = "ERROR: RM_Sprite - Tried to add existing sprite to new ID, existing ID used";
				return s.dec;
			}

		if (resSprites.size() > 0)
		strError = fileNameID <= resSprites.back().ID ? "ERROR: RM_Sprite - fileNameID duplicate or created out of order" : "";
	}

	for (auto& s : resSprites)
		if (s.ID == fileNameID)
			return s.dec;

	spriteResource resCurrentSprite{};
	resCurrentSprite.ID = fileNameID;

	if (i_NewSpriteResource(resCurrentSprite, spriteFileName))
		return resCurrentSprite.dec;

	strError = "ERROR: RM_Sprite - Sprite data was empty...";
	return resCurrentSprite.dec;
}

void olcPGEX_ResourceManager::RM_FreeSpriteData(const std::string& spriteFileName)
{
	strError = "";

	for (auto& s : resSprites)
		if (s.fileName == spriteFileName)
		{
			if (!i_UnloadSpriteData(s))
				strError = "ERROR: RM_FreeSpriteData - Sprite Data Not Found";

			return;
		}

	strError = "ERROR: RM_FreeSpriteData - Sprite File Name Not Found";
}

void olcPGEX_ResourceManager::RM_FreeSpriteData(const int fileNameID)
{
	strError = "";

	for (auto& s : resSprites)
		if (s.ID == fileNameID)
		{
			if (!i_UnloadSpriteData(s))
				strError = "ERROR: RM_FreeSpriteData - Sprite Data Not Found";

			return;
		}

	strError = "ERROR: RM_FreeSpriteData - Sprite ID Not Found";
}

#endif // Implementation Guard
#endif // Header Guard



