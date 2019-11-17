#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H
#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include "GameLog.h"

const std::string resPath = R"(D:\3rok\SEM 5\2D\SDL_2D_game\resources\)";

class ResourceManager
{
public:
	ResourceManager();
	~ResourceManager();
	static SDL_Texture* loadTexture(const std::string& file, SDL_Renderer* ren);
	static void renderTexture(SDL_Texture* tex, SDL_Renderer* ren, SDL_Rect dst, SDL_Rect* clip);
	static void renderTexture(SDL_Texture* tex, SDL_Renderer* ren, int x, int y, int w, int h);
	static void renderTexture(SDL_Texture* tex, SDL_Renderer* ren, int x, int y, SDL_Rect* clip);
	
	// static void set_pixel(SDL_Renderer* renderer, int x, int y, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
	// static void draw_circle(SDL_Renderer* renderer, int n_cx, int n_cy, int radius, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
	static void drawCircle(SDL_Renderer* renderer, int cx, int cy, int radius, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
};

#endif


/*

void ResourceManager::set_pixel(SDL_Renderer* renderer, int x, int y, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	SDL_SetRenderDrawColor(renderer, r, g, b, a);
	SDL_RenderDrawPoint(renderer, x, y);
}
void ResourceManager::draw_circle(SDL_Renderer* renderer, int n_cx, int n_cy, int radius, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	double error = (double)-radius;
	double x = (double)radius - 0.5;
	double y = (double)0.5;
	double cx = n_cx - 0.5;
	double cy = n_cy - 0.5;

	while (x >= y)
	{
		set_pixel(renderer, (int)(cx + x), (int)(cy + y), r, g, b, a);
		set_pixel(renderer, (int)(cx + y), (int)(cy + x), r, g, b, a);

		if (x != 0)
		{
			set_pixel(renderer, (int)(cx - x), (int)(cy + y), r, g, b, a);
			set_pixel(renderer, (int)(cx + y), (int)(cy - x), r, g, b, a);
		}

		if (y != 0)
		{
			set_pixel(renderer, (int)(cx + x), (int)(cy - y), r, g, b, a);
			set_pixel(renderer, (int)(cx - y), (int)(cy + x), r, g, b, a);
		}

		if (x != 0 && y != 0)
		{
			set_pixel(renderer, (int)(cx - x), (int)(cy - y), r, g, b, a);
			set_pixel(renderer, (int)(cx - y), (int)(cy - x), r, g, b, a);
		}

		error += y;
		++y;
		error += y;

		if (error >= 0)
		{
			--x;
			error -= x;
			error -= x;
		}
	}
}

 */