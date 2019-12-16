#ifndef LAYER_H
#define LAYER_H

#include <SDL.h>
#include "cleanup.hpp"
#include "ResourceManager.h"

class Layer
{
	float x, scroolSpeed, directionX;
	SDL_Rect* pos = nullptr;
	SDL_Texture* tex = nullptr;
	
public:
	Layer(float scroolSpeed, std::string path, SDL_Rect* pos, SDL_Renderer* renderer);
	~Layer() = default;

	void moveX(float dir);
	void update();
	void revertUpdate();
	void render(SDL_Renderer* renderer);
	void close();
};

#endif

