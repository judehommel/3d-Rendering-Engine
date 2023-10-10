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
	float x, y, z = 0;
	float xLook, yLook = 0;
	void update()
	{
		if (e.key.keysym.sym == SDLK_w)
			cam.z -= 0.1;
		if (e.key.keysym.sym == SDLK_a)
			cam.x += 0.1;
		if (e.key.keysym.sym == SDLK_s)
			cam.z += 0.1;
		if (e.key.keysym.sym == SDLK_d)
			cam.x -= 0.1;
		if (e.key.keysym.sym == SDLK_e)
			cam.y += 0.1;
		if (e.key.keysym.sym == SDLK_q)
			cam.y -= 0.1;
		if (e.key.keysym.sym == SDLK_r)
		{ 
			cam.x = 0;
			cam.y = 0;
			cam.z = 0;
		}
	}
} cam;

struct
{
public:
	void draw(SDL_Renderer* renderer, float x1, float y1, float z1, float x2, float y2, float z2)
	{
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);

		x1 += cam.x;
		x2 += cam.x;
		y1 += cam.y;
		y2 += cam.y;
		z1 += cam.z;
		z2 += cam.z;

		float depth1 = 100 / (z1);

		float depth2 = 100 / (z2+2.5);

		int scr_x1 = x1 * depth1 + SCREEN_WIDTH / 2;
		int scr_y1 = y1 * depth1 + SCREEN_HEIGHT / 2;

		int scr_x2 = x1 * depth1 + SCREEN_WIDTH / 2;
		int scr_y2 = y2 * depth1 + SCREEN_HEIGHT / 2;

		int scr_x3 = x2 * depth1 + SCREEN_WIDTH / 2;
		int scr_y3 = y1 * depth1 + SCREEN_HEIGHT / 2;

		int scr_x4 = x2 * depth1 + SCREEN_WIDTH / 2;
		int scr_y4 = y2 * depth1 + SCREEN_HEIGHT / 2;



		int scr_x5 = x1 * depth2 + SCREEN_WIDTH / 2;
		int scr_y5 = y1 * depth2 + SCREEN_HEIGHT / 2;

		int scr_x6 = x1 * depth2 + SCREEN_WIDTH / 2;
		int scr_y6 = y2 * depth2 + SCREEN_HEIGHT / 2;

		int scr_x7 = x2 * depth2 + SCREEN_WIDTH / 2;
		int scr_y7 = y1 * depth2 + SCREEN_HEIGHT / 2;

		int scr_x8 = x2 * depth2 + SCREEN_WIDTH / 2;
		int scr_y8 = y2 * depth2 + SCREEN_HEIGHT / 2;
		
		//left and right front lines
		if (cam.z <= (z1 + cam.z))
		{
			SDL_RenderDrawLine(renderer, scr_x1, scr_y1, scr_x2, scr_y2);
			SDL_RenderDrawLine(renderer, scr_x3, scr_y3, scr_x4, scr_y4);
		}

		//top and bottom front lines
		if (cam.z <= (z1 + cam.z))
		{
			SDL_RenderDrawLine(renderer, scr_x1, scr_y1, scr_x3, scr_y3);
			SDL_RenderDrawLine(renderer, scr_x2, scr_y2, scr_x4, scr_y4);
		}

		//left and right back lines
 		if (cam.z >= (z2 + abs(cam.z) - 1))
		{
			SDL_RenderDrawLine(renderer, scr_x5, scr_y5, scr_x6, scr_y6);
			SDL_RenderDrawLine(renderer, scr_x7, scr_y7, scr_x8, scr_y8);
		}

		//top and bottom back lines
		if (cam.z >= (z2 + abs(cam.z) - 1))
		{
			SDL_RenderDrawLine(renderer, scr_x5, scr_y5, scr_x7, scr_y7);
			SDL_RenderDrawLine(renderer, scr_x6, scr_y6, scr_x8, scr_y8);
		}

		//bottom inner lines
		if (cam.z <= (z1 + cam.z) && cam.z >= (z2 + abs(cam.z) - 1))
		{
			SDL_RenderDrawLine(renderer, scr_x1, scr_y1, scr_x5, scr_y5);
			SDL_RenderDrawLine(renderer, scr_x3, scr_y3, scr_x7, scr_y7);
		}

		//top inner lines
		if (cam.z <= (z1 + cam.z) && cam.z >= (z2 + abs(cam.z) - 1))
		{
			SDL_RenderDrawLine(renderer, scr_x2, scr_y2, scr_x6, scr_y6);
			SDL_RenderDrawLine(renderer, scr_x4, scr_y4, scr_x8, scr_y8);
		}

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

SDL_Texture* loadTexture(std::string path)
{
	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL)
	{
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
	}
	else
	{
		//Create texture from surface pixels
		newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
		if (newTexture == NULL)
		{
			printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		}

		//Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
	}

	return newTexture;
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

			rectangular_prism.draw(renderer, 1, 1, 1, -1, -1, -1);

			//Update screen
			SDL_RenderPresent(renderer);

			//Handle events on queue
			while (SDL_PollEvent(&e) != 0)
			{
				if (e.type == SDL_KEYDOWN)
					cam.update();

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
