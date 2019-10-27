#include <iostream>
#include <ctime>
#include "SDL.h"
#include "SDL_image.h"
#include "cleanup.hpp"
#include "Game.h"
#include "Player.h"


enum
{
	SDL_JOYSTICK_AXIS_LEFTX = 0,
	SDL_JOYSTICK_AXIS_LEFTY = 1,
	SDL_JOYSTICK_AXIS_RIGHTX = 3,
	SDL_JOYSTICK_AXIS_RIGHTY = 4,
};

Game* game = new Game(std::cout);

int main(int argc, char **argv)
{
	const int maxFPS = 60;
	const int frameDelay = 1000 / maxFPS;
	Uint32 frameStart;
	int frameTime;
	
	game->init("Zad1", false);
	
	while (game->running())
	{
		frameStart = SDL_GetTicks();
		
		game->handleEvents();
		game->update();
		game->render();

		frameTime = SDL_GetTicks() - frameStart;
		if (frameDelay > frameTime)
		{
			// SDL_Delay(frameDelay - frameTime);
		}
	}

	game->close();
	// system("PAUSE");
	return 0;
}

/*
	To setup SDL2 in VS Community Project
	1.    Download SDL2-for-vc
	2.    Extract
	3.    Create Project (ConsoleApplication x86 / x64)
	4.    [Configuration == ALL] [Platform == ALL] Project Properties
	4.1   C/C++ > General > Additional Include Directories > (path\to\SDL2\include)
															 (path\to\SDL2_image\include)
	4.2   Linker > General > Additional Library Directories
	4.2.1 [Platform == Win32] (path\to\SDL2\lib\x86)
							  (path\to\SDL2_image\lib\x86)
	4.2.2 [Platform == x64] (path\to\SDL2\lib\x86)
							(path\to\SDL2_image\lib\x86)
	4.3   [Platform == ALL] Linker > Input > Additional Dependencies > (SDL2.lib SDL2main.lib SDL2_image.lib)
	4.4   Debug > Environment > (PATH=path\to\libs) where all DLLs required to run app are (ex. SDL2.dll)
*/


/*
	srand(time(nullptr));

	auto p1 = Player(TILE_SIZE * (rand() % (SCREEN_WIDTH / TILE_SIZE)),
					 TILE_SIZE * (rand() % (SCREEN_HEIGHT / TILE_SIZE)), TILE_SIZE, TILE_SIZE, 1);
	auto p2 = Player(TILE_SIZE * (rand() % (SCREEN_WIDTH / TILE_SIZE)),
					 TILE_SIZE * (rand() % (SCREEN_HEIGHT / TILE_SIZE)), TILE_SIZE, TILE_SIZE, 1);

 */
 /*void drawTriangle(SDL_Renderer* renderer, int x, int y, int a, int h)
 {
	 if (h > 0)
	 {
		 SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, 0xFF);
		 int pointsX[] = {
			 x, x - (a / 2), x + (a / 2) };

		 int pointsY[] = {
			 y - (2 * (h / 3)), y + (h / 3), y + (h / 3) };
		 for (int i = 0; i < (pointsX[2] - pointsX[1]); i++)
		 {
			 SDL_RenderDrawLine(renderer, pointsX[0], pointsY[0], pointsX[1] + i, pointsY[1]);
		 }
	 }
 }*/
 /*else if (e.type == SDL_JOYAXISMOTION)
 {
	 // std::cout << "joy\n";
	 if (e.jaxis.which == 0) // which player joy
	 {
		 //X axis motion
		 if (e.jaxis.axis == SDL_JOYSTICK_AXIS_RIGHTX)
		 {
			 if (abs(e.jaxis.value) > JOYSTICK_DEAD_ZONE)
				 p2.moveX(e.jaxis.value);
			 else
				 p2.moveX(0);
		 }
		 // Y axis motion
		 if (e.jaxis.axis == SDL_JOYSTICK_AXIS_RIGHTY)
		 {
			 if (abs(e.jaxis.value) > JOYSTICK_DEAD_ZONE)
				 p2.moveY(e.jaxis.value);
			 else
				 p2.moveY(0);
		 }
	 }
 }*/