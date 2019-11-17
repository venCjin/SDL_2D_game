#ifndef MAP_H
#define MAP_H

#include <cmath>
#include <ctime>
#include <fstream>
#include <SDL.h>
#include "cleanup.hpp"
#include "ResourceManager.h"
#include "Player.h"
#include "Tile.h"

const int LEVEL_WIDTH = 1280;
const int LEVEL_HEIGHT = 1280;

// const int TILE_SIZE = 64;
const int TILE_WIDTH = 64;
const int TILE_HEIGHT = 64;
const int TOTAL_TILES = 400;
const int TOTAL_TILE_SPRITES = 3;

// Tile types
const int TILE_FLOOR = 0;
const int TILE_WALL = 1;
const int TILE_KEY = 2;

struct Win
{
	int index, x, y;
};

class Map
{
	SDL_Texture* tex = nullptr;
	SDL_Rect* tileTypeClip = new SDL_Rect[TOTAL_TILE_SPRITES];
	SDL_Renderer* renderer;
	int levelWidth, levelHeight;
	Tile** tiles = new Tile*[TOTAL_TILES];
	Win win;
public:
	Map(int levelWidth, int levelHeight, SDL_Renderer* renderer);
	~Map();

	bool init();
	bool setTiles();
	void render(SDL_Rect* clip) const;
	void genStartPoint(int& x, int& y);
	int touchesWall(Player& object);
	void showDirection(SDL_Rect* camera);
	bool isWin(Player& object);
	void close();
};

#endif

