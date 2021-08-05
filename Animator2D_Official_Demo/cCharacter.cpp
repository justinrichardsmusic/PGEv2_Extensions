#include "cCharacter.h"

void cCharacter::InitialiseCharacter(olcPGEX_ResourceManager &rm)
{
	animator.AddAnimation("Climb_Up", 0.6f, 8, rm.RM_Sprite("CharacterSprite.png"), { 276, 652 }, { 96, 192 }, { 48, 192 });

	animator.AddAnimation("Stand_Up_Left", 0.3f, 4, rm.RM_Sprite("CharacterSprite.png"), { 612, 420 }, { 112, 192 }, { 56, 192 }, { 0, 0 }, true, true);
	animator.AddAnimation("Stand_Up_Right", 0.3f, 4, rm.RM_Sprite("CharacterSprite.png"), { 612, 420 }, { 112, 192 }, { 56, 192 }, { 0, 0 }, true, true, false, { -1.0f, 0.0f });

	animator.AddAnimation("Crouch_Left", 0.3f, 4, rm.RM_Sprite("CharacterSprite.png"), { 276, 420 }, { 112, 192 }, { 56, 192 });
	animator.AddAnimation("Crouch_Right", 0.3f, 4, rm.RM_Sprite("CharacterSprite.png"), { 276, 420 }, { 112, 192 }, { 56, 192 }, { 0, 0 }, true, false, false, { -1.0f, 0.0f });

	animator.AddAnimation("Crouched_Left", 1.0f, 1, rm.RM_Sprite("CharacterSprite.png"), { 612, 420 }, { 112, 192 }, { 56, 192 });
	animator.AddAnimation("Crouched_Right", 1.0f, 1, rm.RM_Sprite("CharacterSprite.png"), { 612, 420 }, { 112, 192 }, { 56, 192 }, { 0, 0 }, true, false, false, { -1.0f, 0.0f });

	animator.AddAnimation("Jump_Left", 0.3f, 3, rm.RM_Sprite("CharacterSprite.png"), { 276, 211 }, { 128, 192 }, { 64, 192 });
	animator.AddAnimation("Jump_Right", 0.3f, 3, rm.RM_Sprite("CharacterSprite.png"), { 276, 211 }, { 128, 192 }, { 64, 192 }, { 0, 0 }, true, false, false, { -1.0f, 0.0f });

	animator.AddAnimation("Jumping_Left", 1.0f, 1, rm.RM_Sprite("CharacterSprite.png"), { 532, 211 }, { 128, 192 }, { 64, 192 });
	animator.AddAnimation("Jumping_Right", 1.0f, 1, rm.RM_Sprite("CharacterSprite.png"), { 532, 211 }, { 128, 192 }, { 64, 192 }, { 0, 0 }, true, false, false, { -1.0f, 0.0f });

	animator.AddAnimation("Walk_Left", 0.8f, 12, rm.RM_Sprite("CharacterSprite.png"), { 276, 12 }, { 88, 192 }, { 44, 192 });
	animator.AddAnimation("Walk_Right", 0.8f, 12, rm.RM_Sprite("CharacterSprite.png"), { 276, 12 }, { 88, 192 }, { 44, 192 }, { 0, 0 }, true, false, false, { -1.0f, 0.0f });
}

olc::vf2d cCharacter::TranslateCurrentPosition(olc::vf2d origin)
{
	olc::vf2d newPos = vecPos;

	// Calculate Sin and Cosine of the currnet angle (unless angle is 0)
	float s = fAngle == 0.0f ? 0.0f : sinf(fAngle);
	float c = fAngle == 0.0f ? 1.0f : cosf(fAngle);

	// Translate position back to the origin
	newPos.x -= origin.x;
	newPos.y -= origin.y;

	// Rotate the new position
	float fTempX = newPos.x * c - newPos.y * s;
	float fTempY = newPos.x * s + newPos.y * c;

	// Translate back to the origin
	newPos.x = fTempX + origin.x;
	newPos.y = fTempY + origin.y;

	return newPos;
}
