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


#include <string>
#include <iostream>
using namespace std;

#include <stdlib.h> /*srand.rand*/

class Enemy{

public:
	//enemy texture
	SDL_Texture *texture;

	//enemy rectangle for position (X,Y) and size (W,H)
	SDL_Rect posRect;

	//movement direction values
	float xDir, yDir;

	//enemy speed
	float speed;

	//enemy float postion to prevent precision loss
	float pos_X, pos_Y;

	//enemy angle
	float angle;

	//point for the center of your object. used for rotation
	SDL_Point center;

	//enemy creation method, requires the renderer,  a path the needed image
	Enemy(SDL_Renderer *renderer, string filePath);

	//enemy update - requires deltaTime to be passed
	void Update(float deltaTime);

	//enemy draw - requires renderer to be passed
	void Draw(SDL_Renderer *renderer);

	//enemy reset
	void Reset();

	//enemy destruction method
	~Enemy();
};
