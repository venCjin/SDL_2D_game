#ifndef PLAYER_H
#define PLAYER_H

#include <cmath>
#include <cstdio>
#include <SDL.h>
#include "ResourceManager.h"

class Player
{
private:
	// float directionX = 0.0f; // zakres od -1 do 1
	// float directionY = 0.0f; // zakres od -1 do 1

public:
	float speed, radius;
	float directionX = 0;
	float directionY = 0;
	float x, y;
	unsigned width, height;

	Player(float x, float y, unsigned width, unsigned height, float speed, float radius);
	~Player();

	virtual void update();
	virtual void render(SDL_Rect* cam, SDL_Renderer* renderer);
	bool inLevelBounds(int lvl_w, int lvl_h);
	// void moveX(float direction_x);
	// void moveY(float direction_y);
	void moveX(int direction_x);
	void moveY(int direction_y);
	int distanceX(Player &p2) const;
	int distanceY(Player &p2) const;
};

#endif

#ifndef SMOOTH_PLAYER_H
#define SMOOTH_PLAYER_H
class SmoothPlayer : public Player
{
public:
	SmoothPlayer(float x, float y, unsigned width, unsigned height, float speed, float radius);

	float lastDirectionX = 0.0f;
	float lastDirectionY = 0.0f;
	const float epsilon = 0.004f;
	void update() override;
	void render(SDL_Rect* cam, SDL_Renderer* renderer) override;
};

#endif

