#ifndef GAME_H
#define GAME_H

#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include "cleanup.hpp"
#include "GameLog.h"
#include "ResourceManager.h"
#include "Player.h"

// TODO controllers class
const int JOYSTICK_DEAD_ZONE = 8000;
// TODO map class
const int TILE_SIZE = 64;

const int playersMaxDistance = 400;

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

	// TODO export to other class
	SDL_Texture* background = nullptr;
	SDL_Texture* player = nullptr;

	// TODO controllers class
	SDL_GameController* gameController = nullptr;
	SDL_Joystick* joy = nullptr;

	// TODO players
	SmoothPlayer p1 = SmoothPlayer(0, 0, 32, 32, 2.2);
	Player p2 = Player(20, 20, 32, 32, 2.2);
};

#endif