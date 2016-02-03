// Example program:
// Using SDL2 to create an application window

#if defined(_WIN32) || (_WIN64)

#include "SDL.h"
#include "SDL_image.h"

#endif

#if defined(__APPLE__)

#include "SDL2/SDL.h"
#include "SDL2_image/SDL_image.h"

#endif

#if defined(__linux__)

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"

#endif

#if defined(_WIN32) || (_WIN64)
#include <direct.h>
#define getcw _getcw
#endif

#if defined(__linux__)
#include <unistd.h>
#endif

#include <stdio.h>
#include <iostream>
using namespace std;

// create the SDL Rectangle for the background texture's position and size  - background 1 and 2
SDL_Rect bkgd1Pos, bkgd2Pos;

//set speed for background
int bkgdSpeed = 100;

// set temp variables to hold movement - background 1
float BG1pos_X = 0, BG1pos_Y = 0;
float BG2pos_X = 0, BG2pos_Y = -768;

// code for the frame rate independence
float deltaTime = 0.0;
int thisTime = 0;
int lastTime = 0;

//move the background
void UpdateBackground(float deltaTime){

	//Update background 1
	BG1pos_Y +=(bkgdSpeed *1)* deltaTime;
	bkgd1Pos.y = (int)(BG1pos_Y + 0.5f);

	//reset when off the bottom of the screen
	if(bkgd1Pos.y >= 768){
		bkgd1Pos.y = -768;
		BG1pos_Y = bkgd1Pos.y;

	}
	//Update background 2

	BG2pos_Y +=(bkgdSpeed *1)* deltaTime;
	bkgd2Pos.y = (int)(BG2pos_Y + 0.5f);

	//reset when off the bottom of the screen
	if(bkgd2Pos.y >= 768){
		bkgd2Pos.y = -768;
		BG2pos_Y = bkgd2Pos.y;
	}
}

//****************************New****************************
//Analog joystick dead zone
const int JOYSTICK_DEAD_ZONE = 800;

//joystick direction vars
float xDir, yDir;

//cursor float vars for movement
float pos_X, pos_Y;

//create SDL Rectangle for the 2 player graphic
SDL_Rect cursorPos, activePos;

//vaR FROM CURSOR SPEED
int cursorSpeed = 400;

void moveCursor(const SDL_ControllerAxisEvent event) {

	if(event.which == 0){
		if(event.axis == 0) {

			if(event.value < - JOYSTICK_DEAD_ZONE){
				xDir = -1.0f;
			}else if (event.value > JOYSTICK_DEAD_ZONE){
				xDir = 1.0f;
			} else {
				xDir = 0.0f;
			}
		}

		if(event.axis == 1) {

				if(event.value < - JOYSTICK_DEAD_ZONE){
					yDir = -1.0f;
				}else if (event.value > JOYSTICK_DEAD_ZONE){
					yDir = 1.0f;
				} else {
					yDir = 0.0f;
				}
			}
		}
	}

void UpdateCursor(float deltaTime){
	//Update Cursor
	pos_X += (cursorSpeed * xDir) * deltaTime;
	pos_Y += (cursorSpeed * yDir) * deltaTime;

	cursorPos.x = (int) (pos_X + 0.5f);
	cursorPos.y = (int) (pos_Y + 0.5f);

	activePos.x = cursorPos.x;
	activePos.y = cursorPos.y;

	if(cursorPos.x < 0){
		cursorPos.x = 0;
		pos_X = cursorPos.x;
	}

	if(cursorPos.x > 1024 - cursorPos.w){
		cursorPos.x = 1024 - cursorPos.w;
		pos_X = cursorPos.x;
	}

	if(cursorPos.y < 0){
		cursorPos.y = 0;
		pos_Y = cursorPos.y;
	}

	if(cursorPos.y > 768 - cursorPos.h){
		cursorPos.y = 768 - cursorPos.h;
		pos_Y = cursorPos.y;
	}
}

// variables for all Menus button over
bool players1Over = false, players2Over = false, instructionsOver = false,
		quitOver = false, menuOver = false, playerOver = false;

//class header includes
#include "player.h"

