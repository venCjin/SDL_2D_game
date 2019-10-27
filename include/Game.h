#ifndef GAME_H
#define GAME_H

#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include "cleanup.hpp"
#include "GameLog.h"
#include "ResourceManager.h"
#include "Player.h"
#include "Camera.h"
#include "Map.h"

// TODO controllers class
const int JOYSTICK_DEAD_ZONE = 8000;
const int lvl_w = 3072;
const int lvl_h = 2048;

class Game
{
public:
	Game(std::ostream& log, int width = 640, int height = 480);
	~Game();

	void init(const char* title, bool fullscreen);
	void handleEvents();
	void update();
	void render();
	bool running();
	void close();

private:
	int width;
	int height;
	std::ostream& log;
	
	bool isRunning = false;
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;

	SDL_GameController* gameController = nullptr;

	// TODO players
	SmoothPlayer* p1 = nullptr;
	Player* p2 = nullptr;

	// Camera* cam = nullptr;
	SDL_Rect* cam = nullptr;
	Map* map = nullptr;
};

#endif