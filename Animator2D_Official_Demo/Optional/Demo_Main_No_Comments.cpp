/*
	+-------------------------------------------------------------+
	|           olcPGEX_Animator2D     Official Demo              |
	|            programmed by  JustinRichardsMusic				  |
	+-------------------------------------------------------------+

	Author
	~~~~~~
	Justin Richards
*/

#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

#include "olcPGEX_SplashScreen.h"
#define OLC_PGEX_RESOURCE_MANAGER_IMPLEMENTATION
#include "olcPGEX_ResourceManager.h"
#define ANIMATOR_IMPLEMENTATION
#include "olcPGEX_Animator2D.h"

#include "cCharacter.h"

class Animator2D_Demo : public olc::PixelGameEngine
{
public:
	Animator2D_Demo()
	{
		sAppName = "olcPGEX_Animator2D Official Demo";
	}

private:
	enum class CURRENT_DEMO
	{
		CHARACTER,
		FLAME,
	}
	CurrentDemo = CURRENT_DEMO::CHARACTER, NextDemo = CURRENT_DEMO::CHARACTER;

	enum class DEMO_STATE
	{
		INIT,
		INTRO,
		USER_INTERACTION,
		EXIT,
	}
	DemoState = DEMO_STATE::INIT, NextDemoState = DEMO_STATE::INIT;

	struct sFlame
	{
		olc::vf2d vecPos = { 0, 0 };
		olc::vf2d vecDistancePos = { 0, 0 };

		float fAngle = 0.0f;
		float fDistance = 0.0f;

		olcPGEX_Animator2D animator;

		void InitialiseFlame(olcPGEX_ResourceManager& rm)
		{
			animator.AddBillboardAnimation("FlameMiddle", 0.4f, 6, rm.RM_Sprite("Flame.png"), { 0, 0 }, { 20, 40 }, { 0, 0 });
			animator.AddBillboardAnimation("FlameLeft", 0.6f, 6, rm.RM_Sprite("Flame.png"), { 0, 0 }, { 20, 40 }, { 6, -4 });
			animator.AddBillboardAnimation("FlameRight", 0.7f, 6, rm.RM_Sprite("Flame.png"), { 0, 0 }, { 20, 40 }, { -5, 6 });

			animator.ScaleAnimation("FlameMiddle", { 2.0f, 4.0f });
			animator.ScaleAnimation("FlameLeft", { 2.8f, 3.0f });
			animator.ScaleAnimation("FlameRight", { 2.6f, 3.2f });

			animator.TintAnimation("FlameLeft", olc::PixelF(1.0f, 1.0f, 1.0f, 0.6f));
			animator.TintAnimation("FlameRight", olc::PixelF(1.0f, 1.0f, 1.0f, 0.4f));
		}

		olc::vf2d TranslatePositionAlongDistance()
		{
			vecDistancePos = vecPos;
			vecDistancePos.y -= fDistance;

			float s = fAngle == 0.0f ? 0.0f : sinf(fAngle);
			float c = fAngle == 0.0f ? 1.0f : cosf(fAngle);

			vecDistancePos.x -= vecPos.x;
			vecDistancePos.y -= vecPos.y;

			float fTempX = vecDistancePos.x * c - vecDistancePos.y * s;
			float fTempY = vecDistancePos.x * s + vecDistancePos.y * c;

			vecDistancePos.x = fTempX + vecPos.x;
			vecDistancePos.y = fTempY + vecPos.y;

			return vecDistancePos;
		}
	};

	olcPGEX_ResourceManager rm;	
	olcPGEX_SplashScreen splashScreen;

	cCharacter someGuy;		
	sFlame flame;			

