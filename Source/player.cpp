#include "player.h"

//analog joystick dead zone
const int JOYSTICK_DEAD_ZONE = 8000;

//player creation method
Player::Player(SDL_Renderer *renderer, int pNum, string filePath, float x, float y)
{
	// set the player number 0 or 1
	playerNum = pNum;

	// set the float for player speed
	speed = 500.0f;

	// see if this is player 1, or player 2 , and create the correct file path
	if(playerNum == 0){
		//create the player 1 texture
		playerPath = filePath + "player.png";
	}else{
		//create the player 2 textures
		playerPath = filePath + "player2.png";
	}

	// load the surface
	surface = IMG_Load(playerPath.c_str());

	//load the surface into the texture
	texture = SDL_CreateTextureFromSurface(renderer, surface);

	//free the surface for later use
	SDL_FreeSurface(surface);

	//set the SDL_Rect X and Y for the player
	posRect.x = x;
	posRect.y = y;

	//use SDL_QueryTexture to get the W and H of the player's texture
	int w, h;

	SDL_QueryTexture(texture, NULL, NULL, &w, &h);

	posRect.w = w;
	posRect.h =h;

	// set the movement floats to the players original X and Y
	pos_X = x;
	pos_Y = y;

	// set the xDir and yDir for the joysticks
	xDir = 0;
	yDir = 0;

}

//player joystick button method
void Player::OnControllerButton(const SDL_ControllerButtonEvent event)
{
	//if the player's number is 0 and the joystick button is from joystick 0
	if(event.which == 0 && playerNum ==0)
	{
		//if A button
		if(event.button == 0)
		{
			cout << "Player 1 - Button A" << endl;

		}

	}

	//if the player's number is 1 and the joystick button is from joystick 1
	if(event.which == 1 && playerNum == 1)
	{
		//if A button
		if(event.button ==0)
		{
			cout << "Player 1 - Button A" << endl;

		}

	}

}

//player joystick axis methos
void Player::OnControllerAxis(const SDL_ControllerAxisEvent event)
{
	//axis movements and button presses both sent here as SDL_ControllerAxisEvent structures

	// if the player'sa number is 0 and the joystick axis is from joystick 0
	if(event.which == 0 && playerNum ==0)
	{
		//x axis
		if(event.axis == 0)
		{

			if(event.value < - JOYSTICK_DEAD_ZONE)
			{
				xDir = -1.0f; //left
			}
			else if(event.value > JOYSTICK_DEAD_ZONE)
			{
				xDir = 1.0f; // right
			}
			else
			{
				xDir = 0.0f; //none
			}
		}

		//y axis
		if(event.axis == 1)
		{

			if(event.value < - JOYSTICK_DEAD_ZONE)
			{
				yDir = -1.0f; //left
			}
			else if(event.value > JOYSTICK_DEAD_ZONE)
			{
				yDir = 1.0f; // right
			}
			else
			{
				yDir = 0.0f; //none
			}
		}

	}

	// if the player'sa number is 0 and the joystick axis is from joystick 0
	if(event.which == 1 && playerNum == 1)
	{
		//x axis
		if(event.axis == 0)
		{

			if(event.value < - JOYSTICK_DEAD_ZONE)
			{
				xDir = -1.0f; //left
			}
			else if(event.value > JOYSTICK_DEAD_ZONE)
			{
				xDir = 1.0f; // right
			}
			else
			{
				xDir = 0.0f; //none
			}
		}

		//y axis
		if(event.axis == 1)
		{

			if(event.value < - JOYSTICK_DEAD_ZONE)
			{
				yDir = -1.0f; //left
			}
			else if(event.value > JOYSTICK_DEAD_ZONE)
			{
				yDir = 1.0f; // right
			}
			else
			{
				yDir = 0.0f; //none
			}
		}

	}
}

//Player update method
void Player::Update(float deltaTime)
{
	//adjust position floats based on speed, direction of joystick axis and deltaTime
	pos_X += (speed * xDir) * deltaTime;
	pos_Y += (speed * yDir) * deltaTime;

	//update palyer position with code to account for precision loss
	posRect.x = (int)(pos_X + 0.5f);
	posRect.y = (int)(pos_Y + 0.5f);

	if (posRect.x < 0){
		posRect.x = 0;
		pos_X = posRect.x;
	}

	if (posRect.x > 1024 - posRect.w){
		posRect.x = 1024 - posRect.w;
		pos_X = posRect.x;
	}

	if (posRect.y < 0){
		posRect.y = 0;
		pos_Y = posRect.y;
	}

	if (posRect.y > 768 - posRect.h){
		posRect.y = 768 - posRect.h;
		pos_Y = posRect.y;
	}

}

//player draw method
void Player::Draw(SDL_Renderer *renderer)
{
	//draw the player texture using the vars texture and posRect
	SDL_RenderCopy(renderer, texture, NULL, &posRect);

}

//player desteuction method
Player::~Player()
{
	SDL_DestroyTexture(texture);

}


