#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <string>
#include <cmath>

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//Loads individual image as texture
SDL_Texture* loadTexture(std::string path);

//The window we'll be rendering to
SDL_Window* window = NULL;

//The window renderer
SDL_Renderer* renderer = NULL;

//Event handler
SDL_Event e;

using namespace std;

struct
{
	float x = 0;
	float y = 0;
	float z = 0;
	float xLook = 0;
	float yLook = 0;
	float speed = 0.001;
	int xDirection = 0;
	int yDirection = 0;
	int zDirection = 0;
	void getinput()
	{
		const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);

		if (currentKeyStates[SDL_SCANCODE_W])
			zDirection = -1;
		else if (currentKeyStates[SDL_SCANCODE_S])
			zDirection = 1;
		else
			zDirection = 0;

		if (currentKeyStates[SDL_SCANCODE_A])
			xDirection = 1;
		else if (currentKeyStates[SDL_SCANCODE_D])
			xDirection = -1;
		else 
			xDirection = 0;

		if (currentKeyStates[SDL_SCANCODE_E])
			yDirection = 1;
		else if (currentKeyStates[SDL_SCANCODE_Q])
			yDirection = -1;
		else
			yDirection = 0;

		if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_r)
		{ 
			x = 0;
			y = 0;
			z = 0;
			xDirection = 0;
			yDirection = 0;
			zDirection = 0;
		}
	}
	void updateLocation()
	{
		x += speed * xDirection;
		y += speed * yDirection;
		z += speed * zDirection;
	}
} cam;

struct
{
public:
	float get3dPoint(float x, float y, float z, string xOry)
	{
		float depth = 500/z;

		x = x * depth + SCREEN_WIDTH / 2;
		y = y * depth + SCREEN_HEIGHT / 2;

		if (xOry == "x")
			return x;
		if (xOry == "y")
			return y;
	}

	void draw(SDL_Renderer* renderer, float x1, float y1, float z1, float x2, float y2, float z2)
	{
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);

		x1 += cam.x;
		x2 += cam.x;
		y1 += cam.y;
		y2 += cam.y;
		z1 += cam.z;
		z2 += cam.z;

		float scr_x1 = get3dPoint(x1, y1, z1, "x");
		float scr_y1 = get3dPoint(x1, y1, z1, "y");

		float scr_x2 = get3dPoint(x1, y2, z1, "x");
		float scr_y2 = get3dPoint(x1, y2, z1, "y");

		float scr_x3 = get3dPoint(x2, y1, z1, "x");
		float scr_y3 = get3dPoint(x2, y1, z1, "y");

		float scr_x4 = get3dPoint(x2, y2, z1, "x");
		float scr_y4 = get3dPoint(x2, y2, z1, "y");


		float scr_x5 = get3dPoint(x1, y1, z2, "x");
		float scr_y5 = get3dPoint(x1, y1, z2, "y");

		float scr_x6 = get3dPoint(x1, y2, z2, "x");
		float scr_y6 = get3dPoint(x1, y2, z2, "y");

		float scr_x7 = get3dPoint(x2, y1, z2, "x");
		float scr_y7 = get3dPoint(x2, y1, z2, "y");

		float scr_x8 = get3dPoint(x2, y2, z2, "x");
		float scr_y8 = get3dPoint(x2, y2, z2, "y");
		
		//left and right front lines
		SDL_RenderDrawLine(renderer, scr_x1, scr_y1, scr_x2, scr_y2);
		SDL_RenderDrawLine(renderer, scr_x3, scr_y3, scr_x4, scr_y4);

		//top and bottom front lines
		SDL_RenderDrawLine(renderer, scr_x1, scr_y1, scr_x3, scr_y3);
		SDL_RenderDrawLine(renderer, scr_x2, scr_y2, scr_x4, scr_y4);

		//left and right back lines
		SDL_RenderDrawLine(renderer, scr_x5, scr_y5, scr_x6, scr_y6);
		SDL_RenderDrawLine(renderer, scr_x7, scr_y7, scr_x8, scr_y8);

		//top and bottom back lines
		SDL_RenderDrawLine(renderer, scr_x5, scr_y5, scr_x7, scr_y7);
		SDL_RenderDrawLine(renderer, scr_x6, scr_y6, scr_x8, scr_y8);

		//bottom inner lines
		SDL_RenderDrawLine(renderer, scr_x1, scr_y1, scr_x5, scr_y5);
		SDL_RenderDrawLine(renderer, scr_x3, scr_y3, scr_x7, scr_y7);

		//top inner lines
		SDL_RenderDrawLine(renderer, scr_x2, scr_y2, scr_x6, scr_y6);
		SDL_RenderDrawLine(renderer, scr_x4, scr_y4, scr_x8, scr_y8);

	}
private:

} rectangular_prism;

bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			printf("Warning: Linear texture filtering not enabled!");
		}

		//Create window
		window = SDL_CreateWindow("3d Rendering Engine", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (window == NULL)
		{
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			//Create renderer for window
			renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
			if (renderer == NULL)
			{
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags))
				{
					printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
					success = false;
				}
			}
		}
	}

	return success;
}

void close()
{
	//Destroy window	
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	window = NULL;
	renderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

int main(int argc, char* args[])
{
	//Start up SDL and create window
	if (!init())
	{
		printf("Failed to initialize!\n");
	}
	else
	{
		//Main loop flag
		bool quit = false;




		//While application is running
		while (!quit)
		{
			//Clear screen
			SDL_SetRenderDrawColor(renderer, 230, 230, 230, 0);
			SDL_RenderClear(renderer);

			cam.updateLocation();
			rectangular_prism.draw(renderer, 1, 1, 1, -1, -1, -1);

			//Update screen
			SDL_RenderPresent(renderer);

			//Handle events on queue
			while (SDL_PollEvent(&e) != 0)
			{
				if (e.type == SDL_KEYDOWN || e.type == SDL_KEYUP)
					cam.getinput();

				//User requests quit
				if (e.type == SDL_QUIT)
				{
					quit = true;
				}
			}
		}
	}

	//Free resources and close SDL
	close();

	return 0;
}
