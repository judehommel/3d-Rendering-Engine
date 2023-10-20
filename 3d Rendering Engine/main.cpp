#include <SDL.h>
#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <string>

//Screen dimension constants
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

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
		x += speed * forwardDirection * deltaTime * sin(yLook);
		x += speed * horizontalDirection * deltaTime * sin(yLook + 1.57);
		z += speed * horizontalDirection * deltaTime * sin(-yLook);

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
private:
	const float speed = 0.007;
	const float sensitivity = 0.002;
	char xLookDir = 0;
	char yLookDir = 0;
	char zLookDir = 0;
	char horizontalDirection = 0;
	char verticalDirection = 0;
	char forwardDirection = 0;
} cam;

vector<vector <SDL_Vertex>> listOfVerticies;
int quad = 0;

struct
{
public:
	void render3dShapes()
	{
		sort(listOfVerticies.begin(), listOfVerticies.end(), [](const std::vector<SDL_Vertex>& a, const std::vector<SDL_Vertex>& b) { return a[4].position.x > b[4].position.x; });

		for (int o = 0; o < quad-1; o++)
		{

			//if (listOfVerticies[o][4].position.x > listOfVerticies[o + 1][4].position.x)
				//iter_swap(listOfVerticies.begin() + o, listOfVerticies.begin() + o + 1);

			for (int i = 0; i < listOfVerticies[o].size(); i += 4)
			{
				SDL_Vertex vertices1[] =
				{
					listOfVerticies[o][0],
					listOfVerticies[o][1],
					listOfVerticies[o][2]
				};

				SDL_Vertex vertices2[] =
				{
					listOfVerticies[o][1],
					listOfVerticies[o][2],
					listOfVerticies[o][3]
				};



				SDL_RenderGeometry(renderer, NULL, vertices1, 3, NULL, 0);
				SDL_RenderGeometry(renderer, NULL, vertices2, 3, NULL, 0);
			}
		}

		listOfVerticies = {};
		quad = 0;
	}
	
	void draw3dRhombus(float x1, float y1, float z1, float w, float h, SDL_Color color)
	{
		//top quads
		draw3dQuad(x1, y1, z1, x1 + w, y1 - h / 2, z1, x1, y1 - h / 2, z1 - w, x1, y1 - h, z1, color);
		draw3dQuad(x1, y1, z1, x1 - w, y1 - h / 2, z1, x1, y1 - h / 2, z1 - w, x1, y1 - h, z1, color);
		draw3dQuad(x1, y1, z1, x1 + w, y1 - h / 2, z1, x1, y1 - h / 2, z1 + w, x1, y1 - h, z1, color);
		draw3dQuad(x1, y1, z1, x1 - w, y1 - h / 2, z1, x1, y1 - h / 2, z1 + w, x1, y1 - h, z1, color);
	};

	void draw3dRect(float x1, float y1, float z1, float x2, float y2, float z2, SDL_Color color)
	{
		//front
		draw3dQuad(x1, y1, z1, x2, y1, z1, x1, y2, z1, x2, y2, z1, color);

		//back
		draw3dQuad(x1, y1, z2, x2, y1, z2, x1, y2, z2, x2, y2, z2, color);

		//top
		draw3dQuad(x1, y2, z1, x2, y2, z1, x1, y2, z2, x2, y2, z2, color);

		//bottom
		draw3dQuad(x1, y1, z1, x2, y1, z1, x1, y1, z2, x2, y1, z2, color);

		//left
		draw3dQuad(x1, y1, z1, x1, y1, z2, x1, y2, z1, x1, y2, z2, color);

		//right
		draw3dQuad(x2, y1, z1, x2, y1, z2, x2, y2, z1, x2, y2, z2, color);
	}
	
