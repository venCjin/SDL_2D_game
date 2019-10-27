#include "Map.h"

Map::Map(int levelWidth, int levelHeight, SDL_Renderer* renderer)
	: levelWidth(levelWidth), levelHeight(levelHeight), renderer(renderer)
{
}

bool Map::init()
{
	tex = ResourceManager::loadTexture(resPath + "/images/stone_tiles_3072x2048.png", renderer);
	if (tex == nullptr)
	{
		SDL_SetError("Tile texture didn't load!");
		return false;
	}
	return true;
}

void Map::render(SDL_Rect* clip) const
{
	//Set rendering space and render to screen
	SDL_Rect renderQuad = { 0, 0, levelWidth, levelHeight };

	//Set clip rendering dimensions
	if (clip != NULL)
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	//Render to screen
	SDL_RenderCopy(renderer, tex, clip, &renderQuad);
}

void Map::close()
{
	cleanup(tex);
}