	const float fGravity = 9.8f;	

public:
	bool OnUserCreate() override
	{
		splashScreen.SetOptions(3, 2, 2.0f, 0.0f, olc::WHITE, olc::DARK_CYAN, olc::CYAN, olc::BLACK);

		someGuy.InitialiseCharacter(rm);
		flame.InitialiseFlame(rm);

		Clear(olc::WHITE);

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		switch (DemoState)
		{
		case DEMO_STATE::INIT:
		{
			if (splashScreen.AnimateSplashScreen(fElapsedTime))
				return true;

			someGuy.vecPos = { ScreenWidth() / 2, ScreenHeight() + 192 };
			someGuy.animator.StopAll();						
			someGuy.animator.Play("Climb_Up");						

			someGuy.fAngle = 0.0f;
			someGuy.bCrouching = false;
			someGuy.bGrounded = true;
			someGuy.bFacingRight = false;

			flame.vecPos = { (float)ScreenWidth() / 2, (float)ScreenHeight() / 2 };	
			flame.animator.StopAll();												
			flame.fDistance = 130.0f;											
			flame.fAngle = 2.67f;												

			flame.animator.ScaleAnimation("FlameMiddle", { 2.0f, 4.0f });
			flame.animator.ScaleAnimation("FlameLeft", { 2.8f, 3.0f });
			flame.animator.ScaleAnimation("FlameRight", { 2.6f, 3.2f });

			flame.animator.TintAnimation("FlameLeft", olc::PixelF(1.0f, 1.0f, 1.0f, 0.6f));
			flame.animator.TintAnimation("FlameRight", olc::PixelF(1.0f, 1.0f, 1.0f, 0.4f));

			flame.animator.Play("FlameMiddle");
			flame.animator.Play("FlameLeft");
			flame.animator.Play("FlameRight");

			NextDemoState = DEMO_STATE::INTRO;

		} break;

		case DEMO_STATE::INTRO:
		{
			if (CurrentDemo == CURRENT_DEMO::CHARACTER)
			{
				static bool bFirstFrame = true;
				if (bFirstFrame)
					Clear(olc::WHITE);

				bFirstFrame = false;

				static bool bClimbing = true;

				static float fAlpha = 0.0f;
				if (fAlpha < 1.0f)
				{
					fAlpha += fElapsedTime / 2.0f;
					if (fAlpha > 1.0f) fAlpha = 1.0f;

					someGuy.animator.TintAnimation("Climb_Up", olc::PixelF(1.0f, 1.0f, 1.0f, fAlpha));
				}

				if (bClimbing)
					someGuy.vecPos.y -= fElapsedTime * 120;				

				if (bClimbing && someGuy.vecPos.y <= ScreenHeight() / 2 + 128)
				{
					bClimbing = false;
					someGuy.animator.Stop("Climb_Up");		
					someGuy.animator.Play("Stand_Up_Left", true);	
					someGuy.animator.SetNextAnimation("Stand_Up_Left", "Walk_Left");

					someGuy.vecPos.y = ScreenHeight() / 2 + 96;			
				}

				someGuy.animator.UpdateAnimations(fElapsedTime);			
				someGuy.animator.DrawAnimationFrame(someGuy.vecPos);		

				if (someGuy.animator.GetAnim("Walk_Left")->bIsPlaying)
				{
					NextDemoState = DEMO_STATE::USER_INTERACTION;

					DrawString({ ScreenWidth() / 2 - 168, ScreenHeight() - 68 }, "ROTATE GRAVITY - A, D", olc::GREY, 2);
					DrawString({ ScreenWidth() / 2 - 224, ScreenHeight() - 44 }, "MOVE - UP, DOWN, LEFT, RIGHT", olc::DARK_GREY, 2);
					DrawString({ ScreenWidth() / 2 - 112, ScreenHeight() - 24 }, "ESCAPE TO EXIT", olc::GREY, 2);

					DrawString({ ScreenWidth() / 2 - 220, 4 }, "https://github.com/justinrichardsmusic/PGEv2_Extensions", olc::DARK_CYAN);

					DrawString({ ScreenWidth() / 2 - 264, 24 }, "SWITCH DEMO - PRESS 1, or PRESS 2", olc::DARK_GREY, 2);

					bFirstFrame = true;
					bClimbing = true;
					fAlpha = 0.0f;
				}

			}
			else if (CurrentDemo == CURRENT_DEMO::FLAME)
			{
				static bool bFirstFrame = true;
				if (bFirstFrame)
					Clear(olc::BLACK);

				bFirstFrame = false;

				static float fAlpha = 0.0f;
				if (fAlpha < 1.0f)
				{
					fAlpha += fElapsedTime / 2.0f;
					if (fAlpha > 1.0f) fAlpha = 1.0f;

					flame.animator.TintAnimation("FlameMiddle", olc::PixelF(1.0f, 1.0f, 1.0f, fAlpha));
					flame.animator.TintAnimation("FlameLeft", olc::PixelF(1.0f, 1.0f, 1.0f, fAlpha));
					flame.animator.TintAnimation("FlameRight", olc::PixelF(1.0f, 1.0f, 1.0f, fAlpha));
				}
				else
				{
					NextDemoState = DEMO_STATE::USER_INTERACTION;
					bFirstFrame = true;
					fAlpha = 0.0f;
				}

				flame.animator.UpdateAnimations(fElapsedTime);
				flame.animator.DrawAnimationFrame(flame.TranslatePositionAlongDistance(), flame.fAngle);

			}

		} break;

		case DEMO_STATE::USER_INTERACTION:
		{
			if (CurrentDemo == CURRENT_DEMO::CHARACTER)
			{
				bool bKeyProcessed = false;

				if (GetKey(olc::Key::ESCAPE).bReleased)
				{
					bKeyProcessed = true;

					NextDemoState = DEMO_STATE::EXIT;
				}

				if (GetKey(olc::Key::K2).bReleased)
				{
					NextDemo = CURRENT_DEMO::FLAME;
					NextDemoState = DEMO_STATE::INIT;
				}

				if (GetKey(olc::Key::A).bHeld)
				{
					bKeyProcessed = true;

					someGuy.fAngle -= fElapsedTime;
					if (someGuy.fAngle < 0.0f) someGuy.fAngle += 6.28f;
				}

				if (GetKey(olc::Key::D).bHeld)
				{
					bKeyProcessed = true;

					someGuy.fAngle += fElapsedTime;
					if (someGuy.fAngle > 6.28f) someGuy.fAngle -= 6.28f;
				}

				if (GetKey(olc::Key::UP).bPressed && !bKeyProcessed && !someGuy.bCrouching && someGuy.bGrounded)
				{
					bKeyProcessed = true;

					someGuy.bGrounded = false;	
					someGuy.vecVel.y = -6.0f;		

					someGuy.animator.StopAll();
					someGuy.animator.Play(someGuy.bFacingRight ? "Stand_Up_Right" : "Stand_Up_Left", true, 2);

					if (someGuy.bFacingRight)
					{
						someGuy.animator.SetNextAnimation("Stand_Up_Right", "Jump_Right", true);
						someGuy.animator.SetNextAnimation("Jump_Right", "Jumping_Right");
					}
					else
					{
						someGuy.animator.SetNextAnimation("Stand_Up_Left", "Jump_Left", true);
						someGuy.animator.SetNextAnimation("Jump_Left", "Jumping_Left");
					}
				}

				if (someGuy.bGrounded)
				{
					if (GetKey(olc::Key::DOWN).bPressed && !bKeyProcessed && !someGuy.bCrouching)
					{
						bKeyProcessed = true;
						someGuy.bCrouching = true;					

						someGuy.animator.StopAll();
						someGuy.animator.Play(someGuy.bFacingRight ? "Crouch_Right" : "Crouch_Left", true);

						if (someGuy.bFacingRight)
							someGuy.animator.SetNextAnimation("Crouch_Right", "Crouched_Right");
						else
							someGuy.animator.SetNextAnimation("Crouch_Left", "Crouched_Left");

					}

					if (GetKey(olc::Key::DOWN).bReleased && !bKeyProcessed && someGuy.bCrouching)
					{
						bKeyProcessed = true;

						someGuy.animator.StopAll();
						someGuy.animator.Play(someGuy.bFacingRight ? "Stand_Up_Right" : "Stand_Up_Left", true);

						if (someGuy.bFacingRight)
							someGuy.animator.SetNextAnimation("Stand_Up_Right", "Walk_Right");
						else
							someGuy.animator.SetNextAnimation("Stand_Up_Left", "Walk_Left");

					}

					if (GetKey(olc::Key::LEFT).bPressed && !bKeyProcessed && !someGuy.bCrouching)
					{
						bKeyProcessed = true;

						if (!someGuy.animator.GetAnim("Walk_Left")->bIsPlaying)
						{
							someGuy.animator.StopAll();
							someGuy.animator.Play("Walk_Left");
							someGuy.bFacingRight = false;
						}
					}

					if (GetKey(olc::Key::RIGHT).bPressed && !bKeyProcessed && !someGuy.bCrouching)
					{
						bKeyProcessed = true;

						if (!someGuy.animator.GetAnim("Walk_Right")->bIsPlaying)
						{
							someGuy.animator.StopAll();
							someGuy.animator.Play("Walk_Right");
							someGuy.bFacingRight = true;
						}
					}
				}

				if (someGuy.bCrouching)
				{
					if (someGuy.animator.GetAnim("Walk_Right")->bIsPlaying ||
						someGuy.animator.GetAnim("Walk_Left")->bIsPlaying)
						someGuy.bCrouching = false;

				}

				if (!someGuy.bGrounded)
				{
					someGuy.vecVel.y += fGravity * fElapsedTime;
					someGuy.vecPos += someGuy.vecVel;

					if (someGuy.vecPos.y >= ScreenHeight() / 2 + 128)
					{
						someGuy.bGrounded = true;
						someGuy.vecVel.y = 0.0f;
						someGuy.vecPos.y = ScreenHeight() / 2 + 96;

						someGuy.bCrouching = true;
						someGuy.animator.StopAll();
						someGuy.animator.Play(someGuy.bFacingRight ? "Stand_Up_Right" : "Stand_Up_Left", true);

						if (someGuy.bFacingRight)
							someGuy.animator.SetNextAnimation("Stand_Up_Right", "Walk_Right");
						else
							someGuy.animator.SetNextAnimation("Stand_Up_Left", "Walk_Left");

					}
				}

				olc::vf2d vecTranslatedPos = someGuy.TranslateCurrentPosition(olc::vf2d(ScreenWidth() / 2, ScreenHeight() / 2 + 96));

				someGuy.animator.UpdateAnimations(fElapsedTime);						
				someGuy.animator.DrawAnimationFrame(vecTranslatedPos, someGuy.fAngle);	

			}
			else if (CurrentDemo == CURRENT_DEMO::FLAME)
			{
				if (GetKey(olc::Key::ESCAPE).bReleased)
					NextDemoState = DEMO_STATE::EXIT;


				if (GetKey(olc::Key::K1).bReleased)
				{
					NextDemo = CURRENT_DEMO::CHARACTER;
					NextDemoState = DEMO_STATE::INIT;
				}


				olc::vf2d vecFlameMiddleScale = flame.animator.GetAnim("FlameMiddle")->vecScale;
				olc::vf2d vecFlameLeftScale = flame.animator.GetAnim("FlameLeft")->vecScale;
				olc::vf2d vecFlameRightScale = flame.animator.GetAnim("FlameRight")->vecScale;

				if (GetKey(olc::Key::UP).bHeld && flame.fDistance < 200.0f)
				{
					flame.fDistance += fElapsedTime * 50.0f;
					flame.animator.ScaleAnimation("FlameMiddle", vecFlameMiddleScale + (vecFlameMiddleScale * 0.5f * fElapsedTime));
					flame.animator.ScaleAnimation("FlameLeft", vecFlameMiddleScale + (vecFlameMiddleScale * 0.5f * fElapsedTime));
					flame.animator.ScaleAnimation("FlameRight", vecFlameMiddleScale + (vecFlameMiddleScale * 0.5f * fElapsedTime));
				}

				if (GetKey(olc::Key::DOWN).bHeld && flame.fDistance > 30.0f)
				{
					flame.fDistance -= fElapsedTime * 50.0f;
					flame.animator.ScaleAnimation("FlameMiddle", vecFlameMiddleScale - (vecFlameMiddleScale * 0.5f * fElapsedTime));
					flame.animator.ScaleAnimation("FlameLeft", vecFlameMiddleScale - (vecFlameMiddleScale * 0.5f * fElapsedTime));
					flame.animator.ScaleAnimation("FlameRight", vecFlameMiddleScale - (vecFlameMiddleScale * 0.5f * fElapsedTime));
				}

				if (GetKey(olc::Key::R).bReleased)
				{
					flame.animator.TintAnimation("FlameMiddle", olc::Pixel(255, 63, 63, flame.animator.GetAnim("FlameMiddle")->pTint.a));
					flame.animator.TintAnimation("FlameLeft", olc::Pixel(255, 63, 63, flame.animator.GetAnim("FlameLeft")->pTint.a));
					flame.animator.TintAnimation("FlameRight", olc::Pixel(255, 63, 63, flame.animator.GetAnim("FlameRight")->pTint.a));
				}

				if (GetKey(olc::Key::G).bReleased)
				{
					flame.animator.TintAnimation("FlameMiddle", olc::Pixel(63, 255, 63, flame.animator.GetAnim("FlameMiddle")->pTint.a));
					flame.animator.TintAnimation("FlameLeft", olc::Pixel(63, 255, 63, flame.animator.GetAnim("FlameLeft")->pTint.a));
					flame.animator.TintAnimation("FlameRight", olc::Pixel(63, 255, 63, flame.animator.GetAnim("FlameRight")->pTint.a));
				}

				if (GetKey(olc::Key::B).bReleased)
				{
					flame.animator.TintAnimation("FlameMiddle", olc::Pixel(63, 63, 255, flame.animator.GetAnim("FlameMiddle")->pTint.a));
					flame.animator.TintAnimation("FlameLeft", olc::Pixel(63, 63, 255, flame.animator.GetAnim("FlameLeft")->pTint.a));
					flame.animator.TintAnimation("FlameRight", olc::Pixel(63, 63, 255, flame.animator.GetAnim("FlameRight")->pTint.a));
				}

				if (GetKey(olc::Key::W).bReleased)
				{
					flame.animator.TintAnimation("FlameMiddle", olc::Pixel(255, 255, 255, flame.animator.GetAnim("FlameMiddle")->pTint.a));
					flame.animator.TintAnimation("FlameLeft", olc::Pixel(255, 255, 255, flame.animator.GetAnim("FlameLeft")->pTint.a));
					flame.animator.TintAnimation("FlameRight", olc::Pixel(255, 255, 255, flame.animator.GetAnim("FlameRight")->pTint.a));
				}

				flame.animator.UpdateAnimations(fElapsedTime);
				flame.animator.DrawAnimationFrame(flame.TranslatePositionAlongDistance(), flame.fAngle);

				Clear(olc::BLACK);

				DrawString({ ScreenWidth() / 2 - 136, ScreenHeight() - 64 }, "TINT - R, G, B, W", olc::DARK_GREY, 2);
				DrawString({ ScreenWidth() / 2 - 208, ScreenHeight() - 44 }, "CHANGE DISTANCE - UP, DOWN", olc::GREY, 2);
				DrawString({ ScreenWidth() / 2 - 112, ScreenHeight() - 24 }, "ESCAPE TO EXIT", olc::DARK_GREY, 2);

				DrawString({ ScreenWidth() / 2 - 220, 4 }, "https://github.com/justinrichardsmusic/PGEv2_Extensions", olc::CYAN);

				DrawString({ ScreenWidth() / 2 - 264, 24 }, "SWITCH DEMO - PRESS 1, or PRESS 2", olc::GREY, 2);

				DrawLine(flame.vecPos, flame.vecDistancePos, olc::YELLOW);
				DrawCircle(flame.vecPos, 8, olc::RED);
				DrawCircle(flame.vecDistancePos, 4, olc::GREEN);

				flame.fAngle += fElapsedTime;
				if (flame.fAngle > 6.28f)
					flame.fAngle -= 6.28f;

			}

		} break;

		case DEMO_STATE::EXIT:
		{
			if (CurrentDemo == CURRENT_DEMO::CHARACTER)
			{
				static bool bFirstFrame = true;
				if (bFirstFrame)
				{
					bFirstFrame = false;
					someGuy.vecPos = someGuy.TranslateCurrentPosition(olc::vf2d(ScreenWidth() / 2, ScreenHeight() / 2 + 96));
				}

				static float fTimeToExit = 3.0f;
				static float fAlpha = 1.0f;

				if (fTimeToExit == 3.0f)
				{
					someGuy.animator.StopAll();
					someGuy.animator.Play("Jump_Right", true);
					someGuy.animator.SetNextAnimation("Jump_Right", "Jumping_Right");
				}

				someGuy.animator.TintAnimation("Jumping_Right", olc::PixelF(1.0f, fAlpha, fAlpha, fAlpha));
				someGuy.animator.ScaleAnimation("Jumping_Right", { fAlpha, fAlpha });

				Clear(olc::PixelF(1.0f, fAlpha, fAlpha, fAlpha));

				fTimeToExit -= fElapsedTime;

				fAlpha = fTimeToExit / 3.0f;
				if (fAlpha < 0.0f) fAlpha = 0.0f;

				if (fTimeToExit <= 0.0f) return false;

				someGuy.vecPos.x -= fElapsedTime * 3.0f;
				someGuy.vecPos.y -= fElapsedTime * 6.0f;

				someGuy.animator.UpdateAnimations(fElapsedTime);							
				someGuy.animator.DrawAnimationFrame(someGuy.vecPos, fAlpha * 0.5f - 1.0f);	
			}
			else if (CurrentDemo == CURRENT_DEMO::FLAME)
			{
				static bool bFirstFrame = true;
				static olc::Pixel pCurrentTint = olc::WHITE;

				if (bFirstFrame)
				{
					Clear(olc::BLACK);

					pCurrentTint = flame.animator.GetAnim("FlameMiddle")->pTint;
				}

				bFirstFrame = false;

				static float fAlpha = 1.0f;
				if (fAlpha > 0.0f)
				{
					fAlpha -= fElapsedTime / 2.0f;
					if (fAlpha < 0.0f) fAlpha = 0.0f;

					flame.animator.TintAnimation("FlameMiddle", olc::Pixel(pCurrentTint.r, pCurrentTint.g, pCurrentTint.b, fAlpha * 255));
					flame.animator.TintAnimation("FlameLeft", olc::Pixel(pCurrentTint.r, pCurrentTint.g, pCurrentTint.b, fAlpha * 255));
					flame.animator.TintAnimation("FlameRight", olc::Pixel(pCurrentTint.r, pCurrentTint.g, pCurrentTint.b, fAlpha * 255));
				}
				else
				{
					return false;
				}

				flame.animator.UpdateAnimations(fElapsedTime);
				flame.animator.DrawAnimationFrame(flame.vecDistancePos, flame.fAngle);
			}

		} break;

		default:
		{
			return false;
		} break;
		}

		DemoState = NextDemoState;

		CurrentDemo = NextDemo;

		return true;
	}
};

int main()
{
	Animator2D_Demo demo;
	if (demo.Construct(720, 720, 1, 1, false, true))
		demo.Start();

	return 0;
}
