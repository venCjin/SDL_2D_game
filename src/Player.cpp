#include "Player.h"

Player::Player(float x, float y, unsigned width, unsigned height, float speed, float radius)
	: x(x), y(y), width(width), height(height), speed(speed), radius(radius)
{
}

Player::~Player()
{
}

void Player::update()
{
	if (directionX != 0)
	{
		x += directionX * speed;
	}

	if (directionY != 0)
	{
		y += directionY * speed;
	}
}

void Player::render(SDL_Rect* cam, SDL_Renderer* renderer)
{
	/* PLAYER 1*/
	// renderTexture(player, renderer, p1.x, p1.y, p1.width, p1.height);
	SDL_Rect fillRect = { x - radius - cam->x, y - radius - cam->y, radius*2, radius*2 };
	SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
	SDL_RenderFillRect(renderer, &fillRect);
	/* PLAYER 1*/
}

bool Player::inLevelBounds(int lvl_w, int lvl_h)
{
	if (x + directionX * speed > radius
		&& y + directionY * speed > radius
		&& x + directionX * speed < lvl_w - radius
		&& y + directionY * speed < lvl_h - radius) return true;
	else return false;
}

/*
void Player::moveX(float direction_x)
{
	directionX = direction_x / 32767.0f;
}

void Player::moveY(float direction_y)
{
	directionY = direction_y / 32767.0f;
}
*/

void Player::moveX(int direction_x)
{
	directionX = direction_x;
}

void Player::moveY(int direction_y)
{
	directionY = direction_y;
}

int Player::distanceX(Player &p2) const
{
	return abs(x + (directionX * speed) - p2.x);
}

int Player::distanceY(Player &p2) const
{
	return abs(y + (directionY * speed) - p2.y);
}



/*  SMOOTH  */

SmoothPlayer::SmoothPlayer(float x, float y, unsigned width, unsigned height, float speed, float radius)
	: Player(x, y, width, height, speed, radius)
{
}

void SmoothPlayer::update()
{
	
	if (directionX != 0)
	{
		x += directionX * speed;
		lastDirectionX = directionX;
	}
	else
	{
		if (abs(lastDirectionX) > 0.1f)
		{
			x += lastDirectionX * speed;
			
			if (lastDirectionX > 0)
			{
				// printf("smooth >\n");
				lastDirectionX -= epsilon;
			}
			else if (lastDirectionX < 0)
			{
				// printf("smooth <\n");
				lastDirectionX += epsilon;
			}
		}
	}

	if (directionY != 0)
	{
		y += directionY * speed;
		lastDirectionY = directionY;
	}
	else
	{
		if (abs(lastDirectionY) > 0.1f)
		{
			y += lastDirectionY * speed;

			if (lastDirectionY < 0)
			{
				// printf("smooth ^\n");
				lastDirectionY += epsilon;
			}
			else if (lastDirectionY > 0)
			{
				// printf("smooth v\n");
				lastDirectionY -= epsilon;
			}
		}
	}
}

void SmoothPlayer::render(SDL_Rect* cam, SDL_Renderer* renderer)
{
	/* PLAYER 2*/
	ResourceManager::drawCircle(renderer, x - cam->x, y - cam->y, radius, 255, 0, 0, 127);
	/* PLAYER 2*/
}
