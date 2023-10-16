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

//Frees media and shuts down SDL
void close();

//The window we'll be rendering to
SDL_Window* window = NULL;

//The window renderer
SDL_Renderer* renderer = NULL;

//Event handler
SDL_Event e;

double deltaTime = 0;

using namespace std;

struct
{
public:
	float x = 0;
	float y = 0;
	float z = 0;
	float xLook = 0;
	float yLook = 0;
	float zLook = 0;
	float speed = 0.007;
	float sensitivity = 0.002;
	int xLookDir = 0;
	int yLookDir = 0;
	int zLookDir = 0;
	int horizontalDirection = 0;
	int verticalDirection = 0;
	int forwardDirection = 0;
	void getinput()
	{
		const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);

		if (currentKeyStates[SDL_SCANCODE_W])
			forwardDirection = -1;
		else if (currentKeyStates[SDL_SCANCODE_S])
			forwardDirection = 1;
		else
			forwardDirection = 0;

		if (currentKeyStates[SDL_SCANCODE_D])
			horizontalDirection = -1;
		else if (currentKeyStates[SDL_SCANCODE_A])
			horizontalDirection = 1;
		else 
			horizontalDirection = 0;

		if (currentKeyStates[SDL_SCANCODE_E])
			verticalDirection = 1;
		else if (currentKeyStates[SDL_SCANCODE_Q])
			verticalDirection = -1;
		else
			verticalDirection = 0;

		if (currentKeyStates[SDL_SCANCODE_LEFT])
			yLookDir = -1;
		else if (currentKeyStates[SDL_SCANCODE_RIGHT])
			yLookDir = 1;
		else
			yLookDir = 0;

		if (currentKeyStates[SDL_SCANCODE_UP])
			xLookDir = 1;
		else if (currentKeyStates[SDL_SCANCODE_DOWN])
			xLookDir = -1;
		else
			xLookDir = 0;

		if (currentKeyStates[SDL_SCANCODE_R])
		{ 
			x = 0;
			y = 0;
			z = 0;
			horizontalDirection = 0;
			verticalDirection = 0;
			forwardDirection = 0;
			xLook = 0;
			yLook = 0;
		}
	}
	void updateLocation()
	{
		//x += speed * horizontalDirection * deltaTime;
		y += speed * verticalDirection * deltaTime;
		z += speed * forwardDirection * deltaTime * sin(yLook + 1.57);
		x += speed * forwardDirection * deltaTime * yLook;
		x += speed * horizontalDirection * deltaTime * sin(yLook + 1.57);
		z += speed * horizontalDirection * deltaTime * -yLook;

		xLook += sensitivity * xLookDir * deltaTime;
		yLook += sensitivity * yLookDir * deltaTime;

		if (xLook > M_PI / 2)
			xLook = M_PI / 2;
		else if (xLook < -M_PI / 2)
			xLook = -M_PI / 2;

		if (yLook > M_PI)
			yLook = -M_PI;
		else if (yLook < -M_PI)
			yLook = M_PI;
	}
} cam;

struct
{
public:
	float get3dPoint(float x, float y, float z, char xOry)
	{
		//int focal = 500;
		//float scale_factor = focal / zRel;

		//float xScreen = ((xRel * cos(cam.xLook)) - (zRel * sin(cam.xLook))) * scale_factor + SCREEN_WIDTH / 2;
		//float yScreen = ((yRel * cos(cam.yLook)) + (xRel * sin(cam.yLook))) * scale_factor + SCREEN_HEIGHT / 2;

		//if (z < cam.z)
			//return 0;



		//Relative to player coords
		float xRel = x - cam.x;
		float yRel = y - cam.y;
		float zRel = z - cam.z;

		float dx = cos(cam.yLook) * (sin(cam.zLook) * yRel + cos(cam.zLook) * xRel) - sin(cam.yLook) * zRel;
		float dy = sin(cam.xLook) * (cos(cam.yLook) * zRel + sin(cam.yLook) * (sin(cam.zLook) * yRel + cos(cam.zLook) * xRel)) + cos(cam.xLook) * (cos(cam.zLook) * yRel - sin(cam.zLook) * xRel);
		float dz = cos(cam.xLook) * (cos(cam.yLook) * zRel + sin(cam.yLook) * (sin(cam.zLook) * yRel + cos(cam.zLook) * xRel)) - sin(cam.xLook) * (cos(cam.zLook) * yRel - sin(cam.zLook) * xRel);

		float ex = SCREEN_WIDTH / 2;
		float ey = SCREEN_HEIGHT / 2;
		float ez = 500;

		float xScreen = dx * (ez / dz) + ex;
		float yScreen = dy * (ez / dz) + ey;

		if (xOry == 'x')
		{
			return xScreen;
		}
		else if (xOry == 'y')
		{
			return yScreen;
		}
	}
	
