#ifndef PLAYER_H
#define PLAYER_H

#include <cmath>
#include <cstdio>
#include <SDL.h>
#include "ResourceManager.h"
#include "Tile.h"

class Player // rect
{
private:
	// float directionX = 0.0f; // zakres od -1 do 1
	// float directionY = 0.0f; // zakres od -1 do 1

public:
	float speed, radius;
	float directionX = 0.0f;
	float directionY = 0.0f;
	float x, y;
	/*unsigned width, height;*/

	Player(float x, float y, /*unsigned width, unsigned height,*/ float speed, float radius);
	virtual ~Player();

	virtual void update();
	virtual void revertUpdate();
	virtual void render(SDL_Rect* cam, SDL_Renderer* renderer);
	bool outOfLevelBounds(int lvl_w, int lvl_h);
	static bool checkCollision(SDL_Rect a, SDL_Rect b);
	virtual bool checkCollision(SDL_Rect b);
	// void moveX(float direction_x);
	// void moveY(float direction_y);
	void moveX(int direction_x);
	void moveY(int direction_y);
	int distanceX(Player &p2) const;
	int distanceY(Player &p2) const;
};

class SmoothPlayer : public Player // circle
{
public:
	SmoothPlayer(float x, float y, /*unsigned width, unsigned height,*/ float speed, float radius);

	float lastDirectionX = 0.0f;
	float lastDirectionY = 0.0f;
	const float epsilon = 0.002f;
	void update() override;
	void revertUpdate() override;
	void render(SDL_Rect* cam, SDL_Renderer* renderer) override;
	bool checkCollision(SDL_Rect b) override;
};

#endif

