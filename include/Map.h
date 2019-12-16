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
const int LEVEL_HEIGHT = 480;

// const int TILE_SIZE = 64;
const int TILE_WIDTH = 64;
const int TILE_HEIGHT = 64;
const int TOTAL_TILES = 160;
const int TOTAL_TILE_SPRITES = 3;

// Tile types
const int TILE_AIR = 0;
const int TILE_GRASS = 1;
const int TILE_DIRT = 2;

class Map
{
	SDL_Texture* tex = nullptr;
	SDL_Rect* tileTypeClip = new SDL_Rect[TOTAL_TILE_SPRITES];
	Tile** tiles = new Tile*[TOTAL_TILES];
public:
	Map() = default;
	~Map() = default;

	bool init(SDL_Renderer* renderer);
	bool setTiles();
	void render(SDL_Rect* clip, SDL_Renderer* renderer);
	int touchesGrass(Player& object);
	void close();
};

#endif

