/*
	+-------------------------------------------------------------+
	|           olcPGEX_Animator2D     Official Demo              |
	|            programmed by  JustinRichardsMusic				  |
	+-------------------------------------------------------------+

	What is this?
	~~~~~~~~~~~~~
	The heavily commented source code to demonstrate the use of the
	olcPGEX_Animator2D.h PixelGameEngine extension.  It's purpose is
	to provide a complete framework to help explain how best to use
	this animator extension and all of its features.


	What does it do?:
	~~~~~~~~~~~~~~~
	Sets up a Character class in which to add animations to, as well as
	control those animations through both automation and user input.

	Using a simple state machine the program is broken down into separate
	sections to make understanding what's going on more easy.

	The code is heavily commented for learning purposes, however a version
	where the comments have been removed is also available.

	Other PGEXs included in this demonstration are olcPGEX_SplashScreen and
	olcPGEX_ResourceManager.  These are not required to use the animator,
	however they are used in this demo for simplicity / licensing reasons.

	The code below hopes to show how to initialise, control, and interogate
	the animator controller and it's animations in order to provide a large
	amount of flexibility when programming with the olcPGEX_Animator2D
	extension.

	Hopefully you find this animator controller class as useful as I have,
	and	enjoy using it as much as I enjoyed creating it...


	Author
	~~~~~~
	Justin Richards
	
*/


// Always define OLC_PGE_APPLICATION at least (and only) once in your project
#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

// Include the PGEXs found in the 'justinrichardsmusic' github
#include "olcPGEX_SplashScreen.h"
#define OLC_PGEX_RESOURCE_MANAGER_IMPLEMENTATION
#include "olcPGEX_ResourceManager.h"
#define ANIMATOR_IMPLEMENTATION
#include "olcPGEX_Animator2D.h"

// Character class to organise the animations and other data our character will need
#include "cCharacter.h"

// Main PixelGameEngine class
class Animator2D_Demo : public olc::PixelGameEngine
{
public:
	Animator2D_Demo()
	{
		// Set the Application Name
		sAppName = "olcPGEX_Animator2D Official Demo";
	}

private:
	// Declarations

	// State machine to determine which demo is currently active
	enum class CURRENT_DEMO
	{
		CHARACTER,
		FLAME,
	}
	CurrentDemo = CURRENT_DEMO::CHARACTER, NextDemo = CURRENT_DEMO::CHARACTER;

	// State machine to organise the different sequences in our demo
	enum class DEMO_STATE
	{
		INIT,
		INTRO,
		USER_INTERACTION,
		EXIT,
	}
	DemoState = DEMO_STATE::INIT, NextDemoState = DEMO_STATE::INIT;

	// Our flame demo is simpler than the character so a simple struct will suffice
	struct sFlame
	{
		olc::vf2d vecPos = { 0, 0 };
		olc::vf2d vecDistancePos = { 0, 0 };

		float fAngle = 0.0f;
		float fDistance = 0.0f;

		olcPGEX_Animator2D animator;

		void InitialiseFlame(olcPGEX_ResourceManager& rm)
		{
			// Create 3 separate Billboard Animations which can be played at different speeds and at slightly different locations to make
			// a more realistic flame effect (Also some minor scaling and alpha tinting for extra effect)
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
			// Similar to the cCharacter version but modified slightly

			vecDistancePos = vecPos;
			vecDistancePos.y -= fDistance;

			// Calculate Sin and Cosine of the currnet angle (unless angle is 0)
			float s = fAngle == 0.0f ? 0.0f : sinf(fAngle);
			float c = fAngle == 0.0f ? 1.0f : cosf(fAngle);

			// Translate position back to the origin
			vecDistancePos.x -= vecPos.x;
			vecDistancePos.y -= vecPos.y;

			// Rotate the new position
			float fTempX = vecDistancePos.x * c - vecDistancePos.y * s;
			float fTempY = vecDistancePos.x * s + vecDistancePos.y * c;

			// Translate back to the origin
			vecDistancePos.x = fTempX + vecPos.x;
			vecDistancePos.y = fTempY + vecPos.y;

			return vecDistancePos;
		}
	};

	olcPGEX_ResourceManager rm;			// using the resource manager for convenience in handling our sprite resources
	olcPGEX_SplashScreen splashScreen;	// Since this source code will be publicly available a splash screen will satisfy license conditions
	
	cCharacter someGuy;					// Create an instance of our cCharacter class to act as our playable character
	sFlame flame;						// Create an instance of the flame for the second demo

