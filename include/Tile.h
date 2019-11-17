#ifndef TILE_H
#define TILE_H

#include <SDL.h>

class Tile
{
public:
	//Initializes position and type
	Tile(int x, int y, int width, int height, int tileType);

	//Get the tile type
	int getType();

	//Get the collision box
	SDL_Rect getBox();

private:
	//The attributes of the tile
	SDL_Rect mBox;

	//The tile type
	int mType;
};

#endif