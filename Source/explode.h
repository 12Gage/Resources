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

class Explode{

public:

	//boolean for the the state of the explosion
	bool active;

	//explosion texture
	SDL_Texture * texture;

	//explosion rectangle for position (X,Y) and size (W,H)
	SDL_Rect posRect;

	//explosion rectangle for poaition (X,Y) and size (W,H)
	SDL_Rect drawRect;

	//width and height of each frame place width and height of entire image
	int FrameWidth, FrameHeight, textureWidth, textureHeight;

	//float values to track time until next frame of animation
	float frameCounter;

	//explosion creation meothd. requires the renderer, a path to the needed image, an X positon, a Y position
	Explode(SDL_Renderer *renderer, string fiePath, float x, float y);

	//explode update - requires deltaTime be passed
	void Update(float deltaTime);

	//explode dreaw - requires renderer be passed
	void Draw(SDL_Renderer *renderer);

	//explode reset
	void Reset();

	//explode destruction method
	~Explode();
};