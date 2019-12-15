#include "Map.h"

Map::Map(int levelWidth, int levelHeight, SDL_Renderer* renderer)
	: levelWidth(levelWidth), levelHeight(levelHeight), renderer(renderer)
{
	srand(time(nullptr));
}

Map::~Map()
{
	close();
}

bool Map::init()
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
	std::ifstream map(R"(D:\3rok\SEM 5\2D\SDL_2D_game\resources\images\maze.map)");

	//If the map couldn't be loaded
	if (map.fail())
	{
		// SDL_SetError(R"(Texture not found: D:\3rok\SEM 5\2D\SDL_2D_game\resources\images\maze.map)");
		printf(R"(Texture not found: D:\3rok\SEM 5\2D\SDL_2D_game\resources\images\maze.map)""\n");
		tilesLoaded = false;
	}
	else
	{
		//Initialize the tiles
		for (int i = 0; i < TOTAL_TILES; ++i)
		{
			//Determines what kind of tile will be made
			int tileType = -1;

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

			//If the number is a valid tile number
			if ((tileType >= 0) && (tileType < TOTAL_TILE_SPRITES))
			{
				tiles[i] = new Tile(x, y, TILE_WIDTH, TILE_HEIGHT, tileType);
				if (tileType == TILE_KEY) win = { i, x, y };
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
			tileTypeClip[TILE_FLOOR].x = 0;
			tileTypeClip[TILE_FLOOR].y = 0;
			tileTypeClip[TILE_FLOOR].w = TILE_WIDTH;
			tileTypeClip[TILE_FLOOR].h = TILE_HEIGHT;

			tileTypeClip[TILE_WALL].x = TILE_WIDTH;
			tileTypeClip[TILE_WALL].y = 0;
			tileTypeClip[TILE_WALL].w = TILE_WIDTH;
			tileTypeClip[TILE_WALL].h = TILE_HEIGHT;

			tileTypeClip[TILE_KEY].x = TILE_WIDTH * 2;
			tileTypeClip[TILE_KEY].y = 0;
			tileTypeClip[TILE_KEY].w = TILE_WIDTH;
			tileTypeClip[TILE_KEY].h = TILE_HEIGHT;
		}
	}

	//Close the file
	map.close();

	//If the map was loaded fine
	return tilesLoaded;
}

void Map::render(SDL_Rect* camera) const
{
	//Render level
	for (int i = 0; i < TOTAL_TILES; ++i)
	{
		// If the tile is on screen
		if (Player::checkCollision(*camera, tiles[i]->getBox()))
		{
			// Show the tile
			ResourceManager::renderTexture(tex, renderer, tiles[i]->getBox().x - camera->x, tiles[i]->getBox().y - camera->y, &tileTypeClip[tiles[i]->getType()]);
		}
	}
}

void Map::genStartPoint(int& x, int& y)
{
	int i;
	do
	{
		i = rand() % TOTAL_TILES;
	} while (tiles[i]->getType() != TILE_FLOOR);
	x = tiles[i]->getBox().x + tiles[i]->getBox().w / 2;
	y = tiles[i]->getBox().y + tiles[i]->getBox().h / 2;
}

int Map::touchesWall(Player& object)
{
	//Go through the tiles
	for (int i = 0; i < TOTAL_TILES; ++i)
	{
		//If the tile is a wall type tile
		if ((tiles[i]->getType() == TILE_WALL))
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

void Map::showDirection(SDL_Rect* camera)
{
	// key is not on the screen
	SDL_Rect winTile = tiles[win.index]->getBox();
	if (!Player::checkCollision(*camera, winTile))
	{
		winTile.x += winTile.w/2;
		winTile.y += winTile.h/2;
		
		int x, y, offset = 12;

		if (winTile.x > camera->x + camera->w - offset)
		{
			x = camera->w - offset;
		}
		else if (winTile.x < camera->x + offset)
		{
			x = camera->x + offset;
		}
		else x = winTile.x - camera->x;

		if (winTile.y > camera->y + camera->h - offset)
		{
			y = camera->h - offset;
		}
		else if (winTile.y < camera->y + offset)
		{
			y = camera->y + offset;
		}
		else
		{
			y = winTile.y - camera->y;
		}

		ResourceManager::drawCircle(renderer, x, y, offset-2, 255, 255, 0, 255);
	}
}

bool Map::isWin(Player& object)
{
	//If the collision box touches the key tile
	if (object.checkCollision(tiles[win.index]->getBox()))
	{
		printf("Win!");
		return true;
	}
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
