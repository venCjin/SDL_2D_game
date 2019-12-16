#include "Map.h"

bool Map::init(SDL_Renderer* renderer)
{
	tex = ResourceManager::loadTexture(resPath + "/images/tileset.png", renderer);
	if (tex == nullptr)
	{
		SDL_SetError("Tile texture didn't load!");
		return false;
	}
	return setTiles();
}

bool Map::setTiles()
{
	//Success flag
	bool tilesLoaded = true;

	//The tile offsets
	int x = 0, y = 0;

	//Open the map
	std::ifstream map(R"(D:\3rok\SEM 5\2D\SDL_2D_game\resources\images\level.map)");

	//If the map couldn't be loaded
	if (map.fail())
	{
		// SDL_SetError(R"(Texture not found: D:\3rok\SEM 5\2D\SDL_2D_game\resources\images\level.map)");
		printf(R"(Texture not found: D:\3rok\SEM 5\2D\SDL_2D_game\resources\images\level.map)""\n");
		tilesLoaded = false;
	}
	else
	{
		//Determines what kind of tile will be made
		int tileType;
		
		//Initialize the tiles
		for (int i = 0; i < TOTAL_TILES; ++i)
		{
			tileType = -1;

			//Read tile from map file
			map >> tileType;

			//If the was a problem in reading the map
			if (map.fail())
			{
				//Stop loading map
				printf("Error loading map: Unexpected end of file!\n");
				tilesLoaded = false;
				break;
			}

			//If the number is a valid tile number and not AIR
			else if ((tileType >= 0) && (tileType < TOTAL_TILE_SPRITES))
			{
				tiles[i] = new Tile(x, y, TILE_WIDTH, TILE_HEIGHT, tileType);
			}
			//If we don't recognize the tile type
			else
			{
				//Stop loading map
				printf("Error loading map: Invalid tile type at %d!\n", i);
				tilesLoaded = false;
				break;
			}

			//Move to next tile spot
			x += TILE_WIDTH;

			//If we've gone too far
			if (x >= LEVEL_WIDTH)
			{
				//Move back
				x = 0;

				//Move to the next row
				y += TILE_HEIGHT;
			}
		}

		//Clip the sprite sheet
		if (tilesLoaded)
		{
			tileTypeClip[TILE_AIR].x = 0;
			tileTypeClip[TILE_AIR].y = 0;
			tileTypeClip[TILE_AIR].w = TILE_WIDTH;
			tileTypeClip[TILE_AIR].h = TILE_HEIGHT;

			tileTypeClip[TILE_GRASS].x = TILE_WIDTH;
			tileTypeClip[TILE_GRASS].y = 0;
			tileTypeClip[TILE_GRASS].w = TILE_WIDTH;
			tileTypeClip[TILE_GRASS].h = TILE_HEIGHT;

			tileTypeClip[TILE_DIRT].x = TILE_WIDTH * 2;
			tileTypeClip[TILE_DIRT].y = 0;
			tileTypeClip[TILE_DIRT].w = TILE_WIDTH;
			tileTypeClip[TILE_DIRT].h = TILE_HEIGHT;
		}
	}

	//Close the file
	map.close();

	//If the map was loaded fine
	return tilesLoaded;
}

void Map::render(SDL_Rect* camera, SDL_Renderer* renderer)
{
	//Render level
	for (int i = 0; i < TOTAL_TILES; ++i)
	{
		// If the tile is on screen
		if (Player::checkCollision(*camera, tiles[i]->getBox()) && tiles[i]->getType()!=TILE_AIR)
		{
			// Show the tile
			ResourceManager::renderTexture(tex, renderer, tiles[i]->getBox().x - camera->x, tiles[i]->getBox().y - camera->y, &tileTypeClip[tiles[i]->getType()]);
		}
	}
}

int Map::touchesGrass(Player& object)
{
	//Go through the tiles
	for (int i = 0; i < TOTAL_TILES; ++i)
	{
		//If the tile is a wall type tile
		if ((tiles[i]->getType() == TILE_GRASS))
		{
			//If the collision box touches the wall tile
			if (object.checkCollision(tiles[i]->getBox()))
			{
				return true;
			}
		}
	}

	//If no wall tiles were touched
	return false;
}

void Map::close()
{
	cleanup(tex);
	//Deallocate tiles
	for (int i = 0; i < TOTAL_TILES; ++i)
	{
		if (tiles[i] != nullptr)
		{
			delete tiles[i];
			tiles[i] = nullptr;
		}
	}
}
