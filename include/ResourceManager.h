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