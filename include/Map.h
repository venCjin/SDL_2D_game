#ifndef MAP_H
#define MAP_H

#include <cmath>
#include <SDL.h>
#include "cleanup.hpp"
#include "ResourceManager.h"

const int TILE_SIZE = 64;

class Map
{
	SDL_Texture* tex = nullptr;
	SDL_Renderer* renderer;
	int levelWidth, levelHeight;
public:
	Map(int levelWidth, int levelHeight, SDL_Renderer* renderer);
	~Map() = default;

	bool init();
	void render(SDL_Rect* clip) const;

	void close();
};

#endif

