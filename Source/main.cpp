// Example program:
// Using SDL2 to create an application window

#if defined(_WIN32) || (_WIN64)

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"

#endif

#if defined(__APPLE__)

#include "SDL2/SDL.h"
#include "SDL2_image/SDL_image.h"
#include "SDL2_mixer/SDL_mixer.h"

#endif

#if defined(__linux__)

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_mixer.h"

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
const int JOYSTICK_DEAD_ZONE = 8000;

//joystick direction vars
float xDir, yDir;

//cursor float vars for movement
float pos_X, pos_Y;

//create SDL Rectangle for the 2 player graphic
SDL_Rect cursorPos, activePos;

//var from cursor speed
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
#include "enemy.h"
#include <vector>
#include <stdlib.h> /*srand.rand*/
#include <time.h>   /*time*/
#include "explode.h"

//variable to hold the list of enemies: for 1 player game - 6 total, for 2 player game - 12 total
vector<Enemy> enemyList;

vector<Explode> explodeList;

void MakeExplosion(int x, int y){
	//see if there is an explosion not active to use
	for(int i = 0; i < explodeList.size(); i ++)
	{
		//see if the bullet is not active
		if(explodeList[i].active == false){

			//set explosions to active
			explodeList [i].active = true;

			//use some math in the x position to get the bullet close to
			//the center of the player using player width
			explodeList[i].posRect.x = x;
			explodeList[i].posRect.y = y;

			//once explosion is found, break out of loop
			break;
		}
	}
}