int main(int argc, char* argv[]) {

#if defined(_WIN32) || (_WIN64)
	cout << "Running on Windows" << endl;
	cout << "Added on Windows" << endl;

	string s_cwd(getcwd(NULL, 0));

	string s_cwd_images = s_cwd + "\\Resources\\Images";

#endif

#if defined(__linux__)
	cout << "Running on Linux" << endl;
	cout << "Added on the Linux platform" << endl;

	//get the current working directory
	string s_cwd(getcwd(NULL,0));

	//create a string linking to the mac's images folder
	string s_cwd_images = s_cwd + "/Resources/Images/";

#endif

#if defined(__APPLE__)
	cout << "Running on Apple" << endl;
	//get the current working directory
	string s_cwd(getcwd(NULL,0));

	//create a string linking to the mac's images folder
	string s_cwd_images = s_cwd + "/Resources/Images/";

	//test
	cout << s_cwd_images << endl;
#endif

    SDL_Window *window;                    // Declare a pointer

    SDL_Init(SDL_INIT_EVERYTHING);              // Initialize SDL2

    // Create an application window with the following settings:
    window = SDL_CreateWindow(
        "An SDL2 window",                  // window title
        SDL_WINDOWPOS_UNDEFINED,           // initial x position
        SDL_WINDOWPOS_UNDEFINED,           // initial y position
        1024,                               // width, in pixels
        768,                               // height, in pixels
        SDL_WINDOW_OPENGL                  // flags - see below
    );

    // Check that the window was successfully created
    if (window == NULL) {
        // In the case that the window could not be made...
        printf("Could not create window: %s\n", SDL_GetError());
        return 1;
    }

    //declare renderer
    SDL_Renderer * renderer = NULL;

    // create the renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    //****************************************Create Players - START ***********
    Player player1 = Player(renderer, 0, s_cwd_images.c_str(), 250.0, 500.0);

    //***** Create Background *****
    string BKGDpath = s_cwd_images + "/placeholder 2.png";

    // create a SDL surface to hold the background image
    SDL_Surface * surface = IMG_Load(BKGDpath.c_str());

    //create a SDL texture
    SDL_Texture *bkgd1;

    // place surface into the texture
    bkgd1 = SDL_CreateTextureFromSurface(renderer, surface);

    //create a SDL texture
    SDL_Texture *bkgd2;

    // place surface into the texture
    bkgd2 = SDL_CreateTextureFromSurface(renderer, surface);

    // free the SDL surface
    SDL_FreeSurface(surface);

    //create the SDL_Rectangle for the texture's position and size = -x,y,w,h

    //set the X, Y, W, and H for the Rectangle
    bkgd1Pos.x = 0;
    bkgd1Pos.y = 0;
    bkgd1Pos.w = 1024;
    bkgd1Pos.h = 768;

    //create the SDL_Rectangle for the texture's position and size = -x,y,w,h

    //set the X, Y, W, and H for the Rectangle
    bkgd2Pos.x = 0;
    bkgd2Pos.y = -768;
    bkgd2Pos.w = 1024;
    bkgd2Pos.h = 768;

    //***** Create Main Menu - Start *****

    // create a SDL surface to hold the background image
    surface = IMG_Load((s_cwd_images + "/title.png").c_str());

    //create a SDL texture
    SDL_Texture *title;

    // place surface into the texture
    title = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_FreeSurface(surface);

    // create SDL Rectangle for the title graphic
    SDL_Rect titlePos;

    //create the SDL_Rectangle for the texture's position and size - title
    titlePos.x = 275;
    titlePos.y = -100;
    titlePos.w = 500;
    titlePos.h = 500;
    // title - end

    //////////////////////////1 player - start


    // create a SDL surface to hold the player 1 image
    surface = IMG_Load((s_cwd_images + "/One_Player_Game.png").c_str());

    string Player1path = s_cwd_images + "/One_Player_Game.png";

    // create a SDL surface to hold the player 1 image
    surface = IMG_Load(Player1path.c_str());


    //create a SDL texture
    SDL_Texture *Player1N;

    // place surface into the texture
    Player1N = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_FreeSurface(surface);


	// create a SDL surface to hold the player 1 image
	surface = IMG_Load((s_cwd_images + "/One_Player_Game_o.png").c_str());

	string Player1Opath = s_cwd_images + "/One_Player_Game_o.png";

	// create a SDL surface to hold the player 1 image
	surface = IMG_Load(Player1Opath.c_str());


	//create a SDL texture
	SDL_Texture *Player1O;

	// place surface into the texture
	Player1O = SDL_CreateTextureFromSurface(renderer, surface);

	SDL_FreeSurface(surface);

    // create SDL Rectangle for the title graphic
    SDL_Rect Player1NPos;

    //create the SDL_Rectangle for the texture's position and size - title
    Player1NPos.x = 275;
    Player1NPos.y = 100;
    Player1NPos.w = 511;
    Player1NPos.h = 300;
    ////////////////////////////////// 1 player - end


	//////////////////////////2 player - start


	// create a SDL surface to hold the player 2 image
	surface = IMG_Load((s_cwd_images + "/Two_Player_Game.png").c_str());

	string Player2path = s_cwd_images + "/Two_Player_Game.png";

	// create a SDL surface to hold the player 2 image
	surface = IMG_Load(Player2path.c_str());


	//create a SDL texture
	SDL_Texture *Player2N;

	// place surface into the texture
	Player2N = SDL_CreateTextureFromSurface(renderer, surface);

	SDL_FreeSurface(surface);


	// create a SDL surface to hold the player 2 image
	surface = IMG_Load((s_cwd_images + "/Two_Player_Game_o.png").c_str());

	string Player2Opath = s_cwd_images + "/Two_Player_Game_o.png";

	// create a SDL surface to hold the player 2 image
	surface = IMG_Load(Player2Opath.c_str());


	//create a SDL texture
	SDL_Texture *Player2O;

	// place surface into the texture
	Player2O = SDL_CreateTextureFromSurface(renderer, surface);

	SDL_FreeSurface(surface);

	// create SDL Rectangle for the player 2 graphic
	SDL_Rect Player2NPos;

	//create the SDL_Rectangle for the texture's position and size - title
	Player2NPos.x = 265;
	Player2NPos.y = 200;
	Player2NPos.w = 511;
	Player2NPos.h = 300;
	////////////////////////////////// player - end


	//////////////////////////instructions - start


	// create a SDL surface to hold the instructions image
	surface = IMG_Load((s_cwd_images + "/Instructions.png").c_str());

	string Instructionspath = s_cwd_images + "/Instructions.png";

	// create a SDL surface to hold the instructions image
	surface = IMG_Load(Instructionspath.c_str());


	//create a SDL texture
	SDL_Texture *InstructionsN;

	// place surface into the texture
	InstructionsN = SDL_CreateTextureFromSurface(renderer, surface);

	SDL_FreeSurface(surface);


	// create a SDL surface to hold the instructions image
	surface = IMG_Load((s_cwd_images + "/Instructions_o.png").c_str());

	string InstructionsOpath = s_cwd_images + "/Instructions_o.png";

	// create a SDL surface to hold the instructions image
	surface = IMG_Load(InstructionsOpath.c_str());


	//create a SDL texture
	SDL_Texture *InstructionsO;

	// place surface into the texture
	InstructionsO = SDL_CreateTextureFromSurface(renderer, surface);

	SDL_FreeSurface(surface);

	// create SDL Rectangle for the player 2 graphic
	SDL_Rect InstructionsNPos;

	//create the SDL_Rectangle for the texture's position and size - title
	InstructionsNPos.x = 275;
	InstructionsNPos.y = 300;
	InstructionsNPos.w = 511;
	InstructionsNPos.h = 300;
	////////////////////////////////// instructions - end


	//////////////////////////quit - start


	// create a SDL surface to hold the instructions image
	surface = IMG_Load((s_cwd_images + "/Quit_Game.png").c_str());

	string Quitpath = s_cwd_images + "/Quit_Game.png";

	// create a SDL surface to hold the instructions image
	surface = IMG_Load(Quitpath.c_str());


	//create a SDL texture
	SDL_Texture *QuitN;

	// place surface into the texture
	QuitN = SDL_CreateTextureFromSurface(renderer, surface);

	SDL_FreeSurface(surface);


	// create a SDL surface to hold the instructions image
	surface = IMG_Load((s_cwd_images + "/Quit_Game_o.png").c_str());

	string QuitOpath = s_cwd_images + "/Quit_Game_o.png";

	// create a SDL surface to hold the instructions image
	surface = IMG_Load(QuitOpath.c_str());


	//create a SDL texture
	SDL_Texture *QuitO;

	// place surface into the texture
	QuitO = SDL_CreateTextureFromSurface(renderer, surface);

	SDL_FreeSurface(surface);

	// create SDL Rectangle for the player 2 graphic
	SDL_Rect QuitNPos;

	//create the SDL_Rectangle for the texture's position and size - title
	QuitNPos.x = 325;
	QuitNPos.y = 400;
	QuitNPos.w = 511;
	QuitNPos.h = 300;
	////////////////////////////////// quit - end

    //***** Create Main Menu - End *****


	//***** Create Instructions Menu - Start *****


	// create a SDL surface to hold the background image
	surface = IMG_Load((s_cwd_images + "/Instructions_text.png").c_str());

	// Instructions Graphic - start
	string InstructionsTextpath = s_cwd_images + "/Instructions_text.png";

	// create a SDL surface to hold the background image
	surface = IMG_Load(InstructionsTextpath.c_str());


	//create a SDL texture
	SDL_Texture *InstructionsText;

	// place surface into the texture
	InstructionsText = SDL_CreateTextureFromSurface(renderer, surface);

	SDL_FreeSurface(surface);

	// create SDL Rectangle for the title graphic
	SDL_Rect InstructionsTextPos;

	//create the SDL_Rectangle for the texture's position and size - title
	InstructionsTextPos.x = 275;
	InstructionsTextPos.y = 150;
	InstructionsTextPos.w = 500;
	InstructionsTextPos.h = 500;

	//Instructions Graphic - end


	// small return button - start


	// create a SDL surface to hold the background image
	surface = IMG_Load((s_cwd_images + "/Return_to_Main_Menu.png").c_str());

	string RmenuNpath = s_cwd_images + "/Return_to_Main_Menu.png";

	// create a SDL surface to hold the background image
	surface = IMG_Load(RmenuNpath.c_str());


	//create a SDL texture
	SDL_Texture *RmenuN;

	// place surface into the texture
	RmenuN = SDL_CreateTextureFromSurface(renderer, surface);

	SDL_FreeSurface(surface);

	// small return button - start


	// create a SDL surface to hold the background image
	surface = IMG_Load((s_cwd_images + "/Return_to_Main_Menu_o.png").c_str());

	string RmenuOpath = s_cwd_images + "/Return_to_Main_Menu_o.png";

	// create a SDL surface to hold the background image
	surface = IMG_Load(RmenuOpath.c_str());


	//create a SDL texture
	SDL_Texture *RmenuO;

	// place surface into the texture
	RmenuO = SDL_CreateTextureFromSurface(renderer, surface);

	SDL_FreeSurface(surface);

	// create SDL Rectangle for the title graphic
	SDL_Rect RmenuNPos;

	//create the SDL_Rectangle for the texture's position and size - title
	RmenuNPos.x = 50;
	RmenuNPos.y = 550;
	RmenuNPos.w = 300;
	RmenuNPos.h = 300;

	//small return button - end

	//***** Create Instructions Menu - end *****



	//***** Create Win Menu - Start *****

	// create a SDL surface to hold the background image
	surface = IMG_Load((s_cwd_images + "/You_Win.png").c_str());

	//create a SDL texture
	SDL_Texture *WinText;

	// place surface into the texture
	WinText = SDL_CreateTextureFromSurface(renderer, surface);

	SDL_FreeSurface(surface);

	// create SDL Rectangle for the title graphic
	SDL_Rect WinTextPos;

	//create the SDL_Rectangle for the texture's position and size - title
	WinTextPos.x = 275;
	WinTextPos.y = 100;
	WinTextPos.w = 500;
	WinTextPos.h = 500;

	//Win Graphic - end


	// small Play Again button - start

	// create a SDL surface to hold the background image
	surface = IMG_Load((s_cwd_images + "/Play_Again.png").c_str());

	//create a SDL texture
	SDL_Texture *PlayAgainN;

	// place surface into the texture
	PlayAgainN = SDL_CreateTextureFromSurface(renderer, surface);

	SDL_FreeSurface(surface);

	// small return button - start

	// create a SDL surface to hold the background image
	surface = IMG_Load((s_cwd_images + "/Play_Again_o.png").c_str());

	//create a SDL texture
	SDL_Texture *PlayAgainO;

	// place surface into the texture
	PlayAgainO = SDL_CreateTextureFromSurface(renderer, surface);

	SDL_FreeSurface(surface);

	// create SDL Rectangle for the title graphic
	SDL_Rect PlayAgainNPos;

	//create the SDL_Rectangle for the texture's position and size - title
	PlayAgainNPos.x = 750;
	PlayAgainNPos.y = 550;
	PlayAgainNPos.w = 300;
	PlayAgainNPos.h = 300;

	//small return button - end

	//***** Create Win Menu - end *****

	//***** Create Lose Menu - Start *****

	// create a SDL surface to hold the background image
	surface = IMG_Load((s_cwd_images + "/You_Lose.png").c_str());

	//create a SDL texture
	SDL_Texture *LoseText;

	// place surface into the texture
	LoseText = SDL_CreateTextureFromSurface(renderer, surface);

	SDL_FreeSurface(surface);

	// create SDL Rectangle for the title graphic
	SDL_Rect LoseTextPos;

	//create the SDL_Rectangle for the texture's position and size - title
	LoseTextPos.x = 275;
	LoseTextPos.y = 100;
	LoseTextPos.w = 500;
	LoseTextPos.h = 500;

	//Lose Graphic - end

	//***** Create Lose Menu - end *****




    //**********Create CURSOR - start**********
    // create cursor
    string CURSORpath = s_cwd_images + "/cursor.png";

    // create a SDL surface to hold the background image
    surface = IMG_Load(CURSORpath.c_str());

    //create a SDL texture
    SDL_Texture * cursor;

    // place surface into the texture
    cursor = SDL_CreateTextureFromSurface(renderer, surface);

    //create the SDL_Rectangle for the texture's position and size = -x,y,w,h
    //SDL_Rect cursorPos, activePos;

    //set the X, Y, W, and H for the Rectangle
    cursorPos.x = 0;
    cursorPos.y = 0;
    cursorPos.w = 50;
    cursorPos.h = 40;

	//set the X, Y, W, and H for the Rectangle
	activePos.x = 1;
	activePos.y = 1;
	activePos.w = 1;
	activePos.h = 1;

	//var from cursor speed
	//int cursorSpeed = 400;

	//**********Create CURSOR - end**********

/*
	//The surface contained by the window
	SDL_Surface* screenSurface = NULL;
	//Get window surface
	screenSurface = SDL_GetWindowSurface( window );
	//Fill the surface white
	SDL_FillRect( screenSurface, NULL, SDL_MapRGB( screenSurface->format, 0, 42, 254 ) );
	//Update the surface
	SDL_UpdateWindowSurface( window );
*/

	//*****set up a Game Controller variable *****
	SDL_GameController* gGameController = NULL;

	//****Open Game Controller *****
	gGameController = SDL_GameControllerOpen(0);

	//***** Turn on Game Controller Events
	SDL_GameControllerEventState(SDL_ENABLE);

	//***** SDL Event to handle input
	SDL_Event event;

	//set up variables for the game states
	enum GameState{MENU, INSTRUCTIONS, PLAYERS1, PLAYERS2,WIN,LOSE};

	//set up the initial state
	GameState gameState = MENU;

	//boolean values to control movement through the states
	bool menu, instructions, players1, players2, win, lose, quit;

	quit = false;


    // The window is open: could enter program loop here (see SDL_PollEvent())
	while(!quit)
	{
		switch(gameState)
		{
		case MENU:
			menu  = true;

			while(menu)
			{
				// set up frame rate for the sections, or case
				thisTime = SDL_GetTicks();
				deltaTime = (float)(thisTime - lastTime)/1000;
				lastTime = thisTime;

				//check for input events
				if(SDL_PollEvent(&event))
				{

					//check to see if the SDL Window is closed - player clicks X in window
					if(event.type == SDL_QUIT)
					{

						quit = true;
						menu = false;
						break;
					}

					switch(event.type)
					{
						case SDL_CONTROLLERBUTTONDOWN:

							if(event.cdevice.which == 0)
							{
								if(event.cbutton.button == SDL_CONTROLLER_BUTTON_A)
								{
									//*********************New*******************
									//if player chooses 1 player game
									if(players1Over){
										menu = false;
										gameState = PLAYERS1;
										players1Over = false;
									}

									//if player chooses 2 player game
									if(players2Over){
										menu = false;
										gameState = PLAYERS2;
										players2Over = false;
									}

									//if player chooses instructions
									if(instructionsOver){
										menu = false;
										gameState = INSTRUCTIONS;
										instructionsOver = false;
									}

									//if player chooses quit
									if(quitOver){
										menu = false;
										quit = true;
										quitOver = false;
									}
									//*********************New*******************
								}
							}
							break;
							//****************************New****************************
						case SDL_CONTROLLERAXISMOTION:

							moveCursor(event.caxis);
							break;
					}
				}

				//Update background
				UpdateBackground(deltaTime);

				//Update cursor
				UpdateCursor(deltaTime);

				players1Over = SDL_HasIntersection(&activePos, &Player1NPos);
				players2Over = SDL_HasIntersection(&activePos, &Player2NPos);
				instructionsOver = SDL_HasIntersection(&activePos, &InstructionsNPos);
				quitOver = SDL_HasIntersection(&activePos, &QuitNPos);

				//Start Drawing
				// Clear SDL renderer
				SDL_RenderClear(renderer);

				//Draw the bkgd image
				SDL_RenderCopy(renderer, bkgd1, NULL, &bkgd1Pos);

				//Draw the bkgd2 image
				SDL_RenderCopy(renderer, bkgd2, NULL, &bkgd2Pos);

				//Draw the title image
				SDL_RenderCopy(renderer, title, NULL, &titlePos);

				//****************************NEW**********************
				// Draw 1 player button
				if(players1Over){
					SDL_RenderCopy(renderer, Player1O, NULL, &Player1NPos);
				} else {
					SDL_RenderCopy(renderer, Player1N, NULL, &Player1NPos);
				}

				// Draw 2 player button
				if(players2Over){
					SDL_RenderCopy(renderer, Player2O, NULL, &Player2NPos);
				} else{
					SDL_RenderCopy(renderer, Player2N, NULL, &Player2NPos);
				}

				// Draw instructions button
				if(instructionsOver){
					SDL_RenderCopy(renderer, InstructionsO, NULL, &InstructionsNPos);
				} else{
					SDL_RenderCopy(renderer, InstructionsN, NULL, &InstructionsNPos);
				}

				// Draw quit button
				if(quitOver){
					SDL_RenderCopy(renderer, QuitO, NULL, &QuitNPos);
				} else{
					SDL_RenderCopy(renderer, QuitN, NULL, &QuitNPos);
				}
				//****************************NEW**********************

				//Draw the cursor image
				SDL_RenderCopy(renderer, cursor, NULL, &cursorPos);

				//SDL Render present
				SDL_RenderPresent(renderer);
			}
			break; // end menu case

		case INSTRUCTIONS:
			instructions  = true;

			while(instructions)
			{
				// set up frame rate for the sections, or case
				thisTime = SDL_GetTicks();
				deltaTime = (float)(thisTime - lastTime) / 1000;
				lastTime = thisTime;

				//check for input events
				if(SDL_PollEvent(&event))
				{

				//check to see if the SDL Window is closed - player clicks X in window
				if(event.type == SDL_QUIT)
				{
					quit = true;
					instructions = false;
					break;
				}

				switch(event.type)
				{
					case SDL_CONTROLLERBUTTONDOWN:

					if(event.cdevice.which == 0)
					{
						if(event.cbutton.button == SDL_CONTROLLER_BUTTON_A)
						{
							//if player chooses main menu game
							if(menuOver){
								instructions = false;
								gameState = MENU;
								menuOver = false;
							}
						}
					}
					break;
					case SDL_CONTROLLERAXISMOTION:

						moveCursor(event.caxis);
						break;
					}
				}

				//Update
				UpdateBackground(deltaTime);

				//cursor
				UpdateCursor(deltaTime);

				//check for cursor intersection with menu button
				menuOver = SDL_HasIntersection(&activePos, &RmenuNPos);

				SDL_RenderClear(renderer);

				//Draw the bkgd image
				SDL_RenderCopy(renderer, bkgd1, NULL, &bkgd1Pos);

				//Draw the bkgd2 image
				SDL_RenderCopy(renderer, bkgd2, NULL, &bkgd2Pos);

				//Draw the title image
				SDL_RenderCopy(renderer, title, NULL, &titlePos);

				//Draw the instruction graphic
				SDL_RenderCopy(renderer, InstructionsText, NULL, &InstructionsTextPos);

				//****************************NEW**********************
				//Draw Main Menu button
				if(menuOver){
					SDL_RenderCopy(renderer, RmenuO, NULL, &RmenuNPos);
				}else {
					SDL_RenderCopy(renderer, RmenuN, NULL, &RmenuNPos);
				}
				//****************************NEW**********************

				//Draw the cursor image
				SDL_RenderCopy(renderer, cursor, NULL, &cursorPos);

				//SDL Render present
				SDL_RenderPresent(renderer);

			}
			break; // end instructions case

		case PLAYERS1:
			players1  = true;

			while(players1)
			{
				thisTime = SDL_GetTicks();
				deltaTime = (float)(thisTime - lastTime) / 1000;
				lastTime = thisTime;

				//check for input events
				if(SDL_PollEvent(&event))
				{
					//check to see if the SDL Window is closed - player clicks X in window
					if(event.type == SDL_QUIT)
					{
						quit = true;
						players1 = false;
						break;
					}

					switch(event.type)
					{
						case SDL_CONTROLLERBUTTONDOWN:
							if(event.cdevice.which == 0)
							{
								if(event.cbutton.button == SDL_CONTROLLER_BUTTON_X)
								{
									players1 = false;
									gameState = WIN;
								}

								if(event.cbutton.button == SDL_CONTROLLER_BUTTON_Y)
								{
									players1 = false;
									gameState = LOSE;
								}

								//send button press info to player1
								player1.OnControllerButton(event.cbutton);

							}
							break;

						case SDL_CONTROLLERAXISMOTION:

							player1.OnControllerAxis(event.caxis);
							break;
						}
					}
				//Update
				UpdateBackground(deltaTime);

				//update player1
				player1.Update(deltaTime);

				SDL_RenderClear(renderer);

				//Draw the bkgd image
				SDL_RenderCopy(renderer, bkgd1, NULL, &bkgd1Pos);

				//Draw the bkgd2 image
				SDL_RenderCopy(renderer, bkgd2, NULL, &bkgd2Pos);

				//draw player
				player1.Draw(renderer);

				//SDL Render present
				SDL_RenderPresent(renderer);

				}
				break; // end players1 case

		case PLAYERS2:
			players2  = true;

			while(players2)
			{
				thisTime = SDL_GetTicks();
				deltaTime = (float)(thisTime - lastTime) / 1000;
				lastTime = thisTime;

				//check for input events
				if(SDL_PollEvent(&event))
				{

					//check to see if the SDL Window is closed - player clicks X in window
					if(event.type == SDL_QUIT)
					{
						quit = true;
						players2 = false;
						break;
					}

					switch(event.type)
					{
						case SDL_CONTROLLERBUTTONDOWN:

						if(event.cdevice.which == 0)
						{
							if(event.cbutton.button == SDL_CONTROLLER_BUTTON_A)
							{
								players2 = false;
								gameState = WIN;
							}

							if(event.cbutton.button == SDL_CONTROLLER_BUTTON_B)
							{
								players2 = false;
								gameState = LOSE;
							}
						}
						break;
					}
				}

				//Update
				UpdateBackground(deltaTime);

				SDL_RenderClear(renderer);

				//Draw the bkgd image
				SDL_RenderCopy(renderer, bkgd1, NULL, &bkgd1Pos);

				//Draw the bkgd2 image
				SDL_RenderCopy(renderer, bkgd2, NULL, &bkgd2Pos);

				//Draw the title image
				SDL_RenderCopy(renderer, Player2N, NULL, &Player2NPos);

				//SDL Render present
				SDL_RenderPresent(renderer);
			}
			break; // end player2 case

		case WIN:
			win  = true;

			while(win)
			{
				thisTime = SDL_GetTicks();
				deltaTime = (float)(thisTime - lastTime) / 1000;
				lastTime = thisTime;

				//check for input events
				if(SDL_PollEvent(&event))
				{
					//check to see if the SDL Window is closed - player clicks X in window
					if(event.type == SDL_QUIT)
					{
						quit = true;
						win = false;
						break;
					}

					switch(event.type)
					{
					case SDL_CONTROLLERBUTTONDOWN:

						if(event.cdevice.which == 0)
						{
							//if player chooses main menu game
							if(menuOver){
								win = false;
								gameState = MENU;
								menuOver = false;
							}

							//if player chooses play again game
							if(playerOver){
								win = false;
								gameState = PLAYERS1;
								playerOver = false;
							}
						}
						break;

					case SDL_CONTROLLERAXISMOTION:

						moveCursor(event.caxis);
						break;
					}
				}
				//Update
				UpdateBackground(deltaTime);

				UpdateCursor(deltaTime);

				//check for cursor button collision
				menuOver = SDL_HasIntersection(&activePos, &RmenuNPos);
				playerOver = SDL_HasIntersection(&activePos, &PlayAgainNPos);

				SDL_RenderClear(renderer);

				//Draw the bkgd image
				SDL_RenderCopy(renderer, bkgd1, NULL, &bkgd1Pos);

				//Draw the bkgd2 image
				SDL_RenderCopy(renderer, bkgd2, NULL, &bkgd2Pos);

				//Draw the Win Text
				SDL_RenderCopy(renderer, WinText, NULL, &WinTextPos);

				//***************************New*******************
				//draw main  menu button
				if(menuOver){
					SDL_RenderCopy(renderer, RmenuO, NULL, &RmenuNPos);
				} else {
					SDL_RenderCopy(renderer, RmenuN, NULL, &RmenuNPos);
				}

				//draw Play Again button
				if(playerOver){
					SDL_RenderCopy(renderer, PlayAgainO, NULL, &PlayAgainNPos);
				} else {
					SDL_RenderCopy(renderer, PlayAgainN, NULL, &PlayAgainNPos);
				}
				//***************************New*******************

				//Draw the cursor image
				SDL_RenderCopy(renderer, cursor, NULL, &cursorPos);

				//SDL Render present
				SDL_RenderPresent(renderer);

			}
			break; // end win case

		case LOSE:
			lose  = true;

			while(lose)
			{
				thisTime = SDL_GetTicks();
				deltaTime = (float)(thisTime - lastTime) / 1000;
				lastTime = thisTime;

				//check for input events
				if(SDL_PollEvent(&event))
				{
					//check to see if the SDL Window is closed - player clicks X in window
					if(event.type == SDL_QUIT)
					{
						quit = true;
						lose = false;
						break;
					}

					switch(event.type)
					{
					case SDL_CONTROLLERBUTTONDOWN:
						if(event.cdevice.which == 0)
						{
							if(event.cbutton.button == SDL_CONTROLLER_BUTTON_A)
							{
								// if player chooses main menu game
								if(menuOver){
									lose = false;
									gameState = MENU;
									menuOver = false;
								}

								//if player chooses play again game
								if(playerOver){
									lose = false;
									gameState = PLAYERS1;
									playerOver = false;
								}
							}
						}
						break;
					case SDL_CONTROLLERAXISMOTION:

						moveCursor(event.caxis);
						break;
					}
				}

				//Update
				UpdateBackground(deltaTime);

				UpdateCursor(deltaTime);

				//check for cursor button collision
				menuOver = SDL_HasIntersection(&activePos, &RmenuNPos);
				playerOver = SDL_HasIntersection(&activePos, &PlayAgainNPos);

				SDL_RenderClear(renderer);

				//Draw the bkgd image
				SDL_RenderCopy(renderer, bkgd1, NULL, &bkgd1Pos);

				//Draw the bkgd2 image
				SDL_RenderCopy(renderer, bkgd2, NULL, &bkgd2Pos);

				//Draw the Lose Text
				SDL_RenderCopy(renderer, LoseText, NULL, &LoseTextPos);

				//***************************New*******************
				//draw main  menu button
				if(menuOver){
					SDL_RenderCopy(renderer, RmenuO, NULL, &RmenuNPos);
				} else {
					SDL_RenderCopy(renderer, RmenuN, NULL, &RmenuNPos);
				}

				//draw Play Again button
				if(playerOver){
					SDL_RenderCopy(renderer, PlayAgainO, NULL, &PlayAgainNPos);
				} else {
					SDL_RenderCopy(renderer, PlayAgainN, NULL, &PlayAgainNPos);
				}
				//***************************New*******************

				//Draw the cursor image
				SDL_RenderCopy(renderer, cursor, NULL, &cursorPos);

				//SDL Render present
				SDL_RenderPresent(renderer);
			}
			break; // end lose case

		}
	}

    //SDL_Delay(3000);  // Pause execution for 3000 milliseconds, for example

    // Close and destroy the window
    SDL_DestroyWindow(window);

    // Clean up
    SDL_Quit();
    return 0;
}





