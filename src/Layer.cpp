#include "Layer.h"

Layer::Layer(float scroolSpeed, std::string path, SDL_Rect* pos, SDL_Renderer* renderer)
: x(0.0f), scroolSpeed(scroolSpeed), directionX(0.0f)
{
	tex = ResourceManager::loadTexture(resPath + path, renderer);
	if (tex == nullptr)
	{
		SDL_SetError("Background texture didn't load!");
		return;
	}
	this->pos = new SDL_Rect(*pos);
}

void Layer::moveX(float dir)
{
	directionX = dir;
}

void Layer::update()
{
	if (directionX != 0)
	{
		x += directionX * scroolSpeed;
	}
}

void Layer::revertUpdate()
{
	if (directionX != 0)
	{
		x -= directionX * scroolSpeed;
	}
}

void Layer::render(SDL_Renderer* renderer)
{
	if (x > 640.0f) x = 0.0f;
	else if (x < 0.0f) x = 640.0f;
	pos->x = x;
	ResourceManager::renderTexture(tex, renderer, 0, 0, pos);
}

void Layer::close()
{
	cleanup(tex);
}