	void draw3dPillar(float x, float y, float z, float w, float l, float h, SDL_Color color)
	{
		SDL_Color baseColor = { color.r - 10, color.g - 10, color.b - 10 };

		draw3dRect(x - w, y, z - l, x + w, y + 0.5, z + l, baseColor);
		draw3dRect(x - w, y + h + 0.7, z - l, x + w, y + h + 1.2, z + l, baseColor);
		w /= 1.3;
		l /= 1.3;
		draw3dRect(x - w, y + 0.5, z - l, x + w, y + 1.2, z + l, baseColor);
		draw3dRect(x - w, y + h, z - l, x + w, y + h + 0.7, z + l, baseColor);
		w /= 1.2;
		l /= 1.2;
		draw3dRect(x - w, y + 1.2, z - l, x + w, y + h, z + l, color);
	}
private:
	float get3dPoint(float x, float y, float z, char xOry)
	{
		//int focal = 500;
		//float scale_factor = focal / zRel;

		//float xScreen = xRel * scale_factor + SCREEN_WIDTH / 2;
		//float yScreen = yRel * scale_factor + SCREEN_HEIGHT / 2;

		//if (z < cam.z)
			//return 0;


		//Relative to player coords
		float xRel = x - cam.x;
		float yRel = y - cam.y;
		float zRel = z - cam.z;

		float dz = cos(cam.xLook) * (cos(cam.yLook) * zRel + sin(cam.yLook) * (sin(cam.zLook) * yRel + cos(cam.zLook) * xRel)) - sin(cam.xLook) * (cos(cam.zLook) * yRel - sin(cam.zLook) * xRel);

		int ex = SCREEN_WIDTH / 2;
		int ey = SCREEN_HEIGHT / 2;
		unsigned short int ez = 500;

		if (dz < 0)
		{
			if (xOry == 'x')
			{
				float dx = cos(cam.yLook) * (sin(cam.zLook) * yRel + cos(cam.zLook) * xRel) - sin(cam.yLook) * zRel;
				int xScreen = dx * (ez / dz) + ex;
				return xScreen;
			}
			else if (xOry == 'y')
			{
				float dy = sin(cam.xLook) * (cos(cam.yLook) * zRel + sin(cam.yLook) * (sin(cam.zLook) * yRel + cos(cam.zLook) * xRel)) + cos(cam.xLook) * (cos(cam.zLook) * yRel - sin(cam.zLook) * xRel);
				int yScreen = dy * (ez / dz) + ey;
				return yScreen;
			}
		}
	}

	void draw3dQuad(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3, float x4, float y4, float z4, SDL_Color color)
	{
		SDL_Vertex vertex_1 = { {get3dPoint(x1, y1, z1, 'x'), get3dPoint(x1, y1, z1, 'y')}, {color.r, color.g, color.b, 255}, {1, 1} };
		SDL_Vertex vertex_2 = { {get3dPoint(x2, y2, z2, 'x'), get3dPoint(x2, y2, z2, 'y')}, {color.r, color.g, color.b, 255}, {1, 1} };
		SDL_Vertex vertex_3 = { {get3dPoint(x3, y3, z3, 'x'), get3dPoint(x3, y3, z3, 'y')}, {color.r, color.g, color.b, 255}, {1, 1} };
		SDL_Vertex vertex_4 = { {get3dPoint(x4, y4, z4, 'x'), get3dPoint(x4, y4, z4, 'y')}, {color.r, color.g, color.b, 255}, {1, 1} };

		listOfVerticies.push_back(vector<SDL_Vertex>());

		float vertexDistFromCam_1 = sqrt(pow((x1 - cam.x), 2) + pow((y1 - cam.y), 2) + pow((z1 - cam.z), 2));
		float vertexDistFromCam_2 = sqrt(pow((x2 - cam.x), 2) + pow((y2 - cam.y), 2) + pow((z2 - cam.z), 2));
		float vertexDistFromCam_3 = sqrt(pow((x3 - cam.x), 2) + pow((y3 - cam.y), 2) + pow((z3 - cam.z), 2));
		float vertexDistFromCam_4 = sqrt(pow((x4 - cam.x), 2) + pow((y4 - cam.y), 2) + pow((z4 - cam.z), 2));

		float min1and2vert = min(vertexDistFromCam_1, vertexDistFromCam_2);
		float min3and4vert = min(vertexDistFromCam_3, vertexDistFromCam_4);

		float minVertDistFromCam = min(min1and2vert, min3and4vert);

		float xMid1and2 = (x1 + x2) / 2;
		float yMid1and2 = (y1 + y2) / 2;
		float zMid1and2 = (z1 + z2) / 2;

		float xMid1and3 = (x1 + x3) / 2;
		float yMid1and3 = (y1 + y3) / 2;
		float zMid1and3 = (z1 + z3) / 2;

		float xMid1and4 = (x1 + x4) / 2;
		float yMid1and4 = (y1 + y4) / 2;
		float zMid1and4 = (z1 + z4) / 2;

		float xMid2and4 = (x1 + x4) / 2;
		float yMid2and4 = (y1 + y4) / 2;
		float zMid2and4 = (z1 + z4) / 2;

		float xMid3and4 = (x3 + x4) / 2;
		float yMid3and4 = (y3 + y4) / 2;
		float zMid3and4 = (z3 + z4) / 2;

		float midpointVert1and2 = sqrt(pow((xMid1and2 - cam.x), 2) + pow((yMid1and2 - cam.y), 2) + pow((zMid1and2 - cam.z), 2));

		float midpointVert1and3 = sqrt(pow((xMid1and3 - cam.x), 2) + pow((yMid1and3 - cam.y), 2) + pow((zMid1and3 - cam.z), 2));

		float midpointVert3and4 = sqrt(pow((xMid3and4 - cam.x), 2) + pow((yMid3and4 - cam.y), 2) + pow((zMid3and4 - cam.z), 2));

		float minMidpointVert = min(min(midpointVert1and2, midpointVert3and4), midpointVert1and3);

		float minDistFromCam = min(minVertDistFromCam, minMidpointVert);

		SDL_Vertex distFromCam = { {minDistFromCam} };

		// Put them into array
		listOfVerticies[quad].push_back(vertex_1);
		listOfVerticies[quad].push_back(vertex_2);
		listOfVerticies[quad].push_back(vertex_3);
		listOfVerticies[quad].push_back(vertex_4);
		listOfVerticies[quad].push_back(distFromCam);

		quad++;

		//SDL_RenderGeometry(renderer, NULL, vertices1, 3, NULL, 0);
		//SDL_RenderGeometry(renderer, NULL, vertices2, 3, NULL, 0);
	}
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
			renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			if (renderer == NULL)
			{
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
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

			//Clear screen
			SDL_SetRenderDrawColor(renderer, 230, 230, 230, 0);
			SDL_RenderClear(renderer);

			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);

