#include "explode.h"

//explode creation method
Explode::Explode(SDL_Renderer *rrenderer, string filePath, float x, float y)
{
	//set the explosion initial state
	active = false;

	//get the directery to the explosion graphic
	string explodePath = filePath + "boom3.png";

	//init from counter
	frameCounter = 0.0f;

	//load the texture into a srface
	SDL_Surface *surface = IMG_Load(explodePath.c_str());

	//create the texture frm the passed renderer and create surface
	texture = SDL_CreateTextureFromSurface(rrenderer, surface);

	//FREE THE SURFACE FOR LATER USE
	SDL_FreeSurface(surface);

	//set the width and height of th bllet's rectangle
	//by querying the textureitself
	int w, h;
	SDL_QueryTexture(texture, NULL, NULL, &w, &h);

	//getting the width and height of the entire image
	textureWidth = w;
	textureHeight = h;

	//setting the individual image size
	posRect.w = 128;
	posRect.h = 128;

	//set the x and y position of the explosion rectangle
	posRect.x = x;
	posRect.y = y;

	//frame width and height of each animation image
	FrameWidth = 128;
	FrameHeight = 128;

	//original draw rectangle
	//used to draw one frame of the animation strip
	drawRect.x = 128;
	drawRect.y = 0;
	drawRect.w = FrameWidth;
	drawRect.h = FrameHeight;

}

//bullet update method
void Explode::Update(float deltaTime) //float delta
{
	//check to see of the bullet is active
	if (active) {

		//add to frame counter
		frameCounter += deltaTime;

		//check to see if enough time has passed
		if (frameCounter >= .035f) {

			//reset frame counter
			frameCounter = 0.0f;

			//add to the drawRect the width of an individual frame
			//to move the drawing area over one
			drawRect.x += FrameWidth;

			//see of we have gone past the edge of the image - width
			if (drawRect.x >= textureWidth)
			{
				//if so, go back to the first - 0
				drawRect.x = 0;

				//set active to false
				active = false;

				//move off screen until needed again
				posRect.x = -1000;
				posRect.y = -1000;

			}
		}
	}
}

//reset the bullet method
void Explode::Reset() {

	//deactivate the bullet
	active = false;

}

//bullet draw method
void Explode::Draw(SDL_Renderer *renderer)
{
	SDL_RenderCopy(renderer, texture, &drawRect, &posRect);
}

//buller destruction
Explode::~Explode()
{
	//SDL_DestroyTexture(texture);
}
