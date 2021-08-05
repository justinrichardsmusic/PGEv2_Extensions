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
#include "olcPGEX_ResourceManager.h"
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
	enum class DEMO_STATE
	{
		INIT,
		INTRO,
		USER_INTERACTION,
		EXIT,
	}
	DemoState, NextDemoState;

	olcPGEX_ResourceManager rm;	
	olcPGEX_SplashScreen splashScreen;
	
	cCharacter someGuy;

	const float fGravity = 9.8f;

public:
	bool OnUserCreate() override
	{
		splashScreen.SetOptions(2, 1, 2.0f, 0.0f, olc::WHITE, olc::DARK_CYAN, olc::CYAN, olc::BLACK);

		DemoState = DEMO_STATE::INIT;
		NextDemoState = DEMO_STATE::INIT;

		someGuy.InitialiseCharacter(rm);

		Clear(olc::WHITE);

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		switch (DemoState)
		{
		case DEMO_STATE::INIT:
		{
			static bool bFirstRunPause = true;
			if (bFirstRunPause)
			{
				if (GetKey(olc::Key::SPACE).bReleased)
					bFirstRunPause = false;

				return true;
			}

			if (splashScreen.AnimateSplashScreen(fElapsedTime))
				return true;

			someGuy.vecPos = { ScreenWidth() / 2, ScreenHeight() + 192 };
			someGuy.animator.Play("Climb_Up");

			NextDemoState = DEMO_STATE::INTRO;

		} break;

		case DEMO_STATE::INTRO:
		{
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
			}

		} break;

		case DEMO_STATE::USER_INTERACTION:
		{
			bool bKeyProcessed = false;

			if (GetKey(olc::Key::ESCAPE).bReleased)
			{
				bKeyProcessed = true;

				NextDemoState = DEMO_STATE::EXIT;
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
					someGuy.animator.SetNextAnimation("Stand_Up_Right" , "Jump_Right", true);
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

		} break;

		case DEMO_STATE::EXIT:
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

		} break;

		default:
		{
			return false;
		} break;
		}

		DemoState = NextDemoState;

		return true;
	}
};

int main()
{
	Animator2D_Demo demo;
	if (demo.Construct(640, 640, 1, 1, false, true))
		demo.Start();

	return 0;
}