			Shape3d.draw3dPillar(-5, -3, 0, 1.5, 1.5, 10, { 150, 80, 100 });
			Shape3d.draw3dPillar(5, -3, 0, 1.5, 1.5, 10, { 50, 150, 90 });
			Shape3d.draw3dPillar(-5, -3, 10, 1.5, 1.5, 10, { 100, 100, 100 });
			Shape3d.draw3dPillar(5, -3, 10, 1.5, 1.5, 10, { 110, 100, 150 });
			Shape3d.draw3dPillar(-5, -3, -10, 1.5, 1.5, 10, { 150, 150, 130 });
			Shape3d.draw3dPillar(5, -3, -10, 1.5, 1.5, 10, { 100, 150, 100 });

			Shape3d.draw3dRhombus(0, 6, -20, 2, 8, { 0, 0, 0 });
			
			Shape3d.draw3dRect(5, 10, -20, 3, 8, -22, { 100, 100, 150 });
			Shape3d.draw3dRect(-5, 10, -20, -3, 8, -22, { 170, 100, 100 });
			Shape3d.draw3dRect(5, -5, -20, 3, -3, -22, { 100, 100, 150 });
			Shape3d.draw3dRect(-5, -5, -20, -3, -3, -22, { 170, 100, 100 });
			Shape3d.draw3dRect(8, 2, -20, 6, 4, -22, { 100, 100, 150 });
			Shape3d.draw3dRect(-8, 2, -20, -6, 4, -22, { 170, 100, 100 });

			//Shape3d.draw3dRect(-20, 0, 20, -50, 50, 20.5, { 100, 100, 100 });
			//Shape3d.draw3dRect(-30, 25, 25, -35, 23, 20, { 200, 200, 200 });

			cam.updateLocation();

			Shape3d.render3dShapes();

			//Update screen
			SDL_RenderPresent(renderer);

			//Handle events on queue
			while (SDL_PollEvent(&e))
			{
				if (e.type == SDL_KEYDOWN || e.type == SDL_KEYUP)
					cam.getinput();
				else if (e.type == SDL_QUIT)
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
