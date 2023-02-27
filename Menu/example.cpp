/////////////////////////////////////
//                                 //
//   olcPGEX_Menu - EXAMPLE CODE   //
//       by  Justin Richards       //
//                                 //
/////////////////////////////////////

#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include "olcPGEX_Menu.h"

class Menu_Example : public olc::PixelGameEngine
{
public:
	Menu_Example() { sAppName = "olcPGEX_Menu Example"; }

private:
	// Menu IDs for improved readability (optional)
	const enum MENU_IDS
	{
		MAIN_MENU,
		SUB_MENU,

		BUTTON_1,
		BUTTON_2,
		BUTTON_3,
		NEXT,
		BACK,
		EXIT,
	};

	// Two menus, and a way to know which menu we want to interact with currently
	olcPGEX_Menu mainMenu;
	olcPGEX_Menu subMenu;
	int nCurrentMenu = MAIN_MENU;

	// OPTION 1 - SINGLE IMAGE FILE
	olc::Renderable rndButtons{};

	// OPTION 2 - SEPARATE IMAGE FILES
	//olc::Renderable rndButton_1{};
	//olc::Renderable rndButton_2{};
	//olc::Renderable rndButton_3{};
	//olc::Renderable rndNext{};
	//olc::Renderable rndBack{};
	//olc::Renderable rndExit{};

	// Program variables
	bool bFirstFrame = true;
	float fSecondsBeforeExit = 1.0f;

public:
	bool OnUserCreate()
	{
		// Load Image File(s) and add individual menu items to each menu

		// OPTION 1 - SINGLE IMAGE FILE
		rndButtons.Load("menu_ALL_128.png");

		mainMenu.AddMenuItem(BUTTON_1, rndButtons.Decal(), { 320.0f, 104.0f }, { 512.0f, 128.0f }, { 0.0f, 0.0f });
		mainMenu.AddMenuItem(BUTTON_2, rndButtons.Decal(), { 320.0f, 248.0f }, { 512.0f, 128.0f }, { 0.0f, 128.0f });
		mainMenu.AddMenuItem(NEXT, rndButtons.Decal(), { 320.0f, 392.0f }, { 512.0f, 128.0f }, { 512.0f, 0.0f });
		mainMenu.AddMenuItem(EXIT, rndButtons.Decal(), { 320.0f, 536.0f }, { 512.0f, 128.0f }, { 512.0f, 256.0f });

		subMenu.AddMenuItem(BUTTON_3, rndButtons.Decal(), { 320.0f, 248.0f }, { 512.0f, 128.0f }, { 0.0f, 256.0f });
		subMenu.AddMenuItem(BACK, rndButtons.Decal(), { 320.0f, 392.0f }, { 512.0f, 128.0f }, { 512.0f, 128.0f });

		// OPTION 2 - SEPARATE IMAGE FILES
		//rndButton_1.Load("menu_button1_128.png");
		//rndButton_2.Load("menu_button2_128.png");
		//rndButton_3.Load("menu_button3_128.png");
		//rndNext.Load("menu_next_128.png");
		//rndBack.Load("menu_back_128.png");
		//rndExit.Load("menu_exit_128.png");

		//mainMenu.AddMenuItem(BUTTON_1, rndButton_1.Decal(), { 320.0f, 104.0f }, { 512.0f, 128.0f });
		//mainMenu.AddMenuItem(BUTTON_2, rndButton_2.Decal(), { 320.0f, 248.0f }, { 512.0f, 128.0f });
		//mainMenu.AddMenuItem(NEXT, rndNext.Decal(), { 320.0f, 392.0f }, { 512.0f, 128.0f });
		//mainMenu.AddMenuItem(EXIT, rndExit.Decal(), { 320.0f, 536.0f }, { 512.0f, 128.0f });

		//subMenu.AddMenuItem(BUTTON_3, rndButton_3.Decal(), { 320.0f, 248.0f }, { 512.0f, 128.0f });
		//subMenu.AddMenuItem(BACK, rndBack.Decal(), { 320.0f, 392.0f }, { 512.0f, 128.0f });
		
		
		// We don't want to see the sub menu just yet
		subMenu.SetMenuVisibility(false);

		return true;
	}