	void draw3dLine(SDL_Renderer* renderer, float x1, float y1, float z1, float x2, float y2, float z2)
	{
		float scr_x1 = get3dPoint(x1, y1, z1, 'x');
		float scr_y1 = get3dPoint(x1, y1, z1, 'y');
		float scr_x2 = get3dPoint(x2, y2, z2, 'x');
		float scr_y2 = get3dPoint(x2, y2, z2, 'y');

		SDL_RenderDrawLine(renderer, scr_x1, scr_y1, scr_x2, scr_y2);
	};

	void draw3dRhombus(SDL_Renderer* renderer, float x1, float y1, float z1, float l, float w)
	{
		//top point
		draw3dLine(renderer, x1, y1, z1, x1 + w, y1 - l / 2, z1);
		draw3dLine(renderer, x1, y1, z1, x1 - w, y1 - l / 2, z1);
		draw3dLine(renderer, x1, y1, z1, x1, y1 - l / 2, z1 + w);
		draw3dLine(renderer, x1, y1, z1, x1, y1 - l / 2, z1 - w);
		draw3dLine(renderer, x1, y1 - l, z1, x1 + w, y1 - l / 2, z1);
		draw3dLine(renderer, x1, y1 - l, z1, x1 - w, y1 - l / 2, z1);
		draw3dLine(renderer, x1, y1 - l, z1, x1, y1 - l / 2, z1 + w);
		draw3dLine(renderer, x1, y1 - l, z1, x1, y1 - l / 2, z1 - w);
		draw3dLine(renderer, x1 + w, y1 - l / 2, z1, x1, y1 - l / 2, z1 + w);
		draw3dLine(renderer, x1 + w, y1 - l / 2, z1, x1, y1 - l / 2, z1 - w);
		draw3dLine(renderer, x1 - w, y1 - l / 2, z1, x1, y1 - l / 2, z1 + w);
		draw3dLine(renderer, x1 - w, y1 - l / 2, z1, x1, y1 - l / 2, z1 - w);
	};

	void draw3dRect(SDL_Renderer* renderer, float x1, float y1, float z1, float x2, float y2, float z2)
	{
		//front
		draw3dLine(renderer, x1, y1, z1, x1, y2, z1);
		draw3dLine(renderer, x1, y2, z1, x2, y2, z1);
		draw3dLine(renderer, x2, y1, z1, x2, y2, z1);
		draw3dLine(renderer, x2, y1, z1, x1, y1, z1);

		//back
		draw3dLine(renderer, x1, y1, z2, x1, y2, z2);
		draw3dLine(renderer, x1, y1, z2, x1, y2, z2);
		draw3dLine(renderer, x1, y2, z2, x2, y2, z2);
		draw3dLine(renderer, x2, y2, z2, x2, y1, z2);
		draw3dLine(renderer, x2, y1, z2, x1, y1, z2);

		//inner (these cause the weird glitches inside box)
		draw3dLine(renderer, x1, y1, z1, x1, y1, z2);
		draw3dLine(renderer, x2, y1, z1, x2, y1, z2);
		draw3dLine(renderer, x1, y2, z1, x1, y2, z2);
		draw3dLine(renderer, x2, y2, z1, x2, y2, z2);
	}
private:

} Shape3d;

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


		Uint64 NOW = SDL_GetPerformanceCounter();
		Uint64 LAST = 0;

		//While application is running
		while (!quit)
		{
			LAST = NOW;
			NOW = SDL_GetPerformanceCounter();

			deltaTime = (double)((NOW - LAST) * 1000 / (double)SDL_GetPerformanceFrequency());

			cout << cam.xLook << ", " << cam.yLook << endl;

			//Clear screen
			SDL_SetRenderDrawColor(renderer, 230, 230, 230, 0);
			SDL_RenderClear(renderer);

			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);

			//Shape3d.draw3dRhombus(renderer, 1, 1, 1, 5, 1);

			
			Shape3d.draw3dRect(renderer, 1, 1, 1, -1, -1, -1); // bottom right
			Shape3d.draw3dRect(renderer, 6, 1, 1, 2, -1, -1); // bottom left
			Shape3d.draw3dRect(renderer, 6, 5, 1, 2, 2, -1); // top left
			Shape3d.draw3dRect(renderer, 1, 3, 1, 0, 2, 0); // top right
			Shape3d.draw3dRect(renderer, -2, -1, 1, -2.5, -0.5, 0.5); // first
			Shape3d.draw3dRect(renderer, -3, -1, 1, -3.1, -1.1, 0.9); // second
			cam.updateLocation();

			//Update screen
			SDL_RenderPresent(renderer);

			//Handle events on queue
			while (SDL_PollEvent(&e))
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