	const float fGravity = 9.8f;		// Gravity constant - used for jumping

public:
	// Initialisation
	bool OnUserCreate() override
	{
		// Set the options for the splash screen that will display at the start of the program
		splashScreen.SetOptions(3, 2, 2.0f, 0.0f, olc::WHITE, olc::DARK_CYAN, olc::CYAN, olc::BLACK);

		// Initialise our player character and flame (passing in the resource manager so we can load our sprite info
		someGuy.InitialiseCharacter(rm);
		flame.InitialiseFlame(rm);

		// Clear the screen to white
		Clear(olc::WHITE);

		return true;
	}

	// Main Update Loop
	bool OnUserUpdate(float fElapsedTime) override
	{
		// Cycle through the state machine
		switch (DemoState)
		{
		case DEMO_STATE::INIT:
		{
			//static bool bFirstRunPause = true;
			//if (bFirstRunPause)
			//{
			//	if (GetKey(olc::Key::SPACE).bReleased)
			//		bFirstRunPause = false;

			//	return true;
			//}

			// Run the splash screen sequence until finished before executing the rest of the program code
			if (splashScreen.AnimateSplashScreen(fElapsedTime))
				return true;

			// Continue running program code now that the splash screen is finished
			
			// Set the position and animation state of our player character and flame
			someGuy.vecPos = { ScreenWidth() / 2, ScreenHeight() + 192 };				// Below the Bottom of the screen, centered along the width
			someGuy.animator.StopAll();													// Stop all animations that might be playing
			someGuy.animator.Play("Climb_Up");											// Play the climbing up animation to begin with
			
			// Reset character attributes to default values
			someGuy.fAngle = 0.0f;
			someGuy.bCrouching = false;
			someGuy.bGrounded = true;
			someGuy.bFacingRight = false;

			flame.vecPos = { (float)ScreenWidth() / 2, (float)ScreenHeight() / 2 };		// Center of the screen
			flame.animator.StopAll();													// Stop all animations that might be playing
			flame.fDistance = 130.0f;													// 10 units from the center
			flame.fAngle = 2.67f;														// rotated a bit

			// Reset scale and tinting of the flame animations
			flame.animator.ScaleAnimation("FlameMiddle", { 2.0f, 4.0f });
			flame.animator.ScaleAnimation("FlameLeft", { 2.8f, 3.0f });
			flame.animator.ScaleAnimation("FlameRight", { 2.6f, 3.2f });

			flame.animator.TintAnimation("FlameLeft", olc::PixelF(1.0f, 1.0f, 1.0f, 0.6f));
			flame.animator.TintAnimation("FlameRight", olc::PixelF(1.0f, 1.0f, 1.0f, 0.4f));

			// Play all 3 flame animations
			flame.animator.Play("FlameMiddle");
			flame.animator.Play("FlameLeft");
			flame.animator.Play("FlameRight");

			// Update the state machine for the next frame
			NextDemoState = DEMO_STATE::INTRO;

		} break;

		case DEMO_STATE::INTRO:
		{
			// Determine which demo sequence is currently active
			if (CurrentDemo == CURRENT_DEMO::CHARACTER)
			{
				// Before the user can take control of the character he should climb up into view

				// Clear the screen on first frame
				static bool bFirstFrame = true;
				if (bFirstFrame)
					Clear(olc::WHITE);

				bFirstFrame = false;

				// Boolean to control what the player is doing
				static bool bClimbing = true;

				// Increase sprite alpha to use with tinting as character comes into view
				static float fAlpha = 0.0f;
				if (fAlpha < 1.0f)
				{
					fAlpha += fElapsedTime / 2.0f;
					if (fAlpha > 1.0f) fAlpha = 1.0f;

					someGuy.animator.TintAnimation("Climb_Up", olc::PixelF(1.0f, 1.0f, 1.0f, fAlpha));
				}

				// Update the character position and animations
				if (bClimbing)
					someGuy.vecPos.y -= fElapsedTime * 120;							// Physically move the player upwards

				// Check to see if the player has reached the middle of the screen height
				if (bClimbing && someGuy.vecPos.y <= ScreenHeight() / 2 + 128)
				{
					bClimbing = false;
					someGuy.animator.Stop("Climb_Up");								// Stop climbing
					someGuy.animator.Play("Stand_Up_Left", true);					// Stand up facing left (PlayOnce set to true)
					someGuy.animator.SetNextAnimation("Stand_Up_Left", "Walk_Left");// Set the next animation to play automatically

					someGuy.vecPos.y = ScreenHeight() / 2 + 96;					// Reposition player
				}

				someGuy.animator.UpdateAnimations(fElapsedTime);					// Update the animation logic
				someGuy.animator.DrawAnimationFrame(someGuy.vecPos);				// Draw the current animation frame

				// Check to see if the walk animation has started
				// Then we know to transition to the next game state
				if (someGuy.animator.GetAnim("Walk_Left")->bIsPlaying)
				{
					NextDemoState = DEMO_STATE::USER_INTERACTION;

					// Display user controls at bottom of the screen
					DrawString({ ScreenWidth() / 2 - 168, ScreenHeight() - 68 }, "ROTATE GRAVITY - A, D", olc::GREY, 2);
					DrawString({ ScreenWidth() / 2 - 224, ScreenHeight() - 44 }, "MOVE - UP, DOWN, LEFT, RIGHT", olc::DARK_GREY, 2);
					DrawString({ ScreenWidth() / 2 - 112, ScreenHeight() - 24 }, "ESCAPE TO EXIT", olc::GREY, 2);

					// Display demo URL at top of the screen
					DrawString({ ScreenWidth() / 2 - 220, 4 }, "https://github.com/justinrichardsmusic/PGEv2_Extensions", olc::DARK_CYAN);

					// Display Demo Mode options
					DrawString({ ScreenWidth() / 2 - 264, 24 }, "SWITCH DEMO - PRESS 1, or PRESS 2", olc::DARK_GREY, 2);

					bFirstFrame = true;
					bClimbing = true;
					fAlpha = 0.0f;
				}

			}
			else if (CurrentDemo == CURRENT_DEMO::FLAME)
			{
				// Before the user can take control of the flame it should first fade into view

				// Clear the screen on first frame
				static bool bFirstFrame = true;
				if (bFirstFrame)
					Clear(olc::BLACK);

				bFirstFrame = false;

				// Increase sprite alpha to use with tinting as character comes into view
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
				// User Input

				// boolean to prevent multiple keys presses being processed in a single frame
				bool bKeyProcessed = false;

				// Exit to the EXIT state
				if (GetKey(olc::Key::ESCAPE).bReleased)
				{
					bKeyProcessed = true;

					NextDemoState = DEMO_STATE::EXIT;
				}

				// Switch between demos
				if (GetKey(olc::Key::K2).bReleased)
				{
					NextDemo = CURRENT_DEMO::FLAME;
					NextDemoState = DEMO_STATE::INIT;
				}

				// Rotate Gravity Counter-Clockwise
				if (GetKey(olc::Key::A).bHeld)
				{
					bKeyProcessed = true;

					someGuy.fAngle -= fElapsedTime;
					if (someGuy.fAngle < 0.0f) someGuy.fAngle += 6.28f;
				}

				// Rotate Gravity Clockwise
				if (GetKey(olc::Key::D).bHeld)
				{
					bKeyProcessed = true;

					someGuy.fAngle += fElapsedTime;
					if (someGuy.fAngle > 6.28f) someGuy.fAngle -= 6.28f;
				}

				// Jump
				if (GetKey(olc::Key::UP).bPressed && !bKeyProcessed && !someGuy.bCrouching && someGuy.bGrounded)
				{
					bKeyProcessed = true;

					someGuy.bGrounded = false;								// Start Jumping
					someGuy.vecVel.y = -6.0f;								// Set jumping velocity

					// Stop all animtaions, and play the Jump animation
					someGuy.animator.StopAll();
					someGuy.animator.Play(someGuy.bFacingRight ? "Stand_Up_Right" : "Stand_Up_Left", true, 2);

					// Set the next animation to play automatically
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

				// Don't process other input if the player is currently not grounded
				if (someGuy.bGrounded)
				{
					if (GetKey(olc::Key::DOWN).bPressed && !bKeyProcessed && !someGuy.bCrouching)
					{
						bKeyProcessed = true;
						someGuy.bCrouching = true;							// Set the character in crouching mode

						// Stop all animtaions, and play the Crouch animation
						someGuy.animator.StopAll();
						someGuy.animator.Play(someGuy.bFacingRight ? "Crouch_Right" : "Crouch_Left", true);

						// Set the next animation to play automatically
						if (someGuy.bFacingRight)
							someGuy.animator.SetNextAnimation("Crouch_Right", "Crouched_Right");
						else
							someGuy.animator.SetNextAnimation("Crouch_Left", "Crouched_Left");

					}

					if (GetKey(olc::Key::DOWN).bReleased && !bKeyProcessed && someGuy.bCrouching)
					{
						bKeyProcessed = true;

						// Stop all animtaions, and play the Stand Up animation
						someGuy.animator.StopAll();
						someGuy.animator.Play(someGuy.bFacingRight ? "Stand_Up_Right" : "Stand_Up_Left", true);

						// Set the next animation to play automatically
						if (someGuy.bFacingRight)
							someGuy.animator.SetNextAnimation("Stand_Up_Right", "Walk_Right");
						else
							someGuy.animator.SetNextAnimation("Stand_Up_Left", "Walk_Left");

					}

					if (GetKey(olc::Key::LEFT).bPressed && !bKeyProcessed && !someGuy.bCrouching)
					{
						bKeyProcessed = true;

						// Check to see if we are already playing the Walk Left animation
						if (!someGuy.animator.GetAnim("Walk_Left")->bIsPlaying)
						{
							// If not, stop all animtaions, and play the Walk Left animation
							someGuy.animator.StopAll();
							someGuy.animator.Play("Walk_Left");
							someGuy.bFacingRight = false;
						}
					}

					if (GetKey(olc::Key::RIGHT).bPressed && !bKeyProcessed && !someGuy.bCrouching)
					{
						bKeyProcessed = true;

						// Check to see if we are already playing the Walk Right animation
						if (!someGuy.animator.GetAnim("Walk_Right")->bIsPlaying)
						{
							// If not, stop all animtaions, and play the Walk Right animation
							someGuy.animator.StopAll();
							someGuy.animator.Play("Walk_Right");
							someGuy.bFacingRight = true;
						}
					}
				}

				// If we come out of a crouch and start walking we need to reset the crouched status
				if (someGuy.bCrouching)
				{
					if (someGuy.animator.GetAnim("Walk_Right")->bIsPlaying ||
						someGuy.animator.GetAnim("Walk_Left")->bIsPlaying)
						someGuy.bCrouching = false;

				}

				// Update character position based on velocity and gravity
				if (!someGuy.bGrounded)
				{
					someGuy.vecVel.y += fGravity * fElapsedTime;
					someGuy.vecPos += someGuy.vecVel;

					// If the player goes below ground level our jump is over
					if (someGuy.vecPos.y >= ScreenHeight() / 2 + 128)
					{
						someGuy.bGrounded = true;
						someGuy.vecVel.y = 0.0f;
						someGuy.vecPos.y = ScreenHeight() / 2 + 96;

						// Jumping is tiring, need to stand up from a crouch again
						someGuy.bCrouching = true;
						someGuy.animator.StopAll();
						someGuy.animator.Play(someGuy.bFacingRight ? "Stand_Up_Right" : "Stand_Up_Left", true);

						// Set the next animation to play automatically
						if (someGuy.bFacingRight)
							someGuy.animator.SetNextAnimation("Stand_Up_Right", "Walk_Right");
						else
							someGuy.animator.SetNextAnimation("Stand_Up_Left", "Walk_Left");

					}
				}

				// Translate the player position based on gravitational rotation before drawing his animation frame
				olc::vf2d vecTranslatedPos = someGuy.TranslateCurrentPosition(olc::vf2d(ScreenWidth() / 2, ScreenHeight() / 2 + 96));

				someGuy.animator.UpdateAnimations(fElapsedTime);						// Update the animation logic
				someGuy.animator.DrawAnimationFrame(vecTranslatedPos, someGuy.fAngle);	// Draw the current animation frame with rotation

			}
			else if (CurrentDemo == CURRENT_DEMO::FLAME)
			{
				// User Input
			
				// Exit to the EXIT state
				if (GetKey(olc::Key::ESCAPE).bReleased)
					NextDemoState = DEMO_STATE::EXIT;


				// Switch between demos
				if (GetKey(olc::Key::K1).bReleased)
				{
					NextDemo = CURRENT_DEMO::CHARACTER;
					NextDemoState = DEMO_STATE::INIT;
				}


				// Cache Flame Animation Scale values (GetAnim is expensive to use each update as it uses string comparison)
				olc::vf2d vecFlameMiddleScale = flame.animator.GetAnim("FlameMiddle")->vecScale;
				olc::vf2d vecFlameLeftScale = flame.animator.GetAnim("FlameLeft")->vecScale;
				olc::vf2d vecFlameRightScale = flame.animator.GetAnim("FlameRight")->vecScale;

				// Distance and Scale modification
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

				// Tinting
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

				// Update flame animations and draw the current frame
				flame.animator.UpdateAnimations(fElapsedTime);
				flame.animator.DrawAnimationFrame(flame.TranslatePositionAlongDistance(), flame.fAngle);
				
				// Need to clear the screen as we are using the DrawLine and DrawCircle routines
				Clear(olc::BLACK);

				// Display user controls at bottom of the screen
				DrawString({ ScreenWidth() / 2 - 136, ScreenHeight() - 64 }, "TINT - R, G, B, W", olc::DARK_GREY, 2);
				DrawString({ ScreenWidth() / 2 - 208, ScreenHeight() - 44 }, "CHANGE DISTANCE - UP, DOWN", olc::GREY, 2);
				DrawString({ ScreenWidth() / 2 - 112, ScreenHeight() - 24 }, "ESCAPE TO EXIT", olc::DARK_GREY, 2);

				// Display demo URL at top of the screen
				DrawString({ ScreenWidth() / 2 - 220, 4 }, "https://github.com/justinrichardsmusic/PGEv2_Extensions", olc::CYAN);

				// Display Demo Mode options
				DrawString({ ScreenWidth() / 2 - 264, 24 }, "SWITCH DEMO - PRESS 1, or PRESS 2", olc::GREY, 2);

				// Display the debug info
				DrawLine(flame.vecPos, flame.vecDistancePos, olc::YELLOW);
				DrawCircle(flame.vecPos, 8, olc::RED);
				DrawCircle(flame.vecDistancePos, 4, olc::GREEN);

				// Increment the angle 
				flame.fAngle += fElapsedTime;
				if (flame.fAngle > 6.28f)
					flame.fAngle -= 6.28f;

			}

		} break;

		case DEMO_STATE::EXIT:
		{
			if (CurrentDemo == CURRENT_DEMO::CHARACTER)
			{
				// Translate player position permanently based on where he last was
				static bool bFirstFrame = true;
				if (bFirstFrame)
				{
					bFirstFrame = false;
					someGuy.vecPos = someGuy.TranslateCurrentPosition(olc::vf2d(ScreenWidth() / 2, ScreenHeight() / 2 + 96));
				}

				// Count down to program termination
				static float fTimeToExit = 3.0f;
				static float fAlpha = 1.0f;

				// Stop any animations that might be playing from last game state
				// and commence the death sequence
				if (fTimeToExit == 3.0f)
				{
					someGuy.animator.StopAll();
					someGuy.animator.Play("Jump_Right", true);
					someGuy.animator.SetNextAnimation("Jump_Right", "Jumping_Right");
				}

				// Tint and Scale the death animation
				someGuy.animator.TintAnimation("Jumping_Right", olc::PixelF(1.0f, fAlpha, fAlpha, fAlpha));
				someGuy.animator.ScaleAnimation("Jumping_Right", { fAlpha, fAlpha });

				// Clear the screen
				Clear(olc::PixelF(1.0f, fAlpha, fAlpha, fAlpha));

				// decrement timer
				fTimeToExit -= fElapsedTime;

				fAlpha = fTimeToExit / 3.0f;
				if (fAlpha < 0.0f) fAlpha = 0.0f; // never allow alpha to go below zero

				// Exit the program if the timer reaches zero
				if (fTimeToExit <= 0.0f) return false;

				// Reposition the character as if he is being blown back
				someGuy.vecPos.x -= fElapsedTime * 3.0f;
				someGuy.vecPos.y -= fElapsedTime * 6.0f;

				someGuy.animator.UpdateAnimations(fElapsedTime);								// Update the animation logic
				someGuy.animator.DrawAnimationFrame(someGuy.vecPos, fAlpha * 0.5f - 1.0f);		// Draw the current animation frame with rotation
			}
			else if (CurrentDemo == CURRENT_DEMO::FLAME)
			{
				// Clear the screen on first frame and set current tint to default (white
				static bool bFirstFrame = true;
				static olc::Pixel pCurrentTint = olc::WHITE;

				if (bFirstFrame)
				{
					Clear(olc::BLACK);
				
					// Cache the tinted colour of at least one of the flame animations to preserve user selected tint
					pCurrentTint = flame.animator.GetAnim("FlameMiddle")->pTint;
				}

				bFirstFrame = false;

				// Decrease sprite alpha until faded out for exiting
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
					// Exit
					return false;
				}

				// Update flame animator
				flame.animator.UpdateAnimations(fElapsedTime);
				flame.animator.DrawAnimationFrame(flame.vecDistancePos, flame.fAngle);
			}

		} break;

		default:
		{
			return false;
		} break;
		}

		// Update the state machine with the state for the next frame
		DemoState = NextDemoState;

		// Update the current demo
		CurrentDemo = NextDemo;

		return true;
	}
};

int main()
{
	// Program Start
	Animator2D_Demo demo;
	if (demo.Construct(720, 720, 1, 1, false, true))
		demo.Start();

	return 0;
}