	bool OnUserUpdate(float fElapsedTime)
	{
		// Use the first update frame to clear the screen to a specific colour and start the main menu transition from clear to solid
		if (bFirstFrame)
		{
			Clear(olc::PixelF(0.0f, 0.25f, 0.25f));
			mainMenu.StartTransition(1.0f);

			bFirstFrame = false;
		}

		// Menu Logic
		//
		// Here is where the PGEX becomes useful in abstracting the 'behind the scenes' menu logic away from
		// the main program. There are a few main steps. Simplified, they are as follows;
		//
		// (1) Let the PGEX process any interactions from the first menu we are intereseted in
		// (2) If this menu is the menu that is currently active and the mouse clicked a menu item then get
		//     the ID of the menu item that was clicked on
		// (3) Use that ID to run program code based on that particular menu item
		// (4) Repeat for any other menus as required
		//
		// The reason we process the interactions from all menus each frame has to do with transitions. It's
		// important to note that a menu can not be interacted with during a transition and none of the menu
		// specific code runs if the menu is not the currently selected one

		int nMenuItem = mainMenu.ProcessMenuInteractions(fElapsedTime, GetMousePos(), GetMouse(0).bReleased);

		if (nCurrentMenu == MAIN_MENU && nMenuItem > -1)
		{
			mainMenu.GetMenuItemByID(nMenuItem);

			if (nMenuItem == EXIT)
			{
				mainMenu.GetMenuItemByID(EXIT)->bIncludeInTransition = false;
				mainMenu.StartTransition(-1.0f, 1.2f);
				Clear(olc::BLACK);
				fSecondsBeforeExit -= fElapsedTime;
			}
			else if (nMenuItem == NEXT)
			{
				mainMenu.StartTransition(-1.0f, 16.0f);
				subMenu.StartTransition(1.0f);
				subMenu.GetMenuItemByID(BUTTON_3)->bVisible = true;
				subMenu.GetMenuItemByID(BUTTON_3)->bIncludeInTransition = true;
				nCurrentMenu = SUB_MENU;
			}
			else if (nMenuItem == BUTTON_1)
			{
				mainMenu.GetMenuItemByID(BUTTON_1)->bToggledOff = true;
				mainMenu.GetMenuItemByID(BUTTON_2)->bToggledOff = false;
			}
			else if (nMenuItem == BUTTON_2)
			{
				mainMenu.GetMenuItemByID(BUTTON_2)->bToggledOff = true;
				mainMenu.GetMenuItemByID(BUTTON_1)->bToggledOff = false;
			}
		}

		nMenuItem = subMenu.ProcessMenuInteractions(fElapsedTime, GetMousePos(), GetMouse(0).bReleased);

		if (nCurrentMenu == SUB_MENU && nMenuItem > -1)
		{
			subMenu.GetMenuItemByID(nMenuItem);

			if (nMenuItem == BACK)
			{
				subMenu.StartTransition(-1.0f, 16.0f);
				mainMenu.StartTransition(1.0f);
				nCurrentMenu = MAIN_MENU;
			}
			else if (nMenuItem == BUTTON_3)
			{
				subMenu.GetMenuItemByID(BUTTON_3)->bVisible = false;
				subMenu.GetMenuItemByID(BUTTON_3)->bIncludeInTransition = false;
			}
		}

		// Exit Code
		if (fSecondsBeforeExit < 1.0f)
		{
			fSecondsBeforeExit -= fElapsedTime;
			if (fSecondsBeforeExit <= 0.0f)
				return false;

		}

		return true;
	}
};

int main()
{
	Menu_Example menu;
	if (menu.Construct(640, 640, 1, 1, false, true))
		menu.Start();

	return 0;
}
