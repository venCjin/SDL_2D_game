#include "Player.h"
#include "Map.h"

Player::Player(float x, float y, /*unsigned width, unsigned height,*/ float speed, float radius)
	: x(x), y(y), /*width(width), height(height),*/ speed(speed), radius(radius)
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

void Player::revertUpdate()
{
	if (directionX != 0)
	{
		x -= directionX * speed;
	}

	if (directionY != 0)
	{
		y -= directionY * speed;
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

bool Player::outOfLevelBounds(int lvl_w, int lvl_h)
{
	if (x < radius
		&& y < radius
		&& x > lvl_w - radius
		&& y > lvl_h - radius) return true;
	else return false;
}

bool Player::checkCollision(SDL_Rect a, SDL_Rect b)
{
	//The sides of the rectangles
	int leftA, leftB;
	int rightA, rightB;
	int topA, topB;
	int bottomA, bottomB;

	//Calculate the sides of rect A
	leftA = a.x;
	rightA = a.x + a.w;
	topA = a.y;
	bottomA = a.y + a.h;

	//Calculate the sides of rect B
	leftB = b.x;
	rightB = b.x + b.w;
	topB = b.y;
	bottomB = b.y + b.h;

	//If any of the sides from A are outside of B
	if (bottomA <= topB)
	{
		return false;
	}

	if (topA >= bottomB)
	{
		return false;
	}

	if (rightA <= leftB)
	{
		return false;
	}

	if (leftA >= rightB)
	{
		return false;
	}

	//If none of the sides from A are outside B
	return true;
}

bool Player::checkCollision(SDL_Rect b)
{
	return checkCollision(SDL_Rect{
		(int)(x - radius),
		(int)(y - radius),
		(int)(radius * 2),
		(int)(radius * 2)
		},
		b);
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
	return abs(x - p2.x);
}

int Player::distanceY(Player &p2) const
{
	return abs(y - p2.y);
}



/*  SMOOTH  */

SmoothPlayer::SmoothPlayer(float x, float y, /*unsigned width, unsigned height,*/ float speed, float radius)
	: Player(x, y, /*width, height,*/ speed, radius)
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

void SmoothPlayer::revertUpdate()
{
	if (directionX != 0)
	{
		x -= directionX * speed;
	}
	lastDirectionX = 0.0f;
	
	if (directionY != 0)
	{
		y -= directionY * speed;
	}
	lastDirectionY = 0.0f;
}

void SmoothPlayer::render(SDL_Rect* cam, SDL_Renderer* renderer)
{
	/* PLAYER 2*/
	ResourceManager::drawCircle(renderer, x - cam->x, y - cam->y, radius, 255, 0, 0, 127);
	/* PLAYER 2*/
}

bool SmoothPlayer::checkCollision(SDL_Rect b)
{
	//Closest point on collision box
    int cX, cY;

    //Find closest x offset
    if( x < b.x )
    {
        cX = b.x;
    }
    else if( x > b.x + b.w )
    {
        cX = b.x + b.w;
    }
    else
    {
        cX = x;
    }

	//Find closest y offset
	if (y < b.y)
	{
		cY = b.y;
	}
	else if (y > b.y + b.h)
	{
		cY = b.y + b.h;
	}
	else
	{
		cY = y;
	}

	//If the closest point is inside the circle
	
	if ((x - cX) * (x - cX) + (y - cY) * (y - cY) < radius * radius)
	{
		//This box and the circle have collided
		return true;
	}

	//If the shapes have not collided
	return false;
}
