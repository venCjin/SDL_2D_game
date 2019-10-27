#include "Player.h"
#include <cstdio>

Player::Player(float x, float y, unsigned width, unsigned height, float speed)
	: x(x), y(y), width(width), height(height), speed(speed)
{
}

Player::~Player()
{
}

void Player::start()
{
}

void Player::update()
{
	if (directionX != 0)
	{
		x += directionX * speed;
		// x += directionX;
	}
	else
	{
	}
	// epsilon

	if (directionY != 0)
	{
		y += directionY * speed;
		// y += directionY;
	}
	else
	{
	}
	//epsilon
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

int Player::distance(Player& p2)
{
	int dx = x + directionX - p2.x;
	int dy = y + directionY - p2.y;
	return sqrt(dx*dx + dy*dy);
}



SmoothPlayer::SmoothPlayer(float x, float y, unsigned width, unsigned height, float speed)
	: Player(x, y, width, height, speed)
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
