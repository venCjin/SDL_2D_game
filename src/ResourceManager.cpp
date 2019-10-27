#include "ResourceManager.h"

ResourceManager::ResourceManager()
{
}


ResourceManager::~ResourceManager()
{
}

/**
* Loads an image into a texture on the rendering device
* @param file The image file to load
* @param ren The renderer to load the texture onto
* @return the loaded texture, or nullptr if something went wrong.
*/
SDL_Texture* ResourceManager::loadTexture(const std::string& file, SDL_Renderer* ren)
{
	SDL_Texture* texture = IMG_LoadTexture(ren, file.c_str());
	if (texture == nullptr)
	{
		GameLog::logSDLError(std::cerr, "LoadTexture");
	}
	return texture;
}

/**
* Draw an SDL_Texture to an SDL_Renderer at some destination rect
* taking a clip of the texture if desired
* @param tex The source texture we want to draw
* @param ren The renderer we want to draw to
* @param dst The destination rectangle to render the texture to
* @param clip The sub-section of the texture to draw (clipping rect)
*		default of nullptr draws the entire texture
*/
void  ResourceManager::renderTexture(SDL_Texture* tex, SDL_Renderer* ren, SDL_Rect dst,
	SDL_Rect* clip = nullptr)
{
	SDL_RenderCopy(ren, tex, clip, &dst);
}

/**
* Draw an SDL_Texture to an SDL_Renderer at position x, y, with some desired
* width and height
* @param tex The source texture we want to draw
* @param ren The renderer we want to draw to
* @param x The x coordinate to draw to
* @param y The y coordinate to draw to
* @param w The width of the texture to draw
* @param h The height of the texture to draw
*/
void  ResourceManager::renderTexture(SDL_Texture* tex, SDL_Renderer* ren, int x, int y, int w, int h)
{
	//Setup the destination rectangle to be at the position we want
	SDL_Rect dst;
	dst.x = x;
	dst.y = y;
	dst.w = w;
	dst.h = h;
	SDL_RenderCopy(ren, tex, nullptr, &dst);
}

/**
* Draw an SDL_Texture to an SDL_Renderer at position x, y, preserving
* the texture's width and height and taking a clip of the texture if desired
* If a clip is passed, the clip's width and height will be used instead of
*	the texture's
* @param tex The source texture we want to draw
* @param ren The renderer we want to draw to
* @param x The x coordinate to draw to
* @param y The y coordinate to draw to
* @param clip The sub-section of the texture to draw (clipping rect)
*		default of nullptr draws the entire texture
*/
void  ResourceManager::renderTexture(SDL_Texture* tex, SDL_Renderer* ren, int x, int y,
	SDL_Rect* clip = nullptr)
{
	SDL_Rect dst;
	dst.x = x;
	dst.y = y;
	if (clip != nullptr)
	{
		dst.w = clip->w;
		dst.h = clip->h;
	}
	else
	{
		SDL_QueryTexture(tex, nullptr, nullptr, &dst.w, &dst.h);
	}
	renderTexture(tex, ren, dst, clip);
}


void ResourceManager::drawCircle(SDL_Renderer* renderer, int cx, int cy, int radius, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	static const int BPP = 4;

	for (double dy = 1; dy < radius; dy += 1.0)
	{
		double dx = floor(sqrt((2.0 * radius * dy) - (dy * dy)));
		int x = cx - dx;
		SDL_SetRenderDrawColor(renderer, r, g, b, a);
		SDL_RenderDrawLine(renderer, cx - dx, cy + dy - radius, cx + dx, cy + dy - radius);
		SDL_RenderDrawLine(renderer, cx - dx, cy - dy + radius, cx + dx, cy - dy + radius);
	}
	SDL_RenderDrawLine(renderer, cx - radius, cy, cx + radius, cy);
}