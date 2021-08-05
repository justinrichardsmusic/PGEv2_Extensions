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
#include "olcPGEX_ResourceManager.h"
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

	// State machine to organise the different sequences in our demo
	enum class DEMO_STATE
	{
		INIT,
		INTRO,
		USER_INTERACTION,
		EXIT,
	}
	DemoState, NextDemoState;

	olcPGEX_ResourceManager rm;			// using the resource manager for convenience in handling our sprite resources
	olcPGEX_SplashScreen splashScreen;	// Since this source code will be publicly available a splash screen will satisfy license conditions
	
	cCharacter someGuy;					// Create an instance of our cCharacter class to act as our playable character

	const float fGravity = 9.8f;		// Gravity constant - used for jumping

public:
	// Initialisation
	bool OnUserCreate() override
	{
		// Set the options for the splash screen that will display at the start of the program
		splashScreen.SetOptions(2, 1, 2.0f, 0.0f, olc::WHITE, olc::DARK_CYAN, olc::CYAN, olc::BLACK);

		// Initialise the demo state machine
		DemoState = DEMO_STATE::INIT;
		NextDemoState = DEMO_STATE::INIT;

		// Initialise our player character (passing in the resource manager so we can load our sprite info
		someGuy.InitialiseCharacter(rm);

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
			
			// Set the position and animation state of our player character
			someGuy.vecPos = { ScreenWidth() / 2, ScreenHeight() + 192 };		// Below the Bottom of the screen, centered along the width
			someGuy.animator.Play("Climb_Up");									// Play the climbing up animation to begin with

			// Update the state machine for the next frame
			NextDemoState = DEMO_STATE::INTRO;

		} break;

		case DEMO_STATE::INTRO:
		{
			// Before the user can take control of the character he should climb up into view

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
			}

		} break;

		case DEMO_STATE::USER_INTERACTION:
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
					someGuy.animator.SetNextAnimation("Stand_Up_Right" , "Jump_Right", true);
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

		} break;

		case DEMO_STATE::EXIT:
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

		} break;

		default:
		{
			return false;
		} break;
		}

		// Update the state machine with the state for the next frame
		DemoState = NextDemoState;

		return true;
	}
};

int main()
{
	// Program Start
	Animator2D_Demo demo;
	if (demo.Construct(640, 640, 1, 1, false, true))
		demo.Start();

	return 0;
}