int main(int argc, char* argv[]) {

	/* initialize random seed; */
	srand (time(NULL));

#if defined(_WIN32) || (_WIN64)
	cout << "Running on Windows" << endl;
	cout << "Added on Windows" << endl;

	string s_cwd(getcwd(NULL, 0));

	string s_cwd_images = s_cwd + "\\Resources\\Images";
	string s_cwd_images = s_cwd + "\\Resources\\Audio";

#endif

#if defined(__linux__)
	cout << "Running on Linux" << endl;
	cout << "Added on the Linux platform" << endl;

	//get the current working directory
	string s_cwd(getcwd(NULL,0));

	//create a string linking to the mac's images folder
	string s_cwd_images = s_cwd + "/Resources/Images/";
	string s_cwd_images = s_cwd + "/Resources/Audio/";

#endif

#if defined(__APPLE__)
	cout << "Running on Apple" << endl;
	//get the current working directory
	string s_cwd(getcwd(NULL,0));

	//create a string linking to the mac's images folder
	string s_cwd_images = s_cwd + "/Resources/Images/";

	//create a string to link to the audio folder on __APPLE__
	string audio_dir = s_cwd+ "/Resources/Audio/";

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

	//***** Turn on Game Controller Events
	SDL_GameControllerEventState(SDL_ENABLE);

	//*****set up a Game Controller variable - 1 *****
	SDL_GameController* gGameController0 = NULL;

	//****Open Game Controller - 1 *****
	gGameController0 = SDL_GameControllerOpen(0);

	//*****set up a Game Controller - 2 variable *****
	SDL_GameController* gGameController1 = NULL;

	//****Open Game Controller - 2 *****
	gGameController1 = SDL_GameControllerOpen(1);



	//***** SDL Event to handle input
	SDL_Event event;

	//set up variables for the game states
	enum GameState{MENU, INSTRUCTIONS, PLAYERS1, PLAYERS2,WIN,LOSE};

	//set up the initial state
	GameState gameState = MENU;

	//boolean values to control movement through the states
	bool menu, instructions, players1, players2, win, lose, quit;

	quit = false;

	//open audio channel
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

	//load a music file
	Mix_Music *bgm = Mix_LoadMUS((audio_dir + "background.mp3").c_str());

	//if the music file is not playing  - play it
	if(!Mix_PlayingMusic())
		Mix_PlayMusic(bgm, -1);

	//set up a sound effect CHUNCK for the button over state
	Mix_Chunk *overSound = Mix_LoadWAV((audio_dir + "sound.wav").c_str());

	//set up a sound effect CHUNK for the buttons pressed state
	Mix_Chunk *pressedSound = Mix_LoadWAV((audio_dir + "pressed.wav").c_str());

	//set up a sound effect CHUNK for the buttons pressed state
	Mix_Chunk *explosionSound = Mix_LoadWAV((audio_dir + "enemy.wav").c_str());


    //****************************************Create Players - START ***********
    Player player1 = Player(renderer, 0, s_cwd_images.c_str(), audio_dir.c_str(), 250.0, 500.0);
    Player player2 = Player(renderer, 1, s_cwd_images.c_str(), audio_dir.c_str(), 750.0, 500.0);

	//create a pool of explosions - 20
	for (int i = 0; i < 20; i++)
	{
		//create the enemy
		Explode tmpExplode(renderer, s_cwd_images, -1000, -1000);

		//add to enemylist
		explodeList.push_back(tmpExplode);
	}

	//bool value to control the over sound effect and the buttons
	bool alreadyOver = false;


    // The window is open: could enter program loop here (see SDL_PollEvent())
	while(!quit)
	{
		switch(gameState)
		{
		case MENU:
			alreadyOver = false;

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
										//play the over sound - player fin through levels, must pause/delay for QUIT
										Mix_PlayChannel(-1,pressedSound,0);
										menu = false;
										gameState = PLAYERS1;
										players1Over = false;
									}

									//if player chooses 2 player game
									if(players2Over){
										//play the over sound - player fin through levels, must pause/delay for QUIT
										Mix_PlayChannel(-1,pressedSound,0);
										menu = false;
										gameState = PLAYERS2;
										players2Over = false;
									}

									//if player chooses instructions
									if(instructionsOver){
										//play the over sound - player fin through levels, must pause/delay for QUIT
										Mix_PlayChannel(-1,pressedSound,0);
										menu = false;
										gameState = INSTRUCTIONS;
										instructionsOver = false;
									}

									//if player chooses quit
									if(quitOver){
										//play the over sound - player fin through levels, must pause/delay for QUIT
										Mix_PlayChannel(-1,pressedSound,0);
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

				//if the cursor is over a button, play the over sound
				if(players1Over || players2Over ||instructionsOver || quitOver){
					if(alreadyOver == false){
						Mix_PlayChannel(-1, overSound, 0);
						alreadyOver = true;
					}
				}

				if(players1Over || players2Over ||instructionsOver || quitOver){
					alreadyOver = false;
				}

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

			alreadyOver = false;

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

				//if the cursor is over a button, play the over sound
				if(menuOver){
					if(alreadyOver == false){
						Mix_PlayChannel(-1, overSound, 0);
						alreadyOver = true;
					}
				}

				if(!menuOver){
					alreadyOver = false;
				}

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
			alreadyOver = false;

			enemyList.clear();

			//reset the player
			player1.Reset();

			players1  = true;

			//create the enemy pool - 6'
			for(int i = 0; i < 6; i++)
			{
				//create the enemy
				Enemy tmpEnemy(renderer, s_cwd_images);

				//add to enemylist
				enemyList.push_back(tmpEnemy);

			}

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
							if(event.cdevice.which == 0  || event.cdevice.which == 1)
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
								if (player1.active) {
									player1.OnControllerButton(event.cbutton);
								}

							}
							break;

						case SDL_CONTROLLERAXISMOTION:
							if (player1.active) {
								player1.OnControllerAxis(event.caxis);
							}
							break;
						}
					}
				//Update
				UpdateBackground(deltaTime);

				//update player1
				if (player1.active) {
					player1.Update(deltaTime, renderer);
				}

				//update the enemies
				for(int i = 0; i < enemyList.size(); i ++)
				{
					//update enemy
					enemyList[i].Update(deltaTime);
				}

				//only check if the player is active
				if (player1.active == true)
				{

					//use the player 1 bullet list to see of the active bullets
					//hit any of the enemies
					for (int i = 0; i < player1.bulletList.size(); i++)
					{
						//check to see if this bullet is active (onscreen)
						if (player1.bulletList[i].active == true) {

							//check all enemies against the active bullet
							for (int j = 0; j < enemyList.size(); j++)
							{
								//if there is collosopm between the two objects
								if (SDL_HasIntersection(&player1.bulletList[i].posRect, &enemyList[j].posRect)) {

									//play explosion sound
									Mix_PlayChannel(-1, explosionSound, 0);

									MakeExplosion(enemyList[j].posRect.x, enemyList[j].posRect.y);

									//reset the enemy
									enemyList[j].Reset();

									//reset the enmy
									player1.bulletList[i].Reset();

									//give the player some points
									player1.playerScore += 50;

									if(player1.playerScore >= 1000)
									{
										//go to win
										players1 = false;
										gameState = WIN;
									}
								}
							}
						}
					}

					//check to see if the enemies hit the player
					for (int i = 0; i < enemyList.size(); i++) {
						//if there is a collosopm between the two objects
						if (SDL_HasIntersection(&player1.posRect, &enemyList[i].posRect)) {

							//play explosion sound
							Mix_PlayChannel(-1, explosionSound, 0);

							MakeExplosion(player1.posRect.x-32, player1.posRect.y-32);

							//reset the enemy
							enemyList[i].Reset();

							//give the player some points
							player1.playerLives -= 1;

							//if game over - player lives == 0
							if (player1.playerLives <= 0) {
								players1 = false;
								gameState = LOSE;
								break;
							}
						}
					}
				}
				//ends

				//create a pool of explosions - 20
				for (int i = 0; i < explodeList.size(); i++)
				{
					//check to see if active
					if (explodeList[i].active == true) {
						//draw explode
						explodeList[i].Update(deltaTime);
					}
				}

				SDL_RenderClear(renderer);

				//Draw the bkgd image
				SDL_RenderCopy(renderer, bkgd1, NULL, &bkgd1Pos);

				//Draw the bkgd2 image
				SDL_RenderCopy(renderer, bkgd2, NULL, &bkgd2Pos);

				//draw the enemies
				for(int i = 0; i < enemyList.size(); i ++)
				{
					//update enemy
					enemyList[i].Draw(renderer);
				}

				//create a pool of explosions - 20
				for (int i = 0; i < explodeList.size(); i++)
				{
					//check to see if active
					if (explodeList[i].active == true) {
						//draw explode
						explodeList[i].Draw(renderer);
					}
				}

				//draw player
				player1.Draw(renderer);

				//SDL Render present
				SDL_RenderPresent(renderer);

				}
				break; // end players1 case

		case PLAYERS2:
			alreadyOver = false;

			enemyList.clear();

			//reset player1 and player2
			player1.Reset();
			player2.Reset();

			players2  = true;

			//create the enemy pool - 12
			for (int i = 0; i < 12; i++)
			{
				//create the enemy
				Enemy tmpEnemy(renderer, s_cwd_images);

					//add to enemylist
					enemyList.push_back(tmpEnemy);
			}

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
							if(event.cbutton.button == SDL_CONTROLLER_BUTTON_X)
							{
								players2 = false;
								gameState = WIN;
							}

							if(event.cbutton.button == SDL_CONTROLLER_BUTTON_Y)
							{
								players2 = false;
								gameState = LOSE;
							}
						}
						if (player1.active) {
							player1.OnControllerButton(event.cbutton);
						}

						if (player2.active) {
							player2.OnControllerButton(event.cbutton);
						}

						break;

						case SDL_CONTROLLERAXISMOTION:

							if (player1.active) {
								player1.OnControllerAxis(event.caxis);
							}

							if (player2.active) {
								player2.OnControllerAxis(event.caxis);
							}
							break;
					}
				}

				//Update
				UpdateBackground(deltaTime);

				//update player1
				if (player1.active) {
					player1.Update(deltaTime, renderer);
				}

				//update player2
				if (player2.active) {
					player2.Update(deltaTime, renderer);
				}

				for (int i = 0; i < enemyList.size(); i++)
				{
					//update enemy
					enemyList[i].Update(deltaTime);
				}

				//only check if the player is active
				if (player1.active == true)
				{

					//use the player 1 bullet list to see of the active bullets
					//hit any of the enemies
					for (int i = 0; i < player1.bulletList.size(); i++)
					{
						//check to see if this bullet is active (onscreen)
						if (player1.bulletList[i].active == true) {

							//check all enemies against the active bullet
							for (int j = 0; j < enemyList.size(); j++)
							{
								//if there is collosopm between the two objects
								if (SDL_HasIntersection(&player1.bulletList[i].posRect, &enemyList[j].posRect)) {

									//play explosion sound
									Mix_PlayChannel(-1, explosionSound, 0);

									MakeExplosion(enemyList[j].posRect.x, enemyList[j].posRect.y);

									//reset the enemy
									enemyList[j].Reset();

									//reset the enmy
									player1.bulletList[i].Reset();

									//give the player some points
									player1.playerScore += 50;

									if(player1.playerScore >= 1000)
									{
										//go to win
										players2 = false;
										gameState = WIN;
									}
								}
							}
						}
					}

					//check to see if the enemies hit the player
					for (int i = 0; i < enemyList.size(); i++) {
						//if there is a collosopm between the two objects
						if (SDL_HasIntersection(&player1.posRect, &enemyList[i].posRect)) {

							//play explosion sound
							Mix_PlayChannel(-1, explosionSound, 0);

							MakeExplosion(player1.posRect.x-32, player1.posRect.y-32);

							//reset the enemy
							enemyList[i].Reset();

							//give the player some points
							player1.playerLives -= 1;

							//if game over - player lives == 0
							if (player1.playerLives <= 0 && player2.playerLives <= 0) {
								players2 = false;
								gameState = LOSE;
								break;
							}
						}
					}
				}
				//player 1ends


				//only check if the player is active
				if (player2.active == true)
				{

					//use the player 1 bullet list to see of the active bullets
					//hit any of the enemies
					for (int i = 0; i < player2.bulletList.size(); i++)
					{
						//check to see if this bullet is active (onscreen)
						if (player2.bulletList[i].active == true) {

							//check all enemies against the active bullet
							for (int j = 0; j < enemyList.size(); j++)
							{
								//if there is collosoim between the two objects
								if (SDL_HasIntersection(&player2.bulletList[i].posRect, &enemyList[j].posRect)) {

									//play explosion sound
									Mix_PlayChannel(-1, explosionSound, 0);

									MakeExplosion(enemyList[j].posRect.x, enemyList[j].posRect.y);

									//reset the enemy
									enemyList[j].Reset();

									//reset the enmy
									player2.bulletList[i].Reset();

									//give the player some points
									player2.playerScore += 50;

									if(player2.playerScore >= 1000)
									{
										//go to win
										players2 = false;
										gameState = WIN;
									}
								}
							}
						}
					}

					//check to see if the enemies hit the player
					for (int i = 0; i < enemyList.size(); i++) {
						//if there is a collosopm between the two objects
						if (SDL_HasIntersection(&player2.posRect, &enemyList[i].posRect)) {

							//play explosion sound
							Mix_PlayChannel(-1, explosionSound, 0);

							MakeExplosion(player2.posRect.x-32, player2.posRect.y-32);

							//reset the enemy
							enemyList[i].Reset();

							//give the player some points
							player2.playerLives -= 1;

							//if game over - player lives == 0
							if (player1.playerLives <= 0 && player2.playerLives <= 0) {
								players2 = false;
								gameState = LOSE;
								break;
							}
						}
					}
				}
				// player 2 ends

				//create a pool of explosions - 20
				for (int i = 0; i < explodeList.size(); i++)
				{
					//check to see if active
					if (explodeList[i].active == true) {
						//draw explode
						explodeList[i].Update(deltaTime);
					}
				}

				SDL_RenderClear(renderer);

				//Draw the bkgd image
				SDL_RenderCopy(renderer, bkgd1, NULL, &bkgd1Pos);

				//Draw the bkgd2 image
				SDL_RenderCopy(renderer, bkgd2, NULL, &bkgd2Pos);

				for (int i = 0; i < enemyList.size(); i++)
				{
					//update enemy
					enemyList[i].Draw(renderer);
				}

				for (int i = 0; i < explodeList.size(); i++)
				{
					//check to see if active
					if (explodeList[i].active == true) {
						//draw explode
						explodeList[i].Update(deltaTime);
					}
				}

				//draw player 1
				player1.Draw(renderer);

				//draw player 2
				player2.Draw(renderer);

				//Draw the title image
				//SDL_RenderCopy(renderer, Player2N, NULL, &Player2NPos);

				//SDL Render present
				SDL_RenderPresent(renderer);
			}
			break; // end player2 case

		case WIN:
			alreadyOver = false;

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
								Mix_PlayChannel(-1, pressedSound, 0);
								win = false;
								gameState = MENU;
								menuOver = false;
							}

							//if player chooses play again game
							if(playerOver){
								Mix_PlayChannel(-1, pressedSound, 0);
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

				//if the cursor is over a button, play the over sound
				if(menuOver || playerOver){
					if(alreadyOver == false){
						Mix_PlayChannel(-1, overSound, 0);
						alreadyOver = true;
					}
				}

				if(!menuOver || !playerOver){
					alreadyOver = false;
				}

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
			alreadyOver = false;

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
									Mix_PlayChannel(-1, pressedSound, 0);
									lose = false;
									gameState = MENU;
									menuOver = false;
								}

								//if player chooses play again game
								if(playerOver){
									Mix_PlayChannel(-1, pressedSound, 0);
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

				//if the cursor is over a button, play the over sound
				if(menuOver || playerOver){
					if(alreadyOver == false){
						Mix_PlayChannel(-1, overSound, 0);
						alreadyOver = true;
					}
				}

				if(!menuOver || !playerOver){
					alreadyOver = false;
				}

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